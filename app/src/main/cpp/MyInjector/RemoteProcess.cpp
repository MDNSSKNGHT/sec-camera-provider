//
// Created by mdnssknght on 2/16/24.
//

#include <elf.h>
#include <sys/ptrace.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <fstream>
#include "RemoteProcess.hpp"

#define LOG_TAG "MyRemoteProcess"
#include "../include/logging.h"

static inline auto i_ptrace(int request, pid_t pid, void *addr, void *data) {
    auto ret = ptrace(request, pid, addr, data);

    LOGD("ptrace with request: %d, pid: %d, addr: %p and data: %p returned %lX", request, pid, addr,
         data, ret);

    return ret;
}

bool RemoteProcess::Attach() {
    return i_ptrace(PTRACE_ATTACH, pid, 0, 0) == 0 && Wait(0);
}

bool RemoteProcess::Detach() {
    return i_ptrace(PTRACE_DETACH, pid, 0, 0) == 0;
}

bool RemoteProcess::Call(FunctionInfo info, long *ret, long *argv, size_t argc) {
    struct user_pt_regs cRegs, bRegs;

    if (!GetRegs(&cRegs)) {
        LOGE("Failed to get registers");
        return false;
    }
    std::memcpy(&bRegs, &cRegs, sizeof(bRegs));

    for (int i = 0; i < argc; i++) {
        cRegs.regs[i] = argv[i];
    }

    // Here we'd check if the `argc` exceeds the maximum parameter count supported
    // by aarch64 and push the remaining parameters into the stack. But we'll skip
    // that part as this implementation is not ready to be publicly released.

    // TODO: push remaining arguments into the stack

    constexpr auto CPSR_T_MASK = (1u << 5);

    cRegs.pc = GetRemoteFunctionAddress(info);
    if (cRegs.pc & 1) {
        // Thumb
        cRegs.pc &= (~1u);
        cRegs.pstate |= CPSR_T_MASK;
    } else {
        // ARM
        cRegs.pstate &= ~CPSR_T_MASK;
    }

    cRegs.regs[30] = GetModuleBase("libc.so");

    if (!SetRegs(&cRegs) || !Continue()) {
        LOGE("Failed to set registers or continue target process");
        return false;
    }

    int status;

    Wait(&status);

    while ((status & 0xff) != 0x7f) {
        if (!Continue()) {
            LOGE("Failed to continue target process");
            return false;
        }
        Wait(&status);
    }

    if (!GetRegs(&cRegs)) {
        LOGE("Failed to get registers after call");
        return false;
    }

    *ret = cRegs.regs[0];

    if (!SetRegs(&bRegs)) {
        LOGE("Failed to set backup registers after call");
        return false;
    }

    return true;
}

bool RemoteProcess::Write(uintptr_t address, uint8_t *data, size_t size) {
    int chunks = size / sizeof(long);

    for (int i = 0; i < chunks; i++) {
        long buffer = 0;
        long offset = i * sizeof(long);

        std::memcpy(&buffer, data + offset, sizeof(buffer));

        if (i_ptrace(PTRACE_POKETEXT, pid, (void *) (address + offset), (void *) buffer) != 0) {
            return false;
        }
    }

    int remain = size % sizeof(long);
    if (remain != 0) {
        long offset = chunks * sizeof(long);
        long buffer = i_ptrace(PTRACE_PEEKTEXT, pid, (void *) (address + offset), 0);

        std::memcpy(&buffer, data + offset, remain);

        if (i_ptrace(PTRACE_POKETEXT, pid, (void *) (address + offset), (void *) buffer) != 0) {
            return false;
        }
    }

    return true;
}

bool RemoteProcess::Read(uintptr_t address, uint8_t *data, size_t size) {
    int chunks = size / sizeof(long);

    for (int i = 0; i < chunks; i++) {
        long offset = i * sizeof(long);
        long buffer = i_ptrace(PTRACE_PEEKTEXT, pid, (void *) (address + offset), 0);

        std::memcpy(data + offset, &buffer, sizeof(buffer));
    }

    int remain = size % sizeof(long);
    if (remain != 0) {
        long offset = chunks * sizeof(long);
        long buffer = i_ptrace(PTRACE_PEEKTEXT, pid, (void *) (address + offset), 0);

        std::memcpy(data + offset, (uint8_t *) &buffer, remain);
    }

    return true;
}

uintptr_t RemoteProcess::GetRemoteFunctionAddress(FunctionInfo info) {
    uintptr_t pModule = GetModuleBase(info.module, false);
    uintptr_t rModule = GetModuleBase(info.module);

    return rModule + (info.pAddress - pModule);
}

uintptr_t RemoteProcess::GetModuleBase(std::string module, bool remote) {
    std::string process = remote ? std::to_string(pid) : "self";

    std::ifstream is("/proc/" + process + "/maps");
    std::string line, str = "0";

    while (getline(is, line)) {

        if (line.find(module) != std::string::npos) {
            str = line.substr(0, line.find("-"));
            break;
        }
    }

    return std::stol(str, 0, 16);
}

bool RemoteProcess::Continue() {
    return i_ptrace(PTRACE_CONT, pid, 0, 0) == 0;
}

bool RemoteProcess::Wait(int *status) {
    return waitpid(pid, status, WUNTRACED) == pid;
}

bool RemoteProcess::GetRegs(struct user_pt_regs *regs) {
    struct iovec iov;

    iov.iov_base = regs;
    iov.iov_len = sizeof(*regs);

    return i_ptrace(PTRACE_GETREGSET, pid, (void *) NT_PRSTATUS, &iov) == 0;
}

bool RemoteProcess::SetRegs(struct user_pt_regs *regs) {
    struct iovec iov;

    iov.iov_base = regs;
    iov.iov_len = sizeof(*regs);

    return i_ptrace(PTRACE_SETREGSET, pid, (void *) NT_PRSTATUS, &iov) == 0;
}
