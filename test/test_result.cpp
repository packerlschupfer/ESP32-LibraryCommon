/**
 * @file test_result.cpp
 * @brief Unit tests for common::Result type
 */

#ifdef UNIT_TEST

#include <unity.h>
#include "../src/Result.h"

using namespace common;

// Test custom error type
enum class TestError {
    NONE = 0,
    DIVIDE_BY_ZERO,
    OVERFLOW,
    UNDERFLOW
};

void test_result_ok_creation() {
    auto result = Result<int>::ok(42);

    TEST_ASSERT_TRUE(result.isOk());
    TEST_ASSERT_FALSE(result.isError());
    TEST_ASSERT_EQUAL(42, result.value());
}

void test_result_error_creation() {
    auto result = Result<int>::error(ErrorCode::INVALID_PARAMETER);

    TEST_ASSERT_FALSE(result.isOk());
    TEST_ASSERT_TRUE(result.isError());
    TEST_ASSERT_EQUAL(ErrorCode::INVALID_PARAMETER, result.error());
}

void test_result_bool_conversion() {
    auto success = Result<int>::ok(1);
    auto failure = Result<int>::error(ErrorCode::TIMEOUT);

    TEST_ASSERT_TRUE(static_cast<bool>(success));
    TEST_ASSERT_FALSE(static_cast<bool>(failure));
}

void test_result_value_or() {
    auto success = Result<int>::ok(42);
    auto failure = Result<int>::error(ErrorCode::NOT_INITIALIZED);

    TEST_ASSERT_EQUAL(42, success.valueOr(0));
    TEST_ASSERT_EQUAL(0, failure.valueOr(0));
}

void test_result_void_ok() {
    auto result = Result<void>::ok();

    TEST_ASSERT_TRUE(result.isOk());
    TEST_ASSERT_FALSE(result.isError());
}

void test_result_void_error() {
    auto result = Result<void>::error(ErrorCode::IO_ERROR);

    TEST_ASSERT_FALSE(result.isOk());
    TEST_ASSERT_TRUE(result.isError());
    TEST_ASSERT_EQUAL(ErrorCode::IO_ERROR, result.error());
}

void test_result_custom_error_type() {
    auto result = Result<int, TestError>::error(TestError::DIVIDE_BY_ZERO);

    TEST_ASSERT_TRUE(result.isError());
    TEST_ASSERT_EQUAL(TestError::DIVIDE_BY_ZERO, result.error());
}

void test_result_map() {
    auto result = Result<int>::ok(21);

    auto doubled = result.map([](int x) { return x * 2; });

    TEST_ASSERT_TRUE(doubled.isOk());
    TEST_ASSERT_EQUAL(42, doubled.value());
}

void test_result_map_error_passthrough() {
    auto result = Result<int>::error(ErrorCode::BUSY);

    auto doubled = result.map([](int x) { return x * 2; });

    TEST_ASSERT_TRUE(doubled.isError());
    TEST_ASSERT_EQUAL(ErrorCode::BUSY, doubled.error());
}

void test_result_and_then() {
    auto divide = [](int x) -> Result<int> {
        if (x == 0) return Result<int>::error(ErrorCode::INVALID_PARAMETER);
        return Result<int>::ok(100 / x);
    };

    auto result = Result<int>::ok(5);
    auto divided = result.andThen(divide);

    TEST_ASSERT_TRUE(divided.isOk());
    TEST_ASSERT_EQUAL(20, divided.value());
}

void test_result_and_then_error_passthrough() {
    auto divide = [](int x) -> Result<int> {
        return Result<int>::ok(100 / x);
    };

    auto result = Result<int>::error(ErrorCode::TIMEOUT);
    auto divided = result.andThen(divide);

    TEST_ASSERT_TRUE(divided.isError());
    TEST_ASSERT_EQUAL(ErrorCode::TIMEOUT, divided.error());
}

void test_make_ok_helper() {
    auto result = makeOk<int>(42);

    TEST_ASSERT_TRUE(result.isOk());
    TEST_ASSERT_EQUAL(42, result.value());
}

void test_make_error_helper() {
    auto result = makeError<int>(ErrorCode::CRC_ERROR);

    TEST_ASSERT_TRUE(result.isError());
    TEST_ASSERT_EQUAL(ErrorCode::CRC_ERROR, result.error());
}

void test_make_void_ok_helper() {
    auto result = makeOk<>();

    TEST_ASSERT_TRUE(result.isOk());
}

// Test runner
void runResultTests() {
    UNITY_BEGIN();

    RUN_TEST(test_result_ok_creation);
    RUN_TEST(test_result_error_creation);
    RUN_TEST(test_result_bool_conversion);
    RUN_TEST(test_result_value_or);
    RUN_TEST(test_result_void_ok);
    RUN_TEST(test_result_void_error);
    RUN_TEST(test_result_custom_error_type);
    RUN_TEST(test_result_map);
    RUN_TEST(test_result_map_error_passthrough);
    RUN_TEST(test_result_and_then);
    RUN_TEST(test_result_and_then_error_passthrough);
    RUN_TEST(test_make_ok_helper);
    RUN_TEST(test_make_error_helper);
    RUN_TEST(test_make_void_ok_helper);

    UNITY_END();
}

#ifdef ARDUINO
#include <Arduino.h>

void setup() {
    delay(2000);
    Serial.begin(115200);
    Serial.println("\n=== LibraryCommon Result Tests ===\n");
    runResultTests();
}

void loop() {}
#else
int main() {
    runResultTests();
    return 0;
}
#endif

#endif // UNIT_TEST
