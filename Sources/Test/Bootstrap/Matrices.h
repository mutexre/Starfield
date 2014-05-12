#ifndef header_a21a2e32d10a
#define header_a21a2e32d10a

namespace Starfield
{
    struct Matrices
    {
        React::MatrixPtr<> identity;
        React::MatrixPtr<> perspective;

        React::MatrixPtr<> starfield;

        Matrices() {
            perspective = React::makeMatrix<>(4);
            perspective->setName("perspective projection");

            starfield = React::makeMatrix<>(4);
            starfield->setName("starfield");
        }
    };
}

#endif
