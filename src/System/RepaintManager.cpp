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

#include "RepaintManager.h"
#include "Component.h"
#include "Window.h"

RepaintManager::RepaintManager(Graphics &newG, WindowManager &newWm)
    : g(newG), wm(newWm)
{
}

void RepaintManager::scheduleRepaint(Component *component)
{
    invalidateEntries(component);
    repaintQueue.push_back(RepaintRequest(component));
}

void RepaintManager::scheduleClear(const Rectangle &area)
{
    repaintQueue.push_back(RepaintRequest(area));
}

#include "System.h"

void RepaintManager::processRepaintQueue(void)
{
    if (!repaintQueue.empty()) {
        g.switchWriteLayer();
        unfoldNestedComponents();

        uint8_t zLevel = 0;
        for (auto window : wm.getWindows()) {
            if (zLevel == 1) {
                scheduleRepaint(window->getOwnedContent());
                unfoldComponent(window->getOwnedContent());
            }
            repaintQueueEntries(window);
            zLevel++;
        }
        repaintQueue.clear();
        g.showPreparedLayer(0, 0, 1024, 600);
    }
}

void RepaintManager::clearRepaintQueue(void)
{
    repaintQueue.clear();
}

void RepaintManager::invalidateEntries(Component *component)
{
    for (RepaintRequest &request : repaintQueue) {
        if (request.component == component) {
            request.valid = false;
        }
    }
}

void RepaintManager::unfoldNestedComponents()
{
    for (const RepaintRequest &request : repaintQueue) {
        if (request.valid && request.component) {
            Component *component = request.component;

            if (Window *window = dynamic_cast<Window *>(component)) {
                unfoldComponent(window);
                component = window->getOwnedContent();
            }
            unfoldComponent(component);
        }
    }
}

void RepaintManager::unfoldComponent(Component *component)
{
    if (component->isVisible()) {
        component->repaint();

        for (auto &nestedComponent : component->getChildren()) {
            unfoldComponent(nestedComponent);
        }
    }
}

void RepaintManager::repaintQueueEntries(Window *window)
{
    for (RepaintRequest &request : repaintQueue) {
        if (request.valid == false) {
            continue;
        }

        if (request.component) {
            Window *componentWindow = request.component->getWindow();
            if ((componentWindow == nullptr) || (componentWindow == window)) {
                paintComponent(request.component);
                request.valid = false;
            }
        } else {
            clearArea(request.areaToClear);
            request.valid = false;
        }
    }
}

void RepaintManager::paintComponent(Component *component)
{
    g.setActiveWindowPosition(component->getScreenX(), component->getScreenY());
    g.setActiveWindowSize(component->getWidth(), component->getHeight());
    component->paint(g);

    if (component->isDimmed()) {
        g.dim(0,
              0,
              component->getWidth(),
              component->getHeight(),
              Colours565::black);
    }
}

void RepaintManager::clearArea(const Rectangle &area)
{
    g.setColour(0x0000);
    g.fillRect(area);
}