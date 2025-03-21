#include "restart_button.h"

namespace esphome {
namespace fully_jarvis_f {
void RestartButton::press_action() { this->parent_->read_all_info(); }

}  // namespace fully_jarvis_f
}  // namespace esphome
