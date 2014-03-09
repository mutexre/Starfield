#ifndef header_F2401FC7AD32
#define header_F2401FC7AD32

namespace Starfield
{
    class Data : public SG::Data
    {
    public:
        Data() {}

        Data(std::shared_ptr<Starfield::Model> model)
        {
            auto buffer = std::make_shared<GL::Buffer>();
            auto target = GL::Buffer::Target::Array;

            buffer->bind(target);
            GL::Buffer::data(target,
                             model->getNumberOfStars() * sizeof(Starfield::Model::Star),
                             nullptr, GL_STATIC_DRAW);

            auto ptr = GL::Buffer::map(target, GL::Buffer::Access::Write);
            if (!ptr) Rt::error(0xC19932E1);

#if 1
            memcpy(ptr, (Rt::u1*)model->stars.data(), model->getNumberOfStars() * sizeof(Starfield::Model::Star));
#else
            starfieldObject->setArray(ptr,
                                      (float*)(((Rt::u1*)model->stars.data()) + offsetof(Starfield::Model::Star, size)), 0,
                                      model->getNumberOfStars(), sizeof(Starfield::Model::Star));

            starfieldObject->setArray(ptr,
                                      (Math::Vector<float>*)(((Rt::u1*)model->stars.data()) + offsetof(Starfield::Model::Star, position)), 0,
                                      model->getNumberOfStars(), sizeof(Starfield::Model::Star));

            starfieldObject->setArray(ptr,
                                      (Color::RGB<float>*)(((Rt::u1*)model->stars.data()) + offsetof(Starfield::Model::Star, color) + offsetof(Color::RGB<float>, r)), 0,
                                      model->getNumberOfStars(), sizeof(Starfield::Model::Star));
#endif

            GL::Buffer::unmap(target);

            setAttr("size", buffer, GL::VertexArray::AttribPointer(1, GL_FLOAT, GL_FALSE,
                                                                   sizeof(Starfield::Model::Star),
                                                                   offsetof(Starfield::Model::Star, size)));

            setAttr("coord", buffer, GL::VertexArray::AttribPointer(3, GL_FLOAT, GL_FALSE,
                                                                    sizeof(Starfield::Model::Star),
                                                                    offsetof(Starfield::Model::Star, position)));

            setAttr("color", buffer, GL::VertexArray::AttribPointer(3, GL_FLOAT, GL_FALSE,
                                                                    sizeof(Starfield::Model::Star),
                                                                    offsetof(Starfield::Model::Star, color)));
        }

        virtual ~Data() {}
    };
}

#endif
