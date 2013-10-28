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

JNIEXPORT jintArray JNICALL Java_com_packtpub_Store_getIntegerArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_IntegerArray)) {
        jintArray lJavaArray = (*pEnv)->NewIntArray(pEnv, lEntry->mLength);
        if (lJavaArray == NULL) {
            return;
        }

        (*pEnv)->SetIntArrayRegion(pEnv, lJavaArray, 0,
            lEntry->mLength, lEntry->mValue.mIntegerArray);
        return lJavaArray;
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setIntegerArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey, jintArray pIntegerArray) {
    jsize lLength = (*pEnv)->GetArrayLength(pEnv, pIntegerArray);
    int32_t* lArray = (int32_t*) malloc(lLength * sizeof(int32_t));
    (*pEnv)->GetIntArrayRegion(pEnv, pIntegerArray, 0, lLength, lArray);
    if ((*pEnv)->ExceptionCheck(pEnv)) {
        free(lArray);
        return;
    }

    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_IntegerArray;
        lEntry->mLength = lLength;
        lEntry->mValue.mIntegerArray = lArray;
    } else {
        free(lArray);
        return;
    }
}

JNIEXPORT jobjectArray JNICALL Java_com_packtpub_Store_getColorArray
(JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_ColorArray)) {
        // Creates a new array with objects of type Id.
        jclass lColorClass = (*pEnv)->FindClass(pEnv, "com/packtpub/Color");
        if (lColorClass == NULL) {
            return NULL;
        }
        jobjectArray lJavaArray = (*pEnv)->NewObjectArray(
            pEnv, lEntry->mLength, lColorClass, NULL);
        (*pEnv)->DeleteLocalRef(pEnv, lColorClass);
        if (lJavaArray == NULL) {
            return NULL;
        }

        // Fills the array with the Color objects stored on the native
        // side, which keeps a global reference to them. So no need
        // to delete or create any reference here.
        int32_t i;
        for (i = 0; i < lEntry->mLength; ++i) {
            (*pEnv)->SetObjectArrayElement(pEnv, lJavaArray, i,
                lEntry->mValue.mColorArray[i]);
            if ((*pEnv)->ExceptionCheck(pEnv)) {
                return NULL;
            }
        }
        return lJavaArray;
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setColorArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey,
      jobjectArray pColorArray) {
    // Allocates a C array of Color objects.
    jsize lLength = (*pEnv)->GetArrayLength(pEnv, pColorArray);
    jobject* lArray = (jobject*) malloc(lLength * sizeof(jobject));
    int32_t i, j;
    for (i = 0; i < lLength; ++i) {
        // Gets the current Color object from the input Java array.
        // Object arrays can be accessed element by element only.
        jobject lLocalColor = (*pEnv)->GetObjectArrayElement(pEnv, pColorArray, i);
        if (lLocalColor == NULL) {
            for (j = 0; j < i; ++j) {
                (*pEnv)->DeleteGlobalRef(pEnv, lArray[j]);
            }
            free(lArray);
            return;
        }

        // The Java Color is going to be stored on the native side.
        // Need to keep a global reference to avoid a potential
        // garbage collection after method returns.
        lArray[i] = (*pEnv)->NewGlobalRef(pEnv, lLocalColor);
        if (lArray[i] == NULL) {
            for (j = 0; j < i; ++j) {
                (*pEnv)->DeleteGlobalRef(pEnv, lArray[j]);
            }
            free(lArray);
            return;
        }
        // We have a global reference to the Color, so we can now get
        // rid of the local one.
        (*pEnv)->DeleteLocalRef(pEnv, lLocalColor);
    }

    // Saves the Color array in the store.
    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_ColorArray;
        lEntry->mLength = lLength;
        lEntry->mValue.mColorArray = lArray;
    } else {
        // If an exception happens, global references must be
        // carefully destroyed or objects will never get garbage
        // collected (as we finally decide not to store them).
        for (j = 0; j < i; ++j) {
            (*pEnv)->DeleteGlobalRef(pEnv, lArray[j]);
        }
        free(lArray);
        return;
    }
}
