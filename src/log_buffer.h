/**
 * @file LogBuffer.h
 * @brief Header file for the LogBuffer class, managing a circular buffer for log messages.
 * @details This class provides a dynamic circular buffer (FIFO) to store log messages with types
 * (ERROR, WARNING, INFO), allowing addition, retrieval, and counting of messages.
 * @version 1.0.0
 * @author ATphonOS
 * @date 2025
 * @license MIT
 */

#ifndef LOG_BUFFER_H
#define LOG_BUFFER_H

#include <Arduino.h>

/**
 * @brief Enumerates the types of log messages.
 */
enum LogMessageType {
  ERROR,              ///< Error message type.
  WARNING,            ///< Warning message type.
  INFO,               ///< Information message type.
  MESSAGE_TYPE_COUNT  ///< Number of valid message types (for validation).
};

/**
 * @brief Structure representing a single log message.
 */
struct LogMessage {
  LogMessageType type;      ///< Type of the log message (ERROR, WARNING, INFO).
  char message[32];         ///< Fixed-size buffer for the message string (adjustable).
};

/**
 * @brief Manages a circular buffer for storing log messages.
 * @details Provides methods to add, retrieve, and count log messages in a dynamic
 * buffer with wrap-around functionality.
 */
class LogBuffer {
private:
  LogMessage* _buffer;       ///< Dynamic array storing log messages.
  size_t _bufferSize;        ///< Total size of the buffer (includes wrap-around space).
  LogMessage* _start;        ///< Pointer to the first message in the buffer.
  LogMessage* _end;          ///< Pointer to the next free space in the buffer.

public:
  /**
   * @brief Constructs a LogBuffer with the specified size.
   * @param bufferSize The number of messages the buffer can hold.
   * @details Allocates memory for the buffer and initializes pointers.
   */
  LogBuffer(size_t bufferSize);

  /**
   * @brief Destructor for LogBuffer.
   * @details Frees the memory allocated for the buffer.
   */
  ~LogBuffer();

  /**
   * @brief Adds a message to the buffer.
   * @param type The type of the message (ERROR, WARNING, INFO).
   * @param message The message string to store (copied to internal buffer).
   * @details Copies the message into the buffer, handling wrap-around if full.
   */
  void addMessage(LogMessageType type, const char* message);

  /**
   * @brief Retrieves the next message from the buffer.
   * @param msg Reference to a LogMessage struct to store the retrieved message.
   * @return True if a message was retrieved, false if the buffer is empty.
   */
  bool getMessage(LogMessage& msg);

  /**
   * @brief Retrieves a message by index.
   * @param msg Reference to a LogMessage struct to store the retrieved message.
   * @param index The index of the message to retrieve (0-based).
   * @return True if the index is valid and a message was retrieved, false otherwise.
   */
  bool getMessage(LogMessage& msg, size_t index);

  /**
   * @brief Returns the number of messages currently in the buffer.
   * @return The count of stored messages.
   */
  size_t countMessages();
};

/**
 * @brief Converts a log message type to its string representation.
 * @param type The log message type (ERROR, WARNING, INFO).
 * @return A const char* pointing to the string representation of the type.
 * @details Returns "ERROR", "WARNING", "INFO", or "UNKNOWN" for invalid types.
 */
const char* getMessageTypeString(LogMessageType type);

#endif // LOG_BUFFER_H