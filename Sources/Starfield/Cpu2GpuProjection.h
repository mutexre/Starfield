#ifndef header_F2401FC7AD32
#define header_F2401FC7AD32

namespace Starfield
{
    class Cpu2GpuProjection : public React::Transform<Cpu2GpuProjection>
    {
        REACT_DEFINE_INPUT(React::ScalarPtr<std::vector<Starfield::Star>>, stars, getStars, setStars, &Cpu2GpuProjection::invalidate)
        REACT_DEFINE_OUTPUT(React::ScalarPtr<std::shared_ptr<SG::Data>>, data, getData, setData, &Cpu2GpuProjection::evaluate)
        REACT_DEFINE_OUTPUT(React::ScalarPtr<std::shared_ptr<SG::DrawCall>>, drawCall, getDrawCall, setDrawCall, &Cpu2GpuProjection::evaluate)

    protected:
        void evaluate() {
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

            auto& dataValue = data->currentValue();
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

            drawCall->currentValue()->init(GL_POINTS, 0, GLsizei(numberOfStars));
            commit(drawCall);
        }

        void invalidate() {
            if (data.get())
                data->invalidate();
        }
    };
}

#endif
