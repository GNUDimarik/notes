#include "com_packtpub_Store.h"
#include <stdint.h>
#include <string.h>
#include "Store.h"


/**
 * Contains the unique store instance in a static variable created
 * when library is loaded.
 */
static Store mStore = { {}, 0 };


/**
 * Initialization/Finialization.
 */
JNIEXPORT void JNICALL Java_com_packtpub_Store_initializeStore
  (JNIEnv* pEnv, jobject pThis) {
    mStore.mLength = 0;
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_finalizeStore
  (JNIEnv* pEnv, jobject pThis) {
    StoreEntry* lEntry = mStore.mEntries;
    StoreEntry* lEntryEnd = lEntry + mStore.mLength;

    // Releases every entry in the store.
    while (lEntry < lEntryEnd) {
        free(lEntry->mKey);
        releaseEntryValue(pEnv, lEntry);

        ++lEntry;
    }
    mStore.mLength = 0;
}

/*
 * Getter/setter on primitives and objects.
 */
JNIEXPORT jboolean JNICALL Java_com_packtpub_Store_getBoolean
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    // Finds the entry if it exists and returns its value.
    // If entry does not exist or is not of the right type, an
    // exception is returned.
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_Boolean)) {
        return lEntry->mValue.mBoolean;
    } else {
        return 0;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setBoolean
  (JNIEnv* pEnv, jobject pThis, jstring pKey, jboolean pBoolean) {
    // Creates a new entry (or finds it if it already exists) in the
    // store.
    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        // Updates entry content with the requested data.
        lEntry->mType = StoreType_Boolean;
        lEntry->mValue.mBoolean = pBoolean;
    }
}

JNIEXPORT jbyte JNICALL Java_com_packtpub_Store_getByte
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_Byte)) {
        return lEntry->mValue.mByte;
    } else {
        return 0;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setByte
  (JNIEnv* pEnv, jobject pThis, jstring pKey, jbyte pByte) {
    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_Byte;
        lEntry->mValue.mByte = pByte;
    }
}

JNIEXPORT jchar JNICALL Java_com_packtpub_Store_getChar
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_Char)) {
        return lEntry->mValue.mChar;
    } else {
        return 0;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setChar
  (JNIEnv* pEnv, jobject pThis, jstring pKey, jchar pChar) {
    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_Char;
        lEntry->mValue.mChar = pChar;
    }
}

JNIEXPORT jdouble JNICALL Java_com_packtpub_Store_getDouble
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_Double)) {
        return lEntry->mValue.mDouble;
    } else {
        return 0.0;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setDouble
  (JNIEnv* pEnv, jobject pThis, jstring pKey, jdouble pDouble) {
    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_Double;
        lEntry->mValue.mDouble = pDouble;
    }
}

JNIEXPORT jfloat JNICALL Java_com_packtpub_Store_getFloat
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_Float)) {
        return lEntry->mValue.mFloat;
    } else {
        return 0.0f;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setFloat
  (JNIEnv* pEnv, jobject pThis, jstring pKey, jfloat pFloat) {
    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_Float;
        lEntry->mValue.mFloat = pFloat;
    }
}

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

JNIEXPORT jlong JNICALL Java_com_packtpub_Store_getLong
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_Long)) {
        return lEntry->mValue.mLong;
    } else {
        return 0;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setLong
  (JNIEnv* pEnv, jobject pThis, jstring pKey, jlong pLong) {
    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_Long;
        lEntry->mValue.mLong = pLong;
    }
}

