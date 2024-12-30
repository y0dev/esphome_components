
#include "module_communication.h"

namespace esphome {
namespace dr_comms {

// Initialize UDP in setup
void DevCommsComponent::setup() {
    udp_.begin(local_port_);
}

// Checks if the WiFi is connected
bool DevCommsComponent::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

// Sends a message using UDP
void DevCommsComponent::sendMessage(const Message& msg) {
    if (!isConnected()) {
        ESP_LOGE("DevCommsComponent", "WiFi not connected, message not sent.");
        return;
    }

    std::string serializedMsg = msg.serialize();
    udp_.beginPacket(receiver_ip_.c_str(), receiver_port_);
    udp_.write(reinterpret_cast<const uint8_t*>(serializedMsg.c_str()), serializedMsg.length());
    udp_.endPacket();
}

// Receives a message from UDP
Message DevCommsComponent::receiveMessage() {
    char incomingPacket[255];
    int packetSize = udp_.parsePacket();
    if (packetSize) {
        int len = udp_.read(incomingPacket, sizeof(incomingPacket) - 1);
        if (len > 0) {
            incomingPacket[len] = '\0';  // Null-terminate the string
            return Message::deserialize(std::string(incomingPacket));
        }
    }
    return Message();  // Return an empty message if none received
}

}  // namespace dr_comms
}  // namespace esphome
