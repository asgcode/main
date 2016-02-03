#ifndef PROCESS_SENSOR
#define PROCESS_SENSOR

#include "Utils.h"

INT ProcessSensorData(UCHAR* pReadData, UINT sizeInBytes, CircularBuffer* pProcessedCBuf);
#endif