#include "fully_jarvis_f.h"

#include <utility>
#ifdef USE_NUMBER
#include "esphome/components/number/number.h"
#endif
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif

namespace esphome {
namespace fully_jarvis_f {

static const char *const TAG = "fully_jarvis_f";

FullyJarvisFComponent::FullyJarvisFComponent() {}

void FullyJarvisFComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "fully_jarvis_f:");
#ifdef USE_BINARY_SENSOR
  LOG_BINARY_SENSOR("  ", "InitializedBinarySensor", this->initialized_binary_sensor_);
#endif
#ifdef USE_BUTTON
  for (size_t i = 0; button::Button * b : this->go_preset_buttons_) {
    LOG_BUTTON("  ", ("GoPresetButton" + std::to_string(++i)).c_str(), b);
  }
  for (size_t i = 0; button::Button * b : this->set_preset_buttons_) {
    LOG_BUTTON("  ", ("SetPresetButton" + std::to_string(++i)).c_str(), b);
  }
  // LOG_BUTTON("  ", "GoPreset1Button", this->go_preset_1_button_);
  // LOG_BUTTON("  ", "GoPreset2Button", this->go_preset_2_button_);
  // LOG_BUTTON("  ", "GoPreset3Button", this->go_preset_3_button_);
  // LOG_BUTTON("  ", "GoPreset4Button", this->go_preset_4_button_);
  // LOG_BUTTON("  ", "SetPreset1Button", this->set_preset_1_button_);
  // LOG_BUTTON("  ", "SetPreset2Button", this->set_preset_2_button_);
  // LOG_BUTTON("  ", "SetPreset3Button", this->set_preset_3_button_);
  // LOG_BUTTON("  ", "SetPreset4Button", this->set_preset_4_button_);

  LOG_BUTTON("  ", "SetMaxHeightButton", this->set_max_height_button_);
  LOG_BUTTON("  ", "SetMinHeightButton", this->set_min_height_button_);
  LOG_BUTTON("  ", "ClearMaxHeightButton", this->clear_max_height_button_);
  LOG_BUTTON("  ", "ClearMinHeightButton", this->clear_min_height_button_);
#endif
#ifdef USE_SENSOR
  LOG_SENSOR("  ", "Preset1HeightSensor", this->preset_1_height_sensor_);
  LOG_SENSOR("  ", "Preset2HeightSensor", this->preset_2_height_sensor_);
  LOG_SENSOR("  ", "Preset3HeightSensor", this->preset_3_height_sensor_);
  LOG_SENSOR("  ", "Preset4HeightSensor", this->preset_4_height_sensor_);

  LOG_SENSOR("  ", "UserLimitMinHeightSensor", this->user_limit_min_height_sensor_);
  LOG_SENSOR("  ", "UserLimitMaxHeightSensor", this->user_limit_max_height_sensor_);
  LOG_SENSOR("  ", "SysLimitMinHeightSensor", this->sys_limit_min_height_sensor_);
  LOG_SENSOR("  ", "SysLimitMaxHeightSensor", this->sys_limit_max_height_sensor_);
#endif
#ifdef USE_TEXT_SENSOR
  LOG_TEXT_SENSOR("  ", "UserLimitSetTextSensor", this->user_limit_set_text_sensor_);
  LOG_TEXT_SENSOR("  ", "StatusTextSensor", this->status_text_sensor_);
#endif
#ifdef USE_SELECT
  LOG_SELECT("  ", "UnitsSelect", this->units_select_);
  LOG_SELECT("  ", "TouchModeSelect", this->touch_mode_select_);
  LOG_SELECT("  ", "KillModeSelect", this->kill_mode_select_);
  LOG_SELECT("  ", "SensitivitySelect", this->sensitivity_select_);
#endif
#ifdef USE_NUMBER
  LOG_NUMBER("  ", "HeightNumber", this->height_number_);
  LOG_NUMBER("  ", "OffsetNumber", this->offset_number_);
#endif
}

void FullyJarvisFComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up fully_jarvis_f...");

  this->check_uart_settings(9600);

  this->wake_up_();

  this->set_timeout("read_all_info", 70, [this]() { this->read_all_info(); });

  ESP_LOGCONFIG(TAG, "fully_jarvis_f setup complete.");
}

void FullyJarvisFComponent::reset_all_sensors_() {
// resets all the settings to a valid initial state

// TODO: some of these sensors could be nullptr (don't exactly know why)
//       we should consider checking this, before we actually call the publish_state fn
#ifdef USE_BINARY_SENSOR
  this->initialized_binary_sensor_->publish_state(false);
#endif

#ifdef USE_SENSOR
  // TODO: all of these preset_x_height sensors depend on sys_limit_min and units
  //       if those are not defined, then these sensors will just report NAN always
  this->preset_1_height_sensor_->publish_state(NAN);
  this->preset_2_height_sensor_->publish_state(NAN);
  this->preset_3_height_sensor_->publish_state(NAN);
  this->preset_4_height_sensor_->publish_state(NAN);

  this->user_limit_min_height_sensor_->publish_state(NAN);
  this->user_limit_max_height_sensor_->publish_state(NAN);
  this->sys_limit_min_height_sensor_->publish_state(NAN);
  this->sys_limit_max_height_sensor_->publish_state(NAN);
#endif
#ifdef USE_TEXT_SENSOR
  this->user_limit_set_text_sensor_->publish_state(
      USER_LIMIT_SET_INT_TO_ENUM.at(UserLimitSetStructure::USER_LIMIT_SET_UNKNOWN));
  // TODO: define values for this text sensor
  this->status_text_sensor_->publish_state("unknown");
#endif
#ifdef USE_SELECT
  this->units_select_->publish_state(UNITS_INT_TO_ENUM.at(UnitsStructure::UNITS_UNKNOWN));
  this->touch_mode_select_->publish_state(TOUCH_MODE_INT_TO_ENUM.at(TouchModeStructure::TOUCH_MODE_UNKNOWN));
  this->kill_mode_select_->publish_state(KILL_MODE_INT_TO_ENUM.at(KillModeStructure::KILL_MODE_UNKNOWN));
  this->sensitivity_select_->publish_state(SENSITIVITY_INT_TO_ENUM.at(SensitivityStructure::SENSITIVITY_UNKNOWN));
#endif
#ifdef USE_NUMBER
  this->height_number_->publish_state(NAN);
  // this->offset_number_->publish_state(NAN);
#endif
}

void FullyJarvisFComponent::read_all_info() {
  // this function will also be called, when any config setting is changed, so that all settings are re-loaded
  // from the desk again

  this->reset_all_sensors_();

  this->get_basic_settings_();  // gets units, kill_mode, touch_mode, sensitivity
  this->get_settings_();        // gets height, preset_1_height, preset_2_height, preset_3_height, preset4_height
  this->get_abs_limits_();      // gets sys_limit_min, sys_limit_max
  this->get_user_limits_();     // gets user_limit_set, user_limit_min, user_limit_max (if set)

  // set a timeout for getting all these settings ~ we had 630 ms in our old code, let's round up to 800ms
  this->set_timeout("ensure_settings_initialized_", 800, [this]() { this->ensure_settings_initialized_(); });
}

