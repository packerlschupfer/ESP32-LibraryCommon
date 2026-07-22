# LibraryCommon - CLAUDE.md

## Overview
Shared utilities for the workspace_Class-* library ecosystem. Provides common Result type and error codes used across all libraries.

## Contents

### Result.h
Type-safe result type for error handling without exceptions:
```cpp
#include "Result.h"

common::Result<int, MyError> divide(int a, int b) {
    if (b == 0) {
        return common::Result<int, MyError>::error(MyError::DivideByZero);
    }
    return common::Result<int, MyError>::ok(a / b);
}

auto result = divide(10, 2);
if (result.isOk()) {
    int value = result.value();
} else {
    MyError err = result.error();
}
```

### ErrorCodes.h
Common error codes shared across libraries:
```cpp
namespace common {
enum class ErrorCode {
    SUCCESS,
    NOT_INITIALIZED,
    TIMEOUT,
    INVALID_PARAMETER,
    // ...
};
}
```

## Usage in Libraries
Libraries define their own error enums and use Result:
```cpp
#include "Result.h"

enum class ModbusError { SUCCESS, TIMEOUT, CRC_ERROR };

template<typename T>
using ModbusResult = common::Result<T, ModbusError>;
```

## Libraries Using This
- IDeviceInstance (`DeviceResult<T>`)
- ModbusDevice (`ModbusResult<T>`)
- EthernetManager (`EthResult<T>`)
- MQTTManager (`MQTTResult<T>`)
- RYN4 (`RelayResult<T>`)

## Design Principles
1. No exceptions (disabled on ESP32)
2. Explicit error handling
3. Type-safe value/error union
4. Zero-cost abstraction when optimized
