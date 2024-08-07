//
// Created by mdnssknght on 5/5/24.
//

#pragma once

#include <string>

struct ExynosCameraSensorInfoBase {};

ExynosCameraSensorInfoBase *proxy_createExynosCameraSensorInfo(int cameraId, int serviceCameraId);

bool proxy_MoonVerifier_initialize(const std::string& json_data);