#import "OpenGLView.h"

@implementation OpenGLView (Events)

- (void)reshape:(NSSize)size {
    starfield->reshape(size.width, size.height);
}

- (void)keyDown:(NSEvent*)event {
    keymap.left.find([event keyCode]);

    Rt::Option<UI::Key> key;
    std::string characters;
    auto chars = [event charactersIgnoringModifiers];

    NSLog(@"key %u \"%@\" \"%@\"\n", [event keyCode], [event characters], [event charactersIgnoringModifiers]);

    auto iter = keymap.left.find([event keyCode]);
    if (iter != keymap.left.end()) {
        auto keyCode = iter->second;
        switch (keyCode) {
            case UI::Key::esc:
                [NSApp terminate:self];
            break;
            
            default: break;
        }

        key.define(keyCode);
    }

    if ([chars length] != 0)
        characters = [chars UTF8String];

    //std::lock_guard<std::mutex> ihsLock(*ihsMutex.get());
    if (starfield.get())
        starfield->keyDown(key, characters, nullptr);
}

- (void)scroll:(float)x y:(float)y dx:(float)dx dy:(float)dy event:(NSEvent*)event {
    starfield->scroll(x, y, dx, dy, nullptr);
    //[self setNeedsDisplay:YES];
}

- (void)move:(float)x y:(float)y dx:(float)dx dy:(float)dy event:(NSEvent*)event {
    starfield->move(x, y, dx, dy, nullptr);
    //NSLog(@"mouse\n");
    //[self setNeedsDisplay:YES];
}

@end
