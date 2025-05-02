#include "log_buffer.h"

// Global LogBuffer Instance
LogBuffer logBuffer(6); // Buffer for 6 messages

void setup() {
  
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }

  // Add test messages
  logBuffer.addMessage(ERROR, "Sensor error");
  logBuffer.addMessage(WARNING, "Low battery");
  logBuffer.addMessage(INFO, "System on");
  logBuffer.addMessage(INFO, "Normal temperature");
  logBuffer.addMessage(ERROR, "Connection failure");
  logBuffer.addMessage(WARNING, "Memory almost full");

  // Number of messages
  Serial.print("Number of messages: ");
  Serial.println(logBuffer.countMessages());

  // Display all messages using getMessage()
  Serial.println("Messages in the buffer (FIFO):");
  LogMessage msg;
  while (logBuffer.getMessage(msg)) {
    Serial.print("Type: ");
    Serial.print(getMessageTypeString(msg.type));
     Serial.print(", Message: ");
    Serial.println(msg.message);
  }

  // Add messages again to test index access
  logBuffer.addMessage(INFO, "Message 1");
  logBuffer.addMessage(WARNING, "Message 2");
  logBuffer.addMessage(ERROR, "Message 3");
  logBuffer.addMessage(INFO, "Message 4");
  logBuffer.addMessage(WARNING, "Message 5");
  logBuffer.addMessage(ERROR, "Message 6");

  // Messages by index
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

   while (logBuffer.getMessage(msg)) {
    Serial.print("Type: ");
    Serial.print(getMessageTypeString(msg.type));
     Serial.print(", Message: ");
    Serial.println(msg.message);
  }
}

void loop() {
}