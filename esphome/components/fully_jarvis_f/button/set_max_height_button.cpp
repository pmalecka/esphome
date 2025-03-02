#include "set_max_height_button.h"

namespace esphome {
namespace fully_jarvis_f {

void SetMaxHeightButton::press_action() { this->parent_->set_max_height(); }

}  // namespace fully_jarvis_f
}  // namespace esphome
