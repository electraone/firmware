# A simple firmware management tool for electra

## Build instructions

- Install a recent rust toolchain via e.g. [rustup](https://rustup.rs/).
- run `cargo build`

Windows and macOS don't have any external dependencies. For linux you'll need to
provide `libasound2-dev` and `libusb-1.0-0-dev` or however the asound and libusb
1.0.0 header packages are named in your distro.

## Usage instructions

### Enter DFU mode

To enter the DFU mode:

    electra-firmware-tools enter-dfu -p $PORT_NAME

Use the third Electra port (named e.g. `MIDIOUT3 (Electra Controller)`).

### Leave DFU mode

To leave the DFU mode:

    electra-firmware-tools leave-dfu

As Electra's using the teensy loader there's no way to discern it from any other
device that might be running the teensy bootloader, thus the first available
device that looks like it's a teensy is restarted.

### Upload the firmware

To upload the firmware to a device in a DFU mode:

    electra-firmware-tools -s $FILE

Electra will be automatically restarted once the firmware is uploaded. The
source file can be either the `hex` (intel hex) or the `img` (Electra firmware).

### Convert hex to img

This is a helper command that's an equivalent of `hex2k66img`:

    electra-firmware-tools convert-ihex-to-img -s $HEXFILE -o $IMGFILE


### List connected MIDI devices

To show the devices recognized by the tool:

    electra-firmware-tools midi list

### Stream the MIDI events from the device

To stream the MIDI data coming from the device:

    electra-firmware-tools midi stream -p $PORT
