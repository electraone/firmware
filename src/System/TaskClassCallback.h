#pragma once

// Wrapper for running class instance functions as tasks
#include <functional>
#include "TaskSchedulerDeclarations.h"

template <typename T>
struct TaskClassCallback;

template <typename Ret, typename... Params>
struct TaskClassCallback<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args)
    {
        callbackFunction(args...);
    }
    static std::function<Ret(Params...)> callbackFunction;
};

template <typename Ret, typename... Params>
std::function<Ret(Params...)>
    TaskClassCallback<Ret(Params...)>::callbackFunction;
