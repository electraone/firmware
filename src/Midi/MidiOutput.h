#pragma once

#include "MidiMessage.h"
#include "MidiMessageTransport.h"
#include "CircularBuffer.h"

class MidiOutput : public MidiJack
{
public:
    MidiOutput(MidiInterface::Type newInterface, uint8_t newPort);
    MidiOutput(MidiInterface::Type newInterface,
               uint8_t newPort,
               uint8_t newChannel,
               uint16_t newRate);
    virtual ~MidiOutput() = default;

    void setPort(uint8_t newPort);
    uint8_t getPort(void) const;
    void setChannel(uint8_t newChannel);
    uint8_t getChannel(void) const;
    void setRate(uint16_t newRate);
    uint16_t getRate(void) const;
    void setTsLastMessage(void);
    bool isReady(void);

    // General MIDI messaging (outgoing)
    static void sendMessage(MidiInterface::Type interface,
                            uint8_t port,
                            MidiMessage &message);

    static bool isIdenticalChange(const MidiMessageTransport &m1,
                                  const MidiMessageTransport &m2);

    static void addToQueue(MidiInterface::Type interface,
                           uint8_t port,
                           MidiMessage &message);

    static void send(MidiInterface::Type interface,
                     uint8_t port,
                     MidiMessage::Type type,
                     uint8_t channel,
                     uint8_t data1,
                     uint8_t data2);
    static void
        send(MidiInterface::Type interface, uint8_t port, MidiMessage &message);
    static void send(MidiInterface::Type interface,
                     uint8_t port,
                     SysexBlock &sysexBlock);
    static void sendControlChange(MidiInterface::Type interface,
                                  uint8_t port,
                                  uint8_t channel,
                                  uint8_t parameterNumber,
                                  uint8_t value);
    static void sendNoteOn(MidiInterface::Type interface,
                           uint8_t port,
                           uint8_t channel,
                           uint8_t noteNumber,
                           uint8_t velocity);
    static void sendNoteOff(MidiInterface::Type interface,
                            uint8_t port,
                            uint8_t channel,
                            uint8_t noteNumber,
                            uint8_t velocity);
    static void sendStart(MidiInterface::Type interface, uint8_t port);
    static void sendStop(MidiInterface::Type interface, uint8_t port);
    static void sendTuneRequest(MidiInterface::Type interface, uint8_t port);
    static void sendProgramChange(MidiInterface::Type interface,
                                  uint8_t port,
                                  uint8_t channel,
                                  uint8_t programNumber);
    static void sendSysEx(MidiInterface::Type interface,
                          uint8_t port,
                          SysexBlock &sysexBlock);
    static void sendSysEx(MidiInterface::Type interface,
                          uint8_t port,
                          uint8_t *data,
                          uint16_t dataLength);
    static void sendSysExPartial(MidiInterface::Type interface,
                                 uint8_t port,
                                 const uint8_t *data,
                                 uint16_t dataLength,
                                 bool complete);
    static void sendPitchBend(MidiInterface::Type interface,
                              uint8_t port,
                              uint8_t channel,
                              uint16_t value);
    static void sendAfterTouchPoly(MidiInterface::Type interface,
                                   uint8_t port,
                                   uint8_t channel,
                                   uint8_t noteNumber,
                                   uint8_t pressure);
    static void sendAfterTouchChannel(MidiInterface::Type interface,
                                      uint8_t port,
                                      uint8_t channel,
                                      uint8_t pressure);
    static void sendClock(MidiInterface::Type interface, uint8_t port);
    static void sendContinue(MidiInterface::Type interface, uint8_t port);
    static void sendActiveSensing(MidiInterface::Type interface, uint8_t port);
    static void sendSystemReset(MidiInterface::Type interface, uint8_t port);
    static void sendSongSelect(MidiInterface::Type interface,
                               uint8_t port,
                               uint8_t song);
    static void sendSongPosition(MidiInterface::Type interface,
                                 uint8_t port,
                                 uint16_t beats);
    static void sendSysEx(MidiInterface::Type interface,
                          uint8_t port,
                          std::vector<uint8_t> data);
    static void sendNrpn(MidiInterface::Type interface,
                         uint8_t port,
                         uint8_t channel,
                         uint16_t parameterNumber,
                         uint16_t midiValue,
                         bool lsbFirst);
    static void sendRpn(MidiInterface::Type interface,
                        uint8_t port,
                        uint8_t channel,
                        uint16_t parameterNumber,
                        uint16_t midiValue);
    static void sendControlChange14Bit(MidiInterface::Type interface,
                                       uint8_t port,
                                       uint8_t channel,
                                       uint16_t parameterNumber,
                                       uint16_t midiValue,
                                       bool lsbFirst);

    static void enableThru(MidiInterface::Type interface,
                           uint8_t port,
                           bool shouldBeEnabled);

    static constexpr uint16_t queueSize = 64;
    static CircularBuffer<MidiMessageTransport, queueSize> outgoingQueue;

private:
    static void indicate(MidiInterface::Type interface,
                         uint8_t port,
                         Direction direction,
                         MidiMessage::Type msgType);
    uint8_t channel;
    uint16_t rate;
    uint32_t tsLastMessage;
};
