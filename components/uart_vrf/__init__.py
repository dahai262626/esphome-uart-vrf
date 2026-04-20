import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.components import uart


DEPENDENCIES = ['uart']
AUTO_LOAD = ['climate']

VRF_ID = 'uart_vrf_id'

uart_vrf_ns = cg.esphome_ns.namespace('uart_vrf')
UartVrfComponent = uart_vrf_ns.class_('UartVrfComponent', cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(UartVrfComponent),
}).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)

# ESPHome 2024.x+: use async/await instead of generator-based yield
async def to_code(config):
    u = await cg.get_variable(config[uart.CONF_UART_ID])
    var = cg.new_Pvariable(config[CONF_ID], u)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
