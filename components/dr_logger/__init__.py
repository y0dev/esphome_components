import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

# Namespace for the custom logger component
logger_ns = cg.esphome_ns.namespace('dr_logger')
LoggerComponent = logger_ns.class_('LoggerComponent', cg.Component)

# Define YAML configuration fields
CONF_SERVER_URL = "server_url"  # Server URL where logs are sent
CONF_DEVICE_AUTH = "device_auth"  # Authentication token for the device
CONF_DEVICE_ID = "device_id"  # Unique identifier for the device
CONF_DEVICE_LOCATION = "device_location"  # Unique identifier for the device

# Define the schema for the YAML configuration
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(LoggerComponent),
    cv.Required(CONF_SERVER_URL): cv.string,  # Required field for server URL
    cv.Required(CONF_DEVICE_AUTH): cv.string,  # Required field for authentication key
    cv.Required(CONF_DEVICE_ID): cv.string,  # Required field for device ID
    cv.Required(CONF_DEVICE_LOCATION): cv.string,  # Required field for device location
}).extend(cv.COMPONENT_SCHEMA)  # Extend with base component schema

# Method to generate C++ code based on YAML configuration
def to_code(config):
    # Create a new C++ variable for the logger component
    var = cg.new_Pvariable(config[CONF_ID])
    # Set the server URL
    cg.add(var.set_server_url(config[CONF_SERVER_URL]))
    # Set the device authentication key
    cg.add(var.set_device_auth(config[CONF_DEVICE_AUTH]))
    # Set the device ID
    cg.add(var.set_device_id(config[CONF_DEVICE_ID]))
    # Set the device location
    cg.add(var.set_device_location(config[CONF_DEVICE_LOCATION]))
    # Register the component with ESPHome
    yield cg.register_component(var, config)