bool FullyJarvisFComponent::is_initialized_() {
#ifdef USE_SENSOR
  // TODO: all of these preset_x_height sensors depend on sys_limit_min and units
  //       if those are not defined, then these sensors will just report NAN always
  bool sensors_initialization_successful = !std::isnan(this->preset_1_height_sensor_->get_raw_state()) &&
                                           !std::isnan(this->preset_2_height_sensor_->get_raw_state()) &&
                                           !std::isnan(this->preset_3_height_sensor_->get_raw_state()) &&
                                           !std::isnan(this->preset_4_height_sensor_->get_raw_state()) &&
                                           !std::isnan(this->user_limit_min_height_sensor_->get_raw_state()) &&
                                           !std::isnan(this->user_limit_max_height_sensor_->get_raw_state()) &&
                                           !std::isnan(this->sys_limit_min_height_sensor_->get_raw_state()) &&
                                           !std::isnan(this->sys_limit_max_height_sensor_->get_raw_state());
  if (!sensors_initialization_successful)
    return false;
#endif
#ifdef USE_TEXT_SENSOR
  bool text_sensors_initialization_successful =
      this->user_limit_set_text_sensor_->get_raw_state() !=
          USER_LIMIT_SET_INT_TO_ENUM.at(UserLimitSetStructure::USER_LIMIT_SET_UNKNOWN) &&
      this->status_text_sensor_->get_raw_state() != "unknown";
  if (!text_sensors_initialization_successful)
    return false;
#endif
#ifdef USE_SELECT
  bool selects_initialization_successful =
      this->units_select_->state != UNITS_INT_TO_ENUM.at(UnitsStructure::UNITS_UNKNOWN) &&
      this->touch_mode_select_->state != TOUCH_MODE_INT_TO_ENUM.at(TouchModeStructure::TOUCH_MODE_UNKNOWN) &&
      this->kill_mode_select_->state != KILL_MODE_INT_TO_ENUM.at(KillModeStructure::KILL_MODE_UNKNOWN) &&
      this->sensitivity_select_->state != SENSITIVITY_INT_TO_ENUM.at(SensitivityStructure::SENSITIVITY_UNKNOWN);
  if (!selects_initialization_successful)
    return false;
#endif
#ifdef USE_NUMBER
  bool numbers_initialization_successful =
      !std::isnan(this->height_number_->state);  // && !std::isnan(this->offset_number_->state);
  if (!numbers_initialization_successful)
    return false;
#endif
  return true;
}

void FullyJarvisFComponent::ensure_settings_initialized_() {
  // validate if all settings have been fetched and set correctly

  if (!this->is_initialized_()) {
    if (++this->fail_counter_ > 10) {
      ESP_LOGV(TAG, "Initalization failed, attempt #%d. Marking as failed.", this->fail_counter_);
      this->mark_failed();
    } else {
      // try to connect again..
      ESP_LOGV(TAG, "Initalization failed, attempt #%d. Retrying..", this->fail_counter_);
      this->read_all_info();
    }
  }

  ESP_LOGV(TAG, "Successfully initialized.");
}

void FullyJarvisFComponent::loop() {
  FullyJarvisFMessage incomingMsg;
  bool isValid = false;
  while (available() > 0 && !isValid) {
    int r = read();
    isValid = incomingMsg.parseAndLoadMessage(r);
  }

  if (isValid) {
    ESP_LOGI(TAG, "Incoming COMMAND: %s (%02x)", incomingMsg.getTypeStr(), incomingMsg.getType());
    ESP_LOGV(TAG, "Incoming COMMAND %s", incomingMsg.toString().c_str());
    this->handle_incoming_data_(incomingMsg);
  }
}

