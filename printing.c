#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;

 void task1(void *arg)
{

 while(1){  
printf("hello from task 1 \n");
  vTaskDelay(1000 / portTICK_PERIOD_MS);
   vTaskDelay(pdMS_TO_TICKS(1000));
 }
}

 void task2(void *arg)
{

 for(int i=0;i<5;i++){  

printf("hello from task 2 \n");
    vTaskDelay(pdMS_TO_TICKS(1000));
  vTaskDelay(1000 / portTICK_PERIOD_MS);
 }
}

void app_main()
{

    xTaskCreate(task1, "task1", 4096, NULL, 10, &myTask1Handle);
   xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 10, &myTask2Handle,1);
}
