#ifndef RENDERER_H
#define RENDERER_H

#include <pthread.h>
#include <EGL/egl.h>
#include <GLES/gl.h>

class Renderer {

public:
    Renderer();
    virtual ~Renderer();


    void start();
    void stop();
    void setWindow(ANativeWindow* window);

private:
    enum RendererThreadMessage {
        MSG_NONE = 0,
        MSG_WINDOW_SET,
        MSG_RENDERER_LOOP_EXIT
    };

    pthread_t _threadId;
    pthread_mutex_t _mutex;
    enum RendererThreadMessage _msg;

    ANativeWindow* _window;

    EGLDisplay _display;
    EGLSurface _surface;
    EGLContext _context;
    GLfloat    _angle;

    void rendererLoop();

    bool initialize();
    void destroy();

    void drawFrame();

    static void* threadStartCallback(void *myself);
};

#endif