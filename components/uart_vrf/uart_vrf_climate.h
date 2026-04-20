#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "./uart_vrf_component.h"

namespace esphome {
namespace uart_vrf {

class UartVrfComponent;

class UartVrfClimate : public Component, public climate::Climate, public Parented<UartVrfComponent> {
    public:
    // Default constructor – required by the new codegen path. The VrfClimate
    // pointer is set later at runtime when a VRF unit is discovered.
    UartVrfClimate() = default;

    void setup() override {}
    void dump_config() override {}
    void control(const climate::ClimateCall &call) override;
    climate::ClimateTraits traits() override;

    // Runtime binding: link a pre-declared UartVrfClimate to a discovered VRF unit.
    void set_core_climate(vrf_protocol::VrfClimate *core_climate) { this->core_climate_ = core_climate; }
    vrf_protocol::VrfClimate *get_core_climate() { return this->core_climate_; }
    bool is_bound() const { return this->core_climate_ != nullptr; }

    protected:
    vrf_protocol::VrfClimate *core_climate_{nullptr};
};

} // namespace uart_vrf
} // namespace esphome
