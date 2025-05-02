Esta documentación también está disponible en **[Inglés](./README.md)**.

# 🔄📜 LogBuffer
## Resumen
`LogBuffer` es una clase C++  diseñada para gestionar un búfer circular (FIFO) que almacena mensajes de registro con tipos asociados (ERROR, WARNING, INFO). Para entornos con recursos limitados, como microcontroladores basados en Arduino, ofreciendo almacenamiento, recuperación y gestión de mensajes.

Diseñada como biblioteca para entornos Arduino.

---

## ⚙️ Características

- Búfer circular para un almacenamiento eficiente de mensajes de registro.
- Soporta tres tipos de mensajes: ERROR, WARNING e INFO.
- Búfer de mensajes de tamaño fijo (32 caracteres por mensaje, ajustable).
- Funcionalidad para añadir, recuperar y contar mensajes con envoltura (wrap-around).
- Recuperación de mensajes por índice o en orden FIFO.
- Representación en cadena de los tipos de mensajes.

> El búfer circular asegura que los mensajes más antiguos se sobrescriban cuando el búfer está lleno, para registros continuos en sistemas con recursos limitados. Búfer de mensajes de tamaño fijo (32 caracteres). 

### 📌 Microcontroladores Compatibles

Compatible con arquitecturas AVR de 8 bits, ARM Cortex-M de 32 bits y Xtensa, siempre que sean compatibles con el IDE de Arduino.

---

## 📦 Instalación

Opción 1: Copiar los archivos e incluirlos:

1. Copia los siguientes archivos en tu proyecto Arduino o proyecto C++ embebido:
   - `log_buffer.h`
   - `log_buffer.cpp`

2. Incluye el encabezado en tu archivo fuente:
   ```cpp
   #include "log_buffer.h"
   ```

Opción 2: Instalar la biblioteca e incluirla:

1. [Descarga](https://github.com/ATphonOS/log_buffer/archive/refs/heads/main.zip) el repositorio.
2. Abre el IDE de Arduino.
3. Ve a Sketch > `Incluir Biblioteca` > `Añadir biblioteca ZIP`... en el menú.
4. Selecciona el archivo ZIP de la biblioteca.
5. Importa la Biblioteca:

Para usar la biblioteca en tu sketch, ve a Sketch > `Incluir Biblioteca`.
Selecciona la biblioteca instalada de la lista para añadir su directiva `#include` a tu código.
```cpp
   #include <log_buffer.h>
```

---

## 📚 Clase: `LogBuffer`

### Constructor y Destructor

```cpp
LogBuffer(size_t bufferSize);
~LogBuffer();
```

| Método | Descripción | Parámetros | Retorno |
|--------|-------------|------------|---------|
| `LogBuffer(size_t bufferSize)` | Construye un LogBuffer con el tamaño especificado. | `bufferSize (size_t)`: número de mensajes que puede contener el búfer | — |
| `~LogBuffer()` | Destructor. Libera la memoria asignada para el búfer. | Ninguno | — |

## 🔓 Métodos Públicos

### Gestión de Mensajes

```cpp
void addMessage(LogMessageType type, const char* message);
bool getMessage(LogMessage& msg);
bool getMessage(LogMessage& msg, size_t index);
size_t countMessages();
```

| Método | Descripción | Parámetros | Retorno |
|--------|-------------|------------|---------|
| `void addMessage(LogMessageType type, const char* message)` | Añade un mensaje al búfer, gestionando la envoltura si está lleno. | `type (LogMessageType)`: tipo de mensaje (ERROR, WARNING, INFO)<br>`message (const char*)`: cadena del mensaje a almacenar | `void` |
| `bool getMessage(LogMessage& msg)` | Recupera el siguiente mensaje en orden FIFO. | `msg (LogMessage&)`: referencia para almacenar el mensaje recuperado | `bool`: `true` si se recuperó un mensaje, `false` si el búfer está vacío |
| `bool getMessage(LogMessage& msg, size_t index)` | Recupera un mensaje por su índice. | `msg (LogMessage&)`: referencia para almacenar el mensaje recuperado<br>`index (size_t)`: índice del mensaje (basado en 0) | `bool`: `true` si el índice es válido y se recuperó un mensaje, `false` en caso contrario |
| `size_t countMessages()` | Devuelve el número de mensajes actualmente en el búfer. | Ninguno | `size_t`: cantidad de mensajes almacenados |

### Función Auxiliar

```cpp
const char* getMessageTypeString(LogMessageType type);
```

| Método | Descripción | Parámetros | Retorno |
|--------|-------------|------------|---------|
| `const char* getMessageTypeString(LogMessageType type)` | Convierte un tipo de mensaje de registro en su representación en cadena. | `type (LogMessageType)`: tipo de mensaje (ERROR, WARNING, INFO) | `const char*`: representación en cadena ("ERROR", "WARNING", "INFO" o "UNKNOWN") |

## 🔒 Miembros Privados

```cpp
LogMessage* _buffer;       // Array dinámico que almacena los mensajes de registro.
size_t _bufferSize;        // Tamaño total del búfer (incluye espacio para envoltura).
LogMessage* _start;        // Puntero al primer mensaje en el búfer.
LogMessage* _end;          // Puntero al siguiente espacio libre en el búfer.
```

---

## 🧪 Ejemplo de Uso

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

## 📤 Ejemplo de Salida del Programa

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

## 🧠 Notas de Diseño
+ Utiliza un búfer circular.
+ El búfer de mensajes de tamaño fijo (32 caracteres) asegura un uso de memoria predecible.
+ Manejo seguro de entradas nulas o inválidas en `addMessage`.
+ Aritmética modular para cálculos de envoltura asegura una gestión robusta del búfer.
+ Diseñado para una huella de memoria mínima, adecuado para microcontroladores.

---

## 🧾 Licencia
Este proyecto está licenciado bajo la Licencia MIT — consulta el archivo [LICENSE](https://github.com/ATphonOS/log_buffer/blob/main/LICENSE.md) para más detalles.
