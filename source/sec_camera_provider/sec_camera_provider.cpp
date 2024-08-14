//
// Created by mdnssknght on 23/07/2024.
//

#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include <reflect.h>
#include <Injector.hpp>

#define LOG_TAG "vendor.samsung.hardware.camera.provider@4.0-service"
#include <logging.hpp>

#include "sec_camera_provider.hpp"

static void thread_task(pid_t pid) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Injector i_sh(pid, "/vendor/lib64/libshadowhook.so");
    Injector i_lm(pid, "/vendor/lib64/libmadness.so");

    LOGW("shadowhook Injector: %d", i_sh.Inject());
    LOGW("libmadness Injector: %d", i_lm.Inject());
}

int main(int argc, char *argv[]) {
    pid_t pid;
    int status;

    if ((pid = fork())) {
        std::thread t(thread_task, pid);
        t.join();

        waitpid(pid, &status, 0);
    } else {
        reflect_execves(SEC_CAMERA_PROVIDER_BIN, argv + 1, NULL, (size_t *) argv - 1);
    }
}