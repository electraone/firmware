#include "MidiOutput.h"
#include "App.h"

/** Constructor
 *
 */
MidiOutput::MidiOutput(MidiInterface::Type newInterface, uint8_t newPort)
    : MidiJack(newInterface, newPort), channel(0), rate(0), tsLastMessage(0)
{
}

void MidiOutput::setPort(uint8_t newPort)
{
    port = newPort;
}

uint8_t MidiOutput::getPort(void) const
{
    return (port);
}

void MidiOutput::setChannel(uint8_t newChannel)
{
    channel = newChannel;
}

uint8_t MidiOutput::getChannel(void) const
{
    return (channel);
}

uint16_t MidiOutput::getRate(void) const
{
    return (rate);
}

void MidiOutput::setTsLastMessage(void)
{
    tsLastMessage = millis();
}

bool MidiOutput::isReady(void)
{
    return (((millis() - tsLastMessage) > rate) ? true : false);
}

/** Send Electra message to Midi outputs.
 *
 */
void MidiOutput::sendMessageNow(MidiMessage &message)
{
    MidiMessage::Type type = message.getType();

    if (type == MidiMessage::Type::Clock) {
        sendClock(getInterfaceType(), getPort());
    }

    else if (type == MidiMessage::Type::Start) {
        sendStart(getInterfaceType(), getPort());
    }

    else if (type == MidiMessage::Type::Continue) {
        sendContinue(getInterfaceType(), getPort());
    }

    else if (type == MidiMessage::Type::Stop) {
        sendStop(getInterfaceType(), getPort());
    }

    else if (type == MidiMessage::Type::NoteOn) {
        sendNoteOn(getInterfaceType(),
                   getPort(),
                   message.getChannel(),
                   message.getData1(),
                   message.getData2());
    }

    else if (type == MidiMessage::Type::NoteOff) {
        sendNoteOff(getInterfaceType(),
                    getPort(),
                    message.getChannel(),
                    message.getData1(),
                    message.getData2());
    }

    else if (type == MidiMessage::Type::AfterTouchPoly) {
        sendAfterTouchPoly(getInterfaceType(),
                           getPort(),
                           message.getChannel(),
                           message.getData1(),
                           message.getData2());
    }

    else if (type == MidiMessage::Type::AfterTouchChannel) {
        sendAfterTouchChannel(getInterfaceType(),
                              getPort(),
                              message.getChannel(),
                              message.getData1());
    }

    else if (type == MidiMessage::Type::ControlChange) {
        sendControlChange(getInterfaceType(),
                          getPort(),
                          message.getChannel(),
                          message.getData1(),
                          message.getData2());
    }

    else if (type == MidiMessage::Type::ProgramChange) {
        sendProgramChange(getInterfaceType(),
                          getPort(),
                          message.getChannel(),
                          message.getData1());
    }

    else if (type == MidiMessage::Type::PitchBend) {
        sendPitchBend(getInterfaceType(),
                      getPort(),
                      message.getChannel(),
                      message.getData1() | message.getData2() << 7);
    }

    else if (type == MidiMessage::Type::SongPosition) {
        sendSongPosition(getInterfaceType(),
                         getPort(),
                         message.getData1() | message.getData2() << 7);
    }

    else if (type == MidiMessage::Type::SongSelect) {
        sendSongSelect(getInterfaceType(), getPort(), message.getData1());
    }

    else if (type == MidiMessage::Type::TuneRequest) {
        sendTuneRequest(getInterfaceType(), getPort());
    }

    else if (type == MidiMessage::Type::ActiveSensing) {
        sendActiveSensing(getInterfaceType(), getPort());
    }

    else if (type == MidiMessage::Type::SystemReset) {
        sendSystemReset(getInterfaceType(), getPort());
    }
}

/** Send an array of Electra messages to Midi outputs.
 *
 */
void MidiOutput::sendMessages(std::vector<MidiMessage> &messages)
{
    for (auto &message : messages) {
        sendMessage(message);
    }
}

/** Send Electra message to Midi outputs.
 *
 */
void MidiOutput::sendMessage(MidiMessage &message)
{
    /*
    // Fetch the device info
    Device *device = getDevice(message->getDeviceId());

    if (!device) {
        return;
    }
    int rate = device->getRate();
    */

    MidiMessageTransport mmt(getInterfaceType(), getPort(), message);

    int rate = 0;

    // Send message immediatelly or add it to the queue according to the rate
    if (rate == 0) {
        sendMessageNow(mmt);
    } else {
        addToQueue(mmt);
    }
}

bool MidiOutput::isIdenticalChange(const MidiMessageTransport &m1,
                                   const MidiMessageTransport &m2)
{
    return (false);
}

