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


def cmake_configure_and_build(project_dir, build_dir, output_dir, arch):
    android_sdk = os.getenv("ANDROID_SDK")
    android_ndk = os.getenv("ANDROID_NDK")

    cmake = f"{android_sdk}/cmake/3.22.1/bin/cmake"
    ninja = f"{android_sdk}/cmake/3.22.1/bin/ninja"

    subprocess.call([cmake,
                     f"-H{project_dir}",
                     "-DCMAKE_SYSTEM_NAME=Android",
                     "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
                     "-DCMAKE_SYSTEM_VERSION=latest",
                     "-DANDROID_PLATFORM=latest",
                     f"-DANDROID_ABI={arch}",
                     f"-DCMAKE_ANDROID_ARCH_ABI={arch}",
                     f"-DANDROID_NDK={android_ndk}",
                     f"-DCMAKE_TOOLCHAIN_FILE={android_ndk}/build/cmake/android.toolchain.cmake",
                     f"-DCMAKE_MAKE_PROGRAM={ninja}",
                     f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={output_dir}",
                     f"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY={output_dir}",
                     f"-DCMAKE_BUILD_TYPE=Debug",
                     f"-B{build_dir}",
                     "-GNinja",
                     "-DANDROID_STL=none"])
    subprocess.call([ninja, "-C", f"{build_dir}"])


def cmake_create_package_file(output_path, cmake_target, cmake_lib_path, cmake_include_path, cmake_link_libs=""):
    cmake_package_file = open(output_path, "w")
    cmake_package_file.write(cmake_package_template.format(
        target=cmake_target,
        lib_path=cmake_lib_path,
        include_path=cmake_include_path,
        link_libraries=cmake_link_libs))


def main():
    project_dir = os.path.dirname(os.path.realpath(__file__))

    # shadowhook
    cmake_configure_and_build(
        project_dir=f"{project_dir}/source_deps/shadowhook-1.0.9",
        build_dir=f"{project_dir}/source_deps/build/shadowhook-1.0.9/arm64-v8a",
        output_dir=f"{project_dir}/external/shadowhook/arm64-v8a",
        arch="arm64-v8a")

    shutil.copytree(f"{project_dir}/source_deps/shadowhook-1.0.9/include",
                    f"{project_dir}/external/shadowhook/include", dirs_exist_ok=True)

    cmake_create_package_file(
        output_path=f"{project_dir}/external/shadowhook/shadowhookConfig.cmake",
        cmake_target="shadowhook::shadowhook",
        cmake_lib_path=f"{project_dir}/external/shadowhook/arm64-v8a/libshadowhook.so",
        cmake_include_path=f"{project_dir}/external/shadowhook/include")

    # libreflect
    cmake_configure_and_build(
        project_dir=f"{project_dir}/source_deps/mettle_libreflect",
        build_dir=f"{project_dir}/source_deps/build/mettle_libreflect/arm64-v8a",
        output_dir=f"{project_dir}/external/mettle_libreflect/arm64-v8a",
        arch="arm64-v8a")

    shutil.copytree(f"{project_dir}/source_deps/mettle_libreflect/include",
                    f"{project_dir}/external/mettle_libreflect/include", dirs_exist_ok=True)

    cmake_create_package_file(
        output_path=f"{project_dir}/external/mettle_libreflect/mettle_libreflectConfig.cmake",
        cmake_target="mettle_libreflect",
        cmake_lib_path=f"{project_dir}/external/mettle_libreflect/arm64-v8a/libreflect.so",
        cmake_include_path=f"{project_dir}/external/mettle_libreflect/include")


if __name__ == "__main__":
    main()
