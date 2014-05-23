#ifndef _STORE_H_
#define _STORE_H_

#include "jni.h"
#include <stdint.h>

#define STORE_MAX_CAPACITY	2

typedef  enum{
	StoreType_Boolean,
	StoreType_Byte,
	StoreType_Char,
    StoreType_Double,
    StoreType_Float,
    StoreType_Integer,
    StoreType_Long,
    StoreType_Short,
    StoreType_String,
    StoreType_Color,

    StoreType_BooleanArray,
    StoreType_ByteArray,
    StoreType_CharArray,
    StoreType_DoubleArray,
    StoreType_FloatArray,
    StoreType_IntegerArray,
    StoreType_LongArray,
    StoreType_ShortArray,
    StoreType_StringArray,
    StoreType_ColorArray
}StoreType;

typedef union{
	uint8_t  mBoolean;
    int8_t   mByte;
    uint16_t mChar;
    double   mDouble;
    float    mFloat;
    int32_t  mInteger;
    int64_t  mLong;
    int16_t  mShort;
    char*    mString;
    jobject  mColor;

    uint8_t*  mBooleanArray;
    int8_t*   mByteArray;
    uint16_t* mCharArray;
    double*   mDoubleArray;
    float*    mFloatArray;
    int32_t*  mIntegerArray;
    int64_t*  mLongArray;
    int16_t*  mShortArray;
    char**    mStringArray;
    jobject*  mColorArray;
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