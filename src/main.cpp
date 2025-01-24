#include "MqttClient.hpp"
#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>

std::atomic<bool> running{true};

void handleMessage(const std::string& message) {
    std::cout << "Received message: " << message << std::endl;
}

void signalHandler(int signal) {
    if (signal == SIGINT) {
        running = false;
    }
}

int main() {
    try {
        std::cout << "Starting RTDataLayer..." << std::endl;
        MqttClient client("RTDataLayer", "localhost", 1883);
        std::cout << "MQTT client created." << std::endl;
        client.setMessageCallback(handleMessage);
        std::cout << "Message callback set." << std::endl;
        client.connect();
        std::cout << "Connected to MQTT broker." << std::endl;
        client.subscribe("sensors/workspace");
        std::cout << "Subscribed to topic 'sensors/workspace'." << std::endl;

        // Imposta il gestore di segnali per SIGINT (Ctrl+C)
        std::signal(SIGINT, signalHandler);

        std::cout << "Press Ctrl+C to exit..." << std::endl;

        while (running) {
            // Mantieni l'applicazione in esecuzione per ricevere messaggi
            // Puoi aggiungere una piccola pausa per ridurre l'uso della CPU
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        client.disconnect();
        std::cout << "Disconnected from MQTT broker. Exiting..." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}