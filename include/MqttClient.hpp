#ifndef MQTTCLIENT_HPP
#define MQTTCLIENT_HPP

#include <mosquitto.h>
#include <string>
#include <functional>

class MqttClient {
public:
    MqttClient(const std::string& id, const std::string& host, int port);
    ~MqttClient();

    void connect();
    void disconnect();
    void subscribe(const std::string& topic);
    void setMessageCallback(std::function<void(const std::string&)> callback);

private:
    static void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* message);
    struct mosquitto* mosq;
    std::function<void(const std::string&)> messageCallback;
};

#endif // MQTTCLIENT_HPP