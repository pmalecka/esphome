#include "clear_max_height_button.h"

namespace esphome {
namespace fully_jarvis_f {

void ClearMaxHeightButton::press_action() { this->parent_->clear_max_height(); }

}  // namespace fully_jarvis_f
}  // namespace esphome
