#include "JsonTools.h"

bool findElement(File &file,
                 const char *elementName,
                 ElementType elementType,
                 size_t endPosition)
{
    if (file.find(elementName) == false) {
        logMessage("findElement: no %s defined", elementName);
        return (false);
    }
    if (file.find(":") == false) {
        logMessage("findElement: %s is not an element", elementName);
        return (false);
    }
    if (elementType == ARRAY) {
        if (file.find("[") == false) {
            logMessage("findElement: %s is not an array", elementName);
            return (false);
        }
    } else if (elementType == OBJECT) {
        if (file.find("{") == false) {
            logMessage("findElement: %s is not an object", elementName);
            return (false);
        }
    }

    if ((endPosition != 0) && (file.position() > endPosition)) {
        logMessage(
            "findElement: not present in the range: element=%s, endPosition=%d",
            elementName,
            endPosition);
        return (false);
    }

    return (true);
}

bool isElementEmpty(File &file)
{
    size_t pos = file.position();
    uint8_t c;
    bool rc = false;

    while ((c = file.read()) && (c <= 32)) {
        logChars(&c, 1);
    }

    if ((c == ']') || (c == '}')) {
        rc = true;
    }

    if (file.seek(pos) == false) {
        logMessage("Preset::isElementEmpty: rewind to original position");
    }

    return (rc);
}
