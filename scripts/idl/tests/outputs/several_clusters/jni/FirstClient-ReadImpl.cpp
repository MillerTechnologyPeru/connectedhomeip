
#include <controller/java/zap-generated/CHIPReadCallbacks.h>

#include <app-common/zap-generated/cluster-objects.h>
#include <zap-generated/CHIPClusters.h>

#include <controller/java/AndroidClusterExceptions.h>
#include <controller/java/CHIPDefaultCallbacks.h>
#include <jni.h>
#include <lib/support/CodeUtils.h>
#include <platform/PlatformManager.h>

#define JNI_METHOD(RETURN, CLASS_NAME, METHOD_NAME)                                                                                \
    extern "C" JNIEXPORT RETURN JNICALL Java_chip_devicecontroller_ChipClusters_00024##CLASS_NAME##_##METHOD_NAME
JNI_METHOD(void, FirstCluster, readSomeIntegerAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::First::Attributes::SomeInteger::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::FirstCluster * cppCluster = reinterpret_cast<chip::Controller::FirstCluster *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPFirstClusterSomeIntegerAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}
