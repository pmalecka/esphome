#pragma once

#include "esphome/components/number/number.h"
#include "../fully_jarvis_f.h"

namespace esphome {
namespace fully_jarvis_f {

class OffsetNumber : public number::Number, public Parented<FullyJarvisFComponent> {
 public:
  OffsetNumber() = default;

 protected:
  void control(float value) override;
};

}  // namespace fully_jarvis_f
}  // namespace esphome
