#pragma once

typedef uint8_t handle_t;

#include "Component.h"

struct Handle {
public:
    Handle() : component(nullptr), id(0)
    {
    }

    Handle(Component *component) : component(component), id(0)
    {
    }

    Handle(Component *component, handle_t id) : component(component), id(id)
    {
    }

    Component *component;
    handle_t id;
};
