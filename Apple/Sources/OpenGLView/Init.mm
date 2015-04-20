#import "OpenGLView.h"
#import <GLCPP/ObjC/Subroutines.h>
#import <string.h>

@implementation OpenGLView (Init)

- (Keymap)createKeymap {
    Keymap map;

    map.left.insert(std::pair<Rt::u4, UI::Key>(49, UI::Key::space));
    map.left.insert(std::pair<Rt::u4, UI::Key>(53, UI::Key::esc));
    map.left.insert(std::pair<Rt::u4, UI::Key>(36, UI::Key::enter));
    map.left.insert(std::pair<Rt::u4, UI::Key>(51, UI::Key::backspace));
    map.left.insert(std::pair<Rt::u4, UI::Key>(126, UI::Key::up));
    map.left.insert(std::pair<Rt::u4, UI::Key>(125, UI::Key::down));
    map.left.insert(std::pair<Rt::u4, UI::Key>(123, UI::Key::left));
    map.left.insert(std::pair<Rt::u4, UI::Key>(124, UI::Key::right));
    map.left.insert(std::pair<Rt::u4, UI::Key>(121, UI::Key::pageUp));
    map.left.insert(std::pair<Rt::u4, UI::Key>(116, UI::Key::pageDown));
    map.left.insert(std::pair<Rt::u4, UI::Key>(115, UI::Key::home));
    map.left.insert(std::pair<Rt::u4, UI::Key>(119, UI::Key::end));

    map.left.insert(std::pair<Rt::u4, UI::Key>(122, UI::Key::f1));
    map.left.insert(std::pair<Rt::u4, UI::Key>(120, UI::Key::f2));
    map.left.insert(std::pair<Rt::u4, UI::Key>(114, UI::Key::f3));
    map.left.insert(std::pair<Rt::u4, UI::Key>(115, UI::Key::f4));
    map.left.insert(std::pair<Rt::u4, UI::Key>(116, UI::Key::f5));
    map.left.insert(std::pair<Rt::u4, UI::Key>(117, UI::Key::f6));
    map.left.insert(std::pair<Rt::u4, UI::Key>(118, UI::Key::f7));
    map.left.insert(std::pair<Rt::u4, UI::Key>(119, UI::Key::f8));
    map.left.insert(std::pair<Rt::u4, UI::Key>(120, UI::Key::f9));
    map.left.insert(std::pair<Rt::u4, UI::Key>(121, UI::Key::f10));
    map.left.insert(std::pair<Rt::u4, UI::Key>(122, UI::Key::f11));
    map.left.insert(std::pair<Rt::u4, UI::Key>(123, UI::Key::f12));

    return map;
}

- (void)configure {
    keymap = [self createKeymap];
    starfield = std::unique_ptr<Starfield::Main>(new Starfield::Main([[[NSBundle mainBundle] resourcePath] UTF8String]));
}

@end
