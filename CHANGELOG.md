# Changelog

## [0.4.0] - 2026-06-18
### Fixed
- Fixed the single-token collision by introducing the `mm *\t` syntax for full-month matches vs `dd\t` for day matches.
- Reset the project version to `0.4.0` to respect semantic versioning.

### Added
- Created `SPEC.md` to protect myself from forgetting my own layout rules.

## [0.3.0] - 2026-06-17
### Added
- Integrated `<time.h>` to pull local system time dynamically.
- Used `sscanf` cascades to extract integers from tokens.

## [0.2.0] - 2026-06-04
### Added
- Built the file processing engine using POSIX `getline`.
- Added conditional `uname` checks to the Makefile for Mac/Linux portability.

## [0.1.0] - 2026-06-01
### Added
- Initialized project directory structure and basic "Hello World" build targets.
