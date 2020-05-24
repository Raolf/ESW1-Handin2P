#include "FreeRTOS.h"
#include "event_groups.h"
#include "appController.h"
#include "semphr.h"
#include "param.h"
#include "message_buffer.h"
#include "LoraDriver.h"

#define BIT_0 ( 1<<0 )
#define BIT_4 ( 1<<4 )

void controllerTask(void* parameters) {

	//printf("controllerTask\n");

	Param param = (Param) parameters;
	MessageBufferHandle_t xMessageBuffer = param->xMessageBuffer1;
	//SemaphoreHandle_t semaphore = param->semaphore1;
	//EventGroupHandle_t xEventGroup = param->eventGroup1;
	

	xEventGroup = xEventGroupCreate();
	//printf("controllerTask set eventgroup: %p\n", xEventGroup);

	EventBits_t uxBits = xEventGroupWaitBits(
		xEventGroup,
		BIT_0 |
		BIT_4,
		pdTRUE,
		pdTRUE,
		500);

	//xSemaphore = xSemaphoreCreateBinary();

	while (1) {
		if ((uxBits & (BIT_0 | BIT_4)) == (BIT_0 | BIT_4)) {
			if (getData(param->sensor1) >= 9) { //Burde være > 9, >= er brugt for at demonstration.
				if (xSemaphoreTake(xSemaphore, 500)) {
					printf(">= 9");
					xSemaphoreGive(xSemaphore);
				}	
			}
			LoraPayload payload = createPayload();
			char transfer[20] = { "S1 d\nS2 d\0" };
			transfer[3] = 48 + getData(param->sensor1);
			transfer[8] = 48 + getData(param->sensor2);

			for (int i = 0; i < 20; i++) {

				payload->bytes[i] = transfer[i];

			}

			//payload->len = sizeof(payload->bytes);
			payload->len = 20;
			xMessageBufferSend(xMessageBuffer, payload->bytes, payload->len, 100);
			xEventGroupClearBits(
				xEventGroup,
				BIT_0 |
				BIT_4);
		}
		else {

		}
		vTaskDelay(500);
	}
	vTaskDelete(NULL);
}

void sendPacket() {

}
