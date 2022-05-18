#pragma once
#include "LocalFile.h"

bool findElement(File &file,
                 const char *elementName,
                 ElementType elementType,
                 size_t endPosition = 0);
bool isElementEmpty(File &file);
