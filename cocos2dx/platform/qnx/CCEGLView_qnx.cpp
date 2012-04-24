/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCEGLView_qnx.h"

#include <ctype.h>
#include <input/screen_helpers.h>
#include <sys/keycodes.h>

#include <bps/bps.h>
#include <bps/event.h>
#include <bps/screen.h>
#include <bps/navigator.h>
#include <bps/orientation.h>
#include <bps/virtualkeyboard.h>

#include "CCSet.h"
#include "CCDirector.h"
#include "CCApplication.h"
#include "ccMacros.h"
#include "CCTouch.h"
#include "CCTouchDispatcher.h"
#include "CCKeypadDispatcher.h"
#include "CCIMEDispatcher.h"


using namespace std;

NS_CC_BEGIN

bool CCEGLView::m_initializedFunctions = false;
const GLubyte *CCEGLView::m_extensions = 0;

//PFNGLGENERATEMIPMAPOESPROC         CCEGLView::glGenerateMipmapOES = 0;
//PFNGLGENFRAMEBUFFERSOESPROC        CCEGLView::glGenFramebuffersOES = 0;
//PFNGLBINDFRAMEBUFFEROESPROC           CCEGLView::glBindFramebufferOES = 0;
//PFNGLFRAMEBUFFERTEXTURE2DOESPROC   CCEGLView::glFramebufferTexture2DOES = 0;
//PFNGLDELETEFRAMEBUFFERSOESPROC     CCEGLView::glDeleteFramebuffersOES = 0;
//PFNGLCHECKFRAMEBUFFERSTATUSOESPROC CCEGLView::glCheckFramebufferStatusOES = 0;

enum TouchType
{
	CC_TOUCH_TYPE_NONE = 0,
	CC_TOUCH_TYPE_BEGIN = 1 << 0,
	CC_TOUCH_TYPE_MOVED = 1 << 1,
	CC_TOUCH_TYPE_ENDED = 1 << 2,
	CC_TOUCH_TYPE_CANCEL = 1 << 3,
};

enum TouchIndex
{
	CC_TOUCH_INDEX_BEGIN = 0,
	CC_TOUCH_INDEX_MOVED,
	CC_TOUCH_INDEX_ENDED,
	CC_TOUCH_INDEX_CANCEL,
	CC_TOUCH_INDEX_MAX
};

enum Orientation
{
    PORTRAIT,
    LANDSCAPE,
    AUTO
};

class CCTouchElement : public CCObject
{
public:
	static CCTouchElement* touchElementWithType(int type, int id, float x, float y)
	{
		CCTouchElement* pRet = new CCTouchElement();
		if (pRet)
		{
			pRet->type = type;
			pRet->id = id;
			pRet->x = x;
			pRet->y = y;
			pRet->autorelease();
		}
		return pRet;
	}
	int type;
	int id;
	float x;
	float y;

};

static Orientation orientation = LANDSCAPE;

static struct {
    EGLint surface_type;
    EGLint red_size;
    EGLint green_size;
    EGLint blue_size;
    EGLint alpha_size;
    EGLint samples;
    EGLint config_id;
} the_configAttr;

static CCEGLView* s_pInstance = NULL;

CCEGLView::CCEGLView()
: m_pEventHandler(NULL)
,  m_isWindowActive(false)
{
    s_pInstance = this;
    m_eglDisplay = EGL_NO_DISPLAY;
    m_eglContext = EGL_NO_CONTEXT;
    m_eglSurface = EGL_NO_SURFACE;
    m_screenEvent = 0;
    m_screenWindow = 0;

    strcpy(m_window_group_id, "");
    snprintf(m_window_group_id, sizeof(m_window_group_id), "%d", getpid());
    bps_initialize();
    navigator_request_events(0);

    navigator_rotation_lock(true);

    the_configAttr.surface_type = EGL_WINDOW_BIT;
    the_configAttr.red_size     = EGL_DONT_CARE;
    the_configAttr.green_size     = EGL_DONT_CARE;
    the_configAttr.blue_size     = EGL_DONT_CARE;
    the_configAttr.alpha_size     = EGL_DONT_CARE;
    the_configAttr.samples         = EGL_DONT_CARE;
    the_configAttr.config_id     = EGL_DONT_CARE;

    m_isGLInitialized = initGL();

    if (m_isGLInitialized)
        initEGLFunctions();
}

