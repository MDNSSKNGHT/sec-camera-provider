import glob
import os
import shutil
import subprocess
import tempfile


def main():
    with (tempfile.TemporaryDirectory() as tmp_dir):
        shutil.copytree("module", tmp_dir, dirs_exist_ok=True)
        shutil.copytree("binaries", tmp_dir, dirs_exist_ok=True)
        shutil.copytree("sepolicy", tmp_dir, dirs_exist_ok=True)
        os.makedirs(f"{tmp_dir}/system/vendor/lib64")
        os.makedirs(f"{tmp_dir}/system/vendor/bin/hw")

        project_dir = os.path.dirname(os.path.realpath(__file__))

        shutil.copy(glob.glob(f"{project_dir}/cmake-build-*/sec_camera_provider").pop(),
                    f"{tmp_dir}/system/vendor/bin/hw/vendor.samsung.hardware.camera.provider@4.0-service_64")

        lib_path = []
        lib_path += glob.glob(f"{project_dir}/cmake-build-*/*.so")
        lib_path += glob.glob(f"{project_dir}/external/shadowhook/*/*.so")
        lib_path += glob.glob(f"{project_dir}/external/mettle_libreflect/*/*.so")

        for lib in lib_path:
            shutil.copy(lib, f"{tmp_dir}/system/vendor/lib64")

        shutil.make_archive("Module", "zip", tmp_dir)

    subprocess.call(["adb", "push", "Module.zip", "/data/local/tmp"])
    subprocess.call(["adb", "shell", "su", "-c", "ksud", "module", "install", "/data/local/tmp/Module.zip"])
    subprocess.call(["adb", "reboot"])


if __name__ == "__main__":
    main()
