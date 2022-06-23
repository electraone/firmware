#include "MidiMessage.h"

MidiMessage::MidiMessage()
    : channel(0), data1(0), data2(0), type(MidiMessage::Type::InvalidType)
{
}

MidiMessage::MidiMessage(uint8_t newChannel,
                         Type newType,
                         uint8_t newData1,
                         uint8_t newData2)
    : channel(newChannel), type(newType), data1(newData1), data2(newData2)
{
}

MidiMessage::MidiMessage(const SysexBlock &sysexBlock)
    : channel(0),
      data1(0),
      data2(0),
      type(MidiMessage::Type::SystemExclusive),
      sysexBlock(sysexBlock)
{
}

uint8_t MidiMessage::getChannel(void) const
{
    return (channel);
}

MidiMessage::Type MidiMessage::getType(void) const
{
    return (type);
}

uint8_t MidiMessage::getData1(void) const
{
    return (data1);
}

uint8_t MidiMessage::getData2(void) const
{
    return (data2);
}

const char *MidiMessage::getDescription(void) const
{
    if (type == MidiMessage::Type::NoteOff) {
        return ("NoneOff");
    } else if (type == MidiMessage::Type::NoteOn) {
        return ("NoneOn");
    } else if (type == MidiMessage::Type::AfterTouchPoly) {
        return ("AfterTouchPoly");
    } else if (type == MidiMessage::Type::ControlChange) {
        return ("ControlChange");
    } else if (type == MidiMessage::Type::ProgramChange) {
        return ("ProgramChange");
    } else if (type == MidiMessage::Type::AfterTouchChannel) {
        return ("AfterTouchChannel");
    } else if (type == MidiMessage::Type::PitchBend) {
        return ("PitchBend");
    } else if (type == MidiMessage::Type::SystemExclusive) {
        return ("SystemExclusive");
    } else if (type == MidiMessage::Type::TimeCodeQuarterFrame) {
        return ("TimeCodeQuarterFrame");
    } else if (type == MidiMessage::Type::SongPosition) {
        return ("SongPosition");
    } else if (type == MidiMessage::Type::SongSelect) {
        return ("SongSelect");
    } else if (type == MidiMessage::Type::TuneRequest) {
        return ("TuneRequest");
    } else if (type == MidiMessage::Type::Clock) {
        return ("Clock");
    } else if (type == MidiMessage::Type::Start) {
        return ("Start");
    } else if (type == MidiMessage::Type::Continue) {
        return ("Continue");
    } else if (type == MidiMessage::Type::Stop) {
        return ("Stop");
    } else if (type == MidiMessage::Type::ActiveSensing) {
        return ("ActiveSensing");
    } else if (type == MidiMessage::Type::SystemReset) {
        return ("SystemReset");
    }

    return ("Unknown");
}

bool MidiMessage::isController(void) const
{
    return (type == MidiMessage::Type::ControlChange);
}

uint8_t MidiMessage::getControllerNumber(void) const
{
    return (data1);
}

uint8_t MidiMessage::getControllerValue(void) const
{
    return (data2);
}

MidiMessage MidiMessage::controllerEvent(uint8_t channel,
                                         uint8_t controllerType,
                                         uint8_t value)
{
    return (MidiMessage(channel,
                        MidiMessage::Type::ControlChange,
                        controllerType & 0x7F,
                        value & 0x7F));
}

bool MidiMessage::isNote(void) const
{
    return (isNoteOn() || isNoteOff());
}

bool MidiMessage::isNoteOn(void) const
{
    return (type == MidiMessage::Type::NoteOn);
}

MidiMessage
    MidiMessage::noteOn(uint8_t channel, uint8_t noteNumber, uint8_t velocity)
{
    return (MidiMessage(channel,
                        MidiMessage::Type::NoteOn,
                        noteNumber & 0x7F,
                        velocity & 0x7F));
}

bool MidiMessage::isNoteOff(void) const
{
    return ((type == MidiMessage::Type::NoteOff)
            || ((type == MidiMessage::Type::NoteOn) && (data2 == 0)));
}

