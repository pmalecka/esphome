#pragma once
#include "esphome/core/defines.h"
#include "esphome/core/component.h"
#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif
#ifdef USE_NUMBER
#include "esphome/components/number/number.h"
#endif
#ifdef USE_BUTTON
#include "esphome/components/button/button.h"
#endif
#ifdef USE_SELECT
#include "esphome/components/select/select.h"
#endif
#ifdef USE_TEXT_SENSOR
#include "esphome/components/text_sensor/text_sensor.h"
#endif
#include "esphome/components/uart/uart.h"
#include "esphome/core/helpers.h"

#include <map>

#include "fully_jarvis_f_message.h"

namespace esphome {
namespace fully_jarvis_f {

#define CHECK_BIT(var, pos) (((var) >> (pos)) & 1)

enum UnitsStructure : uint8_t { UNITS_MM = 0x00, UNITS_IN = 0x01, UNITS_UNKNOWN = 0xFF };

static const std::map<std::string, uint8_t> UNITS_ENUM_TO_INT{
    {"mm", UNITS_MM}, {"in", UNITS_IN}, {"unknown", UNITS_UNKNOWN}};

static const std::map<uint8_t, std::string> UNITS_INT_TO_ENUM{
    {UNITS_MM, "mm"}, {UNITS_IN, "in"}, {UNITS_UNKNOWN, "unknown"}};

enum TouchModeStructure : uint8_t { TOUCH_MODE_SINGLE = 0x00, TOUCH_MODE_CONTINUOUS = 0x01, TOUCH_MODE_UNKNOWN = 0xFF };

static const std::map<std::string, uint8_t> TOUCH_MODE_ENUM_TO_INT{
    {"single", TOUCH_MODE_SINGLE}, {"continuous", TOUCH_MODE_CONTINUOUS}, {"unknown", TOUCH_MODE_UNKNOWN}};

static const std::map<uint8_t, std::string> TOUCH_MODE_INT_TO_ENUM{
    {TOUCH_MODE_SINGLE, "single"}, {TOUCH_MODE_CONTINUOUS, "continuous"}, {TOUCH_MODE_UNKNOWN, "unknown"}};

enum KillModeStructure : uint8_t { KILL_MODE_KILL = 0x00, KILL_MODE_LET_LIVE = 0x01, KILL_MODE_UNKNOWN = 0xFF };

static const std::map<std::string, uint8_t> KILL_MODE_ENUM_TO_INT{
    {"kill", KILL_MODE_KILL}, {"letlive", KILL_MODE_LET_LIVE}, {"unknown", KILL_MODE_UNKNOWN}};

static const std::map<uint8_t, std::string> KILL_MODE_INT_TO_ENUM{
    {KILL_MODE_KILL, "kill"}, {KILL_MODE_LET_LIVE, "letlive"}, {KILL_MODE_UNKNOWN, "unknown"}};

enum SensitivityStructure : uint8_t {
  SENSITIVITY_HIGH = 0x01,
  SENSITIVITY_MEDIUM = 0x02,
  SENSITIVITY_LOW = 0x03,
  SENSITIVITY_UNKNOWN = 0xFF
};

static const std::map<std::string, uint8_t> SENSITIVITY_ENUM_TO_INT{{"high", SENSITIVITY_HIGH},
                                                                    {"medium", SENSITIVITY_MEDIUM},
                                                                    {"low", SENSITIVITY_LOW},
                                                                    {"unknown", SENSITIVITY_UNKNOWN}};

static const std::map<uint8_t, std::string> SENSITIVITY_INT_TO_ENUM{{SENSITIVITY_HIGH, "high"},
                                                                    {SENSITIVITY_MEDIUM, "medium"},
                                                                    {SENSITIVITY_LOW, "low"},
                                                                    {SENSITIVITY_UNKNOWN, "unknown"}};

enum UserLimitSetStructure : uint8_t {
  USER_LIMIT_SET_NONE = 0x00,
  USER_LIMIT_SET_MAX = 0x01,
  USER_LIMIT_SET_MIN = 0x10,
  USER_LIMIT_SET_BOTH = 0x11,
  USER_LIMIT_SET_UNKNOWN = 0xFF
};

static const std::map<std::string, uint8_t> USER_LIMIT_SET_ENUM_TO_INT{{"none", USER_LIMIT_SET_NONE},
                                                                       {"max", USER_LIMIT_SET_MAX},
                                                                       {"min", USER_LIMIT_SET_MIN},
                                                                       {"both", USER_LIMIT_SET_BOTH},
                                                                       {"unknown", USER_LIMIT_SET_UNKNOWN}};
static const std::map<uint8_t, std::string> USER_LIMIT_SET_INT_TO_ENUM{{USER_LIMIT_SET_NONE, "none"},
                                                                       {USER_LIMIT_SET_MAX, "max"},
                                                                       {USER_LIMIT_SET_MIN, "min"},
                                                                       {USER_LIMIT_SET_BOTH, "both"},
                                                                       {USER_LIMIT_SET_UNKNOWN, "unknown"}};
// setup, initialized, going_up, going_down, at_preset_1,
// at_preset_2, at_preset_3, at_preset_4, at_user_min, at_user_max, at_sys_min, at_sys_max
enum StatusStructure : uint8_t {
  STATUS_SETUP,
  STATUS_INITIALIZED,
  STATUS_GOING_UP,
  STATUS_GOING_DOWN,
  STATUS_AT_PRESET_1,
  STATUS_AT_PRESET_2,
  STATUS_AT_PRESET_3,
  STATUS_AT_PRESET_4,
  STATUS_AT_USER_MIN,
  STATUS_AT_USER_MAX,
  STATUS_AT_SYS_MIN,
  STATUS_AT_SYS_MAX,
  STATUS_UNKNOWN = 0xFF
};

static const std::map<std::string, uint8_t> STATUS_ENUM_TO_INT{{"setup", STATUS_SETUP},
                                                               {"initialized", STATUS_INITIALIZED},
                                                               {"going_up", STATUS_GOING_UP},
                                                               {"going_down", STATUS_GOING_DOWN},
                                                               {"at_preset_1", STATUS_AT_PRESET_1},
                                                               {"at_preset_2", STATUS_AT_PRESET_2},
                                                               {"at_preset_3", STATUS_AT_PRESET_3},
                                                               {"at_preset_4", STATUS_AT_PRESET_4},
                                                               {"at_user_min", STATUS_AT_USER_MIN},
                                                               {"at_user_max", STATUS_AT_USER_MAX},
                                                               {"at_sys_min", STATUS_AT_SYS_MIN},
                                                               {"at_sys_max", STATUS_AT_SYS_MAX},
                                                               {"unknown", STATUS_UNKNOWN}};

static const std::map<uint8_t, std::string> STATUS_INT_TO_ENUM{{STATUS_SETUP, "setup"},
                                                               {STATUS_INITIALIZED, "initialized"},
                                                               {STATUS_GOING_UP, "going_up"},
                                                               {STATUS_GOING_DOWN, "going_down"},
                                                               {STATUS_AT_PRESET_1, "at_preset_1"},
                                                               {STATUS_AT_PRESET_2, "at_preset_2"},
                                                               {STATUS_AT_PRESET_3, "at_preset_3"},
                                                               {STATUS_AT_PRESET_4, "at_preset_4"},
                                                               {STATUS_AT_USER_MIN, "at_user_min"},
                                                               {STATUS_AT_USER_MAX, "at_user_max"},
                                                               {STATUS_AT_SYS_MIN, "at_sys_min"},
                                                               {STATUS_AT_SYS_MAX, "at_sys_max"},
                                                               {STATUS_UNKNOWN, "unknown"}};

class FullyJarvisFComponent : public Component, public uart::UARTDevice {
#ifdef USE_SENSOR
  SUB_SENSOR(preset_1_height)
  SUB_SENSOR(preset_2_height)
  SUB_SENSOR(preset_3_height)
  SUB_SENSOR(preset_4_height)
  SUB_SENSOR(user_limit_min_height)
  SUB_SENSOR(user_limit_max_height)
  SUB_SENSOR(sys_limit_min_height)
  SUB_SENSOR(sys_limit_max_height)
#endif
#ifdef USE_BINARY_SENSOR
  SUB_BINARY_SENSOR(initialized)
#endif
#ifdef USE_TEXT_SENSOR
  SUB_TEXT_SENSOR(user_limit_set)
  SUB_TEXT_SENSOR(status)
#endif
#ifdef USE_SELECT
  SUB_SELECT(units)
  SUB_SELECT(touch_mode)
  SUB_SELECT(kill_mode)
  SUB_SELECT(sensitivity)
#endif
#ifdef USE_BUTTON
  SUB_BUTTON(set_max_height)
  SUB_BUTTON(set_min_height)
  SUB_BUTTON(clear_max_height)
  SUB_BUTTON(clear_min_height)
  SUB_BUTTON(restart)
#endif
#ifdef USE_NUMBER
  SUB_NUMBER(height)
  SUB_NUMBER(offset)
#endif

