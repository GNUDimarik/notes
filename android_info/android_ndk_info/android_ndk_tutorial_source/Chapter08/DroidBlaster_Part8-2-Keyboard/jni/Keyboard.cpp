#include "Keyboard.hpp"
#include "Log.hpp"


namespace packt {
    Keyboard::Keyboard(JNI* pJNI):
        mJNI(pJNI),
        ClassActivity(NULL), ClassContext(NULL), ClassWindow(NULL),
        ClassView(NULL), ClassInputMethodManager(NULL) {
    }

    status Keyboard::initialize() {
        JNIEnv* lEnv = mJNI->getEnv();

        // Classes.
        ClassActivity = lEnv->GetObjectClass(mJNI->getActivity());
        mJNI->makeGlobalRef(&ClassActivity);
        if (ClassActivity == NULL) goto ERROR;

        ClassContext = lEnv->FindClass("android/content/Context");
        mJNI->makeGlobalRef(&ClassContext);
        if (ClassContext == NULL) goto ERROR;

        ClassWindow = lEnv->FindClass("android/view/Window");
        mJNI->makeGlobalRef(&ClassWindow);
        if (ClassWindow == NULL) goto ERROR;

        ClassView = lEnv->FindClass("android/view/View");
        mJNI->makeGlobalRef(&ClassView);
        if (ClassView == NULL) goto ERROR;

        ClassInputMethodManager = lEnv->FindClass(
            "android/view/inputmethod/InputMethodManager");
        if (ClassInputMethodManager == NULL) goto ERROR;

        // Methods.
        MethodGetSystemService = lEnv->GetMethodID(ClassActivity,
            "getSystemService",
            "(Ljava/lang/String;)Ljava/lang/Object;");
        if (MethodGetSystemService == NULL) goto ERROR;
        MethodGetWindow = lEnv->GetMethodID(ClassActivity,
            "getWindow", "()Landroid/view/Window;");
        if (MethodGetWindow == NULL) goto ERROR;
        MethodGetDecorView = lEnv->GetMethodID(ClassWindow,
            "getDecorView", "()Landroid/view/View;");
        if (MethodGetDecorView == NULL) goto ERROR;
        MethodGetWindowToken = lEnv->GetMethodID(ClassView,
            "getWindowToken", "()Landroid/os/IBinder;");
        if (MethodGetWindowToken == NULL) goto ERROR;
        MethodHideSoftInput = lEnv->GetMethodID(
            ClassInputMethodManager, "hideSoftInputFromWindow",
            "(Landroid/os/IBinder;I)Z");
        if (MethodHideSoftInput == NULL) goto ERROR;
        MethodShowSoftInput = lEnv->GetMethodID(
            ClassInputMethodManager, "showSoftInput",
            "(Landroid/view/View;I)Z");
        if (MethodShowSoftInput == NULL) goto ERROR;

        // Fields.
        FieldINPUT_METHOD_SERVICE = lEnv->GetStaticFieldID(
          ClassContext, "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
        if (FieldINPUT_METHOD_SERVICE == NULL) goto ERROR;
        return STATUS_OK;

    ERROR:
        finalize();
        return STATUS_KO;
    }

    void Keyboard::finalize() {
        mJNI->deleteGlobalRef(&ClassActivity);
        mJNI->deleteGlobalRef(&ClassContext);
        mJNI->deleteGlobalRef(&ClassWindow);
        mJNI->deleteGlobalRef(&ClassView);
    }

    void Keyboard::show() {
        JNIEnv* lEnv = mJNI->getEnv();
        jobject lActivity = mJNI->getActivity();

        // Retrieves Context.INPUT_METHOD_SERVICE.
        jobject INPUT_METHOD_SERVICE = lEnv->GetStaticObjectField(
            ClassContext, FieldINPUT_METHOD_SERVICE);

        // Runs getSystemService(Context.INPUT_METHOD_SERVICE).
        jobject lInputMethodManager = lEnv->CallObjectMethod(
            lActivity, MethodGetSystemService, INPUT_METHOD_SERVICE);

        // Runs pActivity.getWindow().getDecorView().getWindowToken()
        jobject lWindow = lEnv->CallObjectMethod(lActivity,
            MethodGetWindow);
        jobject lDecorView = lEnv->CallObjectMethod(lWindow,
            MethodGetDecorView);
        jobject lBinder = lEnv->CallObjectMethod(lDecorView,
            MethodGetWindowToken);

        // Runs lInputMethodManager.showSoftInput(...).
        jboolean lResult = lEnv->CallBooleanMethod(
            lInputMethodManager, MethodShowSoftInput, lDecorView, 0);
    }

    void Keyboard::hide() {
        JNIEnv* lEnv = mJNI->getEnv();
        jobject lActivity = mJNI->getActivity();

        // Retrieves Context.INPUT_METHOD_SERVICE.
        jobject INPUT_METHOD_SERVICE = lEnv->GetStaticObjectField(
            ClassContext, FieldINPUT_METHOD_SERVICE);

        // Runs getSystemService(Context.INPUT_METHOD_SERVICE).
        jobject lInputMethodManager = lEnv->CallObjectMethod(
        lActivity, MethodGetSystemService, FieldINPUT_METHOD_SERVICE);

        // Runs pActivity.getWindow().getDecorView().getWindowToken()
        jobject lWindow = lEnv->CallObjectMethod(lActivity,
            MethodGetWindow);
        jobject lDecorView = lEnv->CallObjectMethod(lWindow,
            MethodGetDecorView);
        jobject lBinder = lEnv->CallObjectMethod(lDecorView,
            MethodGetWindowToken);

        // lInputMethodManager.hideSoftInput(...).
        lEnv->CallBooleanMethod(lInputMethodManager,
            MethodHideSoftInput, lBinder, 0);
    }
}
