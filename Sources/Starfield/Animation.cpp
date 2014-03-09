#include <Starfield/Starfield.h>

void Starfield::Animation::evaluate(Rt::u8 t) {
    auto box = boxInput->get().get();
    auto velocity = velocityInput->get().get();
    float z;
    Rt::u8 dt = 0;

    auto frontIter = this->timePoints.begin();
    if (frontIter != this->timePoints.end()) {
        dt = t - *frontIter;
        *frontIter = t;
    }
    else
        this->timePoints.push_front(t);

    z = lastZ.getOrElse(0.0f) + velocity * dt;
    auto n = std::trunc(z / box.length().z);
    z -= n * box.length().z;
    
    lastZ.define(z);

    output[0]->set(z);
    output[0]->notify(this);
}

std::shared_ptr<Starfield::Animation>
Starfield::Animation::create(React::ValuePtr<Rt::u8> timeSource,
                             React::ValuePtr<Rt::Range<Math::Vector<float>>> boxInput,
                             React::ValuePtr<> velocityInput,
                             Rt::Option<React::ValuePtr<>> output)
{
    auto retval = std::make_shared<Animation>();

    retval->setTimeSource(timeSource);
    retval->setVelocityInput(velocityInput);
    retval->setBoxInput(boxInput);

    if (!output.defined) output.define(React::makeScalar<>());
    retval->addOutput(output.get());

    return retval;
}

React::ValuePtr<Rt::Range<Math::Vector<float>>>
Starfield::Animation::getBoxInput() {
    return boxInput;
}

void Starfield::Animation::setBoxInput(React::ValuePtr<Rt::Range<Math::Vector<float>>> box) {
    boxInput = box;
}

React::ValuePtr<>
Starfield::Animation::getVelocityInput() {
    return velocityInput;
}

void Starfield::Animation::setVelocityInput(React::ValuePtr<> velocity) {
    velocityInput = velocity;
}