JNIEXPORT jshort JNICALL Java_com_packtpub_Store_getShort
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_Short)) {
        return lEntry->mValue.mShort;
    } else {
        return 0;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setShort
  (JNIEnv* pEnv, jobject pThis, jstring pKey, jshort pShort) {
    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_Short;
        lEntry->mValue.mShort = pShort;
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

/*
 * Getter/setter on arrays.
 */
JNIEXPORT jbooleanArray JNICALL Java_com_packtpub_Store_getBooleanArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_BooleanArray)) {
        jbooleanArray lJavaArray = (*pEnv)->NewBooleanArray(pEnv, lEntry->mLength);
        if (lJavaArray == NULL) {
            return;
        }

        (*pEnv)->SetBooleanArrayRegion(pEnv, lJavaArray, 0,
            lEntry->mLength, lEntry->mValue.mBooleanArray);

        return lJavaArray;
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setBooleanArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey,
      jbooleanArray pBooleanArray) {
    // Retrieves array content. Here, Get<Primitive>ArrayElements()
    // is used to show the way it works. But as what we want is only
    // a copy, GetBooleanArrayRegion() would be be more efficient.
    jboolean* lArrayTmp = (*pEnv)->GetBooleanArrayElements(pEnv, pBooleanArray, NULL);
    if (lArrayTmp == NULL) {
        return;
    }

    // Finds/creates an entry in the store and fills its content.
    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_BooleanArray;
        // Allocates a new C buffer which is going to hold a copy of
        // the Java array.
        lEntry->mLength = (*pEnv)->GetArrayLength(pEnv, pBooleanArray);
        size_t lBufferLength = lEntry->mLength * sizeof(uint8_t);
        // Malloc return value should theoretically be checked...
        lEntry->mValue.mBooleanArray = (uint8_t*) malloc(lBufferLength);
        memcpy(lEntry->mValue.mBooleanArray, lArrayTmp, lBufferLength);
    }
    // We have performed any modification on the array and thus do
    // not plan to send any modified data back to Java. So uses
    // JNI_ABORT flag for efficiency purpose.
    (*pEnv)->ReleaseBooleanArrayElements(pEnv, pBooleanArray, lArrayTmp, JNI_ABORT);
}

JNIEXPORT jbyteArray JNICALL Java_com_packtpub_Store_getByteArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_ByteArray)) {
        jbyteArray lJavaArray = (*pEnv)->NewByteArray(pEnv, lEntry->mLength);
        if (lJavaArray == NULL) {
            return;
        }

        (*pEnv)->SetByteArrayRegion(pEnv, lJavaArray, 0,
            lEntry->mLength, lEntry->mValue.mByteArray);
        return lJavaArray;
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setByteArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey, jbyteArray pByteArray){
    // Allocates a C array with the same size as the Java array.
    jsize lLength = (*pEnv)->GetArrayLength(pEnv, pByteArray);
    int8_t* lArray = (int8_t*) malloc(lLength * sizeof(int8_t));
    // Copies Java array content directly in this new C array.
    (*pEnv)->GetByteArrayRegion(pEnv, pByteArray, 0, lLength, lArray);
    // GetByteArrayRegion() does not return a value. Thus exceptions
    // need to be checked explicitely (here, an IndexOutOfBound
    // could theoretically occur).
    if ((*pEnv)->ExceptionCheck(pEnv)) {
        free(lArray);
        return;
    }

    // Creates a new store entry containing the C array.
    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_ByteArray;
        lEntry->mLength = lLength;
        lEntry->mValue.mByteArray = lArray;
    } else {
        // If an error occurs, releases what has been allocated.
        free(lArray);
        return;
    }
}

JNIEXPORT jcharArray JNICALL Java_com_packtpub_Store_getCharArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_CharArray)) {
        jcharArray lJavaArray = (*pEnv)->NewCharArray(pEnv, lEntry->mLength);
        if (lJavaArray == NULL) {
            return;
        }

        (*pEnv)->SetCharArrayRegion(pEnv, lJavaArray, 0,
            lEntry->mLength, lEntry->mValue.mCharArray);
        return lJavaArray;
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setCharArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey, jcharArray pCharArray){
    jsize lLength = (*pEnv)->GetArrayLength(pEnv, pCharArray);
    uint16_t* lArray = (uint16_t*) malloc(lLength * sizeof(uint16_t));
    (*pEnv)->GetCharArrayRegion(pEnv, pCharArray, 0, lLength, lArray);
    if ((*pEnv)->ExceptionCheck(pEnv)) {
        free(lArray);
        return;
    }

    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_CharArray;
        lEntry->mLength = lLength;
        lEntry->mValue.mCharArray = lArray;
    } else {
        free(lArray);
        return;
    }
}

