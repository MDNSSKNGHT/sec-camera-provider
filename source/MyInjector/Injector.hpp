//
// Created by mdnssknght on 2/16/24.
//

#pragma once

#include <string>
#include <sys/types.h>

struct Injector {
public:
    Injector(pid_t p, std::string l) : pid(p), lib(l) {}
    int Inject();
    static constexpr auto INJECTOR_SUCCESS = 0;
    static constexpr auto INJECTOR_FAILURE = -1;
private:
    pid_t pid;
    std::string lib;
};
