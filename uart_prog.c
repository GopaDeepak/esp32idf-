#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp-system.h"
#include "esp-log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

static const int RX_BUF_SIZE =128;

#define TXD_PIN(GPIO_NUM_17);
#define RXD_PIN(GPIO_NUM_16);
#define UART UART_NUM_2

int num=0;
void init(void){

const QueueHandle_1 uart_queue
const uart_config_t uart_config ={
	

	.baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
        .source_clk = UART_SCLK_APB,

};

    uart_driver_install(UART, RX_BUF_SIZE * 2, 10, 0, QueueHandle_1,0);
    uart_param_config(UART, &uart_config);
    uart_set_pin(UART, TXD_PIN, RXD_PIN,UART_RTS,UART_CTS);

static void tx_task(void *arg)
{
	char* Txdata = (char*) malloc(100);
    while (1) {
    	sprintf (Txdata, "Hello index = %d\r\n", num++);
        uart_write_bytes(UART, Txdata, strlen(Txdata));
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
	free(Txdata);
}
static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART, data, RX_BUF_SIZE, 500 /portTICK_RATE_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
        }
    }
    free(data);
}


void app_main(void)
{
    init();
    xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);//The stack size for both the tasks is set to 2 Kilobytes.
    xTaskCreate(tx_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES-2, NULL);
}