void FullyJarvisFComponent::handle_incoming_data_(const FullyJarvisFMessage &msg) {
  // TODO: add guards that don't change value if they don't need to be changed.. (as with the height below)
  // add also check if the sensor is even defined (nullptr check) and if the entity type is define (ifdef use_sensor,
  // etc..)
  switch (msg.getType()) {
    case IncomingCommandType::LocPreset1: {
      uint16_t preset_1_raw = msg.getParam<uint16_t>();
      ESP_LOGD(TAG, "preset_1_raw %u", (unsigned int) preset_1_raw);
      float preset_height = preset_val_to_height_(preset_1_raw);
      if (!std::isnan(preset_height))
        this->preset_1_height_sensor_->publish_state(preset_height);
      break;
    }
    case IncomingCommandType::LocPreset2: {
      uint16_t preset_2_raw = msg.getParam<uint16_t>();
      ESP_LOGD(TAG, "preset_2_raw %u", (unsigned int) preset_2_raw);
      float preset_height = preset_val_to_height_(preset_2_raw);
      if (!std::isnan(preset_height))
        this->preset_2_height_sensor_->publish_state(preset_height);
      break;
    }
    case IncomingCommandType::LocPreset3: {
      uint16_t preset_3_raw = msg.getParam<uint16_t>();
      ESP_LOGD(TAG, "preset_3_raw %u", (unsigned int) preset_3_raw);
      float preset_height = preset_val_to_height_(preset_3_raw);
      if (!std::isnan(preset_height))
        this->preset_3_height_sensor_->publish_state(preset_height);
      break;
    }
    case IncomingCommandType::LocPreset4: {
      uint16_t preset_4_raw = msg.getParam<uint16_t>();
      ESP_LOGD(TAG, "preset_4_raw %u", (unsigned int) preset_4_raw);
      float preset_height = preset_val_to_height_(preset_4_raw);
      if (!std::isnan(preset_height))
        this->preset_4_height_sensor_->publish_state(preset_height);
      break;
    }
    case IncomingCommandType::Units: {
      std::string units = UNITS_INT_TO_ENUM.at(msg.getParam<UnitsStructure>());
      this->units_select_->publish_state(units);
      ESP_LOGI(TAG, "Units: %s", units);
      break;
    }
    case IncomingCommandType::TouchMode: {
      std::string touch_mode = TOUCH_MODE_INT_TO_ENUM.at(msg.getParam<TouchModeStructure>());
      this->touch_mode_select_->publish_state(touch_mode);
      ESP_LOGI(TAG, "Touch mode: %s", touch_mode);
      break;
    }
    case IncomingCommandType::KillMode: {
      std::string kill_mode = KILL_MODE_INT_TO_ENUM.at(msg.getParam<KillModeStructure>());
      this->kill_mode_select_->publish_state(kill_mode);
      ESP_LOGI(TAG, "Kill mode: %s", kill_mode);
      break;
    }
    case IncomingCommandType::Sensitivity: {
      std::string sensitivity = SENSITIVITY_INT_TO_ENUM.at(msg.getParam<SensitivityStructure>());
      this->sensitivity_select_->publish_state(sensitivity);
      ESP_LOGI(TAG, "Sensitivity: %s", sensitivity);
      break;
    }
    case IncomingCommandType::MinMaxSet: {
      std::string user_limit_set = USER_LIMIT_SET_INT_TO_ENUM.at(msg.getParam<UserLimitSetStructure>());
      this->user_limit_set_text_sensor_->publish_state(user_limit_set);
      break;
    }
    case IncomingCommandType::MinHeight: {
      uint16_t user_limit_min = msg.getParam<uint16_t>();
      this->user_limit_min_height_sensor_->publish_state(user_limit_min);
      break;
    }
    case IncomingCommandType::MaxHeight: {
      uint16_t user_limit_max = msg.getParam<uint16_t>();
      this->user_limit_max_height_sensor_->publish_state(user_limit_max);
      break;
    }
    case IncomingCommandType::AbsLimits: {
      uint8_t params[4];
      uint8_t paramSize;
      msg.getParamArray(params, paramSize);

      // -1 = probably rounding error inside
      uint16_t sys_limit_max = COMBINE_BYTES(params[0], params[1]) - 1;
      uint16_t sys_limit_min = COMBINE_BYTES(params[2], params[3]) - 1;

      this->sys_limit_max_height_sensor_->publish_state(sys_limit_max);
      this->sys_limit_min_height_sensor_->publish_state(sys_limit_min);
      break;
    }
    case IncomingCommandType::Height: {
      this->last_reported_height_ = msg.getParam<uint16_t>();
      if (this->height_number_ != nullptr &&
          (!this->height_number_->has_state() || this->height_number_->state != this->last_reported_height_)) {
        this->height_number_->publish_state(this->last_reported_height_);
      }
      break;
    }
    default:
      break;
  }

#ifdef USE_BINARY_SENSOR
  bool initialization_successful = this->is_initialized_();
  if (this->initialized_binary_sensor_ != nullptr &&
      (!this->initialized_binary_sensor_->has_state() ||
       this->initialized_binary_sensor_->state != initialization_successful)) {
    this->initialized_binary_sensor_->publish_state(initialization_successful);
  }
#endif
}

