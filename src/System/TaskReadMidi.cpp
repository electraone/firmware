#include "TaskReadMidi.h"
#include "MidiInputCallback.h"
#include "Sysex.h"
#include "App.h" // \todo To be removed.

CircularBuffer<MidiMessageTransport, 5> incomingQueueL1;
CircularBuffer<MidiMessageTransport, 256> incomingQueueL2;

/** This is used when reading of SysEx on CTRL port gets priority.
 */
void readCtrlMidi(void)
{
    MidiInterface::get(MidiInterface::Type::MidiIo)->readNoAction(0);
    MidiInterface::get(MidiInterface::Type::MidiIo)->readNoAction(1);
    MidiInterface::get(MidiInterface::Type::MidiUsbDev)->readNoAction(0);
    MidiInterface::get(MidiInterface::Type::MidiUsbHost)->readNoAction(0);
    MidiInterface::get(MidiInterface::Type::MidiUsbHost)->readNoAction(1);
}

// \todo Deduplicate reading from the physical interfaces.
/** process incomming MIDI messages
 */
void readMidi(void)
{
    // Process inbound MIDI messages on USB MIDI IN port (virtual cables).
    MidiBase *midiInterface =
        MidiInterface::get(MidiInterface::Type::MidiUsbDev);
    if (midiInterface->read(0)) {
        uint8_t port = midiInterface->getActivePort();
        incomingQueueL1.push(MidiMessageTransport(
            MidiInterface::Type::MidiUsbDev,
            port,
            midiInterface->getChannel(port),
            static_cast<MidiMessage::Type>(midiInterface->getType(port)),
            midiInterface->getData1(port),
            midiInterface->getData2(port)));
    }

    // Process inbound MIDI messages on USB host ports.
    for (uint8_t port = 0; port < 2; port++) {
        MidiBase *midiInterface =
            MidiInterface::get(MidiInterface::Type::MidiUsbHost);
        if (midiInterface->read(port)) {
            incomingQueueL1.push(MidiMessageTransport(
                MidiInterface::Type::MidiUsbHost,
                USBDevices[port].midiBus,
                USBDevices[port].midiDevice->getChannel(),
                static_cast<MidiMessage::Type>(
                    USBDevices[port].midiDevice->getType()),
                USBDevices[port].midiDevice->getData1(),
                USBDevices[port].midiDevice->getData2()));
        }
    }

    // Process inbound messages on MIDI DIN5 interfaces.
    for (uint8_t port = 0; port < 2; port++) {
        MidiBase *midiInterface =
            MidiInterface::get(MidiInterface::Type::MidiIo);
        if (midiInterface->read(port)) {
            incomingQueueL1.push(MidiMessageTransport(
                MidiInterface::Type::MidiIo,
                port,
                midiInterface->getChannel(port),
                static_cast<MidiMessage::Type>(midiInterface->getType(port)),
                midiInterface->getData1(port),
                midiInterface->getData2(port)));
        }
    }

    // Process all collected messages
    while (incomingQueueL1.isEmpty() != true) {
        MidiMessageTransport message = incomingQueueL1.shift();

        logMessage("queueL1: received: interface=%d, port=%d, channel=%d, "
                   " type=%d, data1=%d, data2=%d",
                   message.getInterfaceType(),
                   message.getPort(),
                   message.getChannel(),
                   message.getType(),
                   message.getData1(),
                   message.getData2());

        // Indicate incoming data on the display
        App::get()->statusBar.indicate(message.getInterfaceType(),
                                       message.getPort(),
                                       Direction::in,
                                       message.getType());

        // Pass the message for further processing
        if (message.getType() == MidiMessage::Type::SystemExclusive) {
            // Ignore sysex for now. It is handled with callbacks (SysexCallbacks.cpp)
        } else if ((message.getType() == MidiMessage::Type::Clock)) {
            // process system real-time
            MidiInput midiInput(message.getInterfaceType(), message.getPort());
            MidiInputCallback::deviceManager.handleIncomingMidiMessage(
                midiInput, message);
            runOptionalCallbacks(midiInput,
                                 message.getChannel(),
                                 message.getType(),
                                 message.getData1(),
                                 message.getData2());
        } else {
            incomingQueueL2.push(message);
        }
    }
}

/** Process MIDI messages form the L2 queue.
 */
