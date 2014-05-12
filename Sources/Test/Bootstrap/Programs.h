#ifndef header_BFA799B9B930
#define header_BFA799B9B930

namespace Starfield
{
    struct Programs
    {
        React::ScalarPtr<std::shared_ptr<SG::Program>> starfield;

        Programs(ProgramSrc* src) {
            GL::ShaderProgram::FragDataLocations fragDataLocations;
            fragDataLocations[0] = "fragmentColor";

            starfield = React::makeScalarPtr(std::make_shared<SG::Program>(src->starfield, fragDataLocations));
            starfield->setName("starfield");
        }
    };
}

#endif
