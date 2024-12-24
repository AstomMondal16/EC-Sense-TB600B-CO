#include "tb600b_co_driver.h"

void app_main() {
    TB600B_CO sensor;
    tb600b_co_init(&sensor, UART_NUM_1, GPIO_NUM_17, GPIO_NUM_16);
    
    // Example usage
    tb600b_co_change_mode(&sensor);
    uint16_t gas_value = tb600b_co_read_gas(&sensor);
    printf("Gas Value: %d\n", gas_value);

    tb600b_co_read_all(&sensor);
    tb600b_co_change_mode(&sensor); // Switch back to passive mode
}

