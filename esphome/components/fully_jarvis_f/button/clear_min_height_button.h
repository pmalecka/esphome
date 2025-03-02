#pragma once

#include "esphome/components/button/button.h"
#include "../fully_jarvis_f.h"

namespace esphome {
namespace fully_jarvis_f {

class ClearMinHeightButton : public button::Button, public Parented<FullyJarvisFComponent> {
 public:
  ClearMinHeightButton() = default;

 protected:
  void press_action() override;
};

}  // namespace fully_jarvis_f
}  // namespace esphome
