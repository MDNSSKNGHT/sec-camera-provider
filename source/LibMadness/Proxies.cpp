//
// Created by mdnssknght on 5/5/24.
//

#include <shadowhook.h>

#define LOG_TAG "LibMadness"
#include <logging.hpp>

#include "Proxies.hpp"

ExynosCameraSensorInfoBase *proxy_createExynosCameraSensorInfo(int cameraId, int serviceCameraId) {
    SHADOWHOOK_STACK_SCOPE();

    auto pSensorInfo = SHADOWHOOK_CALL_PREV(proxy_createExynosCameraSensorInfo, cameraId, serviceCameraId);

    LOGI("cameraId=%d, serviceCameraId=%d", cameraId, serviceCameraId);

    // 0xa38 is guaranteed to be the offset at which supportedCapabilities is located
    // if the size of ExynosCameraSensorInfoBase struct is 0x19e8. (e.g. in S21FE)
    auto *supportedCapabilities = reinterpret_cast<uint64_t *>(&pSensorInfo[0xa38]);

    // The field `supportedCapabilities` is a bit field where each position
    // corresponds to the availability of a capability for a camera device.
    //
    // ""
    // A capability is a contract that the camera device makes in order to
    // be able to satisfy one or more use cases.
    // ""
    // From CameraCharacteristics#REQUEST_AVAILABLE_CAPABILITIES.
    //
    // The availability for a capability in a camera device is implementation defined.
    //
    // In our case, we set this bit field to `0000 0000 0011 1111`. This value guarantees
    // the availability for most of the capabilities for a LEVEL_FULL hardware level camera
    // device.
    *supportedCapabilities = 0x3f;

    return pSensorInfo;
}

bool proxy_MoonVerifier_initialize(const std::string& json_data) {
    SHADOWHOOK_STACK_SCOPE();

    LOGI("MoonVerifier::initialize() json data: %s", json_data.c_str());

    return false;
}
