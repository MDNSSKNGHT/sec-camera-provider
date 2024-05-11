import glob
import os
import shutil
import subprocess
import tempfile

# CONSTANTS
INTERMEDIATES_CXX = "app/build/intermediates/cxx"
BINARIES = ["vendor_samsung_hardware_camera_provider_4_0_service",
            "vendor.samsung.hardware.camera.provider@4.0-service_64"]

# COMPILE CXX
subprocess.call(['./gradlew', 'buildCMakeRelWithDebInfo'])

# ZIP MODULE
with tempfile.TemporaryDirectory() as tmp_dir:
    shutil.copytree('module', tmp_dir, dirs_exist_ok=True)
    shutil.copytree('sepolicy', tmp_dir, dirs_exist_ok=True)
    shutil.copytree('binaries', tmp_dir, dirs_exist_ok=True)
    os.makedirs(tmp_dir + '/system/vendor/lib64')

    shutil.copy(glob.glob(INTERMEDIATES_CXX + '/RelWithDebInfo/*/obj/arm64-v8a/' + BINARIES[0]).pop(),
                tmp_dir + "/system/vendor/bin/hw/" + BINARIES[1])
    for file in glob.glob(INTERMEDIATES_CXX + '/RelWithDebInfo/*/obj/arm64-v8a/*.so'):
        shutil.copy(file, tmp_dir + '/system/vendor/lib64')

    shutil.make_archive("Module", 'zip', tmp_dir)

# INSTALL MODULE
subprocess.call(["adb", "push", "Module.zip", "/data/local/tmp"])
subprocess.call(["adb", "shell", "su", "-c", "ksud", "module", "install", "/data/local/tmp/Module.zip"])
subprocess.call(["adb", "reboot"])
