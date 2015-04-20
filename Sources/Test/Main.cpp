#include <stdafx.h>
#include <Test/Starfield.h>

void Starfield::Main::setTime(Rt::u8 t) {
    vars.time->set(t);
    vars.time->commit();
}

void Starfield::Main::drawScene()
{
    if (view.get())
    {
#if 0
        GLuint ID, result;
        auto target = GL_SAMPLES_PASSED;
        glGenQueries(1, &ID);
        glBeginQuery(target, ID);
#endif

/*        static GLenum drawBuffer = GL_FRONT;
        glDrawBuffer(drawBuffer);
        drawBuffer = (drawBuffer == GL_BACK ? GL_FRONT : GL_BACK);*/

        view->render();

#if 0
        glEndQuery(target);
        glGetQueryObjectuiv(ID, GL_QUERY_RESULT, &result);
        //glDeleteQueries(ID);
        printf("%llu\n", result);
#endif
    }
}
