#ifndef _JNI_H_
#define _JNI_H_

#include "Types.hpp"

#include <android_native_app_glue.h>
#include <jni.h>

namespace packt {
    class JNI {
    public:
        JNI(android_app* pApplication);
        ~JNI();

        JNIEnv* getEnv();
        jobject getActivity();

        void makeGlobalRef(jobject* pRef);
        void makeGlobalRef(jclass* pRef);
        void deleteGlobalRef(jobject* pRef);
        void deleteGlobalRef(jclass* pRef);

    private:
        android_app* mApplication;
        JNIEnv* mEnv;
    };
}
#endif
