#include "mystorelib.h"
#include "Store.h"
#include <stdint.h>
#include <string.h>

static Store mStore = { {}, 0 };

/*
 * Class:     com_airensoft_store_app_Store
 * Method:    getInteger
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_airensoft_store_app_Store_getInteger
(JNIEnv * pEnv, jobject pThis, jstring pKey) {
	StoreEntry * lEntry = findEntry(pEnv, &mStore, pKey, NULL);
	if(isEntryValid(pEnv, lEntry, StoreType_Integer)) {
		return lEntry->mValue.mInteger;
	} else {
		return 0;
	}
}

/*
 * Class:     com_airensoft_store_app_Store
 * Method:    setInteger
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_com_airensoft_store_app_Store_setInteger
(JNIEnv * pEnv, jobject pThis, jstring pKey, jint pInteger) {
	StoreEntry * lEntry = allocateEntry(pEnv, &mStore, pKey);
	if(lEntry != NULL) {
		lEntry->mType = StoreType_Integer;
		lEntry->mValue.mInteger = pInteger;
	}
}

/*
 * Class:     com_airensoft_store_app_Store
 * Method:    getString
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_airensoft_store_app_Store_getString
(JNIEnv * pEnv, jobject pThis, jstring pKey) {
	StoreEntry * lEntry = findEntry(pEnv, &mStore, pKey, NULL);
	if(isEntryValid(pEnv, lEntry, StoreType_String)) {
		return (*pEnv)->NewStringUTF(pEnv, lEntry->mValue.mString);
	} else {
		return NULL;
	}
}

/*
 * Class:     com_airensoft_store_app_Store
 * Method:    setString
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_airensoft_store_app_Store_setString
(JNIEnv * pEnv, jobject pThis, jstring pKey, jstring pString) {
	const char * lStringTmp = (*pEnv)->GetStringUTFChars(pEnv, pString, NULL);
	if(lStringTmp == NULL) {
		return;
	}
	
	StoreEntry * lEntry = allocateEntry(pEnv, &mStore, pKey);
	if(lEntry != NULL) {
		lEntry->mType = StoreType_String;
		jsize lStringLength = (*pEnv)->GetStringUTFLength(pEnv, pString);
		lEntry->mValue.mString = (char *)malloc(sizeof(char) * (lStringLength+1));
		strcpy(lEntry->mValue.mString, lStringTmp);
	}
	(*pEnv)->ReleaseStringUTFChars(pEnv, pString, lStringTmp);
}

/*
 * Class:     com_airensoft_store_app_Store
 * Method:    getColor
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jobject JNICALL Java_com_airensoft_store_app_Store_getColor
  (JNIEnv * pEnv, jobject pThis, jstring pKey) {
  StoreEntry * lEntry = findEntry(pEnv, &mStore, pKey, NULL);
  	if(isEntryValid(pEnv, lEntry, StoreType_Color)) {
  		return lEntry->mValue.mColor;
  	} else {
  		return NULL;
  	}
}

/*
 * Class:     com_airensoft_store_app_Store
 * Method:    setColor
 * Signature: (Ljava/lang/String;Ljava/lang/jobject;)V
 */
JNIEXPORT void JNICALL Java_com_airensoft_store_app_Store_setColor
  (JNIEnv * pEnv, jobject pThis, jstring pKey, jobject pColor) {
    jobject lColor = (*pEnv)->NewGlobalRef(pEnv, pColor);
    if(lColor == NULL) {
        return;
    }

  	StoreEntry * lEntry = allocateEntry(pEnv, &mStore, pKey);
  	if(lEntry != NULL) {
  		lEntry->mType = StoreType_Color;
  		lEntry->mValue.mColor = lColor;
  	} else {
  	    (*pEnv)->DeleteGlobalRef(pEnv, lColor);
  	}

}