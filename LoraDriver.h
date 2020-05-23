#pragma once
#include <stdint.h>

typedef struct LoraPayload {
	uint8_t len;
	uint8_t* bytes;
}*LoraPayload;

void LoraDriverTask(void* parameters);
LoraPayload createPayload();
void destroyPayload(LoraPayload payload);


