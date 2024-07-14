import glob
import os
import shutil
import subprocess
import tempfile

stripped_native_libs = "./app/build/intermediates/stripped_native_libs/release/out/lib"
cxx_RelWithDebInfo = "./app/build/intermediates/cxx/RelWithDebInfo/*/obj"

subprocess.call(["./gradlew", "stripReleaseDebugSymbols"])

with tempfile.TemporaryDirectory() as tmp_dir:
    shutil.copytree("module", tmp_dir, dirs_exist_ok=True)
    shutil.copytree("binaries", tmp_dir, dirs_exist_ok=True)
    shutil.copytree("sepolicy", tmp_dir, dirs_exist_ok=True)
    os.makedirs(tmp_dir + "/system/vendor/lib64")

    shutil.copy(glob.glob(f"{cxx_RelWithDebInfo}/arm64-v8a/vendor_samsung_hardware_camera_provider_4_0_service").pop(),
                f"{tmp_dir}/system/vendor/bin/hw/vendor.samsung.hardware.camera.provider@4.0-service_64")
    for file in glob.glob(f"{stripped_native_libs}/arm64-v8a/*.so"):
        shutil.copy(file, f"{tmp_dir}/system/vendor/lib64")

    shutil.make_archive("Module", "zip", tmp_dir)

subprocess.call(["adb", "push", "Module.zip", "/data/local/tmp"])
subprocess.call(["adb", "shell", "su", "-c", "ksud", "module", "install", "/data/local/tmp/Module.zip"])
subprocess.call(["adb", "reboot"])
