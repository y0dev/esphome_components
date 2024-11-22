#include "dr_logger.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"

#include <ArduinoJson.h>  // Include ArduinoJson for JSON construction

#if defined(ESP8266)
  #include <WiFiClientSecure.h>
  // #include <WiFiClient.h>
  #include <ESP8266WiFi.h> // <WiFi.h> For ESP32; use <ESP8266WiFi.h> if you're using ESP8266
  #include <ESP8266HTTPClient.h>  // HTTP client library
#elif defined(ESP32)
#include <WiFiClientSecure.h>
  #include <WiFi.h>
#else
  #error "Unsupported platform"
#endif

namespace esphome {
namespace dr_logger {

// Log tag for debugging purposes
static const char *TAG = "dr_logger";

// Set the server URL
void LoggerComponent::set_server_url(const std::string &url) {
  this->server_url_ = url + "/api/v1/logs/";
}

// Set the device authentication key
void LoggerComponent::set_device_auth(const std::string &auth) {
  this->device_auth_ = auth;
}

// Set the device ID
void LoggerComponent::set_device_id(const std::string &id) {
  this->device_id_ = id;
}

// Set the device ID
void LoggerComponent::set_device_location(const std::string &location) {
  this->device_location_ = location;
}

// Send a log message to the server
void LoggerComponent::log_message(const std::string &type, const std::string &status, 
                                  const std::string &message) {

  // Ensure WiFi is connected
  if (WiFi.status() != WL_CONNECTED) {
    ESP_LOGW(TAG, "WiFi not connected. Cannot send log message.");
    return;  // Exit if not connected
  }

  // std::string server_url = "http://192.168.1.97:8133/api/v1/device";
  std::string server_url = this->server_url_ + this->device_id_;
  
  ESP_LOGD(TAG, "Sending log message to server: %s", server_url.c_str());

  WiFiClient client;
  // Create a secure WiFi client
  // WiFiClientSecure client;
  // client.setInsecure();

  HTTPClient http;
  // http.begin(client, server_url.c_str(), 443);
  // Initialize HTTP connection
  if (!http.begin(client, server_url.c_str())) {
    ESP_LOGW(TAG, "Failed to connect to server: %s", server_url.c_str());
    return;
  } else {
    ESP_LOGD(TAG, "Connected to server: %s", server_url.c_str());
  }

  DynamicJsonDocument json_doc(256);
  // StaticJsonDocument<256> json_doc;
  json_doc["type"] = type;
  json_doc["message"] = message;
  json_doc["status"] = status;
  json_doc["location"] = this->device_location_;
  json_doc["device_id"] = this->device_id_;

  // Serialize JSON to string
  std::string json_payload;
  serializeJson(json_doc, json_payload);

  // Set headers, including device authentication
  // http.addHeader("accept", "application/json");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("device-auth", this->device_auth_.c_str());  // Add the auth header
  http.addHeader("Content-Length", String(json_payload.length()));

  // Send the POST request with the JSON payload
  int httpResponseCode = http.POST(json_payload.c_str());
  // int httpResponseCode = http.GET();

  // Check the response code
  if (httpResponseCode > 0) {
    ESP_LOGI(TAG, "HTTP Response Code: %d", httpResponseCode);
    String response = http.getString();  // Get the response payload
    ESP_LOGD(TAG, "Server response: %s", response.c_str());
  } else {
    ESP_LOGD(TAG, "HTTP Response Code: %d", httpResponseCode);
    ESP_LOGE(TAG, "HTTP Request failed, error: %s", http.errorToString(httpResponseCode).c_str());
  }

  // Close the connection
  http.end();
  return;
}

}  // namespace logger_component
}  // namespace esphome
