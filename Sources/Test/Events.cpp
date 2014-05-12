#include <stdafx.h>
#include <Test/Starfield.h>

void Starfield::Main::updateStarScale(float w, float h) {
    static float testScreenHypotSize = hypot(1680.0f, 1050.0f);
    vars.starfield.scale->set(hypot(w, h) / testScreenHypotSize);
    vars.starfield.scale->commit();
}

void Starfield::Main::reshape(float w, float h)
{
    if (view.get()) {
        view->setViewport(GL::Viewport(0, 0, w, h));
        vars.aspectRatio->set(h / w);
        vars.aspectRatio->commit();
        updateStarScale(w, h);
    }
}

void Starfield::Main::keyDown(Rt::Option<UI::Key> key, const std::string& characters, void* event)
{
    if (key.defined) {
        switch (key.get()) {
            case UI::Key::space:
//                running = !running;
            break;

            case UI::Key::up:
                vars.starfield.velocity->set(vars.starfield.velocity->value() * 1.1f);
                vars.starfield.velocity->commit();
            break;

            case UI::Key::down:
                vars.starfield.velocity->set(vars.starfield.velocity->value() / 1.1f);
                vars.starfield.velocity->commit();
            break;

            case UI::Key::left:
            break;

            case UI::Key::right:
            break;

            case UI::Key::backspace: {
            }
            break;
        }
    }

//    printf("s=\"%s\" %u\n", characters.c_str(), characters.size());
    if (characters.size() > 0)
        switch (characters[0]) {
            case 's':
                vars.starfield.visibility->set(!vars.starfield.visibility->get());
            break;
        }
}

void Starfield::Main::keyUp(UI::Key, void* event) {}

void Starfield::Main::scroll(float x, float y, float dx, float dy, void* event)
{}

void Starfield::Main::move(float x, float y, float dx, float dy, void* event)
{}