void FullyJarvisFComponent::send_command_(const FullyJarvisFMessage &msg, uint8_t reps = 1) {
  ESP_LOGI(TAG, "Sending COMMAND: %s (%02x)", msg.getTypeStr(), msg.getType());
  ESP_LOGV(TAG, "Sending COMMAND %s", msg.toString().c_str());

  // if (msg == nullptr) {
  //   ESP_LOGE(TAG, "Error: Message is null");
  //   return;
  // }

  if (msg.getSourceId() == SourceType::Incoming) {
    ESP_LOGE(TAG, "Error: Cannot send an incoming message!");
    return;
  }

  uint8_t packet[MAX_PACKET_SIZE];
  msg.construct(packet);

  for (int i = 0; i < reps; ++i)

    // call the esphome uart write_array fn
    write_array(packet, msg.getPacketLength());
}

void FullyJarvisFComponent::wake_up_() { this->send_command_(FullyJarvisFMessage(OutgoingCommandType::Wake)); }

void FullyJarvisFComponent::get_settings_() {
  this->send_command_(FullyJarvisFMessage(OutgoingCommandType::GetSettings));
}

void FullyJarvisFComponent::get_abs_limits_() {
  this->send_command_(FullyJarvisFMessage(OutgoingCommandType::GetAbsLimits));
}

void FullyJarvisFComponent::get_user_limits_() {
  this->send_command_(FullyJarvisFMessage(OutgoingCommandType::GetUserLimits));
}

void FullyJarvisFComponent::get_basic_settings_() {
  this->send_command_(FullyJarvisFMessage(OutgoingCommandType::GetBasicSettings));
}

void FullyJarvisFComponent::set_offset(uint16_t offset) {
  this->send_command_(FullyJarvisFMessage(OutgoingCommandType::Wake, static_cast<uint16_t>(offset - 6)));
}

void FullyJarvisFComponent::set_height(uint16_t height) {
  // hysteresis correction
  if (height > last_reported_height_)
    height += 2;

  this->send_command_(FullyJarvisFMessage(OutgoingCommandType::MoveTo, static_cast<uint16_t>(height)));
}

void FullyJarvisFComponent::set_units(const std::string &state) {
  uint8_t cmd_value = UNITS_ENUM_TO_INT.at(state);

  if (cmd_value == UnitsStructure::UNITS_UNKNOWN) {
    ESP_LOGW(TAG, "Unknown units value received: [%s]", state);
    return;
  }

  this->send_command_(FullyJarvisFMessage(OutgoingCommandType::SetUnits, cmd_value));

  this->set_timeout("read_all_info", 0, [this]() { this->read_all_info(); });
}

void FullyJarvisFComponent::set_touch_mode(const std::string &state) {
  uint8_t cmd_value = TOUCH_MODE_ENUM_TO_INT.at(state);

  if (cmd_value == TouchModeStructure::TOUCH_MODE_UNKNOWN) {
    ESP_LOGW(TAG, "Unknown touch mode value received: [%s]", state);
    return;
  }

  this->send_command_(FullyJarvisFMessage(OutgoingCommandType::SetTouchMode, cmd_value));

  this->set_timeout("read_all_info", 0, [this]() { this->read_all_info(); });
}

void FullyJarvisFComponent::set_kill_mode(const std::string &state) {
  uint8_t cmd_value = KILL_MODE_ENUM_TO_INT.at(state);

  if (cmd_value == KillModeStructure::KILL_MODE_UNKNOWN) {
    ESP_LOGW(TAG, "Unknown kill mode value received: [%s]", state);
    return;
  }

  this->send_command_(FullyJarvisFMessage(OutgoingCommandType::SetKillMode, cmd_value));

  this->set_timeout("read_all_info", 0, [this]() { this->read_all_info(); });
}

void FullyJarvisFComponent::set_sensitivity(const std::string &state) {
  uint8_t cmd_value = SENSITIVITY_ENUM_TO_INT.at(state);

  if (cmd_value == SensitivityStructure::SENSITIVITY_UNKNOWN) {
    ESP_LOGW(TAG, "Unknown sensitivity value received: [%s]", state);
    return;
  }

  this->send_command_(FullyJarvisFMessage(OutgoingCommandType::SetSensitivity, cmd_value));

  this->set_timeout("read_all_info", 0, [this]() { this->read_all_info(); });
}

