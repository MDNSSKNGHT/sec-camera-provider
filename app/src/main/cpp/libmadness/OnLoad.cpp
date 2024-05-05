//
// Created by mdnssknght on 5/5/24.
//

#include <shadowhook.h>

#define LOG_TAG "libmadness"
#include "../include/logging.h"

#include "Proxies.hpp"

static void __attribute__((constructor)) OnLoad() {
    LOGI("Hello world!");

    shadowhook_init(SHADOWHOOK_MODE_SHARED, true);

    shadowhook_hook_sym_name("/vendor/lib64/libexynoscamera3.so",
                             "_ZN7android28createExynosCameraSensorInfoEii",
                             (void *) proxy_createExynosCameraSensorInfo,
                             nullptr);
}

static void __attribute__((destructor)) OnDestroy() {
    LOGI("Bye, bye");
}