CCEGLView::~CCEGLView()
{
    end();
}

void CCEGLView::initEGLFunctions()
{
    m_extensions = glGetString(GL_EXTENSIONS);
    CCLog((char*)m_extensions);
//    glGenerateMipmapOES = 0;
//    glGenFramebuffersOES = 0;
//    glBindFramebufferOES = 0;
//    glFramebufferTexture2DOES = 0;
//    glDeleteFramebuffersOES = 0;
//    glCheckFramebufferStatusOES = 0;
//
//    if (isGLExtension("GL_OES_framebuffer_object"))
//    {
//        glGenerateMipmapOES = (PFNGLGENERATEMIPMAPOESPROC)eglGetProcAddress("glGenerateMipmapOES");
//        glGenFramebuffersOES = (PFNGLGENFRAMEBUFFERSOESPROC)eglGetProcAddress("glGenFramebuffersOES");
//        glBindFramebufferOES = (PFNGLBINDFRAMEBUFFEROESPROC)eglGetProcAddress("glBindFramebufferOES");
//        glFramebufferTexture2DOES = (PFNGLFRAMEBUFFERTEXTURE2DOESPROC)eglGetProcAddress("glFramebufferTexture2DOES");
//        glDeleteFramebuffersOES = (PFNGLDELETEFRAMEBUFFERSOESPROC)eglGetProcAddress("glDeleteFramebuffersOES");
//        glCheckFramebufferStatusOES = (PFNGLCHECKFRAMEBUFFERSTATUSOESPROC)eglGetProcAddress("glCheckFramebufferStatusOES");
//    }

    m_initializedFunctions = true;
}


void CCEGLView::printEGLInfo(const EGLConfig &config) const
{
    int i;

    fprintf(stderr,"EGL_VENDOR = %s\n", eglQueryString(m_eglDisplay, EGL_VENDOR));
    fprintf(stderr,"EGL_VERSION = %s\n", eglQueryString(m_eglDisplay, EGL_VERSION));
    fprintf(stderr,"EGL_CLIENT_APIS = %s\n", eglQueryString(m_eglDisplay, EGL_CLIENT_APIS));
    fprintf(stderr,"EGL_EXTENSIONS = %s\n\n", eglQueryString(m_eglDisplay, EGL_EXTENSIONS));

    i = -1;
    eglGetConfigAttrib(m_eglDisplay, config, EGL_CONFIG_ID, &i);
    fprintf(stderr,"EGL_CONFIG_ID = %d\n", i);

    i = 0;
    eglGetConfigAttrib(m_eglDisplay, config, EGL_RED_SIZE, &i);
    fprintf(stderr,"EGL_RED_SIZE = %d\n", i);

    i = 0;
    eglGetConfigAttrib(m_eglDisplay, config, EGL_GREEN_SIZE, &i);
    fprintf(stderr,"EGL_GREEN_SIZE = %d\n", i);

    i = 0;
    eglGetConfigAttrib(m_eglDisplay, config, EGL_BLUE_SIZE, &i);
    fprintf(stderr,"EGL_BLUE_SIZE = %d\n", i);

    i = 0;
    eglGetConfigAttrib(m_eglDisplay, config, EGL_ALPHA_SIZE, &i);
    fprintf(stderr,"EGL_ALPHA_SIZE = %d\n", i);

    i = 0;
    eglGetConfigAttrib(m_eglDisplay, config, EGL_DEPTH_SIZE, &i);
    fprintf(stderr,"EGL_DEPTH_SIZE = %d\n", i);

    i = 0;
    eglGetConfigAttrib(m_eglDisplay, config, EGL_LEVEL, &i);
    fprintf(stderr,"EGL_LEVEL = %d\n", i);

    i = 0;
    eglGetConfigAttrib(m_eglDisplay, config, EGL_NATIVE_RENDERABLE, &i);
    fprintf(stderr,"EGL_NATIVE_RENDERABLE = %s\n", i ? "EGL_TRUE" : "EGL_FALSE");

    i = 0;
    eglGetConfigAttrib(m_eglDisplay, config, EGL_NATIVE_VISUAL_TYPE, &i);
    fprintf(stderr,"EGL_NATIVE_VISUAL_TYPE = %d\n", i);

    i = 0;
    eglGetConfigAttrib(m_eglDisplay, config, EGL_RENDERABLE_TYPE, &i);
    fprintf(stderr,"EGL_RENDERABLE_TYPE = 0x%04x\n", i);

    i = 0;
    eglGetConfigAttrib(m_eglDisplay, config, EGL_SURFACE_TYPE, &i);
    fprintf(stderr,"EGL_SURFACE_TYPE = 0x%04x\n", i);

    i = 0;
    eglGetConfigAttrib(m_eglDisplay, config, EGL_TRANSPARENT_TYPE, &i);
    if (i == EGL_TRANSPARENT_RGB)
    {
        fprintf(stderr,"EGL_TRANSPARENT_TYPE = EGL_TRANSPARENT_RGB\n");

        i = 0;
        eglGetConfigAttrib(m_eglDisplay, config, EGL_TRANSPARENT_RED_VALUE, &i);
        fprintf(stderr,"EGL_TRANSPARENT_RED = 0x%02x\n", i);

        i = 0;
        eglGetConfigAttrib(m_eglDisplay, config, EGL_TRANSPARENT_GREEN_VALUE, &i);
        fprintf(stderr,"EGL_TRANSPARENT_GREEN = 0x%02x\n", i);

        i = 0;
        eglGetConfigAttrib(m_eglDisplay, config, EGL_TRANSPARENT_BLUE_VALUE, &i);
        fprintf(stderr,"EGL_TRANSPARENT_BLUE = 0x%02x\n\n", i);
    }
    else
    {
        fprintf(stderr,"EGL_TRANSPARENT_TYPE = EGL_NONE\n\n");
    }
}

