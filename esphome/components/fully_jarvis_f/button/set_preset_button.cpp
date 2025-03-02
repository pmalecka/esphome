#include "set_preset_button.h"

namespace esphome {
namespace fully_jarvis_f {

SetPresetButton::SetPresetButton(uint8_t preset) : preset_(preset) {}

void SetPresetButton::press_action() { this->parent_->set_preset(this->preset_); }

}  // namespace fully_jarvis_f
}  // namespace esphome
