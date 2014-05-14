#ifndef _STORE_H_
#define _STORE_H_

#include "jni.h"
#include <stdint.h>

#define STORE_MAX_CAPACITY	2

typedef  enum{
	StoreType_Integer,
	StoreType_String,
	StoreType_Color,
	StoreType_IntegerArray,
	StoreType_ColorArray
}StoreType;

typedef union{
	int32_t     mInteger;
	char*	    mString;
	jobject     mColor;
	int32_t*    mIntegerArray;
	jobject*    mColorArray;
}StoreValue;

typedef struct{
	char * mKey;
	StoreType mType;
	StoreValue mValue;
	int32_t mLength;
}StoreEntry;

typedef struct {
	StoreEntry mEntries[STORE_MAX_CAPACITY];
	int32_t mLength;
}Store;

int32_t isEntryValid(JNIEnv * pEnv, StoreEntry * pEntry, StoreType pType);
StoreEntry * allocateEntry(JNIEnv * pEnv, Store * pStore, jstring pKey);	// 엔트리를 할당하고 반환한다. 반환된 엔트리에 값을 설정한다
StoreEntry * findEntry(JNIEnv * pEnv, Store * pStore, jstring pKey, int32_t * pError);
void releaseEntryValue(JNIEnv * pEnv, StoreEntry * pEntry);

void throwInvalidTypeException(JNIEnv *pEnv);
void throwNotExistingKeyException(JNIEnv *pEnv);
void throwStoreFullException(JNIEnv *pEnv);

#endif