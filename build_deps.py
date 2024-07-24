import os
import shutil
import subprocess

cmake_package_template = \
    'if (NOT TARGET {target})\n' \
    '\tadd_library({target} SHARED IMPORTED)\n' \
    '\tset_target_properties({target} PROPERTIES\n' \
    '\t\tIMPORTED_LOCATION "{lib_path}"\n' \
    '\t\tINTERFACE_INCLUDE_DIRECTORIES "{include_path}"\n' \
    '\t\tINTERFACE_LINK_LIBRARIES "{link_libraries}")\n' \
    'endif()'


def main():
    android_sdk = os.getenv("ANDROID_SDK")
    android_ndk = os.getenv("ANDROID_NDK")

    cmake = f"{android_sdk}/cmake/3.22.1/bin/cmake"
    ninja = f"{android_sdk}/cmake/3.22.1/bin/ninja"

    project_dir = os.path.dirname(os.path.realpath(__file__))

    subprocess.call([cmake,
                     f"-H{project_dir}/source_deps/shadowhook-1.0.9",
                     "-DCMAKE_SYSTEM_NAME=Android",
                     "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
                     "-DCMAKE_SYSTEM_VERSION=latest",
                     "-DANDROID_PLATFORM=latest",
                     "-DANDROID_ABI=arm64-v8a",
                     "-DCMAKE_ANDROID_ARCH_ABI=arm64-v8a",
                     f"-DANDROID_NDK={android_ndk}",
                     f"-DCMAKE_TOOLCHAIN_FILE={android_ndk}/build/cmake/android.toolchain.cmake",
                     f"-DCMAKE_MAKE_PROGRAM={ninja}",
                     f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={project_dir}/external/shadowhook/arm64-v8a",
                     f"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY={project_dir}/external/shadowhook/arm64-v8a",
                     f"-DCMAKE_BUILD_TYPE=Debug",
                     f"-B{project_dir}/source_deps/build/shadowhook-1.0.9/arm64-v8a",
                     "-GNinja",
                     "-DANDROID_STL=none"])
    subprocess.call([ninja, "-C", f"{project_dir}/source_deps/build/shadowhook-1.0.9/arm64-v8a"])

    shutil.copytree(f"{project_dir}/source_deps/shadowhook-1.0.9/include",
                    f"{project_dir}/external/shadowhook/include")

    cmake_config_file = open(f"{project_dir}/external/shadowhook/shadowhookConfig.cmake", "w")
    cmake_config_file.write(cmake_package_template.format(
        target="shadowhook::shadowhook",
        lib_path=f"{project_dir}/external/shadowhook/arm64-v8a/libshadowhook.so",
        include_path=f"{project_dir}/external/shadowhook/include",
        link_libraries=""))


if __name__ == "__main__":
    main()
