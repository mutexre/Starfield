#ifndef header_09AD18AEED6D
#define header_09AD18AEED6D

namespace Starfield
{
    template <typename F>
    class Partition : React::Transform<Partition<F>>
    {
        REACT_DEFINE_INPUT(React::ScalarPtr<Rt::u4>, numberOfStars, getNumberOfStars, setNumberOfStars, &Partition::invalidate)
        REACT_DEFINE_INPUT(React::ScalarPtr<Rt::Range<Math::Vector<F>>>, box, getBox, setBox, &Partition::invalidate)
        REACT_DEFINE_INPUT(React::ScalarPtr<std::vector<Star>>, stars, getStars, setStars, &Partition::invalidate)
        REACT_DEFINE_INPUT(React::ScalarPtr<F>, z, getZ, setZ, &Partition::invalidate)
        REACT_DEFINE_OUTPUT(React::ScalarPtr<Rt::u4>, partition, getPartition, setPartition, &Partition::evaluate)

    protected:
        //
        // First approximate the location of partition point with arithmetic expression.
        // Then, starting from that approximate location find precise i so that z[i] < z < z[i+1].
        //
        void evaluate() {
            auto numberOfStarsValue = numberOfStars->value();
            auto& boxValue = box->value();
            auto& starsValue = stars->value();

            Rt::u4 i = numberOfStarsValue * ((boxValue.length().z - z) / boxValue.length().z);
            i = (i < numberOfStarsValue ? i : numberOfStarsValue - 1);

            Rt::Option<float> last_iz;
            while (1) {
                auto iz = starsValue[i].position.z;

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

            partition->set(i);
            commit(partition);
        }

        void invalidate() {
            if (partition.get())
                partition->invalidate();
        }
    };
}

#endif
