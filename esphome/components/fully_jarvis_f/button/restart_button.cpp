#include "restart_button.h"

namespace esphome {
namespace fully_jarvis_f {
// TODO: a setting like this could be beneficial as a diagnostic thing.. to reset the
//       state machine to initial state and start requesting settings again.. (just an idea)

void RestartButton::press_action() { this->parent_->read_all_info(); }

}  // namespace fully_jarvis_f
}  // namespace esphome
