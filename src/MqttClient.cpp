#include "MqttClient.hpp"
#include <iostream>

MqttClient::MqttClient(const std::string& id, const std::string& host, int port) {
    mosquitto_lib_init();
    mosq = mosquitto_new(id.c_str(), true, this);
    if (!mosq) {
        throw std::runtime_error("Failed to create Mosquitto instance");
    }
    mosquitto_connect(mosq, host.c_str(), port, 60);
    mosquitto_message_callback_set(mosq, MqttClient::on_message);
}

MqttClient::~MqttClient() {
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

void MqttClient::connect() {
    int ret = mosquitto_connect(mosq, nullptr, 1883, 60);
    if (ret != MOSQ_ERR_SUCCESS) {
        throw std::runtime_error("Failed to connect to MQTT broker");
    }
}

void MqttClient::disconnect() {
    mosquitto_disconnect(mosq);
}

void MqttClient::subscribe(const std::string& topic) {
    mosquitto_subscribe(mosq, nullptr, topic.c_str(), 0);
}

void MqttClient::setMessageCallback(std::function<void(const std::string&)> callback) {
    messageCallback = callback;
}

void MqttClient::on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* message) {
    MqttClient* client = static_cast<MqttClient*>(obj);
    if (client->messageCallback) {
        client->messageCallback(std::string(static_cast<char*>(message->payload), message->payloadlen));
    }
}