#include "FreeRTOS.h"
#include <event_groups.h>
#include "param.h"
#include "Sensor.h"
#include <event_groups.h>

#define BIT_0 ( 1<<0 )
#define BIT_4 ( 1<<4 )

void sensorTask1(void* parameters) {

	Param param = (Param) parameters;
	Sensor sensor = param->sensor1;
	uint16_t reading;
	EventGroupHandle_t xEventGroup = param->eventGroup1;

	EventBits_t eventBitsset = xEventGroupSetBits(xEventGroup, BIT_0);

	while (1)
	{
		Measure(sensor);
		eventBitsset;
	}

	destroyParam(param);
	detroySensor(sensor);
	vTaskDelete(NULL);
}

void sensorTask2(void* parameters) {

	Param param = (Param)parameters;
	Sensor sensor = param->sensor1;
	EventGroupHandle_t xEventGroup = param->eventGroup1;

	EventBits_t eventBitsset = xEventGroupSetBits(xEventGroup, BIT_4);

	while (1)
	{
		Measure(sensor);
		eventBitsset;
	}

	destroyParam(param);
	detroySensor(sensor);
	vTaskDelete(NULL);
}

Sensor createSensor() {
	Sensor sensor = pvPortMalloc(sizeof(struct SensorS));
	sensor->measurement = 0;
	return sensor;
};
void  detroySensor(Sensor sensor) {
	vPortFree(sensor);
};
void Measure(Sensor sensor) {
	sensor->measurement = rand()%10;
};
uint8_t getData(Sensor sensor) {
	return sensor->measurement;
};