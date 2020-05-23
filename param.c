#include "param.h"

Param createParam() {
	return pvPortMalloc(sizeof(struct ParamS));
}
void destroyParam(Param param) {
	vPortFree(param);
}