/** Adds the message to the queue if it is not registered there already
 *
 */
void MidiOutput::addToQueue(MidiMessageTransport &message)
{
    bool alreadyExists = false;

    for (int i = 0; i < outgoingQueue.size(); i++) {
        if (MidiOutput::isIdenticalChange(outgoingQueue[i], message)) {
            alreadyExists = true;
        }
    }

    if (alreadyExists == false) {
        outgoingQueue.push(message);
    }
}

/** Send Electra message to Midi outputs.
 *
 */
void MidiOutput::sendMessageNow(MidiMessageTransport &mmt)
{
    sendControlChange(mmt.getInterfaceType(),
                      mmt.getPort(),
                      mmt.getChannel(),
                      mmt.getData1(),
                      mmt.getData2());
}

void MidiOutput::send(MidiInterface::Type interface,
                      uint8_t port,
                      MidiMessage::Type type,
                      uint8_t channel,
                      uint8_t data1,
                      uint8_t data2)
{
    MidiInterface::get(interface)->send(port, type, channel, data1, data2);
    indicate(
        interface, port, Direction::out, MidiMessage::Type::NoteOn); // \todo
}

void MidiOutput::sendControlChange(MidiInterface::Type interface,
                                   uint8_t port,
                                   uint8_t channel,
                                   uint8_t parameterNumber,
                                   uint8_t value)
{
    MidiInterface::get(interface)->sendControlChange(
        port, parameterNumber, value, channel);

    indicate(interface, port, Direction::out, MidiMessage::Type::ControlChange);
}

void MidiOutput::sendNoteOn(MidiInterface::Type interface,
                            uint8_t port,
                            uint8_t channel,
                            uint8_t noteNumber,
                            uint8_t velocity)
{
    MidiInterface::get(interface)->sendNoteOn(
        port, channel, noteNumber, velocity);
    indicate(interface, port, Direction::out, MidiMessage::Type::NoteOn);
}

void MidiOutput::sendNoteOff(MidiInterface::Type interface,
                             uint8_t port,
                             uint8_t channel,
                             uint8_t noteNumber,
                             uint8_t velocity)
{
    MidiInterface::get(interface)->sendNoteOff(
        port, channel, noteNumber, velocity);
    indicate(interface, port, Direction::out, MidiMessage::Type::NoteOff);
}

void MidiOutput::sendStart(MidiInterface::Type interface, uint8_t port)
{
    MidiInterface::get(interface)->sendStart(port);
    indicate(interface, port, Direction::out, MidiMessage::Type::Start);
}

void MidiOutput::sendStop(MidiInterface::Type interface, uint8_t port)
{
    MidiInterface::get(interface)->sendStop(port);
    indicate(interface, port, Direction::out, MidiMessage::Type::Stop);
}

void MidiOutput::sendTuneRequest(MidiInterface::Type interface, uint8_t port)
{
    MidiInterface::get(interface)->sendTuneRequest(port);
    indicate(interface, port, Direction::out, MidiMessage::Type::TuneRequest);
}

void MidiOutput::sendProgramChange(MidiInterface::Type interface,
                                   uint8_t port,
                                   uint8_t channel,
                                   uint8_t programNumber)
{
    MidiInterface::get(interface)->sendProgramChange(
        port, channel, programNumber);
    indicate(interface, port, Direction::out, MidiMessage::Type::ProgramChange);
}

void MidiOutput::sendSysEx(MidiInterface::Type interface,
                           uint8_t port,
                           uint8_t *data,
                           uint16_t dataLength)
{
    MidiInterface::get(interface)->sendSysEx(port, data, dataLength);
    indicate(
        interface, port, Direction::out, MidiMessage::Type::SystemExclusive);
}

void MidiOutput::sendPitchBend(MidiInterface::Type interface,
                               uint8_t port,
                               uint8_t channel,
                               uint16_t value)
{
    MidiInterface::get(interface)->sendPitchBend(port, channel, value);
    indicate(interface, port, Direction::out, MidiMessage::Type::PitchBend);
}

void MidiOutput::sendAfterTouchPoly(MidiInterface::Type interface,
                                    uint8_t port,
                                    uint8_t channel,
                                    uint8_t noteNumber,
                                    uint8_t pressure)
{
    MidiInterface::get(interface)->sendAfterTouchPoly(
        port, channel, noteNumber, pressure);
    indicate(
        interface, port, Direction::out, MidiMessage::Type::AfterTouchPoly);
}

