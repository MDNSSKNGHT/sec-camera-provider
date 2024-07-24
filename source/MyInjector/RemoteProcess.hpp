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
    bool GetRegs(struct user_pt_regs *regs);
    bool SetRegs(struct user_pt_regs *regs);
private:
    pid_t pid;
};
