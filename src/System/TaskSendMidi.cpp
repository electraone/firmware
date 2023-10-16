/*
* Electra One MIDI Controller Firmware
* See COPYRIGHT file at the top of the source tree.
*
* This product includes software developed by the
* Electra One Project (http://electra.one/).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
*/

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
            ERROR,
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
