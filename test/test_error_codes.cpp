/**
 * @file test_error_codes.cpp
 * @brief Unit tests for common::ErrorCode
 */

#ifdef UNIT_TEST

#include <unity.h>
#include "../src/ErrorCodes.h"

using namespace common;

void test_error_code_success_check() {
    TEST_ASSERT_TRUE(isSuccess(ErrorCode::OK));
    TEST_ASSERT_TRUE(isSuccess(ErrorCode::SUCCESS));
    TEST_ASSERT_FALSE(isSuccess(ErrorCode::TIMEOUT));
}

void test_error_code_failure_check() {
    TEST_ASSERT_FALSE(isError(ErrorCode::OK));
    TEST_ASSERT_TRUE(isError(ErrorCode::TIMEOUT));
    TEST_ASSERT_TRUE(isError(ErrorCode::NOT_INITIALIZED));
}

void test_error_code_to_string() {
    TEST_ASSERT_EQUAL_STRING("OK", errorCodeToString(ErrorCode::OK));
    TEST_ASSERT_EQUAL_STRING("Timeout", errorCodeToString(ErrorCode::TIMEOUT));
    TEST_ASSERT_EQUAL_STRING("Not initialized", errorCodeToString(ErrorCode::NOT_INITIALIZED));
    TEST_ASSERT_EQUAL_STRING("Invalid parameter", errorCodeToString(ErrorCode::INVALID_PARAMETER));
    TEST_ASSERT_EQUAL_STRING("Out of memory", errorCodeToString(ErrorCode::OUT_OF_MEMORY));
    TEST_ASSERT_EQUAL_STRING("CRC error", errorCodeToString(ErrorCode::CRC_ERROR));
    TEST_ASSERT_EQUAL_STRING("Device not found", errorCodeToString(ErrorCode::DEVICE_NOT_FOUND));
    TEST_ASSERT_EQUAL_STRING("Connection failed", errorCodeToString(ErrorCode::CONNECTION_FAILED));
    TEST_ASSERT_EQUAL_STRING("Storage full", errorCodeToString(ErrorCode::STORAGE_FULL));
    TEST_ASSERT_EQUAL_STRING("Network error", errorCodeToString(ErrorCode::NETWORK_ERROR));
}

void test_error_code_ranges() {
    // General errors: 1-99
    TEST_ASSERT_LESS_THAN(100, static_cast<int>(ErrorCode::UNKNOWN_ERROR));
    TEST_ASSERT_LESS_THAN(100, static_cast<int>(ErrorCode::TIMEOUT));

    // Device errors: 100-199
    TEST_ASSERT_GREATER_OR_EQUAL(100, static_cast<int>(ErrorCode::DEVICE_NOT_FOUND));
    TEST_ASSERT_LESS_THAN(200, static_cast<int>(ErrorCode::HARDWARE_FAILURE));

    // Communication errors: 200-299
    TEST_ASSERT_GREATER_OR_EQUAL(200, static_cast<int>(ErrorCode::COMMUNICATION_ERROR));
    TEST_ASSERT_LESS_THAN(300, static_cast<int>(ErrorCode::RECEIVE_FAILED));

    // Storage errors: 300-399
    TEST_ASSERT_GREATER_OR_EQUAL(300, static_cast<int>(ErrorCode::STORAGE_ERROR));
    TEST_ASSERT_LESS_THAN(400, static_cast<int>(ErrorCode::MOUNT_FAILED));

    // Network errors: 400-499
    TEST_ASSERT_GREATER_OR_EQUAL(400, static_cast<int>(ErrorCode::NETWORK_ERROR));
    TEST_ASSERT_LESS_THAN(500, static_cast<int>(ErrorCode::SSL_ERROR));
}

void test_error_code_constexpr() {
    // These should be evaluated at compile time
    constexpr bool okIsSuccess = isSuccess(ErrorCode::OK);
    constexpr bool timeoutIsError = isError(ErrorCode::TIMEOUT);

    TEST_ASSERT_TRUE(okIsSuccess);
    TEST_ASSERT_TRUE(timeoutIsError);
}

void test_error_code_unknown() {
    // Test unknown error code returns fallback string
    auto unknown = static_cast<ErrorCode>(9999);
    TEST_ASSERT_EQUAL_STRING("Unknown error code", errorCodeToString(unknown));
}

// Test runner
void runErrorCodeTests() {
    UNITY_BEGIN();

    RUN_TEST(test_error_code_success_check);
    RUN_TEST(test_error_code_failure_check);
    RUN_TEST(test_error_code_to_string);
    RUN_TEST(test_error_code_ranges);
    RUN_TEST(test_error_code_constexpr);
    RUN_TEST(test_error_code_unknown);

    UNITY_END();
}

#ifdef ARDUINO
#include <Arduino.h>

void setup() {
    delay(2000);
    Serial.begin(115200);
    Serial.println("\n=== LibraryCommon ErrorCode Tests ===\n");
    runErrorCodeTests();
}

void loop() {}
#else
int main() {
    runErrorCodeTests();
    return 0;
}
#endif

#endif // UNIT_TEST
