#pragma once  // Ensures the file is included only once during compilation

#include "esphome.h"  // Include ESPHome framework

namespace esphome {
namespace garage_door_controller {

class GarageDoorController : public Component {
 public:
  // Function to set vacation mode
  void set_vacation_mode(bool enabled);

  // Function to check if vacation mode is active
  bool is_vacation_mode();

  // Function to open the garage door
  void open_door();

  // Function to close the garage door
  void close_door();

  // Override setup function from ESPHome to initialize pins
  void setup() override;

  // Override loop function to monitor sensors and act accordingly
  void loop() override;

  // Setter functions for pin assignments
  void set_relay_pin(GPIOPin* pin);
  void set_open_sensor_pin(GPIOPin* pin);
  void set_close_sensor_pin(GPIOPin* pin);

  // Check door state (for use without sensors)
  bool is_door_state_open();

  bool is_door_open();              // Helper function to check if door is open (with sensor)
  bool is_door_closed();            // Helper function to check if door is closed (with sensor)

 private:
  bool vacation_mode_ = false;      // Tracks vacation mode state
  bool is_open_ = false;            // Tracks door open/close state without sensors
  std::string server_url_;          // Server URL for logging
  GPIOPin* relay_pin_ = nullptr;    // GPIO pin controlling the garage door relay
  GPIOPin* open_sensor_pin_ = nullptr;  // GPIO pin for open sensor (optional)
  GPIOPin* close_sensor_pin_ = nullptr; // GPIO pin for close sensor (optional)

};

}  // namespace garage_door_controller
}  // namespace esphome
