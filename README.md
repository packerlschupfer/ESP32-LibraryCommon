# LibraryCommon

Common utilities and error handling for embedded C++ projects.

## Features

- **Result<T, E>** type for type-safe error handling without exceptions
- **ErrorCode** enum with common error types
- Zero-cost abstraction when optimized
- Header-only library (no linking required)
- Compatible with embedded systems where exceptions are disabled

## Why Use Result<T>?

Embedded systems often disable C++ exceptions to save memory and ensure deterministic timing. The `Result<T, E>` type provides explicit error handling while maintaining type safety:

```cpp
// Instead of throwing exceptions:
int divide(int a, int b) {
    if (b == 0) throw std::runtime_error("Division by zero");
    return a / b;
}

// Use Result<T>:
Result<int> divide(int a, int b) {
    if (b == 0) {
        return Result<int>::error(ErrorCode::INVALID_PARAMETER);
    }
    return Result<int>::ok(a / b);
}
```

## Installation

### PlatformIO

Add to your `platformio.ini`:

```ini
lib_deps =
    https://github.com/packerlschupfer/LibraryCommon.git#v0.1.0
```

### Arduino IDE

1. Download ZIP from GitHub releases
2. Sketch → Include Library → Add .ZIP Library

## Usage

### Basic Result<T> Usage

```cpp
#include <Result.h>

using common::Result;
using common::ErrorCode;

Result<int> calculate(int value) {
    if (value < 0) {
        return Result<int>::error(ErrorCode::INVALID_PARAMETER);
    }
    return Result<int>::ok(value * 2);
}

void setup() {
    auto result = calculate(10);

    if (result.isOk()) {
        int value = result.value();
        Serial.printf("Success: %d\n", value);
    } else {
        ErrorCode err = result.error();
        Serial.printf("Error: %d\n", static_cast<int>(err));
    }
}
```

### Result<void> for Operations Without Return Values

```cpp
Result<void> initialize() {
    if (!setupHardware()) {
        return Result<void>::error(ErrorCode::INITIALIZATION_FAILED);
    }
    return Result<void>::ok();
}

auto result = initialize();
if (!result.isOk()) {
    // Handle initialization error
}
```

### Custom Error Types

```cpp
enum class MyError {
    SENSOR_TIMEOUT,
    INVALID_DATA,
    COMMUNICATION_FAILED
};

template<typename T>
using MyResult = common::Result<T, MyError>;

MyResult<float> readSensor() {
    // ... sensor code
    if (timeout) {
        return MyResult<float>::error(MyError::SENSOR_TIMEOUT);
    }
    return MyResult<float>::ok(temperature);
}
```

## API Reference

### Result<T, E>

Template class for holding either a success value or an error.

#### Static Factory Methods

- `Result<T, E>::ok(T value)` - Create successful result
- `Result<T, E>::error(E error)` - Create error result

#### Query Methods

- `bool isOk()` - Returns true if result contains a value
- `bool isError()` - Returns true if result contains an error
- `operator bool()` - Implicit conversion to bool (true if Ok)

#### Value Access

- `T& value()` - Get value reference (undefined if error)
- `const T& value() const` - Get const value reference
- `E& error()` - Get error reference (undefined if ok)
- `const E& error() const` - Get const error reference

### ErrorCode Enum

Common error codes used across libraries:

- `SUCCESS` - Operation successful
- `NOT_INITIALIZED` - Component not initialized
- `TIMEOUT` - Operation timed out
- `INVALID_PARAMETER` - Invalid parameter passed
- `OUT_OF_MEMORY` - Memory allocation failed
- `COMMUNICATION_ERROR` - Communication failure
- `INITIALIZATION_FAILED` - Initialization failed
- `OPERATION_FAILED` - Generic operation failure

See `ErrorCodes.h` for complete list.

## Dependencies

None - header-only library with no external dependencies.

## Platform Support

- ESP32 (Arduino/ESP-IDF)
- Any C++11 or later compiler
- FreeRTOS environments
- Embedded systems with exceptions disabled

## Production Use

This library is extracted from the [ESP32 Boiler Controller](https://github.com/packerlschupfer/esp32-boiler-controller) project where it's used extensively across 18+ libraries for type-safe error handling in safety-critical industrial control systems.

## License

MIT License - see [LICENSE](LICENSE) file for details.

## Author

**packerlschupfer**
- GitHub: [@packerlschupfer](https://github.com/packerlschupfer)

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Related Libraries

This library is part of a larger ESP32 library ecosystem:

- [ESP32-TaskManager](https://github.com/packerlschupfer/ESP32-TaskManager) - FreeRTOS task management
- [ESP32-ModbusDevice](https://github.com/packerlschupfer/ESP32-ModbusDevice) - Modbus RTU device framework
- [Logger](https://github.com/packerlschupfer/Logger) - Async thread-safe logging

See the [ESP32 Boiler Controller](https://github.com/packerlschupfer/esp32-boiler-controller) for a complete example using all libraries together.
