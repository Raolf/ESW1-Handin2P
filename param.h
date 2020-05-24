#pragma once
#include "FreeRTOS.h"
#include "message_buffer.h"
#include "Sensor.h"
#include "semphr.h"
#include <event_groups.h>

EventGroupHandle_t xEventGroup;
SemaphoreHandle_t xSemaphore;

typedef struct ParamS {
	Sensor sensor1;
	Sensor sensor2;
	MessageBufferHandle_t xMessageBuffer1;
	SemaphoreHandle_t semaphore1;
	EventGroupHandle_t eventGroup1;
}* Param;

Param createParam();
void destroyParam(Param param);