import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import CONF_ID
from . import UartVrfComponent, uart_vrf_ns

DEPENDENCIES = ['uart_vrf']

UartVrfClimate = uart_vrf_ns.class_(
    'UartVrfClimate',
    climate.Climate,
    cg.Component,
    cg.Parented.template(UartVrfComponent),
)

CONFIG_SCHEMA = climate.climate_schema(UartVrfClimate).extend({
    cv.GenerateID("uart_vrf_id"): cv.use_id(UartVrfComponent),
}).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    paren = await cg.get_variable(config["uart_vrf_id"])
    # new_Pvariable with no extra args - UartVrfClimate now has a default constructor.
    # The VrfClimate link is set at runtime in on_climate_create_callback.
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, paren)
    await cg.register_component(var, config)
    await climate.register_climate(var, config)
    # Register this climate as a child slot on the parent UartVrfComponent,
    # so at runtime discovered VRF units can be bound to it.
    cg.add(paren.add_child_climate(var))