static void printEglError( GLuint errorCode )
{
    switch( errorCode )
    {
        case EGL_SUCCESS:
            fprintf( stderr, "EGL_SUCCESS");
            break;
        case EGL_NOT_INITIALIZED:
            fprintf( stderr, ">>EGL_NOT_INITIALIZED");
            break;
        case EGL_BAD_ACCESS:
            fprintf( stderr, ">>EGL_BAD_ACCESS");
            break;
        case EGL_BAD_ALLOC:
            fprintf( stderr, ">>EGL_BAD_ALLOC");
            break;
        case EGL_BAD_ATTRIBUTE:
            fprintf( stderr, ">>EGL_BAD_ATTRIBUTE");
            break;
        case EGL_BAD_CONTEXT:
            fprintf( stderr, ">>EGL_BAD_CONTEXT");
            break;
        case EGL_BAD_CONFIG:
            fprintf( stderr, ">>EGL_BAD_CONFIG");
            break;
        case EGL_BAD_CURRENT_SURFACE:
            fprintf( stderr, ">>EGL_BAD_CURRENT_SURFACE");
            break;
        case EGL_BAD_DISPLAY:
            fprintf( stderr, ">>EGL_BAD_DISPLAY");
            break;
        case EGL_BAD_SURFACE:
            fprintf( stderr, ">>EGL_BAD_SURFACE");
            break;
        case EGL_BAD_MATCH:
            fprintf( stderr, ">>EGL_BAD_MATCH");
            break;
        case EGL_BAD_PARAMETER:
            fprintf( stderr, ">>EGL_BAD_PARAMETER");
            break;
        case EGL_BAD_NATIVE_PIXMAP:
            fprintf( stderr, ">>EGL_BAD_NATIVE_PIXMAP");
            break;
        case EGL_BAD_NATIVE_WINDOW:
            fprintf( stderr, ">>EGL_BAD_NATIVE_WIN i=i+2DOW");
            break;
        case EGL_CONTEXT_LOST:
            fprintf( stderr, ">>EGL_CONTEXT_LOST");
            break;
        default:
            fprintf( stderr, ">>Unknown error");
            break;
    }
}

bool CCEGLView::initDriver()
{
    int         err;
    int         egl_ret;
    EGLint      majorVersion;
    EGLint      minorVersion;

    // Get EGL display
    m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    err = eglGetError();
    if (m_eglDisplay == EGL_NO_DISPLAY || err != EGL_SUCCESS)
    {
        printEglError( err );
        return false;
    }

    // Initialize EGL
    egl_ret = eglInitialize(m_eglDisplay, &majorVersion, &minorVersion);
    err = eglGetError();
    if ( egl_ret != EGL_TRUE || err != EGL_SUCCESS )
    {
        printEglError( err );
        return false;
    }

    egl_ret = eglBindAPI(EGL_OPENGL_ES_API);

    return true;
}