 public:
  FullyJarvisFComponent();
  void setup() override;
  void dump_config() override;
  void loop() override;
#ifdef USE_NUMBER
  void set_height(uint16_t height);
  void set_offset(uint16_t offset);
#endif
  void read_all_info();
#ifdef USE_SELECT
  void set_units(const std::string &state);
  void set_touch_mode(const std::string &state);
  void set_kill_mode(const std::string &state);
  void set_sensitivity(const std::string &state);
#endif
#ifdef USE_BUTTON
  // buttons
  void set_go_preset_button(uint8_t preset, button::Button *b);
  void set_set_preset_button(uint8_t preset, button::Button *b);
  void go_preset(uint8_t preset);
  void set_preset(uint8_t preset);
  void set_max_height();
  void set_min_height();
  void clear_max_height();
  void clear_min_height();
#endif

 protected:
  void send_command_(const FullyJarvisFMessage &msg, uint8_t reps);
  void wake_up_();
  void get_settings_();
  void get_abs_limits_();
  void get_user_limits_();
  void get_basic_settings_();

  void handle_incoming_data_(const FullyJarvisFMessage &msg);
  void reset_all_sensors_();
  void ensure_settings_initialized_();
  bool is_initialized_();
  void log_initialization_state_();

  float preset_val_to_height_(uint16_t raw);