void processMidi(void)
{
    while (incomingQueueL2.isEmpty() != true) {
        MidiMessageTransport message = incomingQueueL2.shift();

#ifdef DENBUG
        logMessage("queueL2 [%d]: received: interface=%d, port=%d, channel=%d, "
                   "type=%d, data1=%d, data2=%d",
                   incomingQueueL2.size(),
                   message.getInterfaceType(),
                   message.getPort(),
                   message.getChannel(),
                   message.getType(),
                   message.getData1(),
                   message.getData2());
#endif

        if (App::get()->enableMidi == true) {
            MidiInput midiInput(message.getInterfaceType(), message.getPort());

            // it is a Control message
            if (((message.getInterfaceType() == MidiInterface::Type::MidiUsbDev)
                 || (message.getInterfaceType()
                     == MidiInterface::Type::MidiUsbHost))
                && (message.getPort() == USB_MIDI_PORT_CTRL)) {
                App::get()->handleIncomingControlMessage(midiInput, message);
            } else {
                MidiInputCallback::deviceManager.handleIncomingMidiMessage(
                    midiInput, message);
                runOptionalCallbacks(midiInput,
                                     message.getChannel(),
                                     message.getType(),
                                     message.getData1(),
                                     message.getData2());
            }
        }
    }
}

/** Call optional callback function if it is linked.
 */
void runOptionalCallbacks(MidiInput midiInput,
                          uint8_t channel,
                          MidiMessage::Type type,
                          uint8_t data1,
                          uint8_t data2)
{
    // \todo optional callbacks (Replace with an indexed table???)
    if (type == MidiMessage::Type::Clock) {
        if (MidiInputCallback::onMidiClockCallback) {
            MidiInputCallback::onMidiClockCallback(midiInput);
        }
    } else if (type == MidiMessage::Type::Start) {
        if (MidiInputCallback::onMidiStartCallback) {
            MidiInputCallback::onMidiStartCallback(midiInput);
        }
    } else if (type == MidiMessage::Type::Stop) {
        if (MidiInputCallback::onMidiStopCallback) {
            MidiInputCallback::onMidiStopCallback(midiInput);
        }
    } else if (type == MidiMessage::Type::Continue) {
        if (MidiInputCallback::onMidiContinueCallback) {
            MidiInputCallback::onMidiContinueCallback(midiInput);
        }
    } else if (type == MidiMessage::Type::ActiveSensing) {
        if (MidiInputCallback::onMidiActiveSensingCallback) {
            MidiInputCallback::onMidiActiveSensingCallback(midiInput);
        }
    } else if (type == MidiMessage::Type::SystemReset) {
        if (MidiInputCallback::onMidiSystemResetCallback) {
            MidiInputCallback::onMidiSystemResetCallback(midiInput);
        }
    } else if (type == MidiMessage::Type::TuneRequest) {
        if (MidiInputCallback::onMidiTuneRequestCallback) {
            MidiInputCallback::onMidiTuneRequestCallback(midiInput);
        }
    } else if (type == MidiMessage::Type::ProgramChange) {
        if (MidiInputCallback::onMidiProgramChangeCallback) {
            MidiInputCallback::onMidiProgramChangeCallback(
                midiInput, channel, data1);
        }
    } else if (type == MidiMessage::Type::AfterTouchChannel) {
        if (MidiInputCallback::onMidiAfterTouchChannelCallback) {
            MidiInputCallback::onMidiAfterTouchChannelCallback(
                midiInput, channel, data1);
        }
    } else if (type == MidiMessage::Type::SongSelect) {
        if (MidiInputCallback::onMidiSongSelectCallback) {
            MidiInputCallback::onMidiSongSelectCallback(midiInput, data1);
        }
    } else if (type == MidiMessage::Type::PitchBend) {
        if (MidiInputCallback::onMidiPitchBendCallback) {
            int value = data1 | (data2 << 7);
            MidiInputCallback::onMidiPitchBendCallback(
                midiInput, channel, value);
        }
    } else if (type == MidiMessage::Type::SongPosition) {
        if (MidiInputCallback::onMidiSongPositionCallback) {
            int position = data1 | (data2 << 7);
            MidiInputCallback::onMidiSongPositionCallback(midiInput, position);
        }
    } else if (type == MidiMessage::Type::ControlChange) {
        if (MidiInputCallback::onMidiControlChangeCallback) {
            MidiInputCallback::onMidiControlChangeCallback(
                midiInput, channel, data1, data2);
        }
    } else if (type == MidiMessage::Type::NoteOn) {
        if (MidiInputCallback::onMidiNoteOnCallback) {
            MidiInputCallback::onMidiNoteOnCallback(
                midiInput, channel, data1, data2);
        }
    } else if (type == MidiMessage::Type::NoteOff) {
        if (MidiInputCallback::onMidiNoteOffCallback) {
            MidiInputCallback::onMidiNoteOffCallback(
                midiInput, channel, data1, data2);
        }
    } else if (type == MidiMessage::Type::AfterTouchPoly) {
        if (MidiInputCallback::onMidiAfterTouchPolyCallback) {
            MidiInputCallback::onMidiAfterTouchPolyCallback(
                midiInput, channel, data1, data2);
        }
    }

    if (MidiInputCallback::onMidiMessageCallback) {
        MidiMessage midiMessage(channel, type, data1, data2);
        MidiInputCallback::onMidiMessageCallback(midiInput, midiMessage);
    }
}