bool CCEGLView::createNativeWindow(const EGLConfig &config)
{
    int     usage = SCREEN_USAGE_OPENGL_ES2;

    int     transp = SCREEN_TRANSPARENCY_NONE;
    int     pos[2] = { 0, 0 };
    int     nbuffers = 2;
    EGLint     interval = 1;
    int     format = SCREEN_FORMAT_RGBX8888;
    EGLint  err;

    err = screen_create_context(&m_screenContext, 0);
    if (err)
    {
        fprintf(stderr, "screen_create_context");
        return false;
    }

    err = screen_create_window(&m_screenWindow, m_screenContext);
    if (err)
    {
        fprintf(stderr, "screen_create_window");
        return false;
    }

err = screen_create_window_group(m_screenWindow, m_window_group_id);
    if (err)
    {
        fprintf(stderr, "screen_create_window_group");
        return false;
    }

    err = screen_set_window_property_iv(m_screenWindow, SCREEN_PROPERTY_FORMAT, &format);
    if (err)
    {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_FORMAT)");
        return false;
    }

    err = screen_set_window_property_iv(m_screenWindow, SCREEN_PROPERTY_USAGE, &usage);
    if (err)
    {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_USAGE)");
        return false;
    }

    if (pos[0] != 0 || pos[1] != 0)
    {
        err = screen_set_window_property_iv(m_screenWindow, SCREEN_PROPERTY_POSITION, pos);
        if (err)
        {
            fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_POSITION)");
            return false;
        }
    }

    err = screen_set_window_property_iv(m_screenWindow, SCREEN_PROPERTY_TRANSPARENCY, &transp);
    if (err)
    {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_TRANSPARENCY)");
        return false;
    }

    err = screen_set_window_property_iv(m_screenWindow, SCREEN_PROPERTY_SWAP_INTERVAL, &interval);
    if (err)
    {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_SWAP_INTERVAL)");
        return false;
    }

    screen_display_t screen_disp;
    int rc = screen_get_window_property_pv(m_screenWindow, SCREEN_PROPERTY_DISPLAY, (void **)&screen_disp);
    if (rc)
    {
        fprintf(stderr, "screen_get_window_property_pv(SCREEN_PROPERTY_DISPLAY)");
        return false;
    }

    screen_display_mode_t screen_mode;
    rc = screen_get_display_property_pv(screen_disp, SCREEN_PROPERTY_MODE, (void**)&screen_mode);
    if (rc)
    {
        fprintf(stderr, "screen_get_display_property_pv(SCREEN_PROPERTY_MODE)");
        return false;
    }

    int size[2];
    rc = screen_get_window_property_iv(m_screenWindow, SCREEN_PROPERTY_BUFFER_SIZE, size);
    if (rc)
    {
        fprintf(stderr, "screen_get_window_property_iv(SCREEN_PROPERTY_BUFFER_SIZE)");
        return false;
    }

    int angle = atoi(getenv("ORIENTATION"));
    int buffer_size[2] = { size[0], size[1] };

    if ((angle == 0) || (angle == 180))
    {
        if (((screen_mode.width > screen_mode.height) && (size[0] < size[1])) ||
            ((screen_mode.width < screen_mode.height) && (size[0] > size[1])))
        {
            buffer_size[1] = size[0];
            buffer_size[0] = size[1];
        }
    }
    else if ((angle == 90) || (angle == 270))
    {
        if (((screen_mode.width > screen_mode.height) && (size[0] > size[1])) ||
            ((screen_mode.width < screen_mode.height && size[0] < size[1])))
        {
            buffer_size[1] = size[0];
            buffer_size[0] = size[1];
        }
    }
    else
    {
        fprintf(stderr, "Navigator returned an unexpected orientation angle of %d.\n", angle);
        return false;
    }

    rc = screen_set_window_property_iv(m_screenWindow, SCREEN_PROPERTY_BUFFER_SIZE, buffer_size);
    if (rc)
    {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_BUFFER_SIZE)");
        return false;
    }

    rc = screen_set_window_property_iv(m_screenWindow, SCREEN_PROPERTY_ROTATION, &angle);
    if (rc)
    {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_ROTATION)");
        return false;
    }

    err = screen_create_window_buffers(m_screenWindow, nbuffers);
    if (err)
    {
        fprintf(stderr, "screen_create_window_buffers");
        return false;
    }

    return true;
}

