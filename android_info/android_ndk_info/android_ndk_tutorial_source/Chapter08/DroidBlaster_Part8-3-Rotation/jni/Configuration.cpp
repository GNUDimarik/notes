#include "Configuration.hpp"
#include "Log.hpp"


namespace packt {
    Configuration::Configuration(android_app* pApplication) :
        mApplication(pApplication),
        mRotation(0) {
        AConfiguration* lConfiguration = AConfiguration_new();
        if (lConfiguration == NULL) return;

        int32_t lConfigRes;
        char lI18NBuffer[] = "__";
        // Value tables.
        const char* lOrientation[] = {
            "Unknown", "Portrait", "Landscape", "Square"
        };
        const char* lScreenSize[] = {
            "Unknown", "Small", "Normal", "Large", "X-Large"
        };
        const char* lScreenLong[] = {
            "Unknown", "No", "Yes"
        };

        // Dumps current configuration.
        AConfiguration_fromAssetManager(lConfiguration,
            mApplication->activity->assetManager);
        lConfigRes = AConfiguration_getSdkVersion(lConfiguration);
        Log::info("SDK Version : %d", lConfigRes);
        AConfiguration_getLanguage(lConfiguration, lI18NBuffer);
        Log::info("Language    : %s", lI18NBuffer);
        AConfiguration_getCountry(lConfiguration, lI18NBuffer);
        Log::info("Country     : %s", lI18NBuffer);
        lConfigRes = AConfiguration_getOrientation(lConfiguration);
        Log::info("Orientation : %s (%d)", lOrientation[lConfigRes], lConfigRes);
        lConfigRes = AConfiguration_getDensity(lConfiguration);
        Log::info("Density     : %d dpi", lConfigRes);
        lConfigRes = AConfiguration_getScreenSize(lConfiguration);
        Log::info("Screen Size : %s (%d)", lScreenSize[lConfigRes], lConfigRes);
        lConfigRes = AConfiguration_getScreenLong(lConfiguration);
        Log::info("Long Screen : %s (%d)", lScreenLong[lConfigRes], lConfigRes);
        AConfiguration_delete(lConfiguration);


        // Attaches current thread to the Dalvik VM.
        JavaVM* lJavaVM = mApplication->activity->vm;
        JavaVMAttachArgs lJavaVMAttachArgs;
        lJavaVMAttachArgs.version = JNI_VERSION_1_6;
        lJavaVMAttachArgs.name = "NativeThread";
        lJavaVMAttachArgs.group = NULL;
        JNIEnv* lEnv;
        if (lJavaVM->AttachCurrentThread(&lEnv,
                &lJavaVMAttachArgs) != JNI_OK) {
            Log::error("JNI error while attaching the VM");
            return;
        }
        // Finds screen rotation and get-rid of JNI.
        findRotation(lEnv);
        mApplication->activity->vm->DetachCurrentThread();
    }

    void Configuration::findRotation(JNIEnv* pEnv) {
        jobject WINDOW_SERVICE, lWindowManager, lDisplay;
        jclass ClassActivity,      ClassContext;
        jclass ClassWindowManager, ClassDisplay;
        jmethodID MethodGetSystemService;
        jmethodID MethodGetDefaultDisplay;
        jmethodID MethodGetRotation;
        jfieldID FieldWINDOW_SERVICE;

        jobject lActivity = mApplication->activity->clazz;

        // Classes.
        ClassActivity = pEnv->GetObjectClass(lActivity);
        if (ClassActivity == NULL) goto ERROR;
        ClassContext = pEnv->FindClass("android/content/Context");
        if (ClassContext == NULL) goto ERROR;
        ClassWindowManager = pEnv->FindClass(
            "android/view/WindowManager");
        if (ClassWindowManager == NULL) goto ERROR;
        ClassDisplay = pEnv->FindClass("android/view/Display");
        if (ClassDisplay == NULL) goto ERROR;

        // Methods.
        MethodGetSystemService = pEnv->GetMethodID(ClassActivity,
            "getSystemService",
            "(Ljava/lang/String;)Ljava/lang/Object;");
        if (MethodGetSystemService == NULL) goto ERROR;
        MethodGetDefaultDisplay = pEnv->GetMethodID(
            ClassWindowManager, "getDefaultDisplay",
            "()Landroid/view/Display;");
        if (MethodGetDefaultDisplay == NULL) goto ERROR;
        MethodGetRotation = pEnv->GetMethodID(ClassDisplay,
            "getRotation", "()I");
        if (MethodGetRotation == NULL) goto ERROR;

        // Fields.
        FieldWINDOW_SERVICE = pEnv->GetStaticFieldID(
          ClassContext, "WINDOW_SERVICE", "Ljava/lang/String;");
        if (FieldWINDOW_SERVICE == NULL) goto ERROR;

        // Retrieves Context.WINDOW_SERVICE.
        WINDOW_SERVICE = pEnv->GetStaticObjectField(ClassContext,
            FieldWINDOW_SERVICE);
        // Runs getSystemService(WINDOW_SERVICE).
        lWindowManager = pEnv->CallObjectMethod(lActivity,
            MethodGetSystemService, WINDOW_SERVICE);
        // Runs getDefaultDisplay().getRotation().
        lDisplay = pEnv->CallObjectMethod(lWindowManager,
            MethodGetDefaultDisplay);
        mRotation = pEnv->CallIntMethod(lDisplay, MethodGetRotation);

        pEnv->DeleteLocalRef(ClassActivity);
        pEnv->DeleteLocalRef(ClassContext);
        pEnv->DeleteLocalRef(ClassWindowManager);
        pEnv->DeleteLocalRef(ClassDisplay);
        return;

    ERROR:
        // Not clearing exceptions may cause application to crash.
        pEnv->ExceptionClear();
        packt::Log::error("Error while reading conf with JNI");
        if (ClassActivity != NULL) pEnv->DeleteLocalRef(ClassActivity);
        if (ClassContext != NULL) pEnv->DeleteLocalRef(ClassContext);
        if (ClassWindowManager != NULL) pEnv->DeleteLocalRef(ClassWindowManager);
        if (ClassDisplay != NULL) pEnv->DeleteLocalRef(ClassDisplay);
    }


    screen_rot Configuration::getRotation() {
        return mRotation;
    }
}
