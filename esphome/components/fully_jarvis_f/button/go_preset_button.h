#pragma once

#include "esphome/components/button/button.h"
#include "../fully_jarvis_f.h"

namespace esphome {
namespace fully_jarvis_f {

class GoPresetButton : public button::Button, public Parented<FullyJarvisFComponent> {
 public:
  GoPresetButton(uint8_t preset);

 protected:
  uint8_t preset_;
  void press_action() override;
};

}  // namespace fully_jarvis_f
}  // namespace esphome
