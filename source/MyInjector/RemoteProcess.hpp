//
// Created by mdnssknght on 2/16/24.
//

#pragma once

#include <asm/ptrace.h>
#include <string>
#include <sys/types.h>

struct FunctionInfo {
    std::string module;
    uintptr_t pAddress;
};

#ifdef __aarch64__
typedef struct user_pt_regs ARMRegs;
const auto PARAM_REGS_LEN = 8;
#else
typedef struct pt_regs ARMRegs;
const auto PARAM_REGS_LEN = 4;
#endif

struct RemoteProcess {
public:
    RemoteProcess(pid_t p) : pid(p) {}
    bool Attach();
    bool Detach();
    bool Call(FunctionInfo info, long *ret, long argv[], size_t argc);
    bool Write(uintptr_t address, uint8_t *data, size_t size);
    bool Read(uintptr_t address, uint8_t *data, size_t size);
private:
    uintptr_t GetRemoteFunctionAddress(FunctionInfo info);
    uintptr_t GetModuleBase(std::string module, bool remote = true);
    bool Continue();
    bool Wait(int *status);
    bool GetRegs(ARMRegs *regs);
    bool SetRegs(ARMRegs *regs);
private:
    pid_t pid;
};
