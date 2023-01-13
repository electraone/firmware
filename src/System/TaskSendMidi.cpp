#include "TaskSendMidi.h"
#include "MidiOutput.h"

/*
 * Send queued messages
 */
void sendMidi(void)
{
    MidiMessageTransport message;
    uint32_t idFirst = 0;
    uint8_t i = 0;

    // Process max 10 entries in the queue
    while (!MidiOutput::outgoingQueue.isEmpty() && i < 10) {
        message = MidiOutput::outgoingQueue.first();

        if (message.invalid) {
            MidiOutput::outgoingQueue.shift();
            continue;
        }

        if (idFirst == message.id) {
            break;
        }

        if (i == 0) {
            idFirst = message.id;
        }

        message = MidiOutput::outgoingQueue.shift();

        if (message.getType() == MidiMessage::Type::SystemExclusive) {
            MidiOutput::send(
                message.getInterfaceType(), message.getPort(), message);
        } else {
            MidiOutput::send(message.getInterfaceType(),
                             message.getPort(),
                             message.getType(),
                             message.getChannel(),
                             message.getData1(),
                             message.getData2());
        }

#ifdef DEBUG
        System::logger.write(
            "sendMidi: queued message (%d): interface:%d, port:%d, channel: %d,"
            " parameterNumber: %d, value=%d, type=%s",
            message.id,
            message.getInterfaceType(),
            message.getPort(),
            message.getChannel(),
            message.getData1(),
            message.getData2(),
            message.getDescription());
#endif
        i++;
    }
}
