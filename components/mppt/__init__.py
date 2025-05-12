import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, output
from esphome.const import CONF_ID, CONF_UPDATE_INTERVAL

DEPENDENCIES = ['i2c']
AUTO_LOAD = ['sensor', 'output']

mppt_ns = cg.esphome_ns.namespace('mppt')
MPPTController = mppt_ns.class_('MPPTController', cg.PollingComponent)

CONF_MPPT_CONTROLLER_ID = 'mppt_controller_id'
CONF_VOLTAGE_SENSOR = 'voltage_sensor'
CONF_CURRENT_SENSOR = 'current_sensor'
CONF_OUTPUT_PIN = 'output_pin'
CONF_POWER_SENSOR = 'power_sensor'

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(MPPTController),
    cv.Required(CONF_VOLTAGE_SENSOR): cv.use_id(sensor.Sensor),
    cv.Required(CONF_CURRENT_SENSOR): cv.use_id(sensor.Sensor),
    cv.Required(CONF_OUTPUT_PIN): cv.use_id(output.FloatOutput),
    cv.Required(CONF_POWER_SENSOR): cv.use_id(sensor.Sensor),
    cv.Optional(CONF_UPDATE_INTERVAL, default='1000ms'): cv.update_interval
})

def to_code(config):
    mppt = cg.new_Pvariable(config[CONF_ID])
    cg.add(mppt.set_voltage_sensor(config[CONF_VOLTAGE_SENSOR]))
    cg.add(mppt.set_current_sensor(config[CONF_CURRENT_SENSOR]))
    cg.add(mppt.set_output_pin(config[CONF_OUTPUT_PIN]))
    cg.add(mppt.set_power_sensor(config[CONF_POWER_SENSOR]))
    cg.add(mppt.set_update_interval(config[CONF_UPDATE_INTERVAL]))
    cg.add(mppt.setup())
    cg.register_component(mppt, config)

