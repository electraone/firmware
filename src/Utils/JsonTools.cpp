#include "JsonTools.h"
#include "System.h"

bool findElement(File &file,
                 const char *elementName,
                 ElementType elementType,
                 size_t endPosition)
{
    size_t startPosition = file.position();
    size_t range = endPosition - startPosition;

    //System::logger.write(LOG_ERROR, "findElement (%s): start: %d, end: %d, length: %d", elementName, startPosition, endPosition, range);

    if (endPosition == 0) {
        if (file.find(elementName) == false) {
#ifdef DEBUG
            System::logger.write(
                LOG_ERROR, "findElement: no %s defined", elementName);
#endif /* DEBUG */
            return (false);
        }
    } else {
        if (file.findConstrained(elementName, range) == false) {
#ifdef DEBUG
            System::logger.write(
                LOG_ERROR, "findElement: no %s defined", elementName);
#endif /* DEBUG */
            return (false);
        }
    }
    if (file.find(":") == false) {
#ifdef DEBUG
        System::logger.write(
            LOG_ERROR, "findElement: %s is not an element", elementName);
#endif /* DEBUG */
        return (false);
    }
    if (elementType == ARRAY) {
        if (file.find("[") == false) {
#ifdef DEBUG
            System::logger.write(
                LOG_ERROR, "findElement: %s is not an array", elementName);
#endif /* DEBUG */
            return (false);
        }
    } else if (elementType == OBJECT) {
        if (file.find("{") == false) {
#ifdef DEBUG
            System::logger.write(
                LOG_ERROR, "findElement: %s is not an object", elementName);
#endif /* DEBUG */
            return (false);
        }
    }

    if ((endPosition != 0) && (file.position() > endPosition)) {
#ifdef DEBUG
        System::logger.write(
            LOG_ERROR,
            "findElement: not present in the range: element=%s, endPosition=%d",
            elementName,
            endPosition);
#endif /* DEBUG */
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
        System::logger.write(
            LOG_ERROR, "Preset::isElementEmpty: rewind to original position");
    }

    return (rc);
}
