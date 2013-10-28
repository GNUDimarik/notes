#ifndef _STORE_H_
#define _STORE_H_

#include "jni.h"
#include <stdint.h>


/*
 * Constants.
 */
#define STORE_MAX_CAPACITY 16


/*
 * Types handled by the store.
 */
typedef enum {
    StoreType_Integer, StoreType_String
} StoreType;


/**
 * Store data structures.
 */
typedef union {
    int32_t  mInteger;
    char*    mString;
} StoreValue;

typedef struct {
    char* mKey;
    StoreType mType;
    StoreValue mValue;
} StoreEntry;

typedef struct {
    StoreEntry mEntries[STORE_MAX_CAPACITY];
    int32_t mLength;
} Store;


/*
 * Store helper methods.
 */
int32_t isEntryValid(JNIEnv* pEnv, StoreEntry* pEntry,
                     StoreType pType);

StoreEntry* allocateEntry(JNIEnv* pEnv, Store* pStore, jstring pKey);

StoreEntry* findEntry(JNIEnv* pEnv, Store* pStore, jstring pKey,
                      int32_t* pError);

void releaseEntryValue(JNIEnv* pEnv, StoreEntry* pEntry);

#endif