void FullyJarvisFComponent::go_preset(uint8_t preset) {
  switch (preset) {
    case 1:
      this->send_command_(FullyJarvisFMessage(OutgoingCommandType::MoveToPreset1));
      break;
    case 2:
      this->send_command_(FullyJarvisFMessage(OutgoingCommandType::MoveToPreset2));
      break;
    case 3:
      this->send_command_(FullyJarvisFMessage(OutgoingCommandType::MoveToPreset3));
      break;
    case 4:
      this->send_command_(FullyJarvisFMessage(OutgoingCommandType::MoveToPreset4));
      break;

    default:
      ESP_LOGW(TAG, "Unknown preset number received for go_preset: [%d]", preset);
      break;
  }
}

void FullyJarvisFComponent::set_preset(uint8_t preset) {
  switch (preset) {
    case 1:
      this->send_command_(FullyJarvisFMessage(OutgoingCommandType::SetPreset1));
      if (this->preset_1_height_sensor_ != nullptr) {
        if (this->preset_1_height_sensor_->get_state() != 0)
          this->preset_1_height_sensor_->publish_state(0);
      }
      this->set_timeout("read_all_info", 0, [this]() { this->read_all_info(); });
      break;
    case 2:
      this->send_command_(FullyJarvisFMessage(OutgoingCommandType::SetPreset2));
      if (this->preset_2_height_sensor_ != nullptr) {
        if (this->preset_2_height_sensor_->get_state() != 0)
          this->preset_2_height_sensor_->publish_state(0);
      }
      this->set_timeout("read_all_info", 0, [this]() { this->read_all_info(); });
      break;
    case 3:
      this->send_command_(FullyJarvisFMessage(OutgoingCommandType::SetPreset3));
      if (this->preset_3_height_sensor_ != nullptr) {
        if (this->preset_3_height_sensor_->get_state() != 0)
          this->preset_3_height_sensor_->publish_state(0);
      }
      this->set_timeout("read_all_info", 0, [this]() { this->read_all_info(); });
      break;
    case 4:
      this->send_command_(FullyJarvisFMessage(OutgoingCommandType::SetPreset4));
      if (this->preset_4_height_sensor_ != nullptr) {
        if (this->preset_4_height_sensor_->get_state() != 0)
          this->preset_4_height_sensor_->publish_state(0);
      }
      this->set_timeout("read_all_info", 0, [this]() { this->read_all_info(); });
      break;

    default:
      ESP_LOGW(TAG, "Unknown preset number received for set_preset: [%d]", preset);
      break;
  }
}

void FullyJarvisFComponent::set_max_height() {
  this->send_command_(FullyJarvisFMessage(OutgoingCommandType::SetMaxHeight));

  // TODO: this might not be necessary here, if the reload/restart logic is sound..
  if (this->user_limit_max_height_sensor_ != nullptr) {
    if (this->user_limit_max_height_sensor_->get_state() != 0)
      this->user_limit_max_height_sensor_->publish_state(0);
  }

  if (this->user_limit_set_text_sensor_ != nullptr) {
    std::string user_limit_set_unknown = USER_LIMIT_SET_INT_TO_ENUM.at(UserLimitSetStructure::USER_LIMIT_SET_UNKNOWN);
    if (this->user_limit_set_text_sensor_->get_state() != user_limit_set_unknown)
      this->user_limit_set_text_sensor_->publish_state(user_limit_set_unknown);
  }

  this->set_timeout("read_all_info", 0, [this]() { this->read_all_info(); });
}

