use std::io::stdin;

use midi_msg::{ManufacturerID, MidiMsg, ReceiverContext};
use midir::{ConnectError, MidiInput, MidiOutput, PortInfoError, SendError};
use snafu::prelude::*;

#[derive(Debug, Snafu)]
pub enum Error {
    #[snafu(display("midi init error: {}", source))]
    Init {
        source: midir::InitError,
    },

    #[snafu(display("midi port error: {}", source))]
    PortInfo { source: PortInfoError },

    #[snafu(display("can't find electra midi port {port}"))]
    PortNotFound {
        port: String,
    },

    #[snafu(display("failed to connect to midi: {}", source))]
    ConnectInput {
        source: ConnectError<MidiInput>,
    },

    #[snafu(display("failed to connect to midi: {}", source))]
    ConnectOutput {
        source: ConnectError<MidiOutput>,
    },

    #[snafu(display("failed to read line: {}", source))]
    IO {
        source: std::io::Error,
    },

    #[snafu(display("failed to send midi message: {}", source))]
    Send {
        source: SendError,
    },
}

type Result<T, E = Error> = std::result::Result<T, E>;

/// Lists the available MIDI devices.
pub fn list_midi_devices() -> Result<()>{
    println!("Known MIDI input devices:");
    let midi_in = MidiInput::new("electra-firmware-tools").context(InitSnafu)?;
    for port in midi_in.ports().into_iter() {
        println!("  - \"{}\"", midi_in.port_name(&port).context(PortInfoSnafu)?);
    }

    println!("\nKnown MIDI output devices:");
    let midi_out = MidiOutput::new("electra-firmware-tools").context(InitSnafu)?;
    for port in midi_out.ports().into_iter() {
        println!("  - \"{}\"", midi_out.port_name(&port).context(PortInfoSnafu)?);
    }

    Result::Ok(())
}

/// Streams the MIDI events from the specified device.
/// 
/// TODO: this should try guessing the electra's port
pub fn stream_midi(port_name: String) -> Result<()> {
    let midi_in = MidiInput::new("electra-firmware-tools").context(InitSnafu)?;
    let ports = midi_in.ports();
    let port = ports.iter().find(
        |port|
        midi_in.port_name(port).unwrap_or_else(|_| "".to_string()) == port_name).context(PortNotFoundSnafu { port: port_name })?;
    
    let mut ctx = ReceiverContext::new();
    let _conn = midi_in.connect(port, "electra-firmware-tools", move |stamp, message, _| {
        let (msg, _len) = MidiMsg::from_midi_with_context(message, &mut ctx).expect("failed to parse midi message");
        
        println!("{}: {:?}", stamp, msg);
    }, ()).context(ConnectInputSnafu)?;

    let mut input = String::new();
    stdin().read_line(&mut input).context(IOSnafu)?;

    Result::Ok(())
}

/// Forces the connected Electra into the DFU mode.
/// 
/// This command should be sent to the MIDI3 port aka `MIDIOUT3 (Electra Controller)`.
pub fn enter_dfu(port_name: String) -> Result<()> {
    let midi_out = MidiOutput::new("electra-firmware-tools").context(InitSnafu)?;
    let ports = midi_out.ports();
    let port = ports.iter().find(
        |port|
        midi_out.port_name(port).unwrap_or_else(|_| "".to_string()) == port_name).context(PortNotFoundSnafu { port: port_name })?;
    let mut conn_out = midi_out.connect(port, "electra-firmware-tools").context(ConnectOutputSnafu)?;
    let reset_message = MidiMsg::SystemExclusive {
        msg: midi_msg::SystemExclusiveMsg::Commercial {
            id: ManufacturerID::from((0x21, 0x45)),
            data: vec![0x7F, 0x7F],
        }
    }.to_midi();
    conn_out.send(&reset_message).context(SendSnafu)?;

    Result::Ok(())
}
