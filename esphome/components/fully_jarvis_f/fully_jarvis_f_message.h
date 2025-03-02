#pragma once

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <stddef.h>
#include <stdint.h>

#include <string>
#include <map>

namespace esphome {
namespace fully_jarvis_f {

#define MIN_PACKET_SIZE 6
#define MAX_PARAM_SIZE 4
#define MAX_PACKET_SIZE MIN_PACKET_SIZE + MAX_PARAM_SIZE

#define FIRST_BYTE(X) ((uint8_t) (X >> 8))
#define SECOND_BYTE(X) ((uint8_t) X)
#define COMBINE_BYTES(X, Y) ((((uint16_t) X) << 8) | Y)

enum SourceType : uint8_t {
  Outgoing = 0xF1,  // outgoing (TX) messages, "as if" from the handset (but actually from esphome)
  Incoming = 0xF2   // incoming (RX) messages, from the controlbox
};

enum IncomingCommandType : uint8_t {
  Height = 0x01,
  Error = 0x02,
  Reset = 0x04,
  AbsLimits = 0x07,
  Calibration = 0x1B,
  MovingTo = 0x1B,
  Version = 0x1C,
  MinMaxSet = 0x20,
  MaxHeight = 0x21,
  MinHeight = 0x22,
  MinMaxReached = 0x23,
  Preset = 0x92,

  // Settings
  LocPreset1 = 0x25,
  LocPreset2 = 0x26,
  LocPreset3 = 0x27,
  LocPreset4 = 0x28,
  Units = 0x0E,
  TouchMode = 0x19,
  KillMode = 0x17,
  Sensitivity = 0x1D,
};

static const std::map<uint8_t, std::string> INCOMING_COMMAND_TYPE_INT_TO_ENUM{
    {IncomingCommandType::Height, "Height"},
    {IncomingCommandType::Error, "Error"},
    {IncomingCommandType::Reset, "Reset"},
    {IncomingCommandType::AbsLimits, "AbsLimits"},
    {IncomingCommandType::Calibration, "Calibration"},
    {IncomingCommandType::MovingTo, "MovingTo"},
    {IncomingCommandType::Version, "Version"},
    {IncomingCommandType::MinMaxSet, "MinMaxSet"},
    {IncomingCommandType::MaxHeight, "MaxHeight"},
    {IncomingCommandType::MinHeight, "MinHeight"},
    {IncomingCommandType::MinMaxReached, "MinMaxReached"},
    {IncomingCommandType::Preset, "Preset"},
    {IncomingCommandType::LocPreset1, "LocPreset1"},
    {IncomingCommandType::LocPreset2, "LocPreset2"},
    {IncomingCommandType::LocPreset3, "LocPreset3"},
    {IncomingCommandType::LocPreset4, "LocPreset4"},
    {IncomingCommandType::Units, "Units"},
    {IncomingCommandType::TouchMode, "TouchMode"},
    {IncomingCommandType::KillMode, "KillMode"},
    {IncomingCommandType::Sensitivity, "Sensitivity"}};

enum OutgoingCommandType : uint8_t {
  // My Jarvis has these
  GetBasicSettings =
      0xFE,  // responds with Units, 0f(unknown), 13(unknown), 14(unknown), 15(unknown), 16(unknown), KillMode,
             // 18(unknown), TouchMode, Version - always 0x35 on my desk, Sensitivity, 1E(unknown)
  Stop = 0x2B,
  Disconnect = 0x0A,

