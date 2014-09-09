#ifndef header_F2401FC7AD32
#define header_F2401FC7AD32

namespace Starfield
{
    class Cpu2GpuProjection : public React::Transform<Cpu2GpuProjection>
    {
        REACT_DEFINE_INPUT(React::ScalarPtr<std::vector<Starfield::Star>>, stars, getStars, setStars, &Cpu2GpuProjection::invalidate)
        REACT_DEFINE_INPUT(React::ScalarPtr<std::shared_ptr<SG::Program>>, program, getProgram, setProgram, &Cpu2GpuProjection::invalidate)
        REACT_DEFINE_INPUT(React::ScalarPtr<std::shared_ptr<SG::Material>>, material, getMaterial, setMaterial, &Cpu2GpuProjection::invalidate)
        REACT_DEFINE_OUTPUT(React::ScalarPtr<std::shared_ptr<SG::Object>>, object, getObject, setObject, &Cpu2GpuProjection::evaluate)

    private:
        React::ScalarPtr<std::shared_ptr<SG::Data>> data;
        React::ScalarPtr<std::set<std::string>> disabledArrays;
        React::ScalarPtr<std::shared_ptr<SG::DataBinding>> dataBinding;
        React::ScalarPtr<std::shared_ptr<SG::DrawCall>> drawCall;
        std::shared_ptr<SG::Segment> segment;
        std::vector<std::shared_ptr<SG::Segment>> segments;
        std::shared_ptr<SG::BindData> binder;

    protected:
        void evaluate() {
            segment->setProgram(program);
            segment->setMaterial(material);
            binder->setProgram(program);

            object->currentValue()->setSegments(segments);

            program->value()->bind();
            auto location = program->value()->get()->getAttributeLocation("color");
            if (location.defined) glVertexAttrib4f(location.get(), 1.0f, 1.0f, 1.0f, 0.99f);

            auto buffer = std::make_shared<GL::Buffer>();
            auto target = GL::Buffer::Target::Array;

            auto starsData = stars->value().data();
            auto numberOfStars = stars->value().size();

            buffer->bind(target);
            GL::Buffer::data(target,
                             numberOfStars * sizeof(Starfield::Star),
                             nullptr, GL_STATIC_DRAW);

            auto ptr = GL::Buffer::map(target, GL::Buffer::Access::Write);
            if (!ptr) Rt::error(0xC19932E1);

            memcpy(ptr, (Rt::u1*)starsData, numberOfStars * sizeof(Starfield::Star));
            GL::Buffer::unmap(target);

            auto& dataValue = data->value();
            dataValue->setAttr("size", buffer, GL::VertexArray::AttribPointer(1, GL_FLOAT, GL_FALSE,
                                                                                  sizeof(Starfield::Star),
                                                                                  offsetof(Starfield::Star, size)));

            dataValue->setAttr("coord", buffer, GL::VertexArray::AttribPointer(3, GL_FLOAT, GL_FALSE,
                                                                                   sizeof(Starfield::Star),
                                                                                   offsetof(Starfield::Star, position)));

            dataValue->setAttr("color", buffer, GL::VertexArray::AttribPointer(3, GL_FLOAT, GL_FALSE,
                                                                                   sizeof(Starfield::Star),
                                                                                   offsetof(Starfield::Star, color)));

            commit(data);

            drawCall->value()->init(GL_POINTS, 0, GLsizei(numberOfStars));
            commit(drawCall);

            commit(object);
        }

        void invalidate() {
            if (object.get())
                object->invalidate();
        }

    public:
        Cpu2GpuProjection() {
            data = React::makeScalarPtr(std::make_shared<SG::Data>());
            disabledArrays = React::makeScalarPtr<std::set<std::string>>();
            dataBinding = React::makeScalarPtr(std::make_shared<SG::DataBinding>());
            drawCall = React::makeScalarPtr(std::make_shared<SG::DrawCall>());

            segment = std::make_shared<SG::Segment>(dataBinding, drawCall, false);
            segments.push_back(segment);

            binder = std::make_shared<SG::BindData>();
            binder->setData(data);
            binder->setDisabledArrays(disabledArrays);
            binder->setDataBinding(dataBinding);
        }
    };
}

#endif
