#pragma once

#include <cstdint>
#include <mutex>
#include <utility>

#include "uart_api.h"

namespace UART
{

class Impl
{
    UART_ID _id;
    UartConfig _cfg;
    bool initialised = false;

public:
    mutable std::mutex mutx{};

    Impl() = delete;
    Impl(UART_ID uart_id, UartConfig uart_config)
        : _id{uart_id}, _cfg{uart_config}
    {
        initialised = init();
    }
    ~Impl()
    {
        if (initialised)
            deinit();
    }

    bool init()
    {
        std::scoped_lock lock(mutx);
        return api_uart_init();
    }
    bool deinit()
    {
        std::scoped_lock lock(mutx);
        return api_uart_deinit();
    }

    template <class T>
    bool send(std::span<const T> data)
    {
        const uint16_t len = data.size_bytes() <= UINT16_MAX ?
                                static_cast<uint16_t>(data.size_bytes()) : 0;

        if (not len)
            return false;

        std::scoped_lock lock(mutx);
        return api_uart_send(_id, reinterpret_cast<const uint8_t*>(data.data()), len);
    }

    template <class T>
    bool receive(std::span<T> data)
    {
        uint16_t len = std::min(data.size_bytes(), UINT16_MAX);

        if (not len)
            return false;

        std::scoped_lock lock(mutx);
        return api_uart_receive(_id, reinterpret_cast<uint8_t*>(data.data()), &len);
    }

    UART_ID     id()     const { return _id; }
    UartConfig  config() const { return _cfg; }

};






} // namespace UART