  // These are the original set of messages, taken from https://github.com/phord/Jarvis
  Up = 0x01,
  Down = 0x02,
  SetPreset1 = 0x03,
  SetPreset2 = 0x04,
  SetPreset3 = 0x25,
  SetPreset4 = 0x26,
  MoveToPreset1 = 0x05,
  MoveToPreset2 = 0x06,
  MoveToPreset3 = 0x27,
  MoveToPreset4 = 0x28,
  GetSettings = 0x07,   // responds with LocPreset1,2,3,4 position and Height
  GetAbsLimits = 0x0C,  // responds with AbsLimits
  SetUnits = 0x0E,
  SetOffset = 0x10,
  SetKillMode = 0x17,
  SetTouchMode = 0x19,
  MoveTo = 0x1B,
  GetVersion = 0x1C,  // unkown
  SetSensitivity = 0x1D,
  GetUserLimits = 0x20,  // responds with MinMaxSet, MinHeight
  SetMaxHeight = 0x21,
  SetMinHeight = 0x22,
  ClearMinMax = 0x23,
  Wake = 0x29,  // no ack, just  wakes it up
  SetMoveToLoc = 0x80,
  EnterCalibration = 0x91
};

static const std::map<uint8_t, std::string> OUTGOING_COMMAND_TYPE_INT_TO_ENUM{
    {OutgoingCommandType::GetBasicSettings, "GetBasicSettings"},
    {OutgoingCommandType::Stop, "Stop"},
    {OutgoingCommandType::Disconnect, "Disconnect"},
    {OutgoingCommandType::Up, "Up"},
    {OutgoingCommandType::Down, "Down"},
    {OutgoingCommandType::SetPreset1, "SetPreset1"},
    {OutgoingCommandType::SetPreset2, "SetPreset2"},
    {OutgoingCommandType::SetPreset3, "SetPreset3"},
    {OutgoingCommandType::SetPreset4, "SetPreset4"},
    {OutgoingCommandType::MoveToPreset1, "MoveToPreset1"},
    {OutgoingCommandType::MoveToPreset2, "MoveToPreset2"},
    {OutgoingCommandType::MoveToPreset3, "MoveToPreset3"},
    {OutgoingCommandType::MoveToPreset4, "MoveToPreset4"},
    {OutgoingCommandType::GetSettings, "GetSettings"},
    {OutgoingCommandType::GetAbsLimits, "GetAbsLimits"},
    {OutgoingCommandType::SetUnits, "SetUnits"},
    {OutgoingCommandType::SetOffset, "SetOffset"},
    {OutgoingCommandType::SetKillMode, "SetKillMode"},
    {OutgoingCommandType::SetTouchMode, "SetTouchMode"},
    {OutgoingCommandType::MoveTo, "MoveTo"},
    {OutgoingCommandType::GetVersion, "GetVersion"},
    {OutgoingCommandType::SetSensitivity, "SetSensitivity"},
    {OutgoingCommandType::GetUserLimits, "GetUserLimits"},
    {OutgoingCommandType::SetMaxHeight, "SetMaxHeight"},
    {OutgoingCommandType::SetMinHeight, "SetMinHeight"},
    {OutgoingCommandType::ClearMinMax, "ClearMinMax"},
    {OutgoingCommandType::Wake, "Wake"},
    {OutgoingCommandType::SetMoveToLoc, "SetMoveToLoc"},
    {OutgoingCommandType::EnterCalibration, "EnterCalibration"}};

std::string char2hex(uint8_t value);
std::string array2String(uint8_t *packet, size_t packetSize);

template<class T> T getByteType(uint8_t value);

template<class T, size_t N> bool verifyByteAgainstArray(uint8_t b, T (&array)[N]);

template<class T, size_t N> bool verifyByteAgainstArray(uint8_t b, T (&array)[N]) {
  for (int i = 0; i < N; ++i)
    if (array[i] == b)
      return true;

  return false;
}

class FullyJarvisFMessage {
 public:
  FullyJarvisFMessage();
  FullyJarvisFMessage(SourceType messageSource);
  FullyJarvisFMessage(OutgoingCommandType cmd);
  FullyJarvisFMessage(IncomingCommandType cmd);
  FullyJarvisFMessage(OutgoingCommandType cmd, uint8_t p0);
  FullyJarvisFMessage(IncomingCommandType cmd, uint8_t p0);
  FullyJarvisFMessage(OutgoingCommandType cmd, uint8_t p0, uint8_t p1);
  FullyJarvisFMessage(IncomingCommandType cmd, uint8_t p0, uint8_t p1);
  FullyJarvisFMessage(OutgoingCommandType cmd, uint16_t p01);
  FullyJarvisFMessage(IncomingCommandType cmd, uint16_t p01);
  FullyJarvisFMessage(uint8_t sourceType, uint8_t commandType, size_t paramSize = 0, uint8_t p0 = 0, uint8_t p1 = 0,
                      uint8_t p2 = 0, uint8_t p3 = 0);
  virtual ~FullyJarvisFMessage(){};

  bool parseAndLoadMessage(uint8_t byte);
  void setType(uint8_t val);
  void setSourceId(SourceType tId);
  void setParamSize(size_t size);
  bool setParamN(uint8_t value, size_t n);
  bool setParam(uint8_t p0);
  bool setParam(uint8_t p0, uint8_t p1);
  bool setParam(uint8_t p0, uint8_t p1, uint8_t p2);
  bool setParam(uint16_t p01);
  void setParamArray(uint8_t *params, uint8_t paramSize);
  bool setPacket(uint8_t *data, size_t dataSize);

  uint8_t getType() const;
  std::string getTypeStr() const;

  size_t getPacketLength() const;
  size_t getParamSize() const;
  SourceType getSourceId() const;
  void getParamArray(uint8_t *params, uint8_t &paramSize) const;
  void construct(uint8_t *data) const;
  uint8_t getParamN(size_t n) const;
  std::string toString() const;

  static bool verifyPacket(uint8_t *data, size_t dataSize);
  static uint8_t computeChecksum(uint8_t command, uint8_t paramSize, const uint8_t *params);

  template<typename T> T getParam() const { return static_cast<T>(mParams[0]); }

  template<typename T> bool setParam(T value) { return setParam(static_cast<uint8_t>(value)); }

 protected:
  bool parseByte(uint8_t byte);
  uint8_t mSourceId;
  uint8_t mType;
  uint8_t mParamSize;
  uint8_t mParams[MAX_PARAM_SIZE];

  uint8_t mRawMessage[MAX_PACKET_SIZE];
  size_t mRawMessageSize;

  enum class StateMachineState { Start, Id, Command, ParamSize, Param0, Param1, Param2, Param3, Checksum, End, Parse };
  StateMachineState mStateMachineState;
};

template<> inline uint16_t FullyJarvisFMessage::getParam<uint16_t>() const {
  return COMBINE_BYTES(mParams[0], mParams[1]);
}

}  // namespace fully_jarvis_f
}  // namespace esphome
