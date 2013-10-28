#include "GraphicsService.hpp"
#include "Log.hpp"
#include "Resource.hpp"

#include <GLES/gl.h>
#include <GLES/glext.h>


namespace packt {
    GraphicsService::GraphicsService(android_app* pApplication,
                                     TimeService* pTimeService) :
        mApplication(pApplication),
        mWidth(0), mHeight(0),
        mDisplay(EGL_NO_DISPLAY), mSurface(EGL_NO_CONTEXT),
        mContext(EGL_NO_SURFACE),
        mDevice(NULL), mObjects() {
        Log::info("Creating GraphicsService.");
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
            EGL_DEPTH_SIZE, 16, EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
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

        setup();
        return STATUS_OK;

    ERROR:
        Log::error("Error while starting GraphicsService");
        stop();
        return STATUS_KO;
    }

    void GraphicsService::stop() {
        Log::info("Stopping GraphicsService.");
        mDevice->drop();

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

    void GraphicsService::setup() {
        // Initialize Irrlicht.
        mDevice = irr::createDevice(irr::video::EDT_OGLES1,
            irr::core::dimension2d<irr::u32>(mWidth, mHeight), 32,
            false, false, false, 0);
        mDevice->getFileSystem()->addFolderFileArchive(
            "/sdcard/droidblaster/");
        mDriver = mDevice->getVideoDriver();
        mSceneManager = mDevice->getSceneManager();

        // Scene set-up.
        // Adds a light for dynamic lighting.
        mSceneManager->setAmbientLight(
            irr::video::SColorf(0.85f,0.85f,0.85f));
        mSceneManager->addLightSceneNode(NULL,
            irr::core::vector3df(-150, 200, -50),
            irr::video::SColorf(1.0f, 1.0f, 1.0f), 4000.0f);

        // Camera set-up.
        irr::scene::ICameraSceneNode* lCamera =
            mSceneManager->addCameraSceneNode();
        lCamera->setTarget(
            irr::core::vector3df(mWidth/2, 0.0f, mHeight/2));
        lCamera->setUpVector(
            irr::core::vector3df(0.0f, 0.0f, 1.0f));
        lCamera->setPosition(
            irr::core::vector3df(mWidth/2, mHeight*3/4, mHeight/2));

        // Creates a particle system to simulate stars.
        irr::scene::IParticleSystemSceneNode* lParticleSystem =
                mSceneManager->addParticleSystemSceneNode(false);
        irr::scene::IParticleEmitter* lEmitter =
            lParticleSystem->createBoxEmitter(
            // X, Y, Z of first and second corner.
            irr::core::aabbox3d<irr::f32>(
                -mWidth * 0.1f, -300, mHeight * 1.2f,
                mWidth * 1.1f, -100, mHeight * 1.1f),
            // Direction and emit rate.
            irr::core::vector3df(0.0f,0.0f,-0.25f), 10.0f, 40.0f,
            // darkest and brightest color
            irr::video::SColor(0,255,255,255),
            irr::video::SColor(0,255,255,255),
            // min and max age, angle
            8000.0f, 8000.0f, 0.0f,
            // min and max size.
            irr::core::dimension2df(1.f,1.f),
            irr::core::dimension2df(8.f,8.f));
        lParticleSystem->setEmitter(lEmitter);
        lEmitter->drop();
        // Particle system properties.
        lParticleSystem->setMaterialTexture(0,
            mDriver->getTexture("star.png"));
        lParticleSystem->setMaterialType(
            irr::video::EMT_TRANSPARENT_VERTEX_ALPHA);
        lParticleSystem->setMaterialFlag(
            irr::video::EMF_LIGHTING, false);
        lParticleSystem->setMaterialFlag(
            irr::video::EMF_ZWRITE_ENABLE, false);

        // Initializes resources.
        GraphicsObject::vec_it iObject = mObjects.begin();
        for (; iObject < mObjects.end() ; ++iObject) {
            (*iObject)->initialize(mSceneManager);
        }
    }

    status GraphicsService::update() {
        // Update graphics coordinates from world coordinates.
        GraphicsObject::vec_it iObject = mObjects.begin();
        for (; iObject < mObjects.end() ; ++iObject) {
            (*iObject)->update();
        }

        // Updates 3D scene.
        if (!mDevice->run()) return STATUS_KO;
        mDriver->beginScene(true, true, irr::video::SColor(0,0,0,0));
        mSceneManager->drawAll();
        mDriver->endScene();

        // Shows rendering surface.
        if (eglSwapBuffers(mDisplay, mSurface) != EGL_TRUE) {
            Log::error("Error %d swapping buffers.", eglGetError());
            return STATUS_KO;
        }
        return STATUS_OK;
    }

    GraphicsObject::ptr GraphicsService::registerObject(
      const char* pTexture, const char* pMesh, Location* pLocation) {
        GraphicsObject::ptr lObject(new GraphicsObject(pTexture,
            pMesh, pLocation));
        mObjects.push_back(lObject);
        return mObjects.back();
    }
}
