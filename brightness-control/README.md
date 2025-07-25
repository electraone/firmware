# Brightness Control SysEx Files

These SysEx files control the display brightness on Electra One MK1 running the brightness-control-v3.5.4 firmware.

## Files

| File | Brightness Level | Value | Description |
|------|------------------|-------|-------------|
| `set-brightness-maximum.syx` | Maximum | 0 | Brightest setting |
| `set-brightness-high.syx` | High | 1024 | High brightness |
| `set-brightness-medium.syx` | Medium | 2048 | Medium brightness |
| `set-brightness-low.syx` | Low | 4096 | Low brightness |
| `set-brightness-minimum.syx` | Minimum | 5888 | Dimmest usable setting |

## Usage

Send any of these files to your Electra One MK1's CTRL port using:
- SysEx Librarian (Mac)
- MIDI-OX (Windows)
- Hardware MIDI controllers
- Custom automation scripts

## Expected Response

The controller will respond with an ACK message:
```
F0 00 21 45 7E 01 00 00 F7
```

## Note

The RA8876 display uses inverted PWM control - higher values make the screen darker. These files provide 5 practical brightness levels tested on hardware.