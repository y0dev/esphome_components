# __init__.py
# This file defines and registers the GarageDoorComponent for ESPHome
# It acts as the bridge between the YAML configuration and the C++ code.

import esphome.codegen as cg  # Imports ESPHome's code generation utilities
import esphome.config_validation as cv  # Provides validation utilities for YAML configurations
from esphome import pins  # Allows GPIO pin validation
from esphome.components import sensor  # Enables sensor configuration
from esphome.const import CONF_NAME, CONF_ID, CONF_PIN  # Standard ID field for components

# Define the namespace for the custom component
garage_door_ns = cg.esphome_ns.namespace('garage_door_controller')
GarageDoorComponent = garage_door_ns.class_('GarageDoorController', cg.Component)

# Define the YAML configuration schema for the custom component
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(GarageDoorComponent),  # Unique ID for the component
    cv.Required('relay_pin'): pins.gpio_input_pin_schema,  # Required relay pin definition
     # Optional pin configuration for open and close sensors
    cv.Optional("open_sensor_pin", default=-1): pins.gpio_input_pin_schema,
    cv.Optional("close_sensor_pin", default=-1): pins.gpio_input_pin_schema,
}).extend(cv.COMPONENT_SCHEMA)  # Extend the base component schema

# Function to process the YAML configuration and set up the component
async def to_code(config):
    # Create a new instance of the component
    var = cg.new_Pvariable(config[CONF_ID])
    
    # Set the relay pin
    relay_pin = await cg.gpio_pin_expression(config['relay_pin'])
    cg.add(var.set_relay_pin(relay_pin))
    
    # Set the open sensor pin if specified
    if 'open_sensor_pin' in config and config['open_sensor_pin'] is not None:
        open_sensor_pin = await cg.gpio_pin_expression(config['open_sensor_pin'])
        cg.add(var.set_open_sensor_pin(open_sensor_pin))
    
    # Set the close sensor pin if specified
    if 'close_sensor_pin' in config and config['close_sensor_pin'] is not None:
        close_sensor_pin = await cg.gpio_pin_expression(config['close_sensor_pin'])
        cg.add(var.set_close_sensor_pin(close_sensor_pin))
    
    # Register the component with ESPHome
    await cg.register_component(var, config)
