//
// Created by mdnssknght on 2/16/24.
//

#include <string>
#include <dlfcn.h>
#include <shadowhook.h>

#define LOG_TAG "libmadness"
#include "../include/logging.h"

void* proxy_createExynosCameraSensorInfo(int cameraId, int serviceCameraId) {
    SHADOWHOOK_STACK_SCOPE();

    void *prev = SHADOWHOOK_CALL_PREV(proxy_createExynosCameraSensorInfo, cameraId, serviceCameraId);
    uint8_t *pExynosCameraSensorInfo = (uint8_t *) prev;

    LOGI("cameraId=%d, serviceCameraId=%d", cameraId, serviceCameraId);

    *(unsigned long *) (pExynosCameraSensorInfo + 0xa18) = 0x3f;

    return pExynosCameraSensorInfo;
}

__attribute__((constructor)) void init() {
    LOGI("Hello world!");

    shadowhook_init(SHADOWHOOK_MODE_SHARED, true);

    shadowhook_hook_sym_name("/vendor/lib64/libexynoscamera3.so",
                             "_ZN7android28createExynosCameraSensorInfoEii",
                             reinterpret_cast<void *>(proxy_createExynosCameraSensorInfo),
                             nullptr);
    LOGI("Bye, bye");
}
