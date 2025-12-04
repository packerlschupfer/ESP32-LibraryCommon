/**
 * @file ErrorCodes.h
 * @brief Unified error codes for ESP32 PlatformIO libraries
 *
 * This file provides a standardized set of error codes that can be used
 * across all libraries in the workspace. Libraries can extend these with
 * domain-specific errors while maintaining a common base.
 */

#pragma once

#include <cstdint>

namespace common {

/**
 * @enum ErrorCode
 * @brief Base error codes common to all libraries
 *
 * These error codes cover general failure modes. Libraries should
 * use these where applicable and create domain-specific enums
 * for specialized error conditions.
 *
 * Error code ranges:
 * - 0: Success
 * - 1-99: Common errors (defined here)
 * - 100-199: Device/hardware errors
 * - 200-299: Communication errors
 * - 300-399: Storage errors
 * - 400-499: Network errors
 * - 500+: Library-specific errors
 */
enum class ErrorCode : int16_t {
    // Success
    OK = 0,                         ///< Operation completed successfully
    SUCCESS = 0,                    ///< Alias for OK

    // General errors (1-99)
    UNKNOWN_ERROR = 1,              ///< Unknown or unspecified error
    NOT_INITIALIZED = 2,            ///< Component not initialized
    ALREADY_INITIALIZED = 3,        ///< Component already initialized
    INVALID_PARAMETER = 4,          ///< Invalid parameter provided
    INVALID_STATE = 5,              ///< Invalid state for operation
    NOT_SUPPORTED = 6,              ///< Operation not supported
    NOT_IMPLEMENTED = 7,            ///< Feature not implemented
    BUSY = 8,                       ///< Resource is busy
    WOULD_BLOCK = 9,                ///< Operation would block

    // Resource errors (10-29)
    OUT_OF_MEMORY = 10,             ///< Memory allocation failed
    RESOURCE_EXHAUSTED = 11,        ///< Resource limit reached
    RESOURCE_NOT_FOUND = 12,        ///< Requested resource not found
    RESOURCE_LOCKED = 13,           ///< Resource is locked
    RESOURCE_UNAVAILABLE = 14,      ///< Resource temporarily unavailable

    // Synchronization errors (30-49)
    TIMEOUT = 30,                   ///< Operation timed out
    MUTEX_ERROR = 31,               ///< Mutex operation failed
    SEMAPHORE_ERROR = 32,           ///< Semaphore operation failed
    DEADLOCK_DETECTED = 33,         ///< Potential deadlock detected
    QUEUE_FULL = 34,                ///< Queue is full
    QUEUE_EMPTY = 35,               ///< Queue is empty

    // I/O errors (50-69)
    IO_ERROR = 50,                  ///< General I/O error
    READ_ERROR = 51,                ///< Read operation failed
    WRITE_ERROR = 52,               ///< Write operation failed
    PERMISSION_DENIED = 53,         ///< Permission denied

    // Data errors (70-99)
    DATA_NOT_READY = 70,            ///< Data not yet available
    DATA_CORRUPTED = 71,            ///< Data corruption detected
    CRC_ERROR = 72,                 ///< CRC check failed
    CHECKSUM_ERROR = 73,            ///< Checksum verification failed
    BUFFER_OVERFLOW = 74,           ///< Buffer overflow
    BUFFER_UNDERFLOW = 75,          ///< Buffer underflow
    INVALID_DATA = 76,              ///< Invalid data format

    // Device/hardware errors (100-199)
    DEVICE_NOT_FOUND = 100,         ///< Device not found
    DEVICE_ERROR = 101,             ///< General device error
    DEVICE_BUSY = 102,              ///< Device is busy
    DEVICE_DISCONNECTED = 103,      ///< Device disconnected
    HARDWARE_FAILURE = 104,         ///< Hardware failure detected

    // Communication errors (200-299)
    COMMUNICATION_ERROR = 200,      ///< General communication error
    CONNECTION_FAILED = 201,        ///< Connection attempt failed
    CONNECTION_LOST = 202,          ///< Connection lost
    CONNECTION_REFUSED = 203,       ///< Connection refused
    PROTOCOL_ERROR = 204,           ///< Protocol error
    SEND_FAILED = 205,              ///< Send operation failed
    RECEIVE_FAILED = 206,           ///< Receive operation failed

    // Storage errors (300-399)
    STORAGE_ERROR = 300,            ///< General storage error
    STORAGE_FULL = 301,             ///< Storage is full
    FILE_NOT_FOUND = 302,           ///< File not found
    FILE_EXISTS = 303,              ///< File already exists
    MOUNT_FAILED = 304,             ///< Mount operation failed

