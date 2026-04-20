#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace vrf_protocol { // Forward declaration
  class VrfCmd;
  class VrfGateway;
  class VrfClimate;
}
#include "vrf.h"

namespace esphome {
namespace uart_vrf {

class UartVrfClimate;

class VrfGatewayWrapper {
  public:
  void add_gateway(vrf_protocol::VrfGateway *gateway);
  void consume_data(uint8_t data);
  vrf_protocol::VrfCmd cmd_find_climates();
  vrf_protocol::VrfCmd cmd_query_next_climate();
  std::vector<vrf_protocol::VrfClimate *> get_climates();

  private:
  vrf_protocol::VrfGateway *vrf_gateway_{nullptr};
  std::vector<vrf_protocol::VrfGateway *> gateways_;
  uint8_t next_idx_{0};

  uint8_t get_next_idx();
  void incr_next_idx();
};

class UartVrfComponent : public Component, public uart::UARTDevice {
 public:
  UartVrfComponent(uart::UARTComponent *uartComponent) : Component(), UARTDevice(uartComponent) {
    this->uart_ = uartComponent;
  }
  void setup() override;
  void loop() override;
  void send_cmd(vrf_protocol::VrfCmd cmd);
  uart::UARTComponent *get_uart() { return this->uart_; }

  // Called from climate.py codegen at compile time to register pre-declared
  // climate slots. Runtime-discovered VRF units are bound to these slots in order.
  void add_child_climate(UartVrfClimate *climate) { this->child_climates_.push_back(climate); }

  // Runtime callbacks fired by the VRF protocol layer.
  void on_climate_create_callback(vrf_protocol::VrfClimate *climate);
  void on_climate_state_callback(vrf_protocol::VrfClimate *climate);

 protected:
  uart::UARTComponent *uart_;
  VrfGatewayWrapper *vrf_gateway_wrapper_;
  // Pre-declared UartVrfClimate entities from YAML, registered via add_child_climate().
  std::vector<UartVrfClimate *> child_climates_;
  // How many child slots have been bound to a discovered VRF unit so far.
  size_t bound_count_{0};
  std::vector<std::vector<uint8_t>> pending_cmds_;
  unsigned long last_time_heartbeat_cmds_{0};
  unsigned long last_time_fire_cmd{0};

  void fire_cmd();
  void find_climates();
  void query_next_climate();
};

} // namespace uart_vrf
} // namespace esphome
