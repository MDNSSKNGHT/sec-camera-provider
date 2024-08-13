import glob
import os
import shutil
import subprocess
import tempfile

arch_dictionary = {
    "arm64-v8a": ["aarch64", "lib64", "_64"],
    "armeabi-v7a": ["armv7a", "lib", ""]
}
project_dir = os.path.dirname(os.path.realpath(__file__))


def make_module(arch):

    arm_profile = arch_dictionary[arch][0]
    system_lib = arch_dictionary[arch][1]

    provider_binary = f"vendor.samsung.hardware.camera.provider@4.0-service{arch_dictionary[arch][2]}"

    with (tempfile.TemporaryDirectory() as tmp_dir):
        shutil.copytree("module", tmp_dir, dirs_exist_ok=True)
        shutil.copytree("sepolicy", tmp_dir, dirs_exist_ok=True)
        os.makedirs(f"{tmp_dir}/system/vendor/{system_lib}")
        os.makedirs(f"{tmp_dir}/system/vendor/bin/hw")

        shutil.copy(glob.glob(f"{project_dir}/cmake-build-*-{arm_profile}/sec_camera_provider").pop(),
                    f"{tmp_dir}/system/vendor/bin/hw/{provider_binary}")

        lib_path = []
        lib_path += glob.glob(f"{project_dir}/cmake-build-*-{arm_profile}/*.so")
        lib_path += glob.glob(f"{project_dir}/external/{arch}/shadowhook/*.so")
        lib_path += glob.glob(f"{project_dir}/external/{arch}/mettle_libreflect/*.so")

        for lib in lib_path:
            shutil.copy(lib, f"{tmp_dir}/system/vendor/{system_lib}")

        shutil.make_archive(f"Module-{system_lib}", "zip", tmp_dir)


def main():
    for arch in ["armeabi-v7a", "arm64-v8a"]:
        make_module(arch)

    subprocess.call(["adb", "push", "Module-lib64.zip", "/data/local/tmp"])
    subprocess.call(["adb", "shell", "su", "-c", "apd", "module", "install", "/data/local/tmp/Module-lib64.zip"])
    subprocess.call(["adb", "reboot"])


if __name__ == "__main__":
    main()
