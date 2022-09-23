#include "MidiOutput.h"
#include "App.h"

/** Constructor
 *
 */
MidiOutput::MidiOutput(MidiInterface::Type newInterface, uint8_t newPort)
    : MidiJack(newInterface, newPort), channel(0), rate(0), tsLastMessage(0)
{
}

MidiOutput::MidiOutput(MidiInterface::Type newInterface,
                       uint8_t newPort,
                       uint8_t newChannel,
                       uint16_t newRate)
    : MidiJack(newInterface, newPort),
      channel(newChannel),
      rate(0),
      tsLastMessage(0)
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

void MidiOutput::setRate(uint16_t newRate)
{
    rate = newRate;
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
void MidiOutput::sendMessage(MidiInterface::Type interface,
                             uint8_t port,
                             MidiMessage &message)
{
    addToQueue(interface, port, message);
}

bool MidiOutput::isIdenticalChange(const MidiMessageTransport &m1,
                                   const MidiMessageTransport &m2)
{
    if (m1.getType() == MidiMessage::Type::ControlChange) {
        if ((m1.getInterfaceType() == m2.getInterfaceType())
            && (m1.getPort() == m2.getPort()) && (m1.getType() == m2.getType())
            && (m1.getChannel() == m2.getChannel())
            && (m1.getData1() == m2.getData1())) {
            return true;
        }
    } else if (m1.getType() == MidiMessage::Type::ProgramChange) {
        if ((m1.getInterfaceType() == m2.getInterfaceType())
            && (m1.getPort() == m2.getPort()) && (m1.getType() == m2.getType())
            && (m1.getChannel() == m2.getChannel())) {
            return true;
        }
    } else if (m1.getType() == MidiMessage::Type::SystemExclusive) {
        if ((m1.getInterfaceType() == m2.getInterfaceType())
            && (m1.getPort() == m2.getPort())
            && (m1.getType() == m2.getType())) {
            return true;
        }
    }
    return (false);
}

/** Adds the message to the queue if it is not registered there already
 *
 */
void MidiOutput::addToQueue(MidiInterface::Type interface,
                            uint8_t port,
                            MidiMessage &message)
{
    MidiMessageTransport mmt(interface, port, message);

    for (int i = 0; i < outgoingQueue.size(); i++) {
        if (MidiOutput::isIdenticalChange(outgoingQueue[i], mmt)) {
            outgoingQueue.getObjectPointer(i)->invalid = true;
        }
    }
    outgoingQueue.push(mmt);
}

/** Send Electra message to Midi outputs.
 *
 */
void MidiOutput::send(MidiInterface::Type interface,
                      uint8_t port,
                      MidiMessage::Type type,
                      uint8_t channel,
                      uint8_t data1,
                      uint8_t data2)
{
    MidiInterface::get(interface)->send(port, type, channel, data1, data2);
    indicate(interface, port, Direction::out, type);
}

void MidiOutput::send(MidiInterface::Type interface,
                      uint8_t port,
                      MidiMessage &message)
{
    if (message.getType() == MidiMessage::Type::SystemExclusive) {
        MidiInterface::get(interface)->send(port, message.getSysExBlock());
    } else {
        MidiInterface::get(interface)->send(port,
                                            message.getType(),
                                            message.getChannel(),
                                            message.getData1(),
                                            message.getData2());
    }
    indicate(interface, port, Direction::out, message.getType());
}

void MidiOutput::send(MidiInterface::Type interface,
                      uint8_t port,
                      SysexBlock &sysexBlock)
{
    MidiInterface::get(interface)->sendSysEx(port, sysexBlock);
}

void MidiOutput::sendControlChange(MidiInterface::Type interface,
                                   uint8_t port,
                                   uint8_t channel,
                                   uint8_t parameterNumber,
                                   uint8_t value)
{
    MidiMessage message(
        channel, MidiMessage::Type::ControlChange, parameterNumber, value);
    sendMessage(interface, port, message);
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
    MidiMessage message(
        channel, MidiMessage::Type::ProgramChange, programNumber, 0);
    sendMessage(interface, port, message);
    indicate(interface, port, Direction::out, MidiMessage::Type::ProgramChange);
}

void MidiOutput::sendSysEx(MidiInterface::Type interface,
                           uint8_t port,
                           SysexBlock &sysexBlock)
{
    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
    indicate(
        interface, port, Direction::out, MidiMessage::Type::SystemExclusive);
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

void MidiOutput::sendSysExPartial(MidiInterface::Type interface,
                                  uint8_t port,
                                  const uint8_t *data,
                                  uint16_t dataLength,
                                  bool complete)
{
    MidiInterface::get(interface)->sendSysExPartial(
        port, data, dataLength, complete);
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

void MidiOutput::enableThru(MidiInterface::Type interface,
                            uint8_t port,
                            bool shouldBeEnabled)
{
    if (interface == MidiInterface::Type::MidiIo) {
        MidiInterface::get(interface)->enableThru(port, shouldBeEnabled);
    }
    // other interfaces are not implmented on purpose
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
