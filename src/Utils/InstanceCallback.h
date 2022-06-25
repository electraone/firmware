#pragma once

// Wrapper for running class instance functions as tasks
#include <functional>

template <typename T>
struct InstanceCallback;

template <typename Ret, typename... Params>
struct InstanceCallback<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args)
    {
        return callbackFunction(args...);
    }
    static std::function<Ret(Params...)> callbackFunction;
};

template <typename Ret, typename... Params>
std::function<Ret(Params...)>
    InstanceCallback<Ret(Params...)>::callbackFunction;
