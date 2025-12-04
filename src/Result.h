/**
 * @file Result.h
 * @brief Type-safe result type for error handling without exceptions
 *
 * Provides a Result<T, E> template that encapsulates either a success value
 * or an error, enabling explicit error handling without exceptions.
 */

#pragma once

#include <utility>
#include <type_traits>
#include "ErrorCodes.h"

namespace common {

/**
 * @brief Tag type for constructing error results
 */
struct ErrorTag {};

/**
 * @brief Tag type for constructing success results
 */
struct SuccessTag {};

/**
 * @brief Inline constexpr tags for Result construction
 */
inline constexpr ErrorTag err_tag{};
inline constexpr SuccessTag ok_tag{};

/**
 * @class Result
 * @brief A type-safe result type that holds either a value or an error
 *
 * @tparam T The success value type (use void for operations with no return value)
 * @tparam E The error type (defaults to common::ErrorCode)
 *
 * Usage:
 * @code
 * Result<int> divide(int a, int b) {
 *     if (b == 0) {
 *         return Result<int>::error(ErrorCode::INVALID_PARAMETER);
 *     }
 *     return Result<int>::ok(a / b);
 * }
 *
 * auto result = divide(10, 2);
 * if (result) {
 *     printf("Result: %d\n", result.value());
 * } else {
 *     printf("Error: %s\n", errorCodeToString(result.error()));
 * }
 * @endcode
 */
template<typename T, typename E = ErrorCode>
class Result {
public:
    using ValueType = T;
    using ErrorType = E;

