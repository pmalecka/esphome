#include "height_number.h"

namespace esphome {
namespace fully_jarvis_f {

void HeightNumber::control(float value) {
  this->publish_state(value);
  this->parent_->set_height(value);
}

}  // namespace fully_jarvis_f
}  // namespace esphome
