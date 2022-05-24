use std::{path::Path, thread::sleep, time::Duration};

use hidapi::{HidApi, HidError};
use indicatif::ProgressIterator;
use snafu::prelude::*;

use crate::image::{FIRMWARE_CHUNK_SIZE, LoaderBytes, load_ihex, load_img};

const ELECTRA_DFU_VID: u16 = 0x16C0;
const ELECTRA_DFU_PID: u16 = 0x0478;

#[derive(Debug, Snafu)]
pub enum Error {
    #[snafu(display("failed to initialize HID API: {}", source))]
    HIDInit {
        source: HidError,
    },

    #[snafu(display("failed to connect to the Electra DFU (is the device connected and in the DFU mode?)"))]
    HIDOpen {
        source: HidError,
    },

    #[snafu(display("failed to write the reset packet: {}", source))]
    HIDWrite {
        source: HidError,
    },

    #[snafu(display("failed to load the image: {}", source))]
    Loader {
        source: crate::image::Error,
    },
}

type Result<T, E = Error> = std::result::Result<T, E>;

/// Forces the bootup sequence.
pub fn leave_dfu() -> Result<()> {
    let hidapi = HidApi::new().context(HIDInitSnafu)?;
    let device = hidapi.open(ELECTRA_DFU_VID, ELECTRA_DFU_PID).context(HIDOpenSnafu)?;

    let mut buf = vec![0u8; 1089];
    buf[0] = 0x00;
    buf[1] = 0xFF;
    buf[2] = 0xFF;
    buf[3] = 0xFF;
    
    device.write(&buf).context(HIDWriteSnafu)?;

    Result::Ok(())
}

/// Writes the given firmware to the device.
pub fn write_firmware(img: LoaderBytes) -> Result<()> {
    let hidapi = HidApi::new().context(HIDInitSnafu)?;
    let device = hidapi.open(ELECTRA_DFU_VID, ELECTRA_DFU_PID).context(HIDOpenSnafu)?;

    for chunk in img.bytes().chunks(FIRMWARE_CHUNK_SIZE).progress() {
        let mut buf = vec![0u8];
        buf.extend(chunk);

        device.write(&buf).or_else(|_| {
            println!("failed to write, increasing the delay");
            sleep(Duration::from_millis(50));
            device.write(&buf)
        }).context(HIDWriteSnafu)?;

        sleep(Duration::from_millis(25));
    }

    Result::Ok(())
}

/// Uploads the given ihex or img file to the device.
pub fn upload_firmware(source: &Path) -> Result<()> {
    let img = if source.ends_with(".hex") {
        load_ihex(source).context(LoaderSnafu)?
    } else {
        load_img(source).context(LoaderSnafu)?
    };

    write_firmware(img)?;
    leave_dfu()?;

    Result::Ok(())
}
