#include "StoreWatcher.h"
#include <unistd.h>


// Private method prototypes.
void deleteGlobalRef(JNIEnv* pEnv, jobject* pRef);
JNIEnv* getJNIEnv(JavaVM* pJavaVM);

void* runWatcher(void* pArgs);
void processEntry(JNIEnv* pEnv, StoreWatcher* pWatcher,
                  StoreEntry* pEntry);



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

    // Caches objects.
    pWatcher->mStoreFront = (*pEnv)->NewGlobalRef(pEnv, pStoreFront);
    if (pWatcher->mStoreFront == NULL) goto ERROR;

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
    if ((pEntry->mType == StoreType_Integer)
     && (strcmp(pEntry->mKey, "watcherCounter") == 0))   {
        ++pEntry->mValue.mInteger;
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
    }
}

