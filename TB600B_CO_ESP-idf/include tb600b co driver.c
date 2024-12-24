#include "tb600b_co_driver.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Commands
static const uint8_t ACTIVE_MODE[] = {0xFF, 0x01, 0x78, 0x40, 0x00, 0x00, 0x00, 0x00, 0x47};
static const uint8_t PASSIVE_MODE[] = {0xFF, 0x01, 0x78, 0x41, 0x00, 0x00, 0x00, 0x00, 0x46};
static const uint8_t READ_GAS[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
static const uint8_t READ_ALL[] = {0xFF, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78};
static const uint8_t LIGHT_OFF[] = {0xFF, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77};
static const uint8_t LIGHT_ON[] = {0xFF, 0x01, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76};

void tb600b_co_init(TB600B_CO *sensor, uart_port_t uart_num, int tx_pin, int rx_pin) {
    sensor->uart = uart_num;
    sensor->qna = true;

    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_driver_install(sensor->uart, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(sensor->uart, &uart_config);
    uart_set_pin(sensor->uart, tx_pin, rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

static void send_command(TB600B_CO *sensor, const uint8_t *cmd, size_t length) {
    uart_write_bytes(sensor->uart, (const char *)cmd, length);
    vTaskDelay(pdMS_TO_TICKS(1000)); // Processing time
}

static int read_response(TB600B_CO *sensor, uint8_t *data, size_t length) {
    return uart_read_bytes(sensor->uart, data, length, 20 / portTICK_RATE_MS);
}

void tb600b_co_change_mode(TB600B_CO *sensor) {
    if (sensor->qna) {
        send_command(sensor, ACTIVE_MODE, sizeof(ACTIVE_MODE));
        sensor->qna = false;
    } else {
        send_command(sensor, PASSIVE_MODE, sizeof(PASSIVE_MODE));
        sensor->qna = true;
    }
}

void tb600b_co_led_off(TB600B_CO *sensor) {
    send_command(sensor, LIGHT_OFF, sizeof(LIGHT_OFF));
}

void tb600b_co_led_on(TB600B_CO *sensor) {
    send_command(sensor, LIGHT_ON, sizeof(LIGHT_ON));
}

uint16_t tb600b_co_read_gas(TB600B_CO *sensor) {
    uint8_t response[10]; // Adjust size based on expected response
    if (sensor->qna) {
        send_command(sensor, READ_GAS, sizeof(READ_GAS));
        read_response(sensor, response, sizeof(response));
        return (response[2] << 8) | response[3];
    }
    return 0; // Error handling can be improved
}

void tb600b_co_read_all(TB600B_CO *sensor) {
    uint8_t response[12]; // Adjust size based on expected response
    if (!sensor->qna) {
        printf("You should be in QnA mode to do this.\n");
        return;
    }
    send_command(sensor, READ_ALL, sizeof(READ_ALL));
    read_response(sensor, response, sizeof(response));

    uint16_t gas_ugm3 = (response[2] << 8) | response[3];
    uint16_t gas_ppb = (response[6] << 8) | response[7];
    float temperature = ((response[8] << 8) | response[9]) / 100.0;
    float humidity = ((response[10] << 8) | response[11]) / 100.0;

    printf("Gas (ug/m3): %d\n", gas_ugm3);
    printf("Gas (ppb): %d\n", gas_ppb);
    printf("Temperature: %.2f\n", temperature);
    printf("Humidity: %.2f\n", humidity);
}

