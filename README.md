This documentation is also available in **[Spanish](./README.es.md)**.

# 🔄📜 log_buffer
## Overview
`LogBuffer` is a C++ class designed to manage a circular buffer (FIFO) for storing log messages with associated types (ERROR, WARNING, INFO). It is optimized for memory-constrained environments, such as Arduino-based microcontrollers, and provides message storage, retrieval, and management.

Designed as a library for use in Arduino environments.

---

 ## ⚙️ Features

- Circular buffer for efficient storage of log messages.
- Supports three message types: ERROR, WARNING, and INFO.
- Fixed-size message buffer (32 characters per message, adjustable).
- Add, retrieve, and count messages with wrap-around functionality.
- Retrieve messages by index or in FIFO order.
- String representation of message types.

> The circular buffer ensures that older messages are overwritten when the buffer is full, making it suitable for continuous logging in resource-limited systems. The fixed-size message buffer (32 characters) balances memory usage and message detail.

### 📌 Compatible Microcontrollers

Compatible with 8-bit AVR, 32-bit ARM Cortex-M, and Xtensa architectures, as long as they are compatible with the Arduino IDE.

---

## 📦 Installation

Option 1: Copy the files and include:

1. Copy the following files into your Arduino project or C++ embedded project:
   - `log_buffer.h`
   - `log_buffer.cpp`

2. Include the header in your source file:
   ```cpp
   #include "log_buffer.h"
   ```

Option 2: Install the library and include:

