#include "kill_mode_select.h"

namespace esphome {
namespace fully_jarvis_f {

void KillModeSelect::control(const std::string &value) {
  this->publish_state(value);
  this->parent_->set_kill_mode(state);
}

}  // namespace fully_jarvis_f
}  // namespace esphome
