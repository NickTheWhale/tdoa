# CLI Documentation

## Overview

This file implements a command-line interface (CLI) for configuring and managing settings related to a UWB (Ultra-Wideband) module. It leverages the Zephyr Project's shell subsystem to provide a set of commands for interacting with the configuration settings stored in flash memory.

## Commands

The CLI provides the following commands for configuration management:

### `config dump`

- **Description**: Dumps the current configuration in hexadecimal format.
- **Usage**: `config dump`

### `config print`

- **Description**: Prints the current configuration in a human-readable format. (Note: This command is not implemented.)
- **Usage**: `config print`

### `config erase`

- **Description**: Erases the configuration from flash.
- **Usage**: `config erase`

### `config mode [tag|anchor|0|1]`

- **Description**: Sets or gets the UWB mode. The mode can be set to either 'tag' or 'anchor', which may also be represented as '0' or '1', respectively.
- **Usage**:
  - Set mode: `config mode [tag|anchor|0|1]`
  - Get current mode: `config mode`

### `config address [address]`

- **Description**: Sets or gets the UWB address. The address should be specified in hexadecimal format, separated by colons, semicolons, or dots.
- **Usage**:
  - Set address: `config address xx:xx:xx:xx:xx:xx:xx:xx`
  - Get current address: `config address`

### `config anchor_position [x] [y]`

- **Description**: Sets or gets the UWB anchor position in millimeters.
- **Usage**:
  - Set position: `config anchor_position [x] [y]`
  - Get current position: `config anchor_position`

## Licensing

This software is provided under the MIT License, allowing for free and open use, modification, and distribution of the software.
