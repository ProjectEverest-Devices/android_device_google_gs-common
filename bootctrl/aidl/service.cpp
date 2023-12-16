/*
 * Copyright 2023 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "aidl.android.hardware.boot-service.default"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <hidl/HidlSupport.h>
#include <hidl/HidlTransportSupport.h>

#include "BootControl.h"

using aidl::android::hardware::boot::BootControl;
using aidl::android::hardware::boot::IBootControl;
using ::android::hardware::configureRpcThreadpool;
using ::android::hardware::joinRpcThreadpool;

int main(int, char* argv[]) {
    android::base::InitLogging(argv, android::base::KernelLogger);
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<IBootControl> service = ndk::SharedRefBase::make<BootControl>();

    const std::string instance = std::string(BootControl::descriptor) + "/default";
    auto status = AServiceManager_addService(service->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK) << "Failed to add service " << instance << " " << status;
    LOG(INFO) << "IBootControl AIDL service running...";

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
