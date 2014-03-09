#ifndef header_9B9EB69DFFC9
#define header_9B9EB69DFFC9

namespace Starfield
{
    class Model : public Rt::Object
    {
    public:
        struct Star {
            float size;
            Math::Vector<float> position;
            Color::RGB<float> color;
        };

        typedef std::shared_ptr<Math::Distribution::Bounded<float>> SizeDistribution;
        typedef std::shared_ptr<Math::Distribution::Bounded<Math::Vector<float>>> SpatialDistribution;
        typedef std::shared_ptr<Math::Distribution::Unbounded<Color::RGB<>>> ColorDistribution;

    public:
        std::vector<Star> stars;

    protected:
        Rt::u4 numberOfStars;
        React::ValuePtr<Rt::Range<Math::Vector<float>>> box;

        struct {
            SizeDistribution size;
            SpatialDistribution spatial;
            ColorDistribution color;
        }
        distributions;

    protected:
        void generateStars();

    public:
        Model();
        Model(Rt::u4 numberOfStars,
              SizeDistribution sizeDistr,
              SpatialDistribution spatialDistribution,
              ColorDistribution colorDistr);

        virtual ~Model() {}

        Rt::u4 getNumberOfStars() const;
        void setNumberOfStars(Rt::u4);

        Rt::Range<Math::Vector<float>> getBox() const;
    };
}

#endif
