#!/usr/bin/env bash

set -eux

# MODULE
TEMP_DIR=".temp"
MODULE_NAME="Module.zip"

# CXX
INTERMEDIATES_CXX="app/build/intermediates/cxx"
BIN_CXX_OBJ="vendor_samsung_hardware_camera_provider_4_0_service"

# BIN
BIN_HW="vendor.samsung.hardware.camera.provider@4.0-service_64"

# ADB
SAVE_PATH="/sdcard/Download"

# COMPILE CXX
./gradlew buildCMakeRelWithDebInfo

# ZIP MODULE
[ -d "$TEMP_DIR" ] && rm -rf "$TEMP_DIR"
mkdir "$TEMP_DIR" ; cp -r magisk/* blob/* sepolicy/* "$TEMP_DIR"
cp "$INTERMEDIATES_CXX"/RelWithDebInfo/*/obj/arm64-v8a/"$BIN_CXX_OBJ" "$TEMP_DIR/system/vendor/bin/hw/$BIN_HW"
cp "$INTERMEDIATES_CXX"/RelWithDebInfo/*/obj/arm64-v8a/*.so "$TEMP_DIR/system/vendor/lib64"
pushd "$TEMP_DIR" ; zip -r9 "$MODULE_NAME" ./*
mv ./*.zip .. ; popd ; rm -r "$TEMP_DIR"

# INSTALL MODULE
adb push "$MODULE_NAME" "$SAVE_PATH"
adb shell su -c magisk --install-module "$SAVE_PATH/$MODULE_NAME"
adb reboot
