#pragma once
#include <stdint.h>

typedef struct SensorS {
	uint8_t measurement;
} *Sensor;

void sensorTask1(void* parameters);
void sensorTask2(void* parameters);
Sensor createSensor();
void  detroySensor(Sensor sensor);
void Measure(Sensor sensor);
uint8_t getData(Sensor sensor);

