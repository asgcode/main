#ifndef PROCESS_SENSOR
#define PROCESS_SENSOR

#include "CommonUtils.h"

// Takes and unordered circular buffer and outputs updated processed circular buffer as per following rules
// 1) Only entries which are bigger than least value inside pMaxSortedCBuf would be updated in pMaxSortedCBuf.
// 2) Output circular buffer entries would be sorted order.
INT ProcessSensorData(CircularBuffer* pReadCBuf, CircularBuffer* pMaxSortedCBuf);
#endif