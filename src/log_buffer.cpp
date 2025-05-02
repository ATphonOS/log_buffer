#include "log_buffer.h"

const char* messageTypeStrings[] = {
  "ERROR",
  "WARNING",
  "INFO"
};

LogBuffer::LogBuffer(size_t bufferSize)
  : _bufferSize(bufferSize + 1) {
  _buffer = new LogMessage[_bufferSize];
  if (_buffer == nullptr) {
    // Not enough memory, initialize in safe state
    _bufferSize = 0;
    _start = nullptr;
    _end = nullptr;
    return;
  }
  _start = _buffer;
  _end = _buffer;
}

LogBuffer::~LogBuffer() {
  delete[] _buffer;
}

void LogBuffer::addMessage(LogMessageType type, const char* message) {
  if (_bufferSize == 0) return; //Uninitialized buffer
  _end->type = type;
  // Copy the string, truncating if necessary
  strncpy(_end->message, message ? message : "", sizeof(_end->message) - 1);
  _end->message[sizeof(_end->message) - 1] = '\0'; // Ensure completion
  _end++;
  if (_end == _buffer + _bufferSize) {
    _end = _buffer; // Wrap-around
  }
  if (_end == _start) {
    _start++;
    if (_start == _buffer + _bufferSize) {
      _start = _buffer; // Wrap-around
    }
  }
}

bool LogBuffer::getMessage(LogMessage& msg) {
  if (_bufferSize == 0 || _start == _end) {
    return false;
  }
  msg = *_start;
  _start++;
  if (_start == _buffer + _bufferSize) {
    _start = _buffer; // Wrap-around
  }
  return true;
}

bool LogBuffer::getMessage(LogMessage& msg, size_t index) {
  if (_bufferSize == 0 || index >= countMessages()) {
    return false;
  }
  // Calculate position with modular arithmetic
  LogMessage* target = _start + index;
  if (target >= _buffer + _bufferSize) {
    target -= _bufferSize; // Wrap-around
  }
  msg = *target;
  return true;
}

size_t LogBuffer::countMessages() {
  if (_bufferSize == 0) return 0;
  if (_start <= _end) {
    return _end - _start;
  } else {
    return (_buffer + _bufferSize - _start) + (_end - _buffer);
  }
}

const char* getMessageTypeString(LogMessageType type) {
  if (type >= 0 && type < MESSAGE_TYPE_COUNT) {
    return messageTypeStrings[type];
  }
  return "UNKNOWN";
}