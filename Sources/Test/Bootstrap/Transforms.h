#ifndef header_c6130726cc18
#define header_c6130726cc18

namespace Starfield
{
    template <typename T = float>
    struct Transforms
    {
        std::shared_ptr<React::Multiply<Math::Matrix<T>>> starfield;

        Transforms() {
            starfield = std::make_shared<React::Multiply<Math::Matrix<T>>>();
            starfield->setName("starfield");
            auto output = React::makeMatrix<>(4);
            output->setName("starfield output");
            starfield->setOutput(output);
        }
    };
}

#endif
