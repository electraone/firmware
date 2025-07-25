# Electra One MK1 Brightness Control v3.5.4

## Overview

This release restores the display brightness control feature for the **Electra One MK1** running **firmware v3.5.4**. The brightness control was available in firmware v2.0 but was dropped in later versions.

## What's Included

- **Modified firmware** (`firmware.img`) - Ready to upload firmware with brightness control
- **SysEx control files** (`brightness-control-sysex/`) - 5 brightness level control files
- **Forum post** (`BRIGHTNESS_CONTROL_FORUM_POST.md`) - Complete technical documentation
- **Source code** - Branch: `brightness-control-v3.5.4`

## Quick Installation

1. Upload `firmware.img` to your Electra One MK1 using ElectraOneConsole
2. Test brightness control using the `.syx` files in `brightness-control-sysex/`

## Available Brightness Levels

| File | Level | Value | Description |
|------|--------|-------|-------------|
| `set-brightness-maximum.syx` | Maximum | 0 | Brightest |
| `set-brightness-high.syx` | High | 1024 | High brightness |
| `set-brightness-medium.syx` | Medium | 2048 | Medium brightness |
| `set-brightness-low.syx` | Low | 4096 | Low brightness |
| `set-brightness-minimum.syx` | Minimum | 5888 | Dimmest |

## Technical Details

- **Hardware**: Electra One MK1 only
- **Firmware Base**: v3.5.4 with minimal changes (25 lines added)
- **Persistence**: Brightness settings survive controller reboots
- **Compatibility**: Full backward compatibility with existing presets

## Usage

Send any of the brightness SysEx files to your controller's CTRL port using:
- SysEx Librarian (Mac)
- MIDI-OX (Windows)  
- Hardware MIDI controllers
- Software automation scripts

The controller will respond with an ACK message: `F0 00 21 45 7E 01 00 00 F7`

## Source Code Changes

The implementation required only adding UPDATE command support for ElectraInfo objects in `src/System/TaskProcessSysex.cpp`. All underlying brightness infrastructure was already present in v3.5.4.

## Note

Higher brightness values = darker screen (inverted PWM control). The range 0-5888 provides 5 usable brightness levels.

---

**Branch**: `brightness-control-v3.5.4`  
**Compatible with**: Electra One MK1, firmware v3.5.4  
**Status**: Tested and working