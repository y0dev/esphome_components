#include "garage_door_controller.h"
#include "esphome.h"

namespace esphome {
namespace garage_door_controller {

// Define a TAG for the component logs to make filtering easier
static const char *TAG = "garage_door_controller";

// Setup the relay and optional sensors
void GarageDoorController::setup() {
  if (this->relay_pin_ != nullptr) {
    this->relay_pin_->pin_mode(esphome::gpio::FLAG_OUTPUT);
    ESP_LOGD(TAG, "Relay pin set to OUTPUT on GPIO %d", this->relay_pin_);
  } else {
    ESP_LOGE(TAG, "Relay pin not configured!");
  }

  if (open_sensor_pin_ != nullptr) {
    this->open_sensor_pin_->pin_mode(esphome::gpio::FLAG_PULLUP);
    ESP_LOGI(TAG, "Open sensor pin set to INPUT_PULLUP on GPIO %d", this->open_sensor_pin_);
  } else {
    ESP_LOGD(TAG, "No open sensor pin configured.");
  }

  if (close_sensor_pin_ != nullptr) {
    this->close_sensor_pin_->pin_mode(esphome::gpio::FLAG_PULLUP);
    ESP_LOGI(TAG, "Close sensor pin set to INPUT_PULLUP on GPIO %d", this->close_sensor_pin_);
  } else {
    ESP_LOGD(TAG, "No close sensor pin configured.");
  }
}

// Periodically check sensor status (if they exist)
void GarageDoorController::loop() {
  if (is_vacation_mode()) {
    ESP_LOGW(TAG, "Vacation mode active. Garage door operation disabled.");
    return;
  }

  ESP_LOGD(TAG, "Checking door status...");
}

// Function to open the garage door
void GarageDoorController::open_door() {
  if (is_vacation_mode()) {
    ESP_LOGW(TAG, "Attempted to open door in vacation mode.");
    return;
  }


  if (this->is_open_ || is_door_open()) {
    ESP_LOGI(TAG, "Garage door is already open.");
    return;
  }

  this->relay_pin_->digital_write(HIGH);  // Activate relay to open the door
  ESP_LOGI(TAG, "Garage door opening...");
  
  delay(500);  // Adjust based on your door mechanism
  this->relay_pin_->digital_write(LOW);
  ESP_LOGI(TAG, "Garage door opened successfully.");
}

// Function to close the garage door
void GarageDoorController::close_door() {
  if (is_vacation_mode()) {
    ESP_LOGW(TAG, "Attempted to close door in vacation mode.");
    return;
  }

  if (!this->is_open_ || is_door_closed()) {
    ESP_LOGI(TAG, "Garage door is already closed.");
    return;
  }

  this->relay_pin_->digital_write(HIGH);  // Activate relay to close the door
  ESP_LOGI(TAG, "Garage door closing...");
  
  delay(500);
  this->relay_pin_->digital_write(LOW);
  ESP_LOGI(TAG, "Garage door closed successfully.");
}

// Function to set vacation mode
void GarageDoorController::set_vacation_mode(bool enabled) {
  this->vacation_mode_ = enabled;
  ESP_LOGD(TAG, enabled ? "Vacation mode enabled." : "Vacation mode disabled.");
}

// Check if vacation mode is active
bool GarageDoorController::is_vacation_mode() {
  ESP_LOGD(TAG, "Checking vacation mode: %s", vacation_mode_ ? "Enabled" : "Disabled");
  return vacation_mode_;
}

// Helper to check if door is open (if open sensor is used)
bool GarageDoorController::is_door_open() {
  if (this->open_sensor_pin_ != nullptr) {
    bool door_open = (this->open_sensor_pin_->digital_read() == LOW);  // Assuming LOW means open
    ESP_LOGD(TAG, "Checking door open state: %s", door_open ? "Open" : "Closed");
    return door_open;
  }
  ESP_LOGD(TAG, "No open sensor pin configured, assuming door is closed.");
  return false;  // Assume the door is closed if no sensor is provided
}

// Helper to check if door is closed (if close sensor is used)
bool GarageDoorController::is_door_closed() {
  if (this->close_sensor_pin_ != nullptr) {
    bool door_closed = (this->close_sensor_pin_->digital_read() == LOW);  // Assuming LOW means closed
    ESP_LOGD(TAG, "Checking door closed state: %s", door_closed ? "Closed" : "Open");
    return door_closed;
  }
  ESP_LOGD(TAG, "No close sensor pin configured, assuming door is open.");
  return false;
}

// Setter functions for pin assignments
void GarageDoorController::set_relay_pin(GPIOPin* pin) {
  this->relay_pin_ = pin;
  ESP_LOGI(TAG, "Relay pin set to GPIO %d", this->relay_pin_);
}

void GarageDoorController::set_open_sensor_pin(GPIOPin* pin) {
  this->open_sensor_pin_ = pin;
  ESP_LOGI(TAG, "Open sensor pin set to GPIO %d", this->open_sensor_pin_);
}

void GarageDoorController::set_close_sensor_pin(GPIOPin* pin) {
  this->close_sensor_pin_ = pin;
  ESP_LOGI(TAG, "Close sensor pin set to GPIO %d", this->close_sensor_pin_);
}

// Check if the door is open without sensors
bool GarageDoorController::is_door_state_open() {
  return this->is_open_;
}

}  // namespace garage_door_controller
}  // namespace esphome