1. [Download](https://github.com/ATphonOS/log_buffer/archive/refs/heads/main.zip) the repo.
2. Open Arduino IDE.
3. Go to Sketch > `Include Library` > `Add ZIP libraries`... in the menu.
4. Select the ZIP file from the library.
5. Import the Library:

To use the library in your sketch, go to Sketch > `Include Library`.
Select the installed library from the list to add its `#include` directive to your code.
```cpp
   #include <log_buffer.h>
```

---

## 📚 Class: `LogBuffer`

### Constructor & Destructor

```cpp
LogBuffer(size_t bufferSize);
~LogBuffer();
```

| Method | Description | Parameters | Returns |
|--------|-------------|------------|---------|
| `LogBuffer(size_t bufferSize)` | Constructs a LogBuffer with the specified size. | `bufferSize (size_t)`: number of messages the buffer can hold | — |
| `~LogBuffer()` | Destructor. Frees the memory allocated for the buffer. | None | — |

## 🔓 Public Methods

### Message Management

```cpp
void addMessage(LogMessageType type, const char* message);
bool getMessage(LogMessage& msg);
bool getMessage(LogMessage& msg, size_t index);
size_t countMessages();
```

| Method | Description | Parameters | Returns |
|--------|-------------|------------|---------|
| `void addMessage(LogMessageType type, const char* message)` | Adds a message to the buffer, handling wrap-around if full. | `type (LogMessageType)`: message type (ERROR, WARNING, INFO)<br>`message (const char*)`: message string to store | `void` |
| `bool getMessage(LogMessage& msg)` | Retrieves the next message in FIFO order. | `msg (LogMessage&)`: reference to store the retrieved message | `bool`: `true` if a message was retrieved, `false` if the buffer is empty |
| `bool getMessage(LogMessage& msg, size_t index)` | Retrieves a message by its index. | `msg (LogMessage&)`: reference to store the retrieved message<br>`index (size_t)`: index of the message (0-based) | `bool`: `true` if the index is valid and a message was retrieved, `false` otherwise |
| `size_t countMessages()` | Returns the number of messages currently in the buffer. | None | `size_t`: count of stored messages |

### Helper Function

```cpp
const char* getMessageTypeString(LogMessageType type);
```

| Method | Description | Parameters | Returns |
|--------|-------------|------------|---------|
| `const char* getMessageTypeString(LogMessageType type)` | Converts a log message type to its string representation. | `type (LogMessageType)`: message type (ERROR, WARNING, INFO) | `const char*`: string representation ("ERROR", "WARNING", "INFO", or "UNKNOWN") |

## 🔒 Private Members

```cpp
LogMessage* _buffer;       // Dynamic array storing log messages.
size_t _bufferSize;        // Total size of the buffer (includes wrap-around space).
LogMessage* _start;        // Pointer to the first message in the buffer.
LogMessage* _end;          // Pointer to the next free space in the buffer.
```

---

## 🧪 Example of Use

```cpp
#include "log_buffer.h"

LogBuffer logBuffer(6); // Buffer for 6 messages

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ;
    }
    Serial.println(F("LogBuffer Example"));

    // Add test messages
    logBuffer.addMessage(ERROR, "Sensor error");
    logBuffer.addMessage(WARNING, "Low battery");
    logBuffer.addMessage(INFO, "System on");
    logBuffer.addMessage(INFO, "Normal temperature");
    logBuffer.addMessage(ERROR, "Connection failure");
    logBuffer.addMessage(WARNING, "Memory almost full");

    // Print number of messages
    Serial.print("Number of messages: ");
    Serial.println(logBuffer.countMessages());

    // Display all messages in FIFO order
    Serial.println("Messages in the buffer (FIFO):");
    LogMessage msg;
    while (logBuffer.getMessage(msg)) {
        Serial.print("Type: ");
        Serial.print(getMessageTypeString(msg.type));
        Serial.print(", Message: ");
        Serial.println(msg.message);
    }

    // Add new messages to test index access
    logBuffer.addMessage(INFO, "Message 1");
    logBuffer.addMessage(WARNING, "Message 2");
    logBuffer.addMessage(ERROR, "Message 3");
    logBuffer.addMessage(INFO, "Message 4");
    logBuffer.addMessage(WARNING, "Message 5");
    logBuffer.addMessage(ERROR, "Message 6");

    // Display messages by index
    Serial.println("Messages by index:");
    for (size_t i = 0; i < logBuffer.countMessages(); i++) {
        if (logBuffer.getMessage(msg, i)) {
            Serial.print("Index ");
            Serial.print(i);
            Serial.print(": Type: ");
            Serial.print(getMessageTypeString(msg.type));
            Serial.print(", Message: ");
            Serial.println(msg.message);
        }
    }
}

void loop() {
    // Empty loop
}
```

## 📤 Program Output Example

```
LogBuffer Example
Number of messages: 6
Messages in the buffer (FIFO):
Type: ERROR, Message: Sensor error
Type: WARNING, Message: Low battery
Type: INFO, Message: System on
Type: INFO, Message: Normal temperature
Type: ERROR, Message: Connection failure
Type: WARNING, Message: Memory almost full
Messages by index:
Index 0: Type: INFO, Message: Message 1
Index 1: Type: WARNING, Message: Message 2
Index 2: Type: ERROR, Message: Message 3
Index 3: Type: INFO, Message: Message 4
Index 4: Type: WARNING, Message: Message 5
Index 5: Type: ERROR, Message: Message 6
Type: INFO, Message: Message 1
Type: WARNING, Message: Message 2
Type: ERROR, Message: Message 3
Type: INFO, Message: Message 4
Type: WARNING, Message: Message 5
Type: ERROR, Message: Message 6

```

---

## 🧠 Design Notes
+ Uses a circular buffer to efficiently manage memory and handle message overflow.
+ Fixed-size message buffer (32 characters) ensures predictable memory usage.
+ Safe handling of null or invalid inputs in `addMessage`.
+ Modular arithmetic for wrap-around calculations ensures robust buffer management.
+ Designed for minimal memory footprint, suitable for microcontrollers.

---

## 🧾 License
This project is licensed under the MIT License — see the [LICENSE](https://github.com/ATphonOS/log_buffer/blob/main/LICENSE.md) file for details.