JNIEXPORT jdoubleArray JNICALL Java_com_packtpub_Store_getDoubleArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_DoubleArray)) {
        jdoubleArray lJavaArray = (*pEnv)->NewDoubleArray(pEnv, lEntry->mLength);
        if (lJavaArray == NULL) {
            return;
        }

        (*pEnv)->SetDoubleArrayRegion(pEnv, lJavaArray, 0,
            lEntry->mLength, lEntry->mValue.mDoubleArray);
        return lJavaArray;
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setDoubleArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey,
      jdoubleArray pDoubleArray) {
    jsize lLength = (*pEnv)->GetArrayLength(pEnv, pDoubleArray);
    double* lArray = (double*) malloc(lLength * sizeof(double));
    (*pEnv)->GetDoubleArrayRegion(pEnv, pDoubleArray, 0, lLength, lArray);
    if ((*pEnv)->ExceptionCheck(pEnv)) {
        free(lArray);
        return;
    }

    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_DoubleArray;
        lEntry->mLength = lLength;
        lEntry->mValue.mDoubleArray = lArray;
    } else {
        free(lArray);
        return;
    }
}

JNIEXPORT jfloatArray JNICALL Java_com_packtpub_Store_getFloatArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_FloatArray)) {
        jfloatArray lJavaArray = (*pEnv)->NewFloatArray(pEnv, lEntry->mLength);
        if (lJavaArray == NULL) {
            return;
        }

        (*pEnv)->SetFloatArrayRegion(pEnv, lJavaArray, 0,
            lEntry->mLength, lEntry->mValue.mFloatArray);
        return lJavaArray;
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setFloatArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey,
      jfloatArray pFloatArray) {
    jsize lLength = (*pEnv)->GetArrayLength(pEnv, pFloatArray);
    float* lArray = (float*) malloc(lLength * sizeof(float));
    (*pEnv)->GetFloatArrayRegion(pEnv, pFloatArray, 0, lLength, lArray);
    if ((*pEnv)->ExceptionCheck(pEnv)) {
        free(lArray);
        return;
    }

    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_FloatArray;
        lEntry->mLength = lLength;
        lEntry->mValue.mFloatArray = lArray;
    } else {
        free(lArray);
        return;
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
  (JNIEnv* pEnv, jobject pThis, jstring pKey,
      jintArray pIntegerArray) {
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

JNIEXPORT jlongArray JNICALL Java_com_packtpub_Store_getLongArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_LongArray)) {
        jlongArray lJavaArray = (*pEnv)->NewLongArray(pEnv, lEntry->mLength);
        if (lJavaArray == NULL) {
            return;
        }

        (*pEnv)->SetLongArrayRegion(pEnv, lJavaArray, 0,
            lEntry->mLength, lEntry->mValue.mLongArray);
        return lJavaArray;
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setLongArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey, jlongArray pLongArray){
    jsize lLength = (*pEnv)->GetArrayLength(pEnv, pLongArray);
    int64_t* lArray = (int64_t*) malloc(lLength * sizeof(int64_t));
    (*pEnv)->GetLongArrayRegion(pEnv, pLongArray, 0, lLength, lArray);
    if ((*pEnv)->ExceptionCheck(pEnv)) {
        free(lArray);
        return;
    }

    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_LongArray;
        lEntry->mLength = lLength;
        lEntry->mValue.mLongArray = lArray;
    } else {
        free(lArray);
        return;
    }
}

JNIEXPORT jshortArray JNICALL Java_com_packtpub_Store_getShortArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_ShortArray)) {
        jshortArray lJavaArray = (*pEnv)->NewShortArray(pEnv, lEntry->mLength);
        if (lJavaArray == NULL) {
            return;
        }

        (*pEnv)->SetShortArrayRegion(pEnv, lJavaArray, 0,
            lEntry->mLength, lEntry->mValue.mShortArray);
        return lJavaArray;
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setShortArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey,
      jshortArray pShortArray) {
    jsize lLength = (*pEnv)->GetArrayLength(pEnv, pShortArray);
    int16_t* lArray = (int16_t*) malloc(lLength * sizeof(int16_t));
    (*pEnv)->GetShortArrayRegion(pEnv, pShortArray, 0, lLength, lArray);
    if ((*pEnv)->ExceptionCheck(pEnv)) {
        free(lArray);
        return;
    }

    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_ShortArray;
        lEntry->mLength = lLength;
        lEntry->mValue.mShortArray = lArray;
    } else {
        free(lArray);
        return;
    }
}

