#include "touch_mode_select.h"

namespace esphome {
namespace fully_jarvis_f {

void TouchModeSelect::control(const std::string &value) {
  this->publish_state(value);
  this->parent_->set_touch_mode(state);
}

}  // namespace fully_jarvis_f
}  // namespace esphome
