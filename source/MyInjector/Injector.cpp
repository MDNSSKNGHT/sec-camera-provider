//
// Created by mdnssknght on 2/16/24.
//

#include <dlfcn.h>
#include <sys/mman.h>
#include "RemoteProcess.hpp"

#define LOG_TAG "MyInjector"
#include <logging.hpp>

#include "Injector.hpp"


int Injector::Inject() {

    RemoteProcess rp(pid);

    rp.Attach();

    FunctionInfo iMmap{};
    iMmap.module = "libc.so";
    iMmap.pAddress = (uintptr_t) (void *) mmap;

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

    rp.Call(iMmap, &ret, params, 6);

    LOGD("Remote mmap returned %lx", ret);

    rp.Write(ret, (uint8_t *) lib.c_str(), lib.size());

    FunctionInfo iDlopen{};
    iDlopen.module = "libdl.so";
    iDlopen.pAddress = (uintptr_t) (void *) dlopen;

    params[0] = ret;
    params[1] = RTLD_NOW | RTLD_GLOBAL;

    rp.Call(iDlopen, &ret, params, 2);

    LOGD("Remote dlopen returned %ld", ret);

    if ((void *) ret == nullptr) {
        FunctionInfo iDlerror{};
        iDlerror.module = "libdl.so";
        iDlerror.pAddress = (uintptr_t) (void *) dlerror;

        rp.Call(iDlerror, &ret, nullptr, 0);

        const char err[1024] = {0};
        rp.Read(ret, (uint8_t *) err, sizeof(err));

        LOGD("Remote dlerror returned %s", err);

        rp.Detach();
        return INJECTOR_FAILURE;
    }

    rp.Detach();

    return INJECTOR_SUCCESS;
}