void FullyJarvisFComponent::set_min_height() {
  this->send_command_(FullyJarvisFMessage(OutgoingCommandType::SetMinHeight));

  // TODO: this might not be necessary here, if the reload/restart logic is sound..
  if (this->user_limit_max_height_sensor_ != nullptr) {
    if (!std::isnan(this->user_limit_max_height_sensor_->get_raw_state()))
      this->user_limit_max_height_sensor_->publish_state(NAN);
  }

  if (this->user_limit_set_text_sensor_ != nullptr) {
    std::string user_limit_set_unknown = USER_LIMIT_SET_INT_TO_ENUM.at(UserLimitSetStructure::USER_LIMIT_SET_UNKNOWN);
    if (this->user_limit_set_text_sensor_->get_state() != user_limit_set_unknown)
      this->user_limit_set_text_sensor_->publish_state(user_limit_set_unknown);
  }

  this->set_timeout("read_all_info", 0, [this]() { this->read_all_info(); });
}

void FullyJarvisFComponent::clear_max_height() {
  this->send_command_(FullyJarvisFMessage(OutgoingCommandType::ClearMinMax, static_cast<uint8_t>(0x01)));

  // TODO: this might not be necessary here, if the reload/restart logic is sound..
  if (this->user_limit_max_height_sensor_ != nullptr) {
    if (!std::isnan(this->user_limit_max_height_sensor_->get_raw_state()))
      this->user_limit_max_height_sensor_->publish_state(NAN);
  }

  if (this->user_limit_set_text_sensor_ != nullptr) {
    std::string user_limit_set_unknown = USER_LIMIT_SET_INT_TO_ENUM.at(UserLimitSetStructure::USER_LIMIT_SET_UNKNOWN);
    if (this->user_limit_set_text_sensor_->get_state() != user_limit_set_unknown)
      this->user_limit_set_text_sensor_->publish_state(user_limit_set_unknown);
  }

  this->set_timeout("read_all_info", 0, [this]() { this->read_all_info(); });
}

void FullyJarvisFComponent::clear_min_height() {
  this->send_command_(FullyJarvisFMessage(OutgoingCommandType::ClearMinMax, static_cast<uint8_t>(0x02)));

  // TODO: this might not be necessary here, if the reload/restart logic is sound..
  if (this->user_limit_max_height_sensor_ != nullptr) {
    if (!std::isnan(this->user_limit_max_height_sensor_->get_state()))
      this->user_limit_max_height_sensor_->publish_state(NAN);
  }

  if (this->user_limit_set_text_sensor_ != nullptr) {
    std::string user_limit_set_unknown = USER_LIMIT_SET_INT_TO_ENUM.at(UserLimitSetStructure::USER_LIMIT_SET_UNKNOWN);
    if (this->user_limit_set_text_sensor_->get_state() != user_limit_set_unknown)
      this->user_limit_set_text_sensor_->publish_state(user_limit_set_unknown);
  }

  this->set_timeout("read_all_info", 0, [this]() { this->read_all_info(); });
}

float FullyJarvisFComponent::preset_val_to_height_(uint16_t raw) {
  /*
  /  Desk raw range:
  /  = sysLimitRawMax - sysLimitRawMin
  /  = 0x3B80 - 0x13F5 = 10123
  /
  /  Desk converted range:
  /  = sysLimitMax - sysLimitMin = 649
  /  = conversion = 649 / 10123 = 0.0641 mm
  /  height = sysLimitMin + (raw - sysLimitMin) * conversion
  */

  if (this->sys_limit_min_height_sensor_ == nullptr || !this->sys_limit_min_height_sensor_->has_state() ||
      std::isnan(this->sys_limit_min_height_sensor_->get_state()))
    return NAN;

  if (this->units_select_ == nullptr || !this->units_select_->has_state())
    return NAN;

  float sys_limit = this->sys_limit_min_height_sensor_->get_state();
  std::string units = this->units_select_->state;

  float height_mm = sys_limit + round((float) (raw - 0x13F5) * 0.0642f);
  return units == "inch" ? round((float) height_mm * 0.393f) : round((float) height_mm);
}

#ifdef USE_BUTTON
void FullyJarvisFComponent::set_go_preset_button(uint8_t preset, button::Button *b) {
  this->go_preset_buttons_[preset] = b;
}
void FullyJarvisFComponent::set_set_preset_button(uint8_t preset, button::Button *b) {
  this->set_preset_buttons_[preset] = b;
}
#endif

}  // namespace fully_jarvis_f
}  // namespace esphome
