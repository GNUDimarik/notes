#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "JNI.hpp"
#include "Types.hpp"

namespace packt {
    class Keyboard {
    public:
        Keyboard(JNI* pJNI);

        status initialize();
        void finalize();

        void show();
        void hide();

    private:
        JNI* mJNI;
        // Classes.
        jclass ClassActivity;
        jclass ClassContext;
        jclass ClassWindow;
        jclass ClassView;
        jclass ClassInputMethodManager;
        // Methods.
        jmethodID MethodGetSystemService;
        jmethodID MethodGetWindow;
        jmethodID MethodGetDecorView;
        jmethodID MethodGetWindowToken;
        jmethodID MethodShowSoftInput;
        jmethodID MethodHideSoftInput;
        // Fields.
        jfieldID FieldINPUT_METHOD_SERVICE;
    };
}
#endif
