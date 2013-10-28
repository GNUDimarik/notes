#include "com_packtpub_Store.h"
#include "Store.h"
#include <stdint.h>
#include <string.h>


/**
 * Contains the unique store instance in a static variable created
 * when library is loaded.
 */
static Store mStore = { {}, 0 };


JNIEXPORT jint JNICALL Java_com_packtpub_Store_getInteger
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_Integer)) {
        return lEntry->mValue.mInteger;
    } else {
        return 0;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setInteger
  (JNIEnv* pEnv, jobject pThis, jstring pKey, jint pInteger) {
    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_Integer;
        lEntry->mValue.mInteger = pInteger;
    }
}

JNIEXPORT jstring JNICALL Java_com_packtpub_Store_getString
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_String)) {
        // Converts a C string into a Java String.
        return (*pEnv)->NewStringUTF(pEnv, lEntry->mValue.mString);
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setString
  (JNIEnv* pEnv, jobject pThis, jstring pKey, jstring pString) {
    // Turns the Java string into a temporary C string.
    // GetStringUTFChars() is used here as an example but
    // Here, GetStringUTFChars() to show
    // the way it works. But as what we want is only a copy,
    // GetBooleanArrayRegion() would be be more efficient.
    const char* lStringTmp = (*pEnv)->GetStringUTFChars(pEnv, pString, NULL);
    if (lStringTmp == NULL) {
        return;
    }

    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_String;
        // Copy the temporary C string into its dynamically allocated
        // final location. Then releases the temporary string.
        // Malloc return value should theoretically be checked...
        jsize lStringLength = (*pEnv)->GetStringUTFLength(pEnv, pString);
        lEntry->mValue.mString =
            (char*) malloc(sizeof(char) * (lStringLength + 1));
        strcpy(lEntry->mValue.mString, lStringTmp);
    }
    (*pEnv)->ReleaseStringUTFChars(pEnv, pString, lStringTmp);
}

JNIEXPORT jobject JNICALL Java_com_packtpub_Store_getColor
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_Color)) {
        // Returns a Java object.
        return lEntry->mValue.mColor;
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setColor
  (JNIEnv* pEnv, jobject pThis, jstring pKey, jobject pColor) {
    // The Java Color is going to be stored on the native side.
    // Need to keep a global reference to avoid a potential
    // garbage collection after method returns.
    jobject lColor = (*pEnv)->NewGlobalRef(pEnv, pColor);
    if (lColor == NULL) {
        return;
    }

    // Save the Color reference in the store.
    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_Color;
        lEntry->mValue.mColor = lColor;
    } else {
        (*pEnv)->DeleteGlobalRef(pEnv, lColor);
    }
}
