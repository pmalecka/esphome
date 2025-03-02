#pragma once

#include "esphome/components/select/select.h"
#include "../fully_jarvis_f.h"

namespace esphome {
namespace fully_jarvis_f {

class KillModeSelect : public select::Select, public Parented<FullyJarvisFComponent> {
 public:
  KillModeSelect() = default;

 protected:
  void control(const std::string &value) override;
};

}  // namespace fully_jarvis_f
}  // namespace esphome