#define N_BPS_EVENTS 1

bool CCEGLView::initGL()
{
    EGLConfig             config;
    EGLint                err;
    int usage = 0;
    int num_configs = 0;
    EGLint attrib_list[]= { EGL_RED_SIZE,        8,
                             EGL_GREEN_SIZE,      8,
                             EGL_BLUE_SIZE,       8,
                             EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
                             EGL_RENDERABLE_TYPE, 0,
                             EGL_NONE};

    usage = SCREEN_USAGE_OPENGL_ES2 | SCREEN_USAGE_ROTATION;
    attrib_list[9] = EGL_OPENGL_ES2_BIT;
    EGLint attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    CCLog("CCEGLView 1");
    initDriver();
    CCLog("CCEGLView 2");
    eglChooseConfig(m_eglDisplay, attrib_list, &config, 1, &num_configs);
    CCLog("CCEGLView 3");
    //config = chooseConfig(m_eglDisplay, "rgb565");



    if (config == (EGLConfig)0)
    {
        fprintf(stderr, "Failed to find config!" );
        return false;
    }

    // Create EGL rendering context
    m_eglContext = eglCreateContext( m_eglDisplay, config, EGL_NO_CONTEXT, attributes );
    err = eglGetError( );
    if ( m_eglContext == EGL_NO_CONTEXT )
    {
        fprintf(stderr, "Can't create gles2 context!" );
        printEglError( err );
        return false;
    }
    CCLog("CCEGLView 4");
//    printEGLInfo(config);

    if (!createNativeWindow(config))
    {
        fprintf(stderr, "Unable to create a native window\n");
        return false;
    }CCLog("CCEGLView 5");

    // set up the screen events
    err = screen_create_event(&m_screenEvent);
    if (err)
    {
        fprintf(stderr, "screen_create_event");
        return false;
    }
    CCLog("CCEGLView 6");
#ifdef BPS_EVENTS
    // Request screen events
    screen_request_events(m_screenContext);
#endif

    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, m_screenWindow, NULL);
    if (m_eglSurface == EGL_NO_SURFACE)
    {
        fprintf(stderr, "Demo Thread Init: can't create surface!" );
        printEglError( err );
        return false;
    }

    err = eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
    if (err != EGL_TRUE)
    {
        fprintf(stderr, "Demo Thread Init: can't make current!" );
        return false;
    }

    EGLint width, height;
    CCLog("CCEGLView 7");
    if ((m_eglDisplay == EGL_NO_DISPLAY) || (m_eglSurface == EGL_NO_SURFACE) )
        return EXIT_FAILURE;

    eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_WIDTH, &width);
    eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_HEIGHT, &height);

    setFrameSize(width, height);
    CCLog("CCEGLView 8");
    return true;
}

bool CCEGLView::isOpenGLReady()
{
    return (m_isGLInitialized && m_sSizeInPixel.width != 0 && m_sSizeInPixel.height != 0);
}

void CCEGLView::end()
{
    if (!m_eglContext || !m_eglDisplay)
        return;
#ifdef BPS_EVENTS
    screen_stop_events(m_screenContext);
#endif
    bps_shutdown();

    screen_destroy_event(m_screenEvent);

    eglMakeCurrent(m_eglDisplay, NULL, NULL, NULL);
    eglDestroySurface(m_eglDisplay, m_eglSurface);
    screen_destroy_window(m_screenWindow);
    screen_destroy_context(m_screenContext);
    eglDestroyContext(m_eglDisplay, m_eglContext);
    eglTerminate(m_eglDisplay);
    eglReleaseThread();

    m_eglDisplay = NULL;
    m_eglContext = NULL;
    m_eglSurface = NULL;

    m_isGLInitialized = false;

    exit(0);
}

void CCEGLView::setEventHandler(EventHandler* pHandler)
{
    m_pEventHandler = pHandler;
}

const char* CCEGLView::getWindowGroupId() const
{
    return m_window_group_id;
}

