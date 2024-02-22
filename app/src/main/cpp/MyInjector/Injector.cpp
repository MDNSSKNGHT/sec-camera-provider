//
// Created by mdnssknght on 2/16/24.
//

#include <dlfcn.h>
#include <sys/mman.h>
#include "RemoteProcess.h"
#include "Injector.h"

#define LOG_TAG "MyInjector"
#include "../include/logging.h"

int Injector::Inject() {

    RemoteProcess rp(pid);

    rp.Attach();

    uintptr_t pLibcBase = rp.GetModuleBase("libc.so", false);
    uintptr_t rLibcBase = rp.GetModuleBase("libc.so");
    uintptr_t pMmapAddress = (uintptr_t) (void *) mmap;
    uintptr_t rMmapAddress = rLibcBase + (pMmapAddress - pLibcBase);

    long params[6], ret;

    // Allocate word aligned buffer

    uint8_t pad, remain = lib.size() % sizeof(long);

    if (remain != 0) {
        pad = sizeof(long) - remain;
    } else {
        pad = 0;
    }

    params[0] = 0;
    params[1] = lib.size() + pad;
    params[2] = PROT_READ | PROT_WRITE | PROT_EXEC;
    params[3] = MAP_ANONYMOUS | MAP_PRIVATE;
    params[4] = 0;
    params[5] = 0;

    rp.Call(rMmapAddress, &ret, params, 6);

    LOGD("Remote mmap returned %lx", ret);

    rp.Write(ret, (uint8_t *) lib.c_str(), lib.size());

    uintptr_t pLibdlBase = rp.GetModuleBase("libdl.so", false);
    uintptr_t rLibdlBase = rp.GetModuleBase("libdl.so");
    uintptr_t pDlopenAddress = (uintptr_t) (void *) dlopen;
    uintptr_t rDlopenAddress = rLibdlBase + (pDlopenAddress - pLibdlBase);

    params[0] = ret;
    params[1] = RTLD_NOW | RTLD_GLOBAL;

    rp.Call(rDlopenAddress, &ret, params, 2);

    LOGD("Remote dlopen returned %ld", ret);

    if ((void *) ret == nullptr) {
        uintptr_t pDlerrorAddress = (uintptr_t) (void *) dlerror;
        uintptr_t rDlerrorAddress = rLibdlBase + (pDlerrorAddress - pLibdlBase);

        rp.Call(rDlerrorAddress, &ret, nullptr, 0);

        const char err[1024] = {0};
        rp.Read(ret, (uint8_t *) err, sizeof(err));

        LOGD("Remote dlerror returned %s", err);
    }

    rp.Detach();

    return INJECTOR_SUCCESS;
}
