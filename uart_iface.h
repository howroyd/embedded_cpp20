#pragma once

#include <span>
#include <utility>

#include "uart_api.h"

namespace UART
{

class Iface
{
    using h_impl_t = int;
    h_impl_t h;

public:
    Iface(UART_ID id, UartConfig config = UartConfig{})
    {
        // Check if this ID is already constructed somewhere
        // If so, check the config is the same
            // If not, error (is_valid = false)
            // If so, return a copy to the same instance (is_valid = true)
        // If not, construct it (is_valid = true)
    }

    operator bool() const { return h ? true : false; }

    bool deinit()
    {
        return h->deinit();
    }

    template <class T>
    bool send(std::span<const T> data)
    {
        return h->send(std::foward<std::span<const T>>(data));
    }

    template <class T>
    bool receive(std::span<T> data)
    {
        return h->receive(std::forward<std::span<T>>(data));
    }

    UART_ID     id()     const { return h->id; }
    UartConfig  config() const { return h->cfg; }
};







} // namespace UART