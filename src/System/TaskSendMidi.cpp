#include "TaskSendMidi.h"
#include "MidiOutput.h"

/*
 * Send queued messages
 */
void sendMidi(void)
{
    MidiMessageTransport message;
    MidiMessageTransport messageFirst;
    uint8_t i = 0;

    // Process max 10 entries in the queue
    while ((MidiOutput::outgoingQueue.isEmpty() != true) && (i < 10)) {
        message = MidiOutput::outgoingQueue.shift();

        // Quit if all the messages have been attempted to send
        /*
        if (messageFirst == message) {
            outgoingQueue.unshift(message);
            break;
        }
        */

        // Mark the first message in this run. We use it to detect that all
        // messages were attempted to be sent
        if (i == 0) {
            messageFirst = message;
        }

        if (1) { // message->outputDevice->isReady()
            MidiOutput::sendMessageNow(message);

            logMessage("sendMidi: queued message: channel: %d,"
                       " parameterNumber: %d, type=%d",
                       message.getChannel(),
                       message.getData1(),
                       message.getDescription());
        } else {
            MidiOutput::outgoingQueue.unshift(message);
            logMessage("sendMidi: device is not ready yet");
        }
        i++;
    }
}
