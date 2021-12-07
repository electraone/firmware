mod boxed;
mod image;
mod midi;
mod teensy;

use std::path::Path;
use std::process;

use boxed::ResultBoxed;
use clap::Parser;

#[derive(Parser, Debug)]
#[clap(version = "0.1")]
struct Opts {
    #[clap(subcommand)]
    subcmd: SubCommand,
}

#[derive(Parser, Debug)]
enum SubCommand {
    #[clap(about = "Reboot in the firmware update mode")]
    EnterDfu {
        #[clap(short, long)]
        port: String,
    },
    #[clap(about = "Reboot in the normal mode")]
    LeaveDfu,
    #[clap(about = "MIDI helpers")]
    Midi {
        #[clap(subcommand)]
        cmd: SubCommandMidi,
    },
    #[clap(about = "Convert ihex into img")]
    ConvertIHEXToIMG {
        #[clap(short, long)]
        source: String,
        #[clap(short, long)]
        output: String,
    },
    #[clap(about = "Upload firmware image")]
    Upload {
        #[clap(short, long)]
        source: String,
    }
}

#[derive(Parser, Debug)]
enum SubCommandMidi {
    #[clap(about = "Stream MIDI events from Electra")]
    Stream {
        #[clap(short, long)]
        port: String,
    },
    #[clap(about = "List known MIDI devices")]
    List,
}

fn main() {
    let opts: Opts = Opts::parse();

    let ret = match opts.subcmd {
        SubCommand::EnterDfu { port } =>
            midi::enter_dfu(port).boxed(),
        SubCommand::LeaveDfu =>
            teensy::leave_dfu().boxed(),
        SubCommand::Midi { cmd: SubCommandMidi::Stream { port } } =>
            midi::stream_midi(port).boxed(),
        SubCommand::Midi { cmd: SubCommandMidi::List } =>
            midi::list_midi_devices().boxed(),
        SubCommand::ConvertIHEXToIMG { source, output } =>
            image::convert_ihex_to_img(Path::new(&source), Path::new(&output)).boxed(),
        SubCommand::Upload { source } =>
            teensy::upload_firmware(Path::new(&source)).boxed(),
    };
    
    if let Err(err) = ret {
        eprintln!("{}", err);
        process::exit(1);
    };
}
