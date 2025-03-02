#include "units_select.h"

namespace esphome {
namespace fully_jarvis_f {

void UnitsSelect::control(const std::string &value) {
  this->publish_state(value);
  this->parent_->set_units(state);
}

}  // namespace fully_jarvis_f
}  // namespace esphome
