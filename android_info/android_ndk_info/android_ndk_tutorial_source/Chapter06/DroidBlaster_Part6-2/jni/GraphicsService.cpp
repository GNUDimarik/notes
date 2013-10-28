#include "GraphicsService.hpp"
#include "Log.hpp"
#include "Resource.hpp"

#include <GLES/gl.h>
#include <GLES/glext.h>


namespace packt {
    GraphicsService::GraphicsService(android_app* pApplication,
                                     TimeService* pTimeService) :
        mApplication(pApplication),
        mTimeService(pTimeService),
        mWidth(0), mHeight(0),
        mDisplay(EGL_NO_DISPLAY),
        mSurface(EGL_NO_CONTEXT),
        mContext(EGL_NO_SURFACE),
        mTextures(), mTextureCount(0) {
        Log::info("Creating GraphicsService.");
    }

    GraphicsService::~GraphicsService() {
        Log::info("Destructing GraphicsService.");

        // Deletes all textures.
        for (int32_t i = 0; i < mTextureCount; ++i) {
            delete mTextures[i];
            mTextures[i] = NULL;
        }
        mTextureCount = 0;
    }

    const int32_t& GraphicsService::getHeight() {
        return mHeight;
    }

    const int32_t& GraphicsService::getWidth() {
        return mWidth;
    }

    status GraphicsService::start() {
        Log::info("Starting GraphicsService.");

        EGLint lFormat, lNumConfigs, lErrorResult;
        EGLConfig lConfig;
        // Defines display requirements. 16bits mode here.
        const EGLint lAttributes[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
            EGL_BLUE_SIZE, 5, EGL_GREEN_SIZE, 6, EGL_RED_SIZE, 5,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
        };

        // Retrieves a display connection and initializes it.
        packt_Log_debug("Connecting to the display.");
        mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (mDisplay == EGL_NO_DISPLAY) goto ERROR;
        if (!eglInitialize(mDisplay, NULL, NULL)) goto ERROR;

        // Selects the first OpenGL configuration found.
        packt_Log_debug("Selecting a display config.");
        if(!eglChooseConfig(mDisplay, lAttributes, &lConfig, 1,
            &lNumConfigs) || (lNumConfigs <= 0)) goto ERROR;

        // Reconfigures the Android window with the EGL format.
        packt_Log_debug("Configuring window format.");
        if (!eglGetConfigAttrib(mDisplay, lConfig,
            EGL_NATIVE_VISUAL_ID, &lFormat)) goto ERROR;
        ANativeWindow_setBuffersGeometry(mApplication->window, 0, 0,
            lFormat);
        // Creates the display surface.
        packt_Log_debug("Initializing the display.");
        mSurface = eglCreateWindowSurface(mDisplay, lConfig,
            mApplication->window, NULL);
        if (mSurface == EGL_NO_SURFACE) goto ERROR;
        mContext = eglCreateContext(mDisplay, lConfig, EGL_NO_CONTEXT,
            NULL);
        if (mContext == EGL_NO_CONTEXT) goto ERROR;

        // Activates the display surface.
        packt_Log_debug("Activating the display.");
        if (!eglMakeCurrent(mDisplay, mSurface, mSurface, mContext)
         || !eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &mWidth)
         || !eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &mHeight)
         || (mWidth <= 0) || (mHeight <= 0)) goto ERROR;
        glViewport(0, 0, mWidth, mHeight);

        // Displays information about OpenGL.
        Log::info("Starting GraphicsService");
        Log::info("Version  : %s", glGetString(GL_VERSION));
        Log::info("Vendor   : %s", glGetString(GL_VENDOR));
        Log::info("Renderer : %s", glGetString(GL_RENDERER));
        Log::info("Viewport : %d x %d", mWidth, mHeight);

        if (loadResources() != STATUS_OK) goto ERROR;
        return STATUS_OK;

    ERROR:
        Log::error("Error while starting GraphicsService");
        stop();
        return STATUS_KO;
    }

    void GraphicsService::stop() {
        Log::info("Stopping GraphicsService.");
        unloadResources();

        // Destroys OpenGL context.
        if (mDisplay != EGL_NO_DISPLAY) {
            eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
                           EGL_NO_CONTEXT);
            if (mContext != EGL_NO_CONTEXT) {
                eglDestroyContext(mDisplay, mContext);
                mContext = EGL_NO_CONTEXT;
            }
            if (mSurface != EGL_NO_SURFACE) {
                eglDestroySurface(mDisplay, mSurface);
                mSurface = EGL_NO_SURFACE;
            }
            eglTerminate(mDisplay);
            mDisplay = EGL_NO_DISPLAY;
        }
    }

    status GraphicsService::update() {
        float lTimeStep = mTimeService->elapsed();

        // Clears background color.
        static float lClearColor = 0.0f;
        lClearColor += lTimeStep * 0.01f;
        glClearColor(lClearColor, lClearColor, lClearColor, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Shows rendering surface.
        if (eglSwapBuffers(mDisplay, mSurface) != EGL_TRUE) {
            Log::error("Error %d swapping buffers.", eglGetError());
            return STATUS_KO;
        }
        return STATUS_OK;
    }

    status GraphicsService::loadResources() {
        // Loads all requested textures.
        for (int32_t i = 0; i < mTextureCount; ++i) {
            if (mTextures[i]->load() != STATUS_OK) {
                return STATUS_KO;
            }
        }
        return STATUS_OK;
    }

    status GraphicsService::unloadResources() {
        for (int32_t i = 0; i < mTextureCount; ++i) {
            mTextures[i]->unload();
        }
        return STATUS_OK;
    }

    GraphicsTexture* GraphicsService::registerTexture(
        const char* pPath) {
        // Finds out if texture already loaded.
        for (int32_t i = 0; i < mTextureCount; ++i) {
            if (strcmp(pPath, mTextures[i]->getPath()) == 0) {
                return mTextures[i];
            }
        }

        // Appends a new texture to the texture array.
        GraphicsTexture* lTexture = new GraphicsTexture(
            mApplication, pPath);
        mTextures[mTextureCount++] = lTexture;
        return lTexture;
    }
}