//#define InvalidArray(__array__, __row__, __col__, __type__) \
//	for (int i = 0; i < __row__; ++i) \
//		for (int j = 0; j < __col__; ++j) \
//			__array__[i][j] = (__type__)-1;


bool CCEGLView::HandleEvents()
{
    bps_event_t*    event = NULL;
    mtouch_event_t  mtouch_event;
    int                touch_id = 0;
    int                val = 0;
    int                rc = 0;
    int             domain = 0;
    char             buf[4] = {0};

//    int nTouchType = CC_TOUCH_TYPE_NONE;
//    int nTouchBeginNum = 0;
//    int nTouchMovedNum = 0;
//    int nTouchEndedNum = 0;

    CCDirector* pDirector = CCDirector::sharedDirector();

//    int ids[CC_TOUCH_INDEX_MAX][CC_MAX_TOUCHES];
//    InvalidArray(ids, CC_TOUCH_INDEX_MAX, CC_MAX_TOUCHES, int);
//
//    float xs[CC_TOUCH_INDEX_MAX][CC_MAX_TOUCHES];
//    InvalidArray(xs, CC_TOUCH_INDEX_MAX, CC_MAX_TOUCHES, float);
//
//    float ys[CC_TOUCH_INDEX_MAX][CC_MAX_TOUCHES];
//    InvalidArray(ys, CC_TOUCH_INDEX_MAX, CC_MAX_TOUCHES, float);

    CCArray* pTouchArr = CCArray::arrayWithCapacity(20);

    for (;;)
    {
        rc = bps_get_event(&event, 1);
        assert(rc == BPS_SUCCESS);

#ifdef BPS_EVENTS
        // break if no more events
        if (event == NULL)
            break;
#else
        if (event != NULL)
        {
#endif
        if (m_pEventHandler && m_pEventHandler->HandleBPSEvent(event))
            continue;

        domain = bps_event_get_domain(event);

        if (domain == navigator_get_domain())
        {
            switch (bps_event_get_code(event))
            {
                case NAVIGATOR_SWIPE_DOWN:
                	pDirector->getKeypadDispatcher()->dispatchKeypadMSG(kTypeMenuClicked);
                    break;

                case NAVIGATOR_EXIT:
                    // exit the application
                //    release();
                    break;

                case NAVIGATOR_WINDOW_INACTIVE:
                    if(m_isWindowActive)
                    {
                        CCApplication::sharedApplication().applicationDidEnterBackground();
                        m_isWindowActive = false;
                    }
                    break;

                case NAVIGATOR_WINDOW_ACTIVE:
                    if(!m_isWindowActive)
                    {
                        CCApplication::sharedApplication().applicationWillEnterForeground();
                        m_isWindowActive = true;
                    }
                    break;

                case NAVIGATOR_WINDOW_STATE:
                {
                    switch(navigator_event_get_window_state(event))
                    {
                        case NAVIGATOR_WINDOW_FULLSCREEN:
                            if(!m_isWindowActive)
                            {
                                CCApplication::sharedApplication().applicationWillEnterForeground();
                                m_isWindowActive = true;
                            }
                            break;
                        case NAVIGATOR_WINDOW_THUMBNAIL:
                            if(m_isWindowActive)
                            {
                                CCApplication::sharedApplication().applicationDidEnterBackground();
                                m_isWindowActive = false;
                            }
                            break;
                    }
                    break;
                }

                default:
                    break;
            }
        }
        }
#ifndef BPS_EVENTS
        // for now handle screen events separately from BPS events
        if (screen_get_event(m_screenContext, m_screenEvent, 0) < 0)
        {
            // we have an error condition in the screen event
            break;
        }
        else
        {
#else
        else if (domain == screen_get_domain())
        {
            m_screenEvent = screen_event_get_event(event);
#endif
            rc = screen_get_event_property_iv(m_screenEvent, SCREEN_PROPERTY_TYPE, &val);
            if (rc || val == SCREEN_EVENT_NONE)
                break;

            switch (val)
            {
                case SCREEN_EVENT_CLOSE:
                    fprintf(stderr, "SCREEN CLOSE EVENT!\n");
                    break;

                case SCREEN_EVENT_MTOUCH_RELEASE:
                {
                    screen_get_mtouch_event(m_screenEvent, &mtouch_event, 0);
                    touch_id = mtouch_event.contact_id;


                    pTouchArr->addObject(CCTouchElement::touchElementWithType(CC_TOUCH_TYPE_ENDED, touch_id, (float)(mtouch_event.x), (float)(mtouch_event.y)));

                }
                    break;

                case SCREEN_EVENT_MTOUCH_TOUCH:
                {
                    screen_get_mtouch_event(m_screenEvent, &mtouch_event, 0);
                    touch_id = mtouch_event.contact_id;

                    pTouchArr->addObject(CCTouchElement::touchElementWithType(CC_TOUCH_TYPE_BEGIN, touch_id, (float)(mtouch_event.x), (float)(mtouch_event.y)));

                }

                    break;

                case SCREEN_EVENT_MTOUCH_MOVE:
                {
                    screen_get_mtouch_event(m_screenEvent, &mtouch_event, 0);
                    touch_id = mtouch_event.contact_id;

                    pTouchArr->addObject(CCTouchElement::touchElementWithType(CC_TOUCH_TYPE_MOVED, touch_id, (float)(mtouch_event.x), (float)(mtouch_event.y)));

                }

                    break;

                case SCREEN_EVENT_POINTER:
                    {
                        int buttons;
                        int pair[2];
                        static bool mouse_pressed = false;

                        // this is a mouse move event, it is applicable to a device with a usb mouse or simulator
                        screen_get_event_property_iv(m_screenEvent, SCREEN_PROPERTY_BUTTONS, &buttons);
                        screen_get_event_property_iv(m_screenEvent, SCREEN_PROPERTY_SOURCE_POSITION, pair);

                        if (buttons & SCREEN_LEFT_MOUSE_BUTTON)
                        {
                            if (mouse_pressed)
                            {
                                // Left mouse button was released
                            	int id = 0;
                            	handleTouchesMove(1, &id, (float*)&pair[0], (float*)&pair[1]);
                            }
                            else
                            {
                                // Left mouse button is pressed
                                mouse_pressed = true;
                            	int id = 0;
                            	handleTouchesBegin(1, &id, (float*)&pair[0], (float*)&pair[1]);
                            }
                        }
                        else
                        {
                            if (mouse_pressed)
                            {
                            	int id = 0;
                            	handleTouchesEnd(1, &id, (float*)&pair[0], (float*)&pair[1]);
                            }
                        }
                    }
                    break;

                case SCREEN_EVENT_KEYBOARD:
                    screen_get_event_property_iv(m_screenEvent, SCREEN_PROPERTY_KEY_FLAGS, &val);
                    if (val & KEY_DOWN)
                    {
                        screen_get_event_property_iv(m_screenEvent, SCREEN_PROPERTY_KEY_SYM, &val);

                        if (val >= ' ' && val < '~')
                        {
                            buf[0] = val;
                            buf[1]=  '\0';
                            CCIMEDispatcher::sharedDispatcher()->dispatchInsertText(buf, 1);
                        }
                        else
                        {
                            val = val - 0xf000;
                            buf[0] = val;
                            buf[1]=  '\0';

                            switch (val)
                            {
                                case 8: // backspace
                                    //        CCKeypadDispatcher::sharedDispatcher()->dispatchKeypadMSG(kTypeBackClicked);
                                    CCIMEDispatcher::sharedDispatcher()->dispatchDeleteBackward();
                                    break;

                                default:
                                    CCIMEDispatcher::sharedDispatcher()->dispatchInsertText(buf, 1);
                                    break;
                            }
                        }
                    }

                    break;

                default:
                    break;
            }
        }
    }

    CCArray* pCollectedMoveArr = CCArray::arrayWithCapacity(10);
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(pTouchArr, pObj)
    {
    	CCTouchElement* pElement = (CCTouchElement*)pObj;
    	if (pElement->type == CC_TOUCH_TYPE_BEGIN)
    	{
    		// process the collected move events.
    		if (pCollectedMoveArr->count() > 0)
    		{
    			handleCollectedMoveEvent(pCollectedMoveArr);
    		}
    		CCLOG("handle begin: x = %f, y = %f, id = %d", pElement->x, pElement->y, pElement->id);
    		handleTouchesBegin(1, &pElement->id, &pElement->x, &pElement->y);
    	}
    	else if (pElement->type == CC_TOUCH_TYPE_ENDED)
    	{
    		if (pCollectedMoveArr->count() > 0)
    		{
    			handleCollectedMoveEvent(pCollectedMoveArr);
    		}
    		CCLOG("handle end: x = %f, y = %f, id = %d", pElement->x, pElement->y, pElement->id);
    		handleTouchesEnd(1, &pElement->id, &pElement->x, &pElement->y);
    	}
    	else if (pElement->type == CC_TOUCH_TYPE_MOVED)
    	{
    		// collect move events.
    		pCollectedMoveArr->addObject(pElement);
    	}
    }

	if (pCollectedMoveArr->count() > 0)
	{
		handleCollectedMoveEvent(pCollectedMoveArr);
	}

    return true;
}

#define InvalidSingleDimArray(__array__, __num__, __type__) \
	for (int i = 0; i < __num__; ++i) \
			__array__[i] = (__type__)-1;

void CCEGLView::handleCollectedMoveEvent(CCArray* pCollectedArr)
{
	int ids[CC_MAX_TOUCHES];
	InvalidSingleDimArray(ids, CC_MAX_TOUCHES, int);
	float xs[CC_MAX_TOUCHES];
	InvalidSingleDimArray(xs, CC_MAX_TOUCHES, float);
	float ys[CC_MAX_TOUCHES];
	InvalidSingleDimArray(ys, CC_MAX_TOUCHES, float);

	unsigned int uTouchMovedNum = 0;
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(pCollectedArr, pObj)
	{
		CCTouchElement* pElement = (CCTouchElement*)pObj;
		for (int i = 0; i < CC_MAX_TOUCHES; i++)
		{
			if (ids[i] < 0)
			{
				ids[i] = pElement->id;
				xs[i] = pElement->x;
				ys[i] = pElement->y;
				uTouchMovedNum++;
				CCLOG("move: x=%f, y=%f, id=%d", pElement->x, pElement->y, pElement->id);
				break;
			}
			else if (ids[i] == pElement->id)
			{
				xs[i] = pElement->x;
				ys[i] = pElement->y;
				CCLOG("reset-->move: x=%f, y=%f, id=%d", pElement->x, pElement->y, pElement->id);
				break;
			}
		}
	}

	handleTouchesMove(uTouchMovedNum, ids, xs, ys);
	pCollectedArr->removeAllObjects();
}

void CCEGLView::swapBuffers()
{
    eglSwapBuffers(m_eglDisplay, m_eglSurface);
}

CCEGLView& CCEGLView::sharedOpenGLView()
{
    CCAssert(s_pInstance != NULL, "CCEGLView wasn't constructed yet");
    return *s_pInstance;
}

bool CCEGLView::isGLExtension(const char *searchName) const
{
    const GLubyte *start;
    GLubyte *where, *terminator;

    /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
    start = m_extensions;
    for (;;)
    {
        where = (GLubyte *) strstr((const char *) start, searchName);
        if (!where)
            break;

        terminator = where + strlen(searchName);
        if (where == start || *(where - 1) == ' ')
            if (*terminator == ' ' || *terminator == '\0')
                return true;

        start = terminator;
    }

    return false;
}

void CCEGLView::showKeyboard()
{
    int height;

    virtualkeyboard_get_height(&height);

    float factor = m_fScreenScaleFactor / CC_CONTENT_SCALE_FACTOR();
    height = (float)height / factor;

    CCRect rect_begin(0, 0 - height, m_sSizeInPixel.width / factor, height);
    CCRect rect_end(0, 0, m_sSizeInPixel.width / factor, height);

    CCIMEKeyboardNotificationInfo info;
    info.begin = rect_begin;
    info.end = rect_end;
    info.duration = 0;

    CCIMEDispatcher::sharedDispatcher()->dispatchKeyboardWillShow(info);
    virtualkeyboard_show();
    CCIMEDispatcher::sharedDispatcher()->dispatchKeyboardDidShow(info);
}

void CCEGLView::hideKeyboard()
{
    virtualkeyboard_hide();
}

void CCEGLView::setIMEKeyboardState(bool bOpen)
{
    if (bOpen)
        showKeyboard();
    else
        hideKeyboard();
}

NS_CC_END
