#pragma once

#include "esphome/core/component.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <string>

namespace esphome {
namespace dr_comms {

// Message structure
struct Message {
    std::string command;
    std::string payload;

    // Serialize the message into a string
    std::string serialize() const {
        return command + ":" + payload;
    }

    // Deserialize a string into a Message object
    static Message deserialize(const std::string& data) {
        size_t delimPos = data.find(':');
        if (delimPos != std::string::npos) {
            return {data.substr(0, delimPos), data.substr(delimPos + 1)};
        }
        return {"", ""};
    }
};

class DevCommsComponent : public Component {
 public:
    DevCommsComponent(const std::string& device_name, const std::string& receiver_ip, int receiver_port, int local_port)
        : device_name_(device_name), receiver_ip_(receiver_ip), receiver_port_(receiver_port), local_port_(local_port) {}

    void setup() override;
    void sendMessage(const Message& msg);
    Message receiveMessage();

 private:
    std::string device_name_;
    std::string receiver_ip_;
    int receiver_port_;
    int local_port_;
    WiFiUDP udp_;

    bool isConnected();
};

}  // namespace dr_comms
}  // namespace esphome
