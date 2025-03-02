#include "fully_jarvis_f_message.h"

namespace esphome {
namespace fully_jarvis_f {

std::string char2hex(uint8_t value) {
  char buffer[3];  // 2 characters for hex representation + 1 for null terminator

  // snprintf safely writes to the buffer, ensuring no overflow
  std::snprintf(buffer, sizeof(buffer), "%02x", value);

  // Construct a std::string from the buffer
  // The std::string constructor copies the content of buffer
  return std::string(buffer);
}

std::string array2String(uint8_t *packet, size_t packetSize) {
  std::string result;
  result.reserve(packetSize);  // Reserve space to avoid multiple allocations

  for (size_t i = 0; i < packetSize; ++i) {
    result += char2hex(packet[i]);
    if (i + 1 < packetSize)
      result += ":";
  }

  return result;
}

FullyJarvisFMessage::FullyJarvisFMessage(SourceType messageSource = SourceType::Incoming)
    : mStateMachineState(StateMachineState::Start), mSourceId(messageSource) {}

bool FullyJarvisFMessage::parseAndLoadMessage(uint8_t byte) {
  bool isValid = this->parseByte(byte);
  if (isValid && mStateMachineState == StateMachineState::Parse) {
    return this->setPacket(this->mRawMessage, this->mRawMessageSize);
  }
  return false;
}

bool FullyJarvisFMessage::parseByte(uint8_t byte) {
  // esphome::ESP_LOGD("JarvisStateMachine", "Processing byte: %02X, current state: %02x", byte,
  // (int)mStateMachineState);
  switch (mStateMachineState) {
    case StateMachineState::Start: {
      mRawMessageSize = 0;
      mRawMessage[mRawMessageSize++] = byte;
      if (byte == SourceType::Incoming) {
        mStateMachineState = StateMachineState::Id;
      } else {
        // discard messages that are not from controlbox and log them (this shouldn't happen..)
        mStateMachineState = StateMachineState::Start;
        ESP_LOGE("JarvisStateMachine", "Got Unexpected start octet %02x", byte);
      }
      break;
    }
    case StateMachineState::Id: {
      if (byte == mSourceId) {
        mRawMessage[mRawMessageSize++] = byte;
        mStateMachineState = StateMachineState::Command;
      } else {
        mStateMachineState = StateMachineState::Start;
        ESP_LOGE("JarvisStateMachine", "Got Unexpected Id octet %02x", byte);
      }
      break;
    }
    case StateMachineState::Command: {
      mRawMessage[mRawMessageSize++] = byte;
      mStateMachineState = StateMachineState::ParamSize;
      break;
    }
    case StateMachineState::ParamSize: {
      mRawMessage[mRawMessageSize++] = byte;
      switch (byte) {
        case 0x00:
          mStateMachineState = StateMachineState::Checksum;
          break;
        case 0x01:
          mStateMachineState = StateMachineState::Param0;
          break;
        case 0x02:
          mStateMachineState = StateMachineState::Param1;
          break;
        case 0x03:
          mStateMachineState = StateMachineState::Param2;
          break;
        case 0x04:
          mStateMachineState = StateMachineState::Param3;
          break;
        default: {
          ESP_LOGE("JarvisSerial", "ParamSize Validation Failed: %s --faulty byte: %s",
                   array2String(mRawMessage, mRawMessageSize).c_str(), char2hex(byte).c_str());
          mStateMachineState = StateMachineState::Start;
        }
      }
      break;
    }
    case StateMachineState::Param0: {
      mRawMessage[mRawMessageSize++] = byte;
      mStateMachineState = StateMachineState::Checksum;
      break;
    }
    case StateMachineState::Param1: {
      mRawMessage[mRawMessageSize++] = byte;
      mStateMachineState = StateMachineState::Param0;
      break;
    }
    case StateMachineState::Param2: {
      mRawMessage[mRawMessageSize++] = byte;
      mStateMachineState = StateMachineState::Param1;
      break;
    }
    case StateMachineState::Param3: {
      mRawMessage[mRawMessageSize++] = byte;
      mStateMachineState = StateMachineState::Param2;
      break;
    }
    case StateMachineState::Checksum: {
      uint8_t cmd = mRawMessage[2];
      uint8_t paramSize = mRawMessage[3];

      uint8_t params[MAX_PARAM_SIZE];
      memcpy(params, mRawMessage + 4, paramSize);

      uint8_t chk = this->computeChecksum(cmd, paramSize, params);
      if (chk == byte) {
        mRawMessage[mRawMessageSize++] = byte;
        mStateMachineState = StateMachineState::End;
      } else {
        ESP_LOGE("JarvisSerial", "Checksum failed for message: %s expected: %s, got %s",
                 array2String(mRawMessage, mRawMessageSize).c_str(), char2hex(chk).c_str(), char2hex(byte).c_str());
        mStateMachineState = StateMachineState::Start;
      }
      break;
    }
    case StateMachineState::End: {
      mRawMessage[mRawMessageSize++] = byte;
      mStateMachineState = StateMachineState::Parse;
      return byte == 0x7E;
    }
  }
  return false;
}

FullyJarvisFMessage::FullyJarvisFMessage(IncomingCommandType cmd) : FullyJarvisFMessage(SourceType::Incoming, cmd, 0) {}

FullyJarvisFMessage::FullyJarvisFMessage(IncomingCommandType cmd, uint8_t p0)
    : FullyJarvisFMessage(SourceType::Incoming, cmd, 1, p0) {}

FullyJarvisFMessage::FullyJarvisFMessage(IncomingCommandType cmd, uint8_t p0, uint8_t p1)
    : FullyJarvisFMessage(SourceType::Incoming, cmd, 2, p0, p1) {}

FullyJarvisFMessage::FullyJarvisFMessage(IncomingCommandType cmd, uint16_t p01)
    : FullyJarvisFMessage(SourceType::Incoming, cmd, 2, FIRST_BYTE(p01), SECOND_BYTE(p01)) {}

FullyJarvisFMessage::FullyJarvisFMessage(OutgoingCommandType cmd) : FullyJarvisFMessage(SourceType::Outgoing, cmd, 0) {}

FullyJarvisFMessage::FullyJarvisFMessage(OutgoingCommandType cmd, uint8_t p0)
    : FullyJarvisFMessage(SourceType::Outgoing, cmd, 1, p0) {}

FullyJarvisFMessage::FullyJarvisFMessage(OutgoingCommandType cmd, uint8_t p0, uint8_t p1)
    : FullyJarvisFMessage(SourceType::Outgoing, cmd, 2, p0, p1) {}

FullyJarvisFMessage::FullyJarvisFMessage(OutgoingCommandType cmd, uint16_t p01)
    : FullyJarvisFMessage(SourceType::Outgoing, cmd, 2, FIRST_BYTE(p01), SECOND_BYTE(p01)) {}

FullyJarvisFMessage::FullyJarvisFMessage(uint8_t sourceType, uint8_t commandType, size_t paramSize, uint8_t p0,
                                         uint8_t p1, uint8_t p2, uint8_t p3)
    : mSourceId(sourceType), mType(commandType), mParamSize(paramSize) {
  mParams[0] = p0;
  mParams[1] = p1;
  mParams[2] = p2;
  mParams[3] = p3;
}

std::string FullyJarvisFMessage::toString() const {
  uint8_t packet[MAX_PACKET_SIZE];
  construct(packet);
  return array2String(packet, getPacketLength());
}

uint8_t FullyJarvisFMessage::getType() const { return mType; }

std::string FullyJarvisFMessage::getTypeStr() const {
  switch (static_cast<SourceType>(mSourceId)) {
    case SourceType::Incoming: {
      auto it = INCOMING_COMMAND_TYPE_INT_TO_ENUM.find(mType);
      if (it != INCOMING_COMMAND_TYPE_INT_TO_ENUM.end()) {
        return it->second;
      }
      return "unknown";
      break;
    }
    case SourceType::Outgoing: {
      auto it = OUTGOING_COMMAND_TYPE_INT_TO_ENUM.find(mType);
      if (it != OUTGOING_COMMAND_TYPE_INT_TO_ENUM.end()) {
        return it->second;
      }
      return "unknown";
      break;
    }
    default:
      return "unknown";
  }
}

void FullyJarvisFMessage::setType(uint8_t val) { mType = val; }

size_t FullyJarvisFMessage::getPacketLength() const { return 6 + mParamSize; }

SourceType FullyJarvisFMessage::getSourceId() const { return static_cast<SourceType>(mSourceId); }

void FullyJarvisFMessage::setSourceId(SourceType sourceId) { mSourceId = sourceId; }

size_t FullyJarvisFMessage::getParamSize() const { return mParamSize; }

void FullyJarvisFMessage::setParamSize(size_t paramSize) { mParamSize = paramSize; }

uint8_t FullyJarvisFMessage::getParamN(size_t n) const { return mParams[n]; }

bool FullyJarvisFMessage::setParamN(uint8_t value, size_t n) {
  bool isIndexValid = n < MAX_PARAM_SIZE;
  if (isIndexValid) {
    mParams[n] = value;
  }
  return isIndexValid;
}

bool FullyJarvisFMessage::setParam(uint8_t p0) {
  mParamSize = 1;
  return setParamN(p0, 0);
}

bool FullyJarvisFMessage::setParam(uint8_t p0, uint8_t p1) {
  mParamSize = 2;
  return setParamN(p0, 0) && setParamN(p1, 1);
}

bool FullyJarvisFMessage::setParam(uint8_t p0, uint8_t p1, uint8_t p2) {
  mParamSize = 3;
  return setParamN(p0, 0) && setParamN(p1, 1) && setParamN(p2, 2);
}

bool FullyJarvisFMessage::setParam(uint16_t p01) {
  mParamSize = 2;
  return setParam(FIRST_BYTE(p01), SECOND_BYTE(p01));
}

void FullyJarvisFMessage::getParamArray(uint8_t *params, uint8_t &paramSize) const {
  memcpy(params, mParams, mParamSize);
  paramSize = mParamSize;
}

void FullyJarvisFMessage::setParamArray(uint8_t *params, uint8_t paramSize) {
  memcpy(mParams, params, paramSize);
  mParamSize = paramSize;
}

uint8_t FullyJarvisFMessage::computeChecksum(uint8_t command, uint8_t paramSize, const uint8_t *params) {
  uint8_t checksum = (command + paramSize) % 256;
  for (int i = 0; i < paramSize; ++i)
    checksum = (checksum + params[i]) % 256;
  return checksum;
}

void FullyJarvisFMessage::construct(uint8_t *data) const {
  data[0] = data[1] = mSourceId;
  data[2] = mType;
  data[3] = mParamSize;
  data[4 + mParamSize] = computeChecksum(mType, mParamSize, mParams);
  data[5 + mParamSize] = 0x7E;

  memcpy(data + 4, mParams, mParamSize);
}

bool FullyJarvisFMessage::setPacket(uint8_t *data, size_t dataSize) {
  if (!this->verifyPacket(data, dataSize)) {
    return false;
  }

  mSourceId = data[0];
  mType = data[2];
  mParamSize = data[3];

  memcpy(mParams, data + 4, mParamSize);

  return true;
}

bool FullyJarvisFMessage::verifyPacket(uint8_t *data, size_t dataSize) {
  uint8_t paramSize = data[3];
  if (paramSize > MAX_PARAM_SIZE) {
    return false;
  }

  uint8_t params[MAX_PARAM_SIZE];
  memcpy(params, data + 4, paramSize);

  return data[0] == data[1] && (data[0] == SourceType::Outgoing || data[0] == SourceType::Incoming) &&
         dataSize == static_cast<size_t>(6 + paramSize) &&
         data[dataSize - 2] == computeChecksum(data[2], data[3], params) && data[dataSize - 1] == 0x7E;
}

}  // namespace fully_jarvis_f
}  // namespace esphome