void MidiOutput::sendAfterTouchChannel(MidiInterface::Type interface,
                                       uint8_t port,
                                       uint8_t channel,
                                       uint8_t pressure)
{
    MidiInterface::get(interface)->sendAfterTouchChannel(
        port, channel, pressure);
    indicate(
        interface, port, Direction::out, MidiMessage::Type::AfterTouchChannel);
}

void MidiOutput::sendClock(MidiInterface::Type interface, uint8_t port)
{
    MidiInterface::get(interface)->sendClock(port);
    indicate(interface, port, Direction::out, MidiMessage::Type::Clock);
}

void MidiOutput::sendContinue(MidiInterface::Type interface, uint8_t port)
{
    MidiInterface::get(interface)->sendContinue(port);
    indicate(interface, port, Direction::out, MidiMessage::Type::Continue);
}

void MidiOutput::sendActiveSensing(MidiInterface::Type interface, uint8_t port)
{
    MidiInterface::get(interface)->sendActiveSensing(port);
    indicate(interface, port, Direction::out, MidiMessage::Type::ActiveSensing);
}

void MidiOutput::sendSystemReset(MidiInterface::Type interface, uint8_t port)
{
    MidiInterface::get(interface)->sendSystemReset(port);
    indicate(interface, port, Direction::out, MidiMessage::Type::SystemReset);
}

void MidiOutput::sendSongSelect(MidiInterface::Type interface,
                                uint8_t port,
                                uint8_t song)
{
    MidiInterface::get(interface)->sendSongSelect(port, song);
    indicate(interface, port, Direction::out, MidiMessage::Type::SongSelect);
}

void MidiOutput::sendSongPosition(MidiInterface::Type interface,
                                  uint8_t port,
                                  uint16_t beats)
{
    MidiInterface::get(interface)->sendSongPosition(port, beats);
    indicate(interface, port, Direction::out, MidiMessage::Type::SongPosition);
}

void MidiOutput::sendSysEx(MidiInterface::Type interface,
                           uint8_t port,
                           std::vector<uint8_t> data)
{
    sendSysEx(interface, port, &data[0], data.size());
    indicate(
        interface, port, Direction::out, MidiMessage::Type::SystemExclusive);
}

void MidiOutput::sendNrpn(MidiInterface::Type interface,
                          uint8_t port,
                          uint8_t channel,
                          uint16_t parameterNumber,
                          uint16_t midiValue,
                          bool lsbFirst)
{
    sendControlChange(interface, port, channel, 99, parameterNumber >> 7);
    sendControlChange(interface, port, channel, 98, parameterNumber & 0x7F);

    if (lsbFirst) {
        sendControlChange(interface, port, channel, 38, midiValue >> 7);
        sendControlChange(interface, port, channel, 6, midiValue & 0x7F);
    } else {
        sendControlChange(interface, port, channel, 6, midiValue >> 7);
        sendControlChange(interface, port, channel, 38, midiValue & 0x7F);
    }

    sendControlChange(interface, port, channel, 101, 127);
    sendControlChange(interface, port, channel, 100, 127);
}

void MidiOutput::sendRpn(MidiInterface::Type interface,
                         uint8_t port,
                         uint8_t channel,
                         uint16_t parameterNumber,
                         uint16_t midiValue)
{
    sendControlChange(interface, port, channel, 101, parameterNumber >> 7);
    sendControlChange(interface, port, channel, 100, parameterNumber & 0x7F);
    sendControlChange(interface, port, channel, 6, midiValue >> 7);
    sendControlChange(interface, port, channel, 38, midiValue & 0x7F);
    sendControlChange(interface, port, channel, 101, 127);
    sendControlChange(interface, port, channel, 100, 127);
}

void MidiOutput::sendControlChange14Bit(MidiInterface::Type interface,
                                        uint8_t port,
                                        uint8_t channel,
                                        uint16_t parameterNumber,
                                        uint16_t midiValue,
                                        bool lsbFirst)
{
    if (lsbFirst) {
        sendControlChange(
            interface, port, channel, parameterNumber + 32, midiValue >> 7);
        sendControlChange(
            interface, port, channel, parameterNumber, midiValue & 0x7F);
    } else {
        sendControlChange(
            interface, port, channel, parameterNumber, midiValue >> 7);
        sendControlChange(
            interface, port, channel, parameterNumber + 32, midiValue & 0x7F);
    }
}

void MidiOutput::indicate(MidiInterface::Type interface,
                          uint8_t port,
                          Direction direction,
                          MidiMessage::Type msgType)
{
    App::get()->statusBar.indicate(interface, port, direction, msgType);
}

CircularBuffer<MidiMessageTransport, MidiOutput::queueSize>
    MidiOutput::outgoingQueue;
