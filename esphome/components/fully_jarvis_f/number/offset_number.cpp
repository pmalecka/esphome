#include "offset_number.h"

namespace esphome {
namespace fully_jarvis_f {

void OffsetNumber::control(float value) {
  this->publish_state(value);
  this->parent_->set_offset(value);
}

}  // namespace fully_jarvis_f
}  // namespace esphome
