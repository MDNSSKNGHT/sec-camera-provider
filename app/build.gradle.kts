@file:Suppress("UnstableApiUsage")

plugins {
    id("com.android.library")
}

android {
    namespace = "mdnssknght.vendor.samsung.hardware.camera.provider.service"
    compileSdk = 34

    defaultConfig {
        minSdk = 34
        lint.targetSdk = 34

        ndk {
            //noinspection ChromeOsAbiSupport
            abiFilters += setOf("arm64-v8a")
        }
        externalNativeBuild {
            cmake {
                cppFlags += ""
            }
        }
    }
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }

    buildFeatures {
        prefab = true
    }
    packaging {
        jniLibs {
            pickFirsts.add("**/libshadowhook.so")
        }
    }
}

dependencies {

    implementation("com.bytedance.android:shadowhook:1.0.9")
}