#ifndef _PACKT_RESOURCE_HPP_
#define _PACKT_RESOURCE_HPP_

#include "Types.hpp"

#include <android_native_app_glue.h>

namespace packt {
    class Resource {
    public:
        Resource(android_app* pApplication, const char* pPath);

        status open();
        void close();
        status read(void* pBuffer, size_t pCount);

        const char* getPath();

    private:
        const char* mPath;
        AAssetManager* mAssetManager;
        AAsset* mAsset;
    };
}
#endif
