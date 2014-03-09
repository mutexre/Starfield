#ifndef header_e51caf40669b
#define header_e51caf40669b

namespace Starfield
{
    class Animation : public React::Animation<float, Animation>
    {
    private:
        React::ValuePtr<Rt::Range<Math::Vector<float>>> boxInput;
        React::ValuePtr<> velocityInput;
        Rt::Option<float> lastZ;

    protected:
        virtual void evaluate(Rt::u8 t);

    public:
        static std::shared_ptr<Animation> create(React::ValuePtr<Rt::u8> timeSource,
                                                 React::ValuePtr<Rt::Range<Math::Vector<float>>> boxInput,
                                                 React::ValuePtr<> velocityInput,
                                                 Rt::Option<React::ValuePtr<>> output = Rt::Option<React::ValuePtr<>>());

        React::ValuePtr<Rt::Range<Math::Vector<float>>> getBoxInput();
        void setBoxInput(React::ValuePtr<Rt::Range<Math::Vector<float>>> box);

        React::ValuePtr<> getVelocityInput();
        void setVelocityInput(React::ValuePtr<> velocity);
    };
}

#endif
