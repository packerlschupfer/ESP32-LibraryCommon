# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.0] - 2025-12-04

### Added
- Initial public release
- Result<T, E> template for type-safe error handling
- ErrorCode enum with common error types
- Support for Result<void> for operations without return values
- Custom error type support via template parameter
- Zero-cost abstraction when compiled with optimizations

### Notes
- Production-tested in industrial application
- Previous internal versions (v1.x) not publicly released
- Reset to v0.1.0 for clean public release start