  uint16_t last_reported_height_;
  uint8_t fail_counter_ = 0;
#ifdef USE_BUTTON
  std::vector<button::Button *> go_preset_buttons_ = std::vector<button::Button *>(4);
  std::vector<button::Button *> set_preset_buttons_ = std::vector<button::Button *>(4);
#endif
};

#define LOG_SENSOR_VAL(obj) \
  if ((obj) != nullptr) { \
    ESP_LOGV(TAG, "%s sensor has state: %d, value: '%.2f'", (obj)->get_name().c_str(), (obj)->has_state(), \
             (obj)->get_raw_state()); \
  }

#define LOG_TEXT_SENSOR_VAL(obj) \
  if ((obj) != nullptr) { \
    ESP_LOGV(TAG, "%s text_sensor has state: %d, value: '%s'", (obj)->get_name().c_str(), (obj)->has_state(), \
             (obj)->get_raw_state().c_str()); \
  }

#define LOG_SELECT_VAL(obj) \
  if ((obj) != nullptr) { \
    ESP_LOGV(TAG, "%s select has state: %d, value: '%s'", (obj)->get_name().c_str(), (obj)->has_state(), \
             (obj)->state.c_str()); \
  }

#define LOG_NUMBER_VAL(obj) \
  if ((obj) != nullptr) { \
    ESP_LOGV(TAG, "%s number has state: %d, value: '%.2f'", (obj)->get_name().c_str(), (obj)->has_state(), \
             (obj)->state); \
  }

}  // namespace fully_jarvis_f
}  // namespace esphome
