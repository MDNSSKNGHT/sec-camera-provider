#include <android/log.h>

int main() {

    __android_log_print(ANDROID_LOG_INFO, "MyTag", "Hello World!");

    return 0;
}
