//
// Created by mdnssknght on 2/15/24.
//

#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include "../MyInjector/Injector.h"

#define LOG_TAG "vendor.samsung.hardware.camera.provider@4.0-service"
#include "../include/logging.h"

static void thread_task(pid_t pid) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Injector i_sh(pid, "/vendor/lib64/libshadowhook.so");
    Injector i_lm(pid, "/vendor/lib64/libmadness.so");

    LOGW("shadowhook Injector: %d", i_sh.Inject());
    LOGW("libmadness Injector: %d", i_lm.Inject());
}

int main() {
    pid_t pid;
    int status;

    if ((pid = fork()) != 0) {
        std::thread t(thread_task, pid);

        t.join();

        waitpid(pid, &status, 0);
    } else {
        constexpr auto bin = "/"
                "vendor"   "/"
                "bin"      "/"
                "hw"       "/"
                "vendor.samsung.hardware.camera.provider@4.0-service_64" "-backup";

        return execl(bin, bin, nullptr);
    }

    return status;
}