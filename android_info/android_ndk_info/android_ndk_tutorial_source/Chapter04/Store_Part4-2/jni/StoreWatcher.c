#include "StoreWatcher.h"
#include <unistd.h>


// Private method prototypes.
void makeGlobalRef(JNIEnv* pEnv, jobject* pRef);
void deleteGlobalRef(JNIEnv* pEnv, jobject* pRef);
JNIEnv* getJNIEnv(JavaVM* pJavaVM);

void* runWatcher(void* pArgs);
void processEntry(JNIEnv* pEnv, StoreWatcher* pWatcher,
                  StoreEntry* pEntry);
void processEntryInt(JNIEnv* pEnv, StoreWatcher* pWatcher,
                     StoreEntry* pEntry);
void processEntryString(JNIEnv* pEnv, StoreWatcher* pWatcher,
                        StoreEntry* pEntry);
void processEntryColor(JNIEnv* pEnv, StoreWatcher* pWatcher,
                        StoreEntry* pEntry);



void makeGlobalRef(JNIEnv* pEnv, jobject* pRef) {
    if (*pRef != NULL) {
        jobject lGlobalRef = (*pEnv)->NewGlobalRef(pEnv, *pRef);
        // No need for a local reference any more.
        (*pEnv)->DeleteLocalRef(pEnv, *pRef);
        // Here, lGlobalRef may be null.
        *pRef = lGlobalRef;
    }
}

void startWatcher(JNIEnv* pEnv, StoreWatcher* pWatcher,
    Store* pStore, jobject pStoreFront) {
    // Erases the StoreWatcher structure.
    memset(pWatcher, 0, sizeof(StoreWatcher));
    pWatcher->mState = STATE_OK;
    pWatcher->mStore = pStore;
    // Caches the VM.
    if ((*pEnv)->GetJavaVM(pEnv, &pWatcher->mJavaVM) != JNI_OK) {
        goto ERROR;
    }

    // Caches classes.
    pWatcher->ClassStore = (*pEnv)->FindClass(pEnv,
        "com/packtpub/Store");
    makeGlobalRef(pEnv, &pWatcher->ClassStore);
    if (pWatcher->ClassStore == NULL) goto ERROR;

    pWatcher->ClassColor = (*pEnv)->FindClass(pEnv,
        "com/packtpub/Color");
    makeGlobalRef(pEnv, &pWatcher->ClassColor);
    if (pWatcher->ClassColor == NULL) goto ERROR;


    // Caches Java methods.
    pWatcher->MethodOnAlertInt = (*pEnv)->GetMethodID(pEnv,
        pWatcher->ClassStore, "onAlert", "(I)V");
    if (pWatcher->MethodOnAlertInt == NULL) goto ERROR;

    pWatcher->MethodOnAlertString = (*pEnv)->GetMethodID(pEnv,
        pWatcher->ClassStore, "onAlert", "(Ljava/lang/String;)V");
    if (pWatcher->MethodOnAlertString == NULL) goto ERROR;

    pWatcher->MethodOnAlertColor = (*pEnv)->GetMethodID(pEnv,
        pWatcher->ClassStore, "onAlert", "(Lcom/packtpub/Color;)V");
    if (pWatcher->MethodOnAlertColor == NULL) goto ERROR;

    pWatcher->MethodColorEquals = (*pEnv)->GetMethodID(pEnv,
        pWatcher->ClassColor, "equals", "(Ljava/lang/Object;)Z");
    if (pWatcher->MethodColorEquals == NULL) goto ERROR;

    jmethodID ConstructorColor = (*pEnv)->GetMethodID(pEnv,
        pWatcher->ClassColor, "<init>", "(Ljava/lang/String;)V");
    if (ConstructorColor == NULL) goto ERROR;


    // Caches objects.
    pWatcher->mStoreFront = (*pEnv)->NewGlobalRef(pEnv, pStoreFront);
    if (pWatcher->mStoreFront == NULL) goto ERROR;
    // Creates a new white color and keeps a global reference.
    jstring lColorString = (*pEnv)->NewStringUTF(pEnv, "white");
    if (lColorString == NULL) goto ERROR;

    pWatcher->mColor = (*pEnv)->NewObject(pEnv, pWatcher->ClassColor,
        ConstructorColor, lColorString);
    makeGlobalRef(pEnv, &pWatcher->mColor);
    if (pWatcher->mColor == NULL) goto ERROR;

    // Initializes and launches the native thread. For simplicity
    // purpose, error results are not checked (but we should...).
    pthread_attr_t lAttributes;
    int lError = pthread_attr_init(&lAttributes);
    if (lError) goto ERROR;

    lError = pthread_create(&pWatcher->mThread, &lAttributes,
                            runWatcher, pWatcher);
    if (lError) goto ERROR;

    // Everything went OK.
    return;

ERROR:
    stopWatcher(pEnv, pWatcher);
    return;
}

JNIEnv* getJNIEnv(JavaVM* pJavaVM) {
    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name = "NativeThread";
    lJavaVMAttachArgs.group = NULL;

    JNIEnv* lEnv;
    if ((*pJavaVM)->AttachCurrentThread(pJavaVM, &lEnv,
                          &lJavaVMAttachArgs) != JNI_OK) {
        lEnv = NULL;
    }
    return lEnv;
}

void* runWatcher(void* pArgs) {
    StoreWatcher* lWatcher = (StoreWatcher*) pArgs;
    Store* lStore = lWatcher->mStore;
    JavaVM* lJavaVM = lWatcher->mJavaVM;

    // Attaches current thread to the Dalvik VM so that it is known
    // from Java and can call Java code.
    JNIEnv* lEnv = getJNIEnv(lJavaVM);
    if (lEnv == NULL) goto ERROR;

    // Runs the thread loop.
    int32_t lRunning = 1;
    while (lRunning) {
        sleep(SLEEP_DURATION);

        StoreEntry* lEntry = lWatcher->mStore->mEntries;
        int32_t lScanning = 1;
        while (lScanning) {
            // Critical section begining, one thread at a time.
            // Entries cannot be added or modified.
            (*lEnv)->MonitorEnter(lEnv, lWatcher->mStoreFront);
            lRunning = (lWatcher->mState == STATE_OK);
            StoreEntry* lEntryEnd = lWatcher->mStore->mEntries
                                  + lWatcher->mStore->mLength;
            lScanning = (lEntry < lEntryEnd);

            if (lRunning && lScanning) {
                processEntry(lEnv, lWatcher, lEntry);
            }

            // Critical section end.
            (*lEnv)->MonitorExit(lEnv, lWatcher->mStoreFront);
            // Goes to next element.
            ++lEntry;
        }
    }

ERROR:
    // Stops the thread. It is very important to always detach an
    // attached thread. Allowed if thread is already detached.
    (*lJavaVM)->DetachCurrentThread(lJavaVM);
    pthread_exit(NULL);
}

void processEntry(JNIEnv* pEnv, StoreWatcher* pWatcher,
                  StoreEntry* pEntry) {
    switch (pEntry->mType) {
    case StoreType_Integer:
        processEntryInt(pEnv, pWatcher, pEntry);
        break;
    case StoreType_String:
        processEntryString(pEnv, pWatcher, pEntry);
        break;
    case StoreType_Color:
        processEntryColor(pEnv, pWatcher, pEntry);
        break;
    }
}

void processEntryInt(JNIEnv* pEnv, StoreWatcher* pWatcher,
                     StoreEntry* pEntry) {
    if (strcmp(pEntry->mKey, "watcherCounter") == 0) {
        ++pEntry->mValue.mInteger;
    } else if ((pEntry->mValue.mInteger > 1000) ||
        (pEntry->mValue.mInteger < -1000)) {
        (*pEnv)->CallVoidMethod(pEnv,
            pWatcher->mStoreFront, pWatcher->MethodOnAlertInt,
            (jint) pEntry->mValue.mInteger);
    }
}

void processEntryString(JNIEnv* pEnv, StoreWatcher* pWatcher,
                        StoreEntry* pEntry) {
    if (strcmp(pEntry->mValue.mString, "apple")) {
        jstring lValue = (*pEnv)->NewStringUTF(
            pEnv, pEntry->mValue.mString);
        // Sends a message back to the UI.
        (*pEnv)->CallVoidMethod(pEnv,
            pWatcher->mStoreFront, pWatcher->MethodOnAlertString,
            lValue);
        // We are in a utility method. So releases local references
        // because we do not know the context it are used in.
        (*pEnv)->DeleteLocalRef(pEnv, lValue);
    }
}

void processEntryColor(JNIEnv* pEnv, StoreWatcher* pWatcher,
                       StoreEntry* pEntry) {
    // Checks if entry color is white.
    jboolean lResult = (*pEnv)->CallBooleanMethod(
            pEnv, pWatcher->mColor,
            pWatcher->MethodColorEquals, pEntry->mValue.mColor);
    if (lResult) {
        // Sends a message back to the UI.
        (*pEnv)->CallVoidMethod(pEnv,
            pWatcher->mStoreFront, pWatcher->MethodOnAlertColor,
            pEntry->mValue.mColor);
    }
}

void deleteGlobalRef(JNIEnv* pEnv, jobject* pRef) {
    if (*pRef != NULL) {
        (*pEnv)->DeleteGlobalRef(pEnv, *pRef);
        *pRef = NULL;
    }
}

void stopWatcher(JNIEnv* pEnv, StoreWatcher* pWatcher) {
    if (pWatcher->mState == STATE_OK) {
        // Waits for the watcher thread to stop.
        (*pEnv)->MonitorEnter(pEnv, pWatcher->mStoreFront);
        pWatcher->mState = STATE_KO;
        (*pEnv)->MonitorExit(pEnv, pWatcher->mStoreFront);
        pthread_join(pWatcher->mThread, NULL);

        // Deletes global references to avoid memory leaks.
        deleteGlobalRef(pEnv, &pWatcher->mStoreFront);
        deleteGlobalRef(pEnv, &pWatcher->mColor);
        deleteGlobalRef(pEnv, &pWatcher->ClassStore);
        deleteGlobalRef(pEnv, &pWatcher->ClassColor);
    }
}
