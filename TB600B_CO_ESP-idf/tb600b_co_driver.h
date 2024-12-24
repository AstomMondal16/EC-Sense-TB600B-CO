#ifndef TB600B_CO_DRIVER_H
#define TB600B_CO_DRIVER_H

#include <stdbool.h>
#include "driver/uart.h"

typedef struct {
    uart_port_t uart;
    bool qna;
} TB600B_CO;

// Function declarations
void tb600b_co_init(TB600B_CO *sensor, uart_port_t uart_num, int tx_pin, int rx_pin);
void tb600b_co_change_mode(TB600B_CO *sensor);
void tb600b_co_led_off(TB600B_CO *sensor);
void tb600b_co_led_on(TB600B_CO *sensor);
uint16_t tb600b_co_read_gas(TB600B_CO *sensor);
void tb600b_co_read_all(TB600B_CO *sensor);

#endif // TB600B_CO_DRIVER_H

