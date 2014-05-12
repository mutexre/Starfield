#ifndef header_627816173249
#define header_627816173249

namespace Starfield
{
    class Object : public SG::Object
    {
    private:
        Rt::u4 numberOfStars;

    public:
        React::ScalarPtr<std::shared_ptr<SG::Data>> data;
        React::ScalarPtr<std::shared_ptr<SG::DrawCall>> drawCall;
        React::ScalarPtr<std::set<std::string>> disabledArrays;
        React::ScalarPtr<std::shared_ptr<SG::DataBinding>> dataBinding;

    public:
        Object() {}
        
        Object(const React::ScalarPtr<std::shared_ptr<SG::Program>>& program,
               const React::ScalarPtr<std::shared_ptr<SG::Material>>& material)
        {
            data = React::makeScalarPtr(std::make_shared<SG::Data>());
            disabledArrays = React::makeScalarPtr<std::set<std::string>>();
            dataBinding = React::makeScalarPtr(std::make_shared<SG::DataBinding>());
            drawCall = React::makeScalarPtr(std::make_shared<SG::DrawCall>());
            auto segment = std::make_shared<SG::Segment>(dataBinding, drawCall, program, material, false);
            add(segment);

            program->value()->bind();
            auto location = program->value()->get()->getAttributeLocation("color");
            if (location.defined) glVertexAttrib4f(location.get(), 1.0f, 1.0f, 1.0f, 0.99f);
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
