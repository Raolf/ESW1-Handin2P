#include "FreeRTOS.h"
#include "LoraDriver.h"
#include "message_buffer.h"
#include <stdint.h>
#include <event_groups.h>
#include "param.h"



size_t xRecievedBytes;

void Sent_Upload_Message(LoraPayload payload);

void LoraDriverTask(void* parameters) {

	uint8_t ucRxData[50];
	Param param = (Param) parameters;
	MessageBufferHandle_t xMessageBuffer = param->xMessageBuffer1;
	//SemaphoreHandle_t semaphore = param->semaphore1;
	

	while (1) {

		xRecievedBytes = xMessageBufferReceive(xMessageBuffer, (void*)ucRxData, sizeof(ucRxData), 0);
		if (xRecievedBytes != 0) {
			LoraPayload payload = createPayload();
			payload->bytes = ucRxData;
			payload->len = sizeof(ucRxData);

			if (xSemaphoreTake(xSemaphore, 500)) {
				Sent_Upload_Message(payload);
				xSemaphoreGive(xSemaphore);
			}
			else {
				printf("No semaphore\n");
			}
			vTaskDelay(500);
		}
	
	}
	vTaskDelete(NULL);
}

void Sent_Upload_Message(LoraPayload payload) {
	printf("%s\n", payload->bytes);
	destroyPayload(payload);
}; 

LoraPayload createPayload() {
	LoraPayload payload = pvPortMalloc(sizeof(struct LoraPayloadS));
	payload->bytes = pvPortMalloc(sizeof(int8_t) * 20);
	return payload;
}
void destroyPayload(LoraPayload payload) {
	//vPortFree(payload->bytes);
	vPortFree(payload);
}