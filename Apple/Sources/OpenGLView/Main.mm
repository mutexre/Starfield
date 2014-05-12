#import <math.h>
#import <GLCPP/GL.h>
#import "OpenGLView.h"

@implementation OpenGLView

- (void)prepareScene:(Rt::u8)t {
    starfield->prepareScene(t);
    [self setNeedsDisplay:YES];
}

- (void)drawScene
{
    //std::lock_guard<std::mutex> ihsLock(*ihsMutex.get());
    
#if DRAW_TO_TEXTURE
    glBindFramebuffer(GL_FRAMEBUFFER, drawToTexture->framebuffer->getID());
#endif
    
    starfield->drawScene();

#if DRAW_TO_TEXTURE
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    drawTexture->draw();
    glFlush();
#endif
}

@end
