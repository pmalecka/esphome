#include "sensitivity_select.h"

namespace esphome {
namespace fully_jarvis_f {

void SensitivitySelect::control(const std::string &value) {
  this->publish_state(value);
  this->parent_->set_sensitivity(state);
}

}  // namespace fully_jarvis_f
}  // namespace esphome
