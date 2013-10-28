#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "Types.hpp"

#include <android_native_app_glue.h>
#include <jni.h>

namespace packt {
    typedef int32_t screen_rot;

    const screen_rot ROTATION_0   = 0;
    const screen_rot ROTATION_90  = 1;
    const screen_rot ROTATION_180 = 2;
    const screen_rot ROTATION_270 = 3;

    class Configuration {
    public:
        Configuration(android_app* pApplication);

        screen_rot getRotation();

    private:
        void findRotation(JNIEnv* pEnv);

    private:
        android_app* mApplication;
        screen_rot mRotation;
    };
}
#endif
