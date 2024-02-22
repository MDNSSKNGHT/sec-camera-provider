//
// Created by mdnssknght on 2/16/24.
//

#pragma once

#include <asm/ptrace.h>
#include <string>
#include <sys/types.h>

struct RemoteProcess {
public:
    RemoteProcess(pid_t p) : pid(p) {}
    bool Attach();
    bool Detach();
    bool Call(uintptr_t address, long *ret, long argv[], size_t argc);
    bool Write(uintptr_t address, uint8_t *data, size_t size);
    bool Read(uintptr_t address, uint8_t *data, size_t size);
    uintptr_t GetModuleBase(std::string module, bool remote = true);
private:
    bool Continue();
    bool Wait(int *status);
    bool GetRegs(struct user_pt_regs *regs);
    bool SetRegs(struct user_pt_regs *regs);
private:
    pid_t pid;
};
