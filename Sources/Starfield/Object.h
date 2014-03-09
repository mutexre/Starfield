#ifndef header_627816173249
#define header_627816173249

namespace Starfield
{
    class Object : public SG::Object
    {
    public:
        Rt::u4 numberOfStars;

    public:
        Object(std::shared_ptr<Model> model, std::shared_ptr<SG::Program> program, std::shared_ptr<SG::Material> material) {
            auto data = std::make_shared<Data>(model);
            auto dataBinding = std::make_shared<SG::DataBinding>(data, program);
            auto drawCall = std::make_shared<SG::DrawCall>(GL_POINTS, 0, model->getNumberOfStars());
            auto segment = std::make_shared<SG::Segment>(dataBinding, drawCall, program, material, false);
            add(segment);

            program->bind();
            auto location = program->get()->getAttributeLocation("color");
            if (location.defined) glVertexAttrib4f(location.get(), 1.0f, 1.0f, 1.0f, 0.99f);

            auto box = model->getBox();
            auto zSize = box.length().z;
            auto minZ = box.a.z;

            program->setVariable("zSize", (const Rt::u1*)&zSize);
            program->setVariable("minZ", (const Rt::u1*)&minZ);
        }

#if 0
        Rt::u4 partitionIndex;

        virtual void inheritedRender()
        {
        #if 0
                auto numberOfInstances = model->getNumberOfStars();

                uniformBlock->prepare(GL::Buffer::Access::Write);
                uniformBlock->setScalar(View::OpenGL::Stars::UniformBlock::Description::Variables::startIndex, 0);
                uniformBlock->complete();

                renderers.stars->lines.numberOfInstances = renderers.stars->triangles.numberOfInstances = numberOfInstances;
                SG::Object::inheritedRender();
        #endif

        #if 1
            if (partitionIndex < numberOfStars)
            {
                auto numberOfInstances = numberOfStars - partitionIndex;

                program->setVariable("startIndex", (const Rt::u1*)&partitionIndex);

                model->setNumberOfInstances(numberOfInstances);
                SG::Object::inheritedRender();
            }
        #endif

        #if 1
            if (partitionIndex > 0)
            {
                Rt::u4 i = 0;
                program->setVariable("startIndex", (const Rt::u1*)&i);

                model->setNumberOfInstances(partitionIndex);
                SG::Object::inheritedRender();
            }
        #endif
        }
#endif
    };
}

#endif
