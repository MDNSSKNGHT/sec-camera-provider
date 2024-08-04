#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define LOG_TAG "mettle_libreflect"
#include "logging.h"

#include <reflect.h>


extern char **environ;

void reflect_execv(const unsigned char *elf, char **argv) {
    LOGI("Using default environment %p", (void *) environ);
    
    reflect_execve(elf, argv, NULL);
}

void reflect_execve(const unsigned char *elf, char **argv, char **env) {
    int page_size = getpagesize();

    // When allocating a new stack, be sure to give it lots of space since the OS
    // won't always honor MAP_GROWSDOWN
    uint8_t *stack_bottom = (uint8_t *) mmap(0, page_size * page_size,
                                             PROT_READ | PROT_WRITE,
                                             MAP_ANONYMOUS | MAP_PRIVATE | MAP_GROWSDOWN,
                                             -1, 0);
    uint8_t *stack_top = stack_bottom + (page_size - 1) * page_size;

    LOGI("Allocated new stack %p", (void *) stack_top);
    reflect_execves(elf, argv, env, (size_t *) stack_top);
}

void reflect_execves(const unsigned char *elf, char **argv, char **env, size_t *stack) {
    int fd;
    struct stat stat_buf;
    unsigned char *data = NULL;
    int argc;

    struct mapped_elf exe = {0}, interp = {0};

    if (!is_compatible_elf((ElfW(Ehdr) *) elf)) {
        abort();
    }

    if (env == NULL) {
        env = environ;
    }

    map_elf(elf, &exe);
    if (exe.ehdr == MAP_FAILED) {
        LOGI("Unable to map ELF file: %s", strerror(errno));
        abort();
    }

    if (exe.interp) {
        // Load input ELF executable into memory
        fd = open(exe.interp, O_RDONLY);
        if (fd == -1) {
            LOGI("Failed to open %p: %s", (void *) exe.interp, strerror(errno));
            abort();
        }

        if (fstat(fd, &stat_buf) == -1) {
            LOGI("Failed to fstat(fd): %s", strerror(errno));
            abort();
        }

        data = mmap(NULL, stat_buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (data == MAP_FAILED) {
            LOGI("Unable to read ELF file in: %s", strerror(errno));
            abort();
        }
        close(fd);

        map_elf(data, &interp);
        munmap(data, stat_buf.st_size);
        if (interp.ehdr == MAP_FAILED) {
            LOGI("Unable to map interpreter for ELF file: %s", strerror(errno));
            abort();
        }
        LOGI("Mapped ELF interp file in: %s", exe.interp);
    } else {
        interp = exe;
    }

    for (argc = 0; argv[argc]; argc++);

    stack_setup(stack, argc, argv, env, NULL,
                exe.ehdr, interp.ehdr);

    jump_with_stack(interp.entry_point, stack);
}
