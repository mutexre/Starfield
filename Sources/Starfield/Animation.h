#ifndef header_e51caf40669b
#define header_e51caf40669b

namespace Starfield
{
    template <typename T, typename F>
    class Animation : public React::Transform<Animation<T, F>>
    {
        REACT_DEFINE_INPUT(React::ScalarPtr<T>, timeSource, getTimeSource, setTimeSource, &Animation::invalidate)
        REACT_DEFINE_INPUT(React::ScalarPtr<Rt::Range<Math::Vector<F>>>, box, getBox, setBox, &Animation::invalidate)
        REACT_DEFINE_INPUT(React::ScalarPtr<F>, velocity, getVelocity, setVelocity, &Animation::invalidate)
        REACT_DEFINE_OUTPUT(React::ScalarPtr<F>, output, getOutput, setOutput, &Animation::evaluate)

    private:
        Rt::Option<F> lastZ;
        std::deque<T> timePoints;

    protected:
        void evaluate() {
            auto boxLengthZ = box->value().length().z;
            F z, dt;

            auto t = timeSource->value();
            auto iter = this->timePoints.begin();
            if (iter != this->timePoints.end()) {
                dt = F(double(t - *iter) / 100000000.0);
                *iter = t;
            }
            else {
                this->timePoints.push_front(t);
                dt = F(0);
            }

            z = lastZ.getOrElse(0.0f) + velocity->value() * dt;
            auto n = std::trunc(z / boxLengthZ);
            z -= n * boxLengthZ;

            lastZ.define(z);
printf("%f\n", z);
            output->set(z);
            this->commit(output);
        }

        void invalidate() {
            if (output.get())
                output->invalidate();
        }
    };
}

#endif