JNIEXPORT jobjectArray JNICALL Java_com_packtpub_Store_getStringArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* lEntry = findEntry(pEnv, &mStore, pKey, NULL);
    if (isEntryValid(pEnv, lEntry, StoreType_StringArray)) {
        // An array of String in Java is in fact an array of object.
        jclass lStringClass = (*pEnv)->FindClass(pEnv, "java/lang/String");
        if (lStringClass == NULL) {
            return NULL;
        }
        jobjectArray lJavaArray = (*pEnv)->NewObjectArray(
            pEnv, lEntry->mLength, lStringClass, NULL);
        (*pEnv)->DeleteLocalRef(pEnv, lStringClass);
        if (lJavaArray == NULL) {
            return NULL;
        }

        // Creates a new Java String object for each C string stored.
        // Reference to the String can be removed right after it is
        // added to the Java array, as the latter holds a reference
        // to the String object.
        int32_t i;
        for (i = 0; i < lEntry->mLength; ++i) {
            jstring lString = (*pEnv)->NewStringUTF(pEnv,
                lEntry->mValue.mStringArray[i]);
            if (lString == NULL) {
                return NULL;
            }

            // Puts the new string in the array. Exception are
            // checked because of SetObjectArrayElement() (can raise
            // an ArrayIndexOutOfBounds or ArrayStore Exception).
            // If one occurs, any object created here will be freed
            // as they are all referenced locally only.
            (*pEnv)->SetObjectArrayElement(pEnv, lJavaArray, i,
                                           lString);
            // Note that DeleteLocalRef() can still be called safely
            // even if an exception is raised.
            (*pEnv)->DeleteLocalRef(pEnv, lString);
            if ((*pEnv)->ExceptionCheck(pEnv)) {
                return NULL;
            }
        }
        return lJavaArray;
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL Java_com_packtpub_Store_setStringArray
  (JNIEnv* pEnv, jobject pThis, jstring pKey,
      jobjectArray pStringArray) {
    // Allocates an array of C string.
    jsize lLength = (*pEnv)->GetArrayLength(pEnv, pStringArray);
    char** lArray = (char**) malloc(lLength * sizeof(char*));
    // Fills the C array with a copy of each input Java string.
    int32_t i, j;
    for (i = 0; i < lLength; ++i) {
        // Gets the current Java String from the input Java array.
        // Object arrays can be accessed element by element only.
        jstring lString = (*pEnv)->GetObjectArrayElement(pEnv, pStringArray, i);
        if ((*pEnv)->ExceptionCheck(pEnv)) {
            for (j = 0; j < i; ++j) {
                free(lArray[j]);
            }
            free(lArray);
            return;
        }

        jsize lStringLength = (*pEnv)->GetStringUTFLength(pEnv, lString);
        // Malloc return value should theoretically be checked...
        lArray[i] = (char*) malloc(sizeof(char) * (lStringLength + 1));
        // Directly copies the Java String into our new C buffer.
        // This is usually faster than GetStringUTFChars() which
        // requires copying manually.
        (*pEnv)->GetStringUTFRegion(pEnv, lString, 0, lStringLength, lArray[i]);
        if ((*pEnv)->ExceptionCheck(pEnv)) {
            for (j = 0; j < i; ++j) {
                free(lArray[j]);
            }
            free(lArray);
            return;
        }
        // No need to keep a reference to the Java string anymore.
        (*pEnv)->DeleteLocalRef(pEnv, lString);
    }

    // Creates a new entry with the new String array.
    StoreEntry* lEntry = allocateEntry(pEnv, &mStore, pKey);
    if (lEntry != NULL) {
        lEntry->mType = StoreType_StringArray;
        lEntry->mLength = lLength;
        lEntry->mValue.mStringArray = lArray;
    } else {
        for (j = 0; j < lLength; ++j) {
            free(lArray[j]);
        }
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
