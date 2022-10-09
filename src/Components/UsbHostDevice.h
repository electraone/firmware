#pragma once

#include "Component.h"

enum class uiState { ENABLED, SELECTED, DISABLED };

class UsbHostDevice : public Component
{
public:
    UsbHostDevice(const char *newProduct,
                  const char *newManufacturer,
                  const char *newSerial,
                  uint8_t newPortNumber)
        : product(newProduct),
          manufacturer(newManufacturer),
          serial(newSerial),
          portNumber(newPortNumber),
          isDisabled(false),
          dimmed(false),
          active(false)
    {
    }

    virtual ~UsbHostDevice() = default;

    std::function<void(uint8_t)> onClick;

    void onTouchDown(const TouchEvent &touchEvent) override
    {
        uint16_t x = touchEvent.getX();
        uint16_t y = touchEvent.getY();

        if (!isDisabled) {
            if ((y > 0) && (y < getHeight())) {
                if ((x > 330) && (x < 470)) {
                    onClick(0);
                } else if ((x > 470) && (x < 610)) {
                    onClick(1);
                } else if ((x > 610) && (x < 740)) {
                    onClick(2);
                }
            }
        }
    }

    void usbDevice(Graphics &g,
                   uint16_t x,
                   uint16_t y,
                   uiState state,
                   const char *product,
                   const char *manufacturer,
                   const char *serial)
    {
        g.setBteChromaColor(Colours565::red);

        if (strcmp(product, "Electra Controller") == 0) {
            g.copyFromToolkit(821, 79, x, y, 92, 78);
        } else {
            g.copyFromToolkit(922, 79, x + 14, y, 78, 78);
        }

        if (product[0] != '\0') {
            g.printText(x + 114,
                        y,
                        product,
                        TextStyle::mediumWhiteOnBlack,
                        200,
                        TextAlign::left,
                        2);
            g.printText(x + 114,
                        y + 25,
                        manufacturer,
                        TextStyle::smallWhiteOnBlack,
                        200,
                        TextAlign::left,
                        1);
            g.printText(x + 114,
                        y + 45,
                        serial,
                        TextStyle::smallWhiteOnBlack,
                        200,
                        TextAlign::left,
                        1);
            setDisabled(false);
        } else {
            g.printText(x + 114,
                        y,
                        "NO DEVICE CONNECTED",
                        TextStyle::mediumWhiteOnBlack,
                        200,
                        TextAlign::left,
                        2);
            setDisabled(true);
        }

        if (state == uiState::DISABLED) {
            g.dim(x, y, 300, 78, Colours565::black);
        }
    }

    void usbDevicePorts(Graphics &g,
                        uint16_t x,
                        uint16_t y,
                        uiState state,
                        uint8_t port)
    {
        usbDevicePort(g,
                      x,
                      y,
                      "PORT 1",
                      (port == 0) ? uiState::SELECTED : uiState::ENABLED);
        usbDevicePort(g,
                      x + 120,
                      y,
                      "PORT 2",
                      (port == 1) ? uiState::SELECTED : uiState::ENABLED);
        usbDevicePort(g,
                      x + 240,
                      y,
                      "CTRL",
                      (port == 2) ? uiState::SELECTED : uiState::ENABLED);

        if (state == uiState::DISABLED) {
            g.dim(x, y, 300, 78, Colours565::black);
        }
    }

    void usbDevicePort(Graphics &g,
                       uint16_t x,
                       uint16_t y,
                       const char *label,
                       uiState state)
    {
        // \todo Temporary fix to make sure the Chroma colour is not white
        g.setBteChromaColor(Colours565::red);

        if (state == uiState::SELECTED) {
            g.copyFromToolkit(0, 494, x, y, 100, 40);
            g.printText(x,
                        y + 14,
                        label,
                        TextStyle::mediumBlackOnWhite,
                        100,
                        TextAlign::center);
        } else {
            g.copyFromToolkit(0, 545, x, y, 100, 40);
            g.printText(x,
                        y + 14,
                        label,
                        TextStyle::mediumWhiteOnDimmed,
                        100,
                        TextAlign::center);
        }
    }

    void paint(Graphics &g)
    {
        g.fillAll(Colours565::black);

        usbDevice(g, 0, 0, uiState::ENABLED, product, manufacturer, serial);
        usbDevicePorts(g, 340, 0, uiState::ENABLED, portNumber);

        if (isDisabled == true) {
            g.dimAll();
        }
    }

    void setDisabled(bool shouldBeDisabled)
    {
        // logMessage ("setting disabled %d", shouldBeDisabled);
        isDisabled = shouldBeDisabled;
    }

    void setDimmed(bool shouldBeDimmed)
    {
    }

    void setActive(bool shouldBeActive)
    {
        active = shouldBeActive;
    }

    bool isActive(void) const
    {
        return (active);
    }

    bool isDimmed(void) const
    {
        return (dimmed);
    }

    void setPort(uint8_t newPort)
    {
        portNumber = newPort;
        this->repaint();
    }

private:
    const char *product;
    const char *manufacturer;
    const char *serial;
    uint8_t portNumber;

    bool isDisabled;
    bool dimmed;
    bool active;
};
