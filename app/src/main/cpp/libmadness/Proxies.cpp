//
// Created by mdnssknght on 5/5/24.
//

#include <shadowhook.h>

#define LOG_TAG "LibMadness"
#include "../include/logging.h"

#include "Proxies.hpp"

ExynosCameraSensorInfoBase *proxy_createExynosCameraSensorInfo(int cameraId, int serviceCameraId) {
    SHADOWHOOK_STACK_SCOPE();

    auto pSensorInfo = SHADOWHOOK_CALL_PREV(proxy_createExynosCameraSensorInfo, cameraId, serviceCameraId);

    LOGI("cameraId=%d, serviceCameraId=%d", cameraId, serviceCameraId);

    *(uint64_t *) (pSensorInfo + 0xa38) = 0x3f;

    return pSensorInfo;
}
