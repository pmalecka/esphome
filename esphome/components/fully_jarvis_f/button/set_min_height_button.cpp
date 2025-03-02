#include "set_min_height_button.h"

namespace esphome {
namespace fully_jarvis_f {

void SetMinHeightButton::press_action() { this->parent_->set_min_height(); }

}  // namespace fully_jarvis_f
}  // namespace esphome