MidiMessage
    MidiMessage::noteOff(uint8_t channel, uint8_t noteNumber, uint8_t velocity)
{
    return (MidiMessage(channel,
                        MidiMessage::Type::NoteOff,
                        noteNumber & 0x7F,
                        velocity & 0x7F));
}

MidiMessage MidiMessage::noteOff(uint8_t channel, uint8_t noteNumber)
{
    return (
        MidiMessage(channel, MidiMessage::Type::NoteOff, noteNumber & 0x7F, 0));
}

bool MidiMessage::isNoteOnOrOff(void) const
{
    return ((type == MidiMessage::Type::NoteOn)
            || (type == MidiMessage::Type::NoteOff));
}

uint8_t MidiMessage::getNoteNumber() const
{
    return (data1);
}

void MidiMessage::setNoteNumber(uint8_t newNoteNumber)
{
    data1 = newNoteNumber & 0x7F;
}

uint8_t MidiMessage::getVelocity(void) const
{
    return (data2);
}

void MidiMessage::setVelocity(uint8_t newVelocity)
{
    data2 = newVelocity & 0x7F;
}

bool MidiMessage::isProgramChange(void) const
{
    return (type == MidiMessage::Type::ProgramChange);
}

uint8_t MidiMessage::getProgramChangeNumber(void) const
{
    return (data1);
}

MidiMessage MidiMessage::programChange(uint8_t channel, uint8_t programNumber)
{
    return (MidiMessage(
        channel, MidiMessage::Type::ProgramChange, programNumber & 0x7F, 0));
}

bool MidiMessage::isPitchWheel(void) const
{
    return (type == MidiMessage::Type::PitchBend);
}

uint16_t MidiMessage::getPitchWheelValue(void) const
{
    return (data1 | (data2 << 7));
}

MidiMessage MidiMessage::pitchWheel(uint8_t channel, uint16_t position)
{
    return (MidiMessage(channel,
                        MidiMessage::Type::PitchBend,
                        position & 0x7F,
                        (position >> 7) & 0x7F));
}

bool MidiMessage::isAftertouch(void) const
{
    return (type == MidiMessage::Type::AfterTouchPoly);
}

uint8_t MidiMessage::getAfterTouchNote(void) const
{
    return (data1);
}

uint8_t MidiMessage::getAfterTouchValue(void) const
{
    return (data2);
}

MidiMessage MidiMessage::aftertouchChange(uint8_t channel,
                                          uint8_t noteNumber,
                                          uint8_t aftertouchAmount)
{
    return (MidiMessage(channel,
                        MidiMessage::Type::AfterTouchPoly,
                        noteNumber & 0x7F,
                        aftertouchAmount & 0x7F));
}

bool MidiMessage::isChannelPressure(void) const
{
    return (type == MidiMessage::Type::AfterTouchChannel);
}

uint8_t MidiMessage::getChannelPressureValue(void) const
{
    return (data1);
}

MidiMessage MidiMessage::channelPressureChange(uint8_t channel,
                                               uint8_t pressure)
{
    return (MidiMessage(
        channel, MidiMessage::Type::AfterTouchChannel, pressure & 0x7F, 0));
}

bool MidiMessage::isMidiStart(void) const
{
    return (type == MidiMessage::Type::Start);
}

MidiMessage MidiMessage::midiStart(void)
{
    return (MidiMessage(0, MidiMessage::Type::Start, 0, 0));
}

bool MidiMessage::isMidiContinue(void) const
{
    return (type == MidiMessage::Type::Continue);
}

MidiMessage MidiMessage::midiContinue(void)
{
    return (MidiMessage(0, MidiMessage::Type::Continue, 0, 0));
}

bool MidiMessage::isMidiStop(void) const
{
    return (type == MidiMessage::Type::Stop);
}

MidiMessage MidiMessage::midiStop(void)
{
    return (MidiMessage(0, MidiMessage::Type::Stop, 0, 0));
}

bool MidiMessage::isMidiClock() const
{
    return (type == MidiMessage::Type::Clock);
}

MidiMessage MidiMessage::midiClock(void)
{
    return (MidiMessage(0, MidiMessage::Type::Clock, 0, 0));
}

bool MidiMessage::isMidiTuneRequest(void) const
{
    return (type == MidiMessage::Type::TuneRequest);
}

