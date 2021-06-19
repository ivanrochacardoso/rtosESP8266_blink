#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}


#define GPIO_OUTPUT_IO_0    12
#define GPIO_OUTPUT_IO_1    13
#define GPIO_OUTPUT_IO_2    14
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1)| (1ULL<<GPIO_OUTPUT_IO_2))
//1ULL-->unsigned long long


void task_blink(void* ignore)
{
    gpio16_output_conf();
    while(true) {

        GPIO_OUTPUT_SET(2, 0);
        GPIO_OUTPUT_SET(GPIO_OUTPUT_IO_0, 0);
         GPIO_OUTPUT_SET(GPIO_OUTPUT_IO_1, 1);
         GPIO_OUTPUT_SET(GPIO_OUTPUT_IO_2, 1);

        vTaskDelay(1000/portTICK_RATE_MS);
        GPIO_OUTPUT_SET(2, 1);
        GPIO_OUTPUT_SET(GPIO_OUTPUT_IO_0, 1);
         GPIO_OUTPUT_SET(GPIO_OUTPUT_IO_1, 0);
         GPIO_OUTPUT_SET(GPIO_OUTPUT_IO_2, 0);

        vTaskDelay(1000/portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/


void user_init(void)
{

  printf("TEST BLINK ON GPIO2,12,13,14 ...\n");

	GPIO_ConfigTypeDef io_conf;				 	   //Create a GPIO structure
    io_conf.GPIO_IntrType = GPIO_PIN_INTR_DISABLE;		//Disability interrupt service
    io_conf.GPIO_Mode = GPIO_Mode_Output;			//Set to output mode
    io_conf.GPIO_Pin = GPIO_OUTPUT_PIN_SEL;	 //Specify the GPIO to be configured
    io_conf.GPIO_Pullup = 0;				    //Disable pull down
     
    gpio_config(&io_conf); 




    xTaskCreate(&task_blink, "startup", 2048, NULL, 1, NULL);






}

