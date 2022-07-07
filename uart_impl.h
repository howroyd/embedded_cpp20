#pragma once

#include <cstdint>
#include <mutex>
#include <utility>

#include "uart_api.h"

namespace UART
{

class Impl
{
    UART_ID id;
    UartConfig cfg;
    bool initialised = false;
    mutable std::mutex mutx{};

public:

    Impl() = delete;
    Impl(UART_ID uart_id, UartConfig uart_config)
        : id{uart_id}, cfg{uart_config}
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
        return api_uart_send(id, const uint8_t* buf, uint16_t len);
    }

    template <class T>
    bool receive(std::span<T> data)
    {
        uint16_t len = std::min(data.size_bytes(), UINT16_MAX);

        if (not len)
            return false;

        std::scoped_lock lock(mutx);
        return api_uart_receive(id, reinterpret_cast<uint8_t*>(data.data()), &len);
    }

};






} // namespace UART