#ifndef header_4b6cbca20069
#define header_4b6cbca20069

namespace Starfield
{
    struct Materials {
        React::ScalarPtr<std::shared_ptr<SG::Material>> starfield;

        Materials() {
            starfield = React::makeScalarPtr(std::make_shared<SG::Material>());
        }
    };
}

#endif