    // Network errors (400-499)
    NETWORK_ERROR = 400,            ///< General network error
    NETWORK_UNREACHABLE = 401,      ///< Network unreachable
    HOST_UNREACHABLE = 402,         ///< Host unreachable
    DNS_FAILED = 403,               ///< DNS resolution failed
    SSL_ERROR = 404,                ///< SSL/TLS error
};

/**
 * @brief Convert error code to string representation
 * @param code The error code to convert
 * @return String representation of the error code
 */
inline const char* errorCodeToString(ErrorCode code) {
    switch (code) {
        case ErrorCode::OK: return "OK";
        case ErrorCode::UNKNOWN_ERROR: return "Unknown error";
        case ErrorCode::NOT_INITIALIZED: return "Not initialized";
        case ErrorCode::ALREADY_INITIALIZED: return "Already initialized";
        case ErrorCode::INVALID_PARAMETER: return "Invalid parameter";
        case ErrorCode::INVALID_STATE: return "Invalid state";
        case ErrorCode::NOT_SUPPORTED: return "Not supported";
        case ErrorCode::NOT_IMPLEMENTED: return "Not implemented";
        case ErrorCode::BUSY: return "Busy";
        case ErrorCode::WOULD_BLOCK: return "Would block";
        case ErrorCode::OUT_OF_MEMORY: return "Out of memory";
        case ErrorCode::RESOURCE_EXHAUSTED: return "Resource exhausted";
        case ErrorCode::RESOURCE_NOT_FOUND: return "Resource not found";
        case ErrorCode::RESOURCE_LOCKED: return "Resource locked";
        case ErrorCode::RESOURCE_UNAVAILABLE: return "Resource unavailable";
        case ErrorCode::TIMEOUT: return "Timeout";
        case ErrorCode::MUTEX_ERROR: return "Mutex error";
        case ErrorCode::SEMAPHORE_ERROR: return "Semaphore error";
        case ErrorCode::DEADLOCK_DETECTED: return "Deadlock detected";
        case ErrorCode::QUEUE_FULL: return "Queue full";
        case ErrorCode::QUEUE_EMPTY: return "Queue empty";
        case ErrorCode::IO_ERROR: return "I/O error";
        case ErrorCode::READ_ERROR: return "Read error";
        case ErrorCode::WRITE_ERROR: return "Write error";
        case ErrorCode::PERMISSION_DENIED: return "Permission denied";
        case ErrorCode::DATA_NOT_READY: return "Data not ready";
        case ErrorCode::DATA_CORRUPTED: return "Data corrupted";
        case ErrorCode::CRC_ERROR: return "CRC error";
        case ErrorCode::CHECKSUM_ERROR: return "Checksum error";
        case ErrorCode::BUFFER_OVERFLOW: return "Buffer overflow";
        case ErrorCode::BUFFER_UNDERFLOW: return "Buffer underflow";
        case ErrorCode::INVALID_DATA: return "Invalid data";
        case ErrorCode::DEVICE_NOT_FOUND: return "Device not found";
        case ErrorCode::DEVICE_ERROR: return "Device error";
        case ErrorCode::DEVICE_BUSY: return "Device busy";
        case ErrorCode::DEVICE_DISCONNECTED: return "Device disconnected";
        case ErrorCode::HARDWARE_FAILURE: return "Hardware failure";
        case ErrorCode::COMMUNICATION_ERROR: return "Communication error";
        case ErrorCode::CONNECTION_FAILED: return "Connection failed";
        case ErrorCode::CONNECTION_LOST: return "Connection lost";
        case ErrorCode::CONNECTION_REFUSED: return "Connection refused";
        case ErrorCode::PROTOCOL_ERROR: return "Protocol error";
        case ErrorCode::SEND_FAILED: return "Send failed";
        case ErrorCode::RECEIVE_FAILED: return "Receive failed";
        case ErrorCode::STORAGE_ERROR: return "Storage error";
        case ErrorCode::STORAGE_FULL: return "Storage full";
        case ErrorCode::FILE_NOT_FOUND: return "File not found";
        case ErrorCode::FILE_EXISTS: return "File exists";
        case ErrorCode::MOUNT_FAILED: return "Mount failed";
        case ErrorCode::NETWORK_ERROR: return "Network error";
        case ErrorCode::NETWORK_UNREACHABLE: return "Network unreachable";
        case ErrorCode::HOST_UNREACHABLE: return "Host unreachable";
        case ErrorCode::DNS_FAILED: return "DNS failed";
        case ErrorCode::SSL_ERROR: return "SSL error";
        default: return "Unknown error code";
    }
}

/**
 * @brief Check if error code indicates success
 * @param code The error code to check
 * @return true if code indicates success
 */
constexpr bool isSuccess(ErrorCode code) noexcept {
    return code == ErrorCode::OK;
}

/**
 * @brief Check if error code indicates failure
 * @param code The error code to check
 * @return true if code indicates failure
 */
constexpr bool isError(ErrorCode code) noexcept {
    return code != ErrorCode::OK;
}

} // namespace common
