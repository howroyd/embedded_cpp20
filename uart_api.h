#pragma once

#include <cstdint>
#include <cstdint>
#include <numeric>
#include <iostream>
#include <thread>
#include <chrono>

enum UART_ID
{
    UART0, UART1, UART_INVALID
};

static constexpr std::size_t uart_n_hw_ports = 2;

static_assert( (uart_n_hw_ports - 1) < static_cast<std::size_t>(UART_ID::UART_INVALID) );

struct UartConfig
{
    int baud = 115200;
    int bits = 8;
    bool parity = false;
    int stop_bits = 1;
    bool flow_control = false;
    bool is_hw_instance = false;

    friend constexpr bool operator<=>(const UartConfig&, const UartConfig&) = default;
};

inline bool api_uart_init()
{
    return true;
}

inline bool api_uart_deinit()
{
    return true;
}

inline bool api_uart_send(UART_ID id, const uint8_t* buf, uint16_t len)
{
    using namespace std::chrono_literals;
    using std::cout;

    cout << __func__ << " to UART" << static_cast<int>(id) << "\n";
    for (uint16_t idx = 0 ; idx < len ; ++idx)
    {
        cout << static_cast<int>(buf[idx]) << '\t';
        //buf[idx] = 0;
        std::this_thread::sleep_for(1ms);
    }
    cout << "\n";
    return true;
}

inline bool api_uart_receive(UART_ID id, uint8_t* buf, uint16_t* len)
{
    std::cout << __func__ << " from UART" << static_cast<int>(id) << "\n";

    if (*len > 2)
        *len  = *len - 1;

    std::iota(buf, buf + *len, 16);
    return true;
}