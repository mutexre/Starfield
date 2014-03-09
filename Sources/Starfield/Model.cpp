#include <math.h>
#include <Starfield/Starfield.h>

Starfield::Model::Model() : box(React::makeScalar<Rt::Range<Math::Vector<float>>>())
{}

Starfield::Model::Model(Rt::u4 numberOfStars,
                        SizeDistribution sizeDistr,
                        SpatialDistribution spatialDistribution,
                        ColorDistribution colorDistr)
    : Model()
{
    distributions.size = std::move(sizeDistr);
    distributions.spatial = std::move(spatialDistribution);
    box->get().set(distributions.spatial->getBoundedRange());
    distributions.color = std::move(colorDistr);
    setNumberOfStars(numberOfStars);
}

void Starfield::Model::generateStars()
{
    stars = std::vector<Star>(numberOfStars);

    for (auto& star : stars) {
        star.size = distributions.size->sample();
        star.position = distributions.spatial->sample();
        star.color = distributions.color->sample();
    }

    std::sort(stars.begin(), stars.end(), [](const Star& a, const Star& b) -> bool {
        return a.position.z > b.position.z;
    });
}

Rt::u4 Starfield::Model::getNumberOfStars() const {
    return numberOfStars;
}

void Starfield::Model::setNumberOfStars(Rt::u4 n) {
    numberOfStars = n;
    generateStars();
}

/*
void Starfield::Model::setZ(float z) {
    auto n = std::trunc(z / box.length().z);
    z -= n * box.length().z;
    partitionIndex = findPartitionPoint(z) % numberOfStars;
}
*/

Rt::Range<Math::Vector<float>>
Starfield::Model::getBox() const {
    return box->get().get();
}

/*
//
// First approximate the location of partition point with arithmetic expression.
// Then, starting from that approximate location find precise i so that z[i] < z < z[i+1].
//
Rt::u4 Starfield::Model::findPartitionPoint(float z) {
    Rt::u4 i = numberOfStars * ((box.length().z - z) / box.length().z);
    i = (i < numberOfStars ? i : numberOfStars - 1);

    Rt::Option<float> last_iz;
    while (1) {
        auto iz = stars[i].position.z;

        if (iz < z) {
            if (last_iz.defined) {
                if (last_iz.get() > z) break;
                last_iz.value = iz;
            }
            else
                last_iz.define(iz);
            
            if (i != 0) {
                i--;
                continue;
            }
            else break;
        }

        if (iz > z) {
            if (last_iz.defined) {
                if (last_iz.get() < z) break;
                last_iz.value = iz;
            }
            else
                last_iz.define(iz);

            if (++i != numberOfStars) continue;
            else break;
        }

        break;
    }

    return i;
}
*/
