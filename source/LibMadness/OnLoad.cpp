//
// Created by mdnssknght on 5/5/24.
//

#include <shadowhook.h>

#define LOG_TAG "LibMadness"
#include <logging.hpp>

#include "Proxies.hpp"

static void __attribute__((constructor)) OnLoad() {
    LOGI("Hello world!");

    shadowhook_init(SHADOWHOOK_MODE_SHARED, true);

    shadowhook_hook_sym_name("/vendor/lib64/libexynoscamera3.so",
                             "_ZN7android28createExynosCameraSensorInfoEii",
                             (void *) proxy_createExynosCameraSensorInfo,
                             nullptr);

    shadowhook_hook_sym_name("/vendor/lib64/libMoonVerifier_v1.camera.samsung.so",
                             "_ZN12MoonVerifier10initializeENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE",
                             (void *) proxy_MoonVerifier_initialize,
                             nullptr);
}

static void __attribute__((destructor)) OnDestroy() {
    LOGI("Bye, bye");
}