    /**
     * @brief Construct a success result with a value
     * @param value The success value
     */
    explicit Result(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
        : value_(value), error_(static_cast<E>(0)), hasValue_(true) {}

    /**
     * @brief Construct a success result with a moved value
     * @param value The success value (moved)
     */
    explicit Result(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>)
        : value_(std::move(value)), error_(static_cast<E>(0)), hasValue_(true) {}

    /**
     * @brief Construct an error result
     * @param error The error code
     */
    explicit Result(E error) noexcept
        : value_(), error_(error), hasValue_(false) {}

    /**
     * @brief Construct a success result with tag
     * @param value The success value
     */
    Result(SuccessTag, const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
        : value_(value), error_(static_cast<E>(0)), hasValue_(true) {}

    /**
     * @brief Construct an error result with tag
     * @param error The error code
     */
    Result(ErrorTag, E error) noexcept
        : value_(), error_(error), hasValue_(false) {}

    /**
     * @brief Create a success result
     * @param value The success value
     * @return Result containing the value
     */
    static Result ok(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>) {
        return Result(value);
    }

    /**
     * @brief Create a success result with moved value
     * @param value The success value (moved)
     * @return Result containing the value
     */
    static Result ok(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>) {
        return Result(std::move(value));
    }

    /**
     * @brief Create an error result
     * @param error The error code
     * @return Result containing the error
     */
    static Result error(E error) noexcept {
        return Result(error);
    }

    /**
     * @brief Check if result contains a value (success)
     * @return true if result is successful
     */
    bool isOk() const noexcept { return hasValue_; }

    /**
     * @brief Check if result contains an error
     * @return true if result is an error
     */
    bool isError() const noexcept { return !hasValue_; }

    /**
     * @brief Boolean conversion - true if successful
     */
    explicit operator bool() const noexcept { return hasValue_; }

    /**
     * @brief Get the success value
     * @return Reference to the value
     * @warning Undefined behavior if result is an error
     */
    T& value() & noexcept { return value_; }

    /**
     * @brief Get the success value (const)
     * @return Const reference to the value
     * @warning Undefined behavior if result is an error
     */
    const T& value() const& noexcept { return value_; }

    /**
     * @brief Get the success value (rvalue)
     * @return Rvalue reference to the value
     * @warning Undefined behavior if result is an error
     */
    T&& value() && noexcept { return std::move(value_); }

    /**
     * @brief Get the error code
     * @return The error code
     * @warning Undefined behavior if result is successful
     */
    E error() const noexcept { return error_; }

    /**
     * @brief Get value or default if error
     * @param defaultValue Value to return if result is an error
     * @return The value if successful, defaultValue otherwise
     */
    T valueOr(const T& defaultValue) const& noexcept(std::is_nothrow_copy_constructible_v<T>) {
        return hasValue_ ? value_ : defaultValue;
    }

    /**
     * @brief Get value or default if error (move version)
     * @param defaultValue Value to return if result is an error
     * @return The value if successful, defaultValue otherwise
     */
    T valueOr(T&& defaultValue) && noexcept(std::is_nothrow_move_constructible_v<T>) {
        return hasValue_ ? std::move(value_) : std::move(defaultValue);
    }

    /**
     * @brief Map the value if successful
     * @tparam F Function type
     * @param f Function to apply to value
     * @return Result with mapped value or original error
     */
    template<typename F>
    auto map(F&& f) const -> Result<decltype(f(std::declval<T>())), E> {
        using ResultType = Result<decltype(f(std::declval<T>())), E>;
        if (hasValue_) {
            return ResultType::ok(f(value_));
        }
        return ResultType::error(error_);
    }

    /**
     * @brief Apply function if successful, return error otherwise
     * @tparam F Function type returning Result
     * @param f Function to apply
     * @return Result from function or original error
     */
    template<typename F>
    auto andThen(F&& f) const -> decltype(f(std::declval<T>())) {
        using ResultType = decltype(f(std::declval<T>()));
        if (hasValue_) {
            return f(value_);
        }
        return ResultType::error(error_);
    }

private:
    T value_;
    E error_;
    bool hasValue_;
};

/**
 * @brief Specialization of Result for void (operations with no return value)
 */
template<typename E>
class Result<void, E> {
public:
    using ValueType = void;
    using ErrorType = E;

    /**
     * @brief Construct a success result
     */
    Result() noexcept : error_(static_cast<E>(0)), hasValue_(true) {}

    /**
     * @brief Construct an error result
     * @param error The error code
     */
    explicit Result(E error) noexcept : error_(error), hasValue_(false) {}

    /**
     * @brief Construct a success result with tag
     */
    explicit Result(SuccessTag) noexcept : error_(static_cast<E>(0)), hasValue_(true) {}

    /**
     * @brief Construct an error result with tag
     * @param error The error code
     */
    Result(ErrorTag, E error) noexcept : error_(error), hasValue_(false) {}

    /**
     * @brief Create a success result
     * @return Successful Result
     */
    static Result ok() noexcept {
        return Result();
    }

    /**
     * @brief Create an error result
     * @param error The error code
     * @return Result containing the error
     */
    static Result error(E error) noexcept {
        return Result(error);
    }

    /**
     * @brief Check if result is successful
     * @return true if successful
     */
    bool isOk() const noexcept { return hasValue_; }

    /**
     * @brief Check if result is an error
     * @return true if error
     */
    bool isError() const noexcept { return !hasValue_; }

    /**
     * @brief Boolean conversion - true if successful
     */
    explicit operator bool() const noexcept { return hasValue_; }

    /**
     * @brief Get the error code
     * @return The error code
     */
    E error() const noexcept { return error_; }

private:
    E error_;
    bool hasValue_;
};

/**
 * @brief Helper function to create a success Result
 * @tparam T Value type
 * @tparam E Error type
 * @param value The success value
 * @return Result containing the value
 */
template<typename T, typename E = ErrorCode>
Result<T, E> makeOk(T&& value) {
    return Result<T, E>::ok(std::forward<T>(value));
}

/**
 * @brief Helper function to create a void success Result
 * @tparam E Error type
 * @return Successful void Result
 */
template<typename E = ErrorCode>
Result<void, E> makeOk() {
    return Result<void, E>::ok();
}

/**
 * @brief Helper function to create an error Result
 * @tparam T Value type
 * @tparam E Error type
 * @param error The error code
 * @return Result containing the error
 */
template<typename T, typename E = ErrorCode>
Result<T, E> makeError(E error) {
    return Result<T, E>::error(error);
}

} // namespace common