MidiMessage MidiMessage::midiTuneRequest(void)
{
    return (MidiMessage(0, MidiMessage::Type::TuneRequest, 0, 0));
}

bool MidiMessage::isMidiActiveSensing(void) const
{
    return (type == MidiMessage::Type::ActiveSensing);
}

MidiMessage MidiMessage::midiActiveSensing(void)
{
    return (MidiMessage(0, MidiMessage::Type::ActiveSensing, 0, 0));
}

bool MidiMessage::isMidiSystemReset(void) const
{
    return (type == MidiMessage::Type::SystemReset);
}

MidiMessage MidiMessage::midiSystemReset(void)
{
    return (MidiMessage(0, MidiMessage::Type::SystemReset, 0, 0));
}

bool MidiMessage::isSongPositionPointer(void) const
{
    return (type == MidiMessage::Type::SongPosition);
}

uint16_t MidiMessage::getSongPositionPointerMidiBeat(void) const
{
    return (data1 | (data2 << 7));
}

MidiMessage MidiMessage::songPositionPointer(uint16_t positionInMidiBeats)
{
    return (MidiMessage(0,
                        MidiMessage::Type::SongPosition,
                        positionInMidiBeats & 0x7F,
                        (positionInMidiBeats >> 7) & 0x7F));
}

bool MidiMessage::isSongSelect(void) const
{
    return (type == MidiMessage::Type::SongSelect);
}

uint16_t MidiMessage::getSelectedSong(void) const
{
    return (data1);
}

MidiMessage MidiMessage::songSelect(uint8_t song)
{
    return (MidiMessage(0, MidiMessage::Type::SongSelect, song & 0x7F, 0));
}

bool MidiMessage::isQuarterFrame(void) const
{
    return (type == MidiMessage::Type::TimeCodeQuarterFrame);
}

uint8_t MidiMessage::getQuarterFrameSequenceNumber(void) const
{
    return (data1 >> 4);
}

uint8_t MidiMessage::getQuarterFrameValue(void) const
{
    return (data1 & 0x0F);
}

MidiMessage MidiMessage::quarterFrame(uint8_t sequenceNumber, uint8_t value)
{
    return (MidiMessage(0,
                        MidiMessage::Type::TimeCodeQuarterFrame,
                        ((sequenceNumber << 4) | value) & 0x7F,
                        0));
}

bool MidiMessage::isSysEx(void) const
{
    return (type == MidiMessage::Type::SystemExclusive);
}

uint32_t MidiMessage::getSysExDataSize(void) const
{
    if (type == MidiMessage::Type::SystemExclusive) {
        return (sysexBlock.getLength());
    }
    return (0);
}

const SysexBlock &MidiMessage::getSysExBlock(void) const
{
    return (sysexBlock);
}

size_t MidiMessage::readSysExData(uint8_t *buffer, size_t length)
{
    sysexBlock.seek(0); // always read the message from the beginning

    size_t totalReadBytes = 0;
    size_t readBytes = 0;
    size_t offset = 0;

    while ((readBytes = sysexBlock.readBytes(buffer + offset, length - offset))
           != 0) {
        offset += readBytes;
        totalReadBytes += readBytes;
    }
    return (totalReadBytes);
}

// \todo this has been added to support External MIDI control of the Controller
// app. It needs to be made more general or moved to the app.
MidiMessage::Type MidiMessage::translateType(const char *typeText)
{
    if (typeText) {
        if (strcmp(typeText, "cc7") == 0) {
            return (MidiMessage::Type::ControlChange);
        } else if (strcmp(typeText, "note") == 0) {
            return (MidiMessage::Type::NoteOn);
        } else if (strcmp(typeText, "program") == 0) {
            return (MidiMessage::Type::ProgramChange);
        }
    }
    return (MidiMessage::Type::InvalidType);
}

const char *MidiMessage::translateTypeToText(MidiMessage::Type messageType)
{
    if (messageType == MidiMessage::Type::ControlChange) {
        return "cc7";
    } else if (messageType == MidiMessage::Type::NoteOn) {
        return "note";
    } else if (messageType == MidiMessage::Type::ProgramChange) {
        return "program";
    }

    return ("unknown");
}
