#include "JNI.hpp"
#include "Log.hpp"

namespace packt {
    JNI::JNI(android_app* pApplication):
        mApplication(pApplication),
        mEnv(NULL) {
        Log::info("Attaching native thread to the VM");

        // Attaches current thread to the Dalvik VM so that it is known
        // from Java and can call Java code.
        JavaVM* lJavaVM = mApplication->activity->vm;
        JavaVMAttachArgs lJavaVMAttachArgs;
        lJavaVMAttachArgs.version = JNI_VERSION_1_6;
        lJavaVMAttachArgs.name = "NativeThread";
        lJavaVMAttachArgs.group = NULL;
        if (lJavaVM->AttachCurrentThread(&mEnv,
                &lJavaVMAttachArgs) != JNI_OK) {
            Log::error("JNI error while attaching the VM");
            mEnv = NULL;
        }
    }

    JNI::~JNI() {
        Log::info("Detaching native thread from the VM");

        // Terminates activity after detaching the thread from the VM.
        if (mEnv != NULL) {
            mApplication->activity->vm->DetachCurrentThread();
        }
    }

    JNIEnv* JNI::getEnv() {
        return mEnv;
    }

    jobject JNI::getActivity() {
        return mApplication->activity->clazz;
    }

    void JNI::makeGlobalRef(jobject* pRef) {
        if ((mEnv != NULL) && (*pRef != NULL)) {
            jobject lGlobalRef = mEnv->NewGlobalRef(*pRef);
            // No need for a local reference any more.
            mEnv->DeleteLocalRef(*pRef);
            // Here, lGlobalRef may be null.
            *pRef = lGlobalRef;
        } else {
            Log::error("JNI not ready for makeGlobalRef().");
        }
    }

    void JNI::makeGlobalRef(jclass* pRef) {
        makeGlobalRef(reinterpret_cast<jobject*>(pRef));
    }

    void JNI::deleteGlobalRef(jobject* pRef) {
        if ((mEnv != NULL) && (*pRef != NULL)) {
            mEnv->DeleteGlobalRef(*pRef);
            *pRef = NULL;
        } else {
            Log::error("JNI not ready for deleteGlobalRef().");
        }
    }

    void JNI::deleteGlobalRef(jclass* pRef) {
        deleteGlobalRef(reinterpret_cast<jobject*>(pRef));
    }
}
