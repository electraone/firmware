use std::fs;
use std::io;
use std::path::Path;

use ihex::ReaderError;
use ihex::Record;
use snafu::ResultExt;
use snafu::Snafu;

/// The maximum code size for electra.
const CODE_SIZE: usize = 1048576;

/// The teensy loader block size.
const BLOCK_SIZE: usize = 1024;

/// The teensy loader block prefix.
const BLOCK_PADDING: usize = 64;

/// The total size of the teensy loader block.
pub const FIRMWARE_CHUNK_SIZE: usize = BLOCK_PADDING + BLOCK_SIZE;

#[derive(Debug, Snafu)]
pub enum Error {
    #[snafu(display("failed to parse the hex format: {}", source))]
    Parse { source: ReaderError },

    #[snafu(display("no end of file record found"))]
    NoEOF,

    #[snafu(display("unsupported hex record found"))]
    UnsupportedRecord,

    #[snafu(display("IO error: {}", source))]
    IO { source: io::Error },
}

type Result<T, E = Error> = std::result::Result<T, E>;

/// FirmwareBytes contains the firmware as a ROM memory image.
pub struct FirmwareBytes (Vec<u8>);

/// LoaderBytes contains the firmware as a stream of teensy loader packets.
pub struct LoaderBytes (Vec<u8>);

impl LoaderBytes {
    pub fn bytes(&self) -> &[u8] {
        &self.0
    }
}

/// Parses the ihex format and returns the ROM image.
/// 
/// Returns the [CODE_SIZE] map where any byte not present in the hex is set to
/// 0xFF.
fn ihex_to_mmap(data: &str) -> Result<FirmwareBytes> {
    let reader = ihex::Reader::new(data);
    let mut buf = vec![0xFFu8; CODE_SIZE];
    let mut segment_offset: usize = 0;

    for record in reader {
        match record {
            Ok(record) => {
                match record {
                    Record::Data { offset, value} => {
                        buf.splice(segment_offset + offset as usize..segment_offset + offset as usize + value.len(), value);
                    },
                    Record::EndOfFile => {
                        return Ok(FirmwareBytes(buf));
                    },
                    Record::ExtendedSegmentAddress(address) => {
                        segment_offset = address as usize * 16;
                    },
                    _ => {
                        return Err(Error::UnsupportedRecord);
                    }
                }
            },
            Err(err) => {
                return Err(Error::Parse { source: err });
            }
        }
    }

    Err(Error::NoEOF)
}

/// Converts the ROM image to the teensy loader packets. They are always aligned
/// to the [FIRMWARE_CHUNK_SIZE] chunks.
/// 
/// This is different from the C tooling as we use a simpler algorithm here, our
/// ihex loader will pre-wipe the ROM image for us so we only need to check for
/// empty pages (blocks of [BLOCK_SIZE] that only contain 0xFFs).
fn mmap_to_img(mmap: FirmwareBytes) -> LoaderBytes {
    let mut buf = Vec::new();
    let mut first_chunk = true;

    for (idx, chunk) in mmap.0.chunks(BLOCK_SIZE).enumerate() {
        if !first_chunk && !chunk.iter().any(|b| *b != 0xFF) {
            continue;
        }
        first_chunk = false;

        let addr = idx * BLOCK_SIZE;
        let mut header = vec![0u8; 64];
        header[0] = (addr & 0xFF) as u8;
        header[1] = ((addr >> 8) & 0xFF) as u8;
        header[2] = ((addr >> 16) & 0xFF) as u8;
        buf.append(&mut header);
        buf.extend(chunk);
    }

    LoaderBytes(buf)
}

/// Loads the ihex file.
pub fn load_ihex(source: &Path) -> Result<LoaderBytes> {
    let ihex = fs::read_to_string(source).context(IOSnafu)?;
    let mmap = ihex_to_mmap(&ihex)?;
    let img = mmap_to_img(mmap);

    Ok(img)
}

/// Loads the electra's img file (which is the teensy loader dump).
pub fn load_img(source: &Path) -> Result<LoaderBytes> {
    let img = fs::read(source).context(IOSnafu)?;

    Ok(LoaderBytes(img))
}

/// Converts an ihex file to the img file.
pub fn convert_ihex_to_img(source: &Path, output: &Path) -> Result<()> {
    let img = load_ihex(source)?;
    fs::write(output, img.0).context(IOSnafu)?;
    
    Result::Ok(())
}
