#include "go_preset_button.h"

namespace esphome {
namespace fully_jarvis_f {

GoPresetButton::GoPresetButton(uint8_t preset) : preset_(preset) {}

void GoPresetButton::press_action() { this->parent_->go_preset(this->preset_); }

}  // namespace fully_jarvis_f
}  // namespace esphome
