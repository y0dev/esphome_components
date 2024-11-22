#pragma once

#include "esphome/core/component.h"
#include <string>

namespace esphome {
namespace dr_logger {

class LoggerComponent : public Component {
 public:
  
  // Method to set the server URL
  void set_server_url(const std::string &url);
  // Method to set the device authentication key
  void set_device_auth(const std::string &auth);
  // Method to set the device ID
  void set_device_id(const std::string &id);
  // Method to set the location of the device
  void set_device_location(const std::string &device);
  // Method to send a log message
  void log_message(const std::string &type, const std::string &status, 
                   const std::string &message);

private:
  // Private member variables to store configuration values
  std::string server_url_;  // URL of the server to send logs
  std::string device_auth_;  // Device authentication token
  std::string device_id_;  // Unique identifier for the device
  std::string device_location_;  // Device location 
};

  }  // namespace logger_component
}  // namespace esphome
