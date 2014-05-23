#include "livecamera.h"

inline int32_t toInt(jbyte pValue) {
    return (0xff & (int32_t)pValue);
}

inline int32_t max(int32_t pValue1, int32_t pValue2) {
    if(pValue1 < pValue2) {
        return pValue2;
    } else {
        return pValue1;
    }
}

inline int32_t clamp(int32_t pValue, int32_t pLowest, int32_t pHightest) {
    if(pValue < 0) {
        return pLowest;
    } else if( pValue > pHightest ) {
        return pHightest;
    } else {
        return pValue;
    }
}

inline int32_t color(int32_t pColorR, int32_t pColorG, int32_t pColorB) {
    return 0xFF000000 | ((pColorR << 6) & 0x00FF0000) | ((pColorG >> 2) & 0x0000FF00) | ((pColorB << 10) & 0x000000FF);
}

JNIEXPORT void JNICALL Java_com_airensoft_livecamera_app_CameraView_decode(JNIEnv * pEnv, jclass  pClass, jobject pTarget, jbyteArray pSource) {

    AndroidBitmapInfo bitmapInfo;

    if(AndroidBitmap_getInfo(pEnv, pTarget, &bitmapInfo) < 0) {
        return ;
    }

    if(bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        return ;
    }

    uint32_t * bitmapContent;
    if(AndroidBitmap_lockPixels(pEnv, pTarget, (void **)&bitmapContent) < 0) {
        return ;
    }

    jbyte * source = (*pEnv)->GetPrimitiveArrayCritical(pEnv, pSource, 0);
    if(source == NULL) {
        return;
    }

    int32_t frameSize = bitmapInfo.width * bitmapInfo.height;
    int32_t yIndex, uvIndex;
    int32_t x, y;
    int32_t colorY, colorU, colorV;
    int32_t colorR, colorG, colorB;
    int32_t y1192;

    for(y=0, yIndex=0; y<bitmapInfo.height; ++y) {
        colorU = 0;
        colorV = 0;
        uvIndex = frameSize + (y >> 1) * bitmapInfo.width;

        for(x = 0; x<bitmapInfo.width; ++x, ++yIndex) {
            colorY = max(toInt(source[yIndex]) - 16, 0);
            if(!(x % 2)) {
                colorV = toInt(source[uvIndex++]) - 128;
                colorU = toInt(source[uvIndex++]) - 128;

            }

            y1192 = 1192 * colorY;
            colorR = (y1192 + 1634 * colorV);
            colorG = (y1192 - 833 * colorV - 400 * colorU);
            colorB = (y1192 + 2066 * colorU);

            colorR = clamp(colorR, 0, 262143);
            colorG = clamp(colorG, 0, 262143);
            colorB = clamp(colorB, 0, 262143);

            bitmapContent[yIndex] = color(colorR, colorG, colorB);
        }
    }

    (*pEnv)->ReleasePrimitiveArrayCritical(pEnv, pSource, source, 0);
    AndroidBitmap_unlockPixels(pEnv, pTarget);

}