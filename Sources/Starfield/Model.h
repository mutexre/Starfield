#ifndef header_9B9EB69DFFC9
#define header_9B9EB69DFFC9

namespace Starfield
{
    struct Star {
        float size;
        Math::Vector<float> position;
        Color::RGB<float> color;
    };

    class Model : public React::Transform<Model>
    {
    public:
        using SizeDistribution = std::shared_ptr<Math::Distribution::Bounded<float>>;
        using SpatialDistribution = std::shared_ptr<Math::Distribution::Bounded<Math::Vector<float>>>;
        using ColorDistribution = std::shared_ptr<Math::Distribution::Unbounded<Color::RGB<>>>;

        REACT_DEFINE_INPUT(React::ScalarPtr<Rt::u4>, numberOfStars, getNumberOfStars, setNumberOfStars, &Model::invalidate)
        REACT_DEFINE_INPUT(React::ScalarPtr<Rt::Range<Math::Vector<float>>>, box, getBox, setBox, &Model::invalidate)
        REACT_DEFINE_INPUT(React::ScalarPtr<SizeDistribution>, sizeDistribution, getSizeDistribution, setSizeDistribution, &Model::invalidate)
        REACT_DEFINE_INPUT(React::ScalarPtr<SpatialDistribution>, spatialDistribution, getSpatialDistribution, setSpatialDistribution, &Model::invalidate)
        REACT_DEFINE_INPUT(React::ScalarPtr<ColorDistribution>, colorDistribution, getColorDistribution, setColorDistribution, &Model::invalidate)
        REACT_DEFINE_OUTPUT(React::ScalarPtr<std::vector<Star>>, stars, getStars, setStars, &Model::evaluate)

    protected:
        void evaluate() {
            auto stars = std::vector<Star>(numberOfStars->get());
            auto& sizeDistr = sizeDistribution->get();
            auto& spatialDistr = spatialDistribution->get();
            auto& colorDistr = colorDistribution->get();

            for (auto& star : stars) {
                star.size = sizeDistr->sample();
                star.position = spatialDistr->sample();
                star.color = colorDistr->sample();
            }

            std::sort(stars.begin(), stars.end(), [](const Star& a, const Star& b) -> bool {
                return a.position.z > b.position.z;
            });

            this->stars->set(stars);
            commit(this->stars);
        }

        void invalidate() {
            if (stars.get())
                stars->invalidate();
        }

    public:
        Model() {}
        virtual ~Model() {}
    };
}

#endif
