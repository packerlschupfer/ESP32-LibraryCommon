/**
 * SmokeTest - minimal compile-smoke example for ESP32-LibraryCommon.
 *
 * Exercises the primary public API (LibraryCommon.h -> Result.h + ErrorCodes.h):
 *  - common::Result<T> value + error construction and inspection
 *  - common::Result<void> success/error
 *  - errorCodeToString()
 *  - RETURN_IF_ERROR / RETURN_ERROR_IF macros
 *  - common::makeScopeGuard (RAII)
 * Enough usage to force real template instantiation, compilation and linking.
 */
#include <Arduino.h>
#include <LibraryCommon.h>

using common::ErrorCode;
using common::Result;

// Returns an error or a value depending on the divisor.
static Result<int> divide(int a, int b) {
    RETURN_ERROR_IF(b == 0, ErrorCode::INVALID_PARAMETER);
    return Result<int>::ok(a / b);
}

// Uses RETURN_IF_ERROR to propagate a void Result error.
static Result<void> validate(int x) {
    RETURN_ERROR_IF(x < 0, ErrorCode::INVALID_STATE);
    return Result<void>::ok();
}

void setup() {
    Serial.begin(115200);

    auto good = divide(10, 2);
    auto bad = divide(1, 0);

    if (good) {
        Serial.printf("10/2 = %d\n", good.value());
    }
    if (!bad) {
        Serial.printf("divide error: %s\n", common::errorCodeToString(bad.error()));
    }

    // valueOr + map exercise the templated members.
    int safe = divide(8, 0).valueOr(-1);
    auto mapped = good.map([](int v) { return v + 1; });
    Serial.printf("safe=%d mapped=%d\n", safe, mapped.valueOr(0));

    Result<void> v = validate(-5);
    Serial.printf("validate(-5) ok=%d code=%d\n",
                  static_cast<int>(v.isOk()), static_cast<int>(v.error()));

    // RAII scope guard.
    bool cleaned = false;
    {
        auto guard = common::makeScopeGuard([&]() { cleaned = true; });
        (void)guard;
    }
    Serial.printf("cleaned=%d\n", static_cast<int>(cleaned));
}

void loop() {
    delay(1000);
}
