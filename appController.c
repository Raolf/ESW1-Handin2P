#include "FreeRTOS.h"
#include "appController.h"
#include "event_groups.h"
#include "semphr.h"
#include "param.h"
#include "message_buffer.h""

#define BIT_0 ( 1<<0 )
#define BIT_4 ( 1<<4 )

void controllerTask(void* parameters) {
	
	Param param = (Param) parameters;
	MessageBufferHandle_t xMessageBuffer = param->xMessageBuffer1;
	SemaphoreHandle_t semaphore = param->semaphore1;
	EventGroupHandle_t xEventGroup = param->eventGroup1;
	char transfer [11] = "S1 x\nS2 x\0";

	EventBits_t uxBits = xEventGroupWaitBits(
		xEventGroup,
		BIT_0 |
		BIT_4,
		pdTRUE,
		pdTRUE,
		500);

	while (1) {
			
		if ((uxBits & (BIT_0 | BIT_4)) == (BIT_0 | BIT_4)) {
			if (getData(param->sensor1) >= 9) {
				if (xSemaphoreTake(semaphore, 500)) {
					printf("Number > 9");
					xSemaphoreGive(semaphore);
				}	
			}
			transfer[3] = getData(param->sensor1);
			transfer[8] = getData(param->sensor2);
			xMessageBufferSend(xMessageBuffer, &transfer, sizeof(transfer), 100);
			xEventGroupClearBits(
				xEventGroup,
				BIT_0 |
				BIT_4);
		}
		else {

		}
	}
	vTaskDelete(NULL);
}

void sendPacket() {

}
