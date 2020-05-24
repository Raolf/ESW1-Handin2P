#pragma once
#include <stdint.h>

typedef struct LoraPayloadS {
	uint8_t len;
	uint8_t* bytes;
}*LoraPayload;

void LoraDriverTask(void* parameters);
LoraPayload createPayload();
void destroyPayload(LoraPayload payload);


