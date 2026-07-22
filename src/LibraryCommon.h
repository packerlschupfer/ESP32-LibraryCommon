/*
 * LibraryCommon.h - part of the ESP32-LibraryCommon library
 *
 * Copyright (C) 2025-2026 packerlschupfer
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

/**
 * @file LibraryCommon.h
 * @brief Main include file for LibraryCommon
 *
 * Include this file to get all common utilities:
 * - ErrorCodes: Unified error codes
 * - Result: Type-safe result type for error handling
 * - Common macros and utilities
 */

#pragma once

#include "ErrorCodes.h"
#include "Result.h"

/**
 * @def RETURN_IF_ERROR(expr)
 * @brief Early return if expression returns an error Result
 *
 * Usage:
 * @code
 * Result<void> doSomething() {
 *     RETURN_IF_ERROR(step1());
 *     RETURN_IF_ERROR(step2());
 *     return Result<void>::ok();
 * }
 * @endcode
 */
#define RETURN_IF_ERROR(expr) \
    do { \
        auto _result = (expr); \
        if (!_result) { \
            return ::common::makeErrorAny(_result.error()); \
        } \
    } while (0)

/**
 * @def RETURN_ERROR_IF(condition, error)
 * @brief Return error if condition is true
 *
 * Usage:
 * @code
 * Result<void> validate(int x) {
 *     RETURN_ERROR_IF(x < 0, ErrorCode::INVALID_PARAMETER);
 *     return Result<void>::ok();
 * }
 * @endcode
 */
#define RETURN_ERROR_IF(condition, error) \
    do { \
        if (condition) { \
            return ::common::makeErrorAny(error); \
        } \
    } while (0)

/**
 * @def ASSIGN_OR_RETURN(var, expr)
 * @brief Assign value from Result or return error
 *
 * Usage:
 * @code
 * Result<int> calculate() {
 *     ASSIGN_OR_RETURN(auto value, getValue());
 *     return Result<int>::ok(value * 2);
 * }
 * @endcode
 */
#define ASSIGN_OR_RETURN(var, expr) \
    auto _temp_result_##__LINE__ = (expr); \
    if (!_temp_result_##__LINE__) { \
        return ::common::makeErrorAny(_temp_result_##__LINE__.error()); \
    } \
    var = std::move(_temp_result_##__LINE__).value()

namespace common {

/**
 * @brief Version information for LibraryCommon
 */
struct Version {
    static constexpr int MAJOR = 1;
    static constexpr int MINOR = 0;
    static constexpr int PATCH = 0;

    static constexpr const char* STRING = "1.0.0";
};

/**
 * @brief Utility class for scope-based cleanup (RAII)
 *
 * Usage:
 * @code
 * void process() {
 *     auto* resource = acquireResource();
 *     ScopeGuard cleanup([resource]() { releaseResource(resource); });
 *
 *     // Do work... cleanup will run at end of scope
 * }
 * @endcode
 */
template<typename F>
class ScopeGuard {
public:
    explicit ScopeGuard(F&& func) noexcept
        : func_(std::forward<F>(func)), active_(true) {}

    ~ScopeGuard() {
        if (active_) {
            func_();
        }
    }

    // Move only
    ScopeGuard(ScopeGuard&& other) noexcept
        : func_(std::move(other.func_)), active_(other.active_) {
        other.dismiss();
    }

    ScopeGuard& operator=(ScopeGuard&&) = delete;
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

    /**
     * @brief Dismiss the guard - cleanup will not run
     */
    void dismiss() noexcept { active_ = false; }

private:
    F func_;
    bool active_;
};

/**
 * @brief Create a scope guard with type deduction
 * @param func Cleanup function to run at scope exit
 * @return ScopeGuard instance
 */
template<typename F>
ScopeGuard<F> makeScopeGuard(F&& func) {
    return ScopeGuard<F>(std::forward<F>(func));
}

/**
 * @brief Non-copyable mixin class
 *
 * Inherit from this to make a class non-copyable:
 * @code
 * class MyClass : private NonCopyable {
 *     // ...
 * };
 * @endcode
 */
class NonCopyable {
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

/**
 * @brief Non-movable mixin class
 *
 * Inherit from this to make a class non-movable and non-copyable:
 * @code
 * class Singleton : private NonMovable {
 *     // ...
 * };
 * @endcode
 */
class NonMovable {
protected:
    NonMovable() = default;
    ~NonMovable() = default;

    NonMovable(const NonMovable&) = delete;
    NonMovable& operator=(const NonMovable&) = delete;
    NonMovable(NonMovable&&) = delete;
    NonMovable& operator=(NonMovable&&) = delete;
};

} // namespace common

/**
 * @def SCOPE_EXIT(...)
 * @brief Create an anonymous scope guard
 *
 * Usage:
 * @code
 * void process() {
 *     auto* resource = acquireResource();
 *     SCOPE_EXIT { releaseResource(resource); };
 *
 *     // Do work...
 * }
 * @endcode
 */
#define SCOPE_EXIT_CAT2(x, y) x##y
#define SCOPE_EXIT_CAT(x, y) SCOPE_EXIT_CAT2(x, y)
#define SCOPE_EXIT \
    auto SCOPE_EXIT_CAT(_scope_guard_, __LINE__) = \
        ::common::makeScopeGuard([&]()
#define SCOPE_EXIT_END )
