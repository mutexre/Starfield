#ifndef header_75DBAEAF1E16
#define header_75DBAEAF1E16

#if defined _WIN32 || defined _WIN64
	#include "resource.h"
#endif

namespace Starfield
{
    struct ProgramSrc
    {
        GL::ShaderProgram::Sources starfield;

        struct {
            GL::ShaderProgram::Sources points, edges, faces;
        }
        ihs;

        ProgramSrc(const std::string& resourcesPath)
        {
#if defined _WIN32 || defined _WIN64
            starfield = GL::ShaderProgram::Sources(Rt::readTextResource("GLSL", IDR_STARFIELD_POINTS_VS),
                                                   Rt::readTextResource("GLSL", IDR_STARFIELD_POINTS_FS));
#else
            printf("\"%s\"\n", resourcesPath.c_str());

            starfield = GL::ShaderProgram::Sources(Rt::readFile(resourcesPath + "/Points.vs"),
                                                   Rt::readFile(resourcesPath + "/Points.fs"));
#endif
        }
    };
}

#endif
