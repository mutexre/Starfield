#import <CoreVideo/CVDisplayLink.h>
#import <GLCPP/ObjC/GLView.h>
#import <Test/Starfield.h>

typedef boost::bimap<Rt::u4, UI::Key> Keymap;

@interface OpenGLView : GLView
{
@private
    std::unique_ptr<Starfield::Main> starfield;
    Keymap keymap;
}

- (void)prepareScene;
- (void)drawScene;

@end

@interface OpenGLView(Init)

- (void)setGLOptions;
- (void)onGlReady;

@end

@interface OpenGLView(Events)

- (void)reshape:(NSSize)size;

- (void)keyDown:(NSEvent*)event;
- (void)keyUp:(NSEvent*)event;

- (void)scroll:(float)x y:(float)y dx:(float)dx dy:(float)dy event:(NSEvent*)event;
- (void)move:(float)x y:(float)y dx:(float)dx dy:(float)dy event:(NSEvent*)event;

@end
