#ifndef header_E1A2252D4F2E
#define header_E1A2252D4F2E

namespace Starfield
{
    template <typename T>
    static T mix(T a, T b, T x) {
        return x * a + (T(1) - x) * b;
    }

    class SpatialDistribution : public Math::Distribution::Bounded<Math::Vector<float>>
    {
    private:
        std::unique_ptr<Math::Distribution::Uniform<float>> uniformGenerator;
        Rt::Range<Math::Vector<float>> range;

    public:
        SpatialDistribution(Rt::Range<Math::Vector<float>> range) {
            auto gen = new Math::Distribution::Uniform<float>(Rt::Range<float>(0.0f, 1.0f));
            uniformGenerator = std::unique_ptr<Math::Distribution::Uniform<float>>(gen);
            this->range = range;
        }

        virtual Math::Vector<float> sample() {
            return Math::Vector<float>(range.a.x + range.length().x * uniformGenerator->sample(),
                                       range.a.y + range.length().y * uniformGenerator->sample(),
                                       range.a.z + range.length().z * uniformGenerator->sample());
        }

        virtual Rt::Option<Rt::Range<Math::Vector<float>>> getRange() {
            return Rt::Option<Rt::Range<Math::Vector<float>>>(range);
        }
    };

    class HSVColorDistribution : public Math::Distribution::Unbounded<Color::RGB<float>>
    {
    private:
        std::unique_ptr<Math::Distribution::Uniform<float>> uniformGenerator;
        Rt::Range<Color::HSV<float>> range;

    public:
        HSVColorDistribution(Rt::Range<Color::HSV<float>> range) {
            auto gen = new Math::Distribution::Uniform<float>(Rt::Range<float>(0.0f, 1.0f));
            uniformGenerator = std::unique_ptr<Math::Distribution::Uniform<float>>(gen);
            this->range = range;
        }

        virtual Color::RGB<float> sample() override
        {
            auto h = range.a.h + range.length().h * uniformGenerator->sample();
            h -= ceilf(h);

            auto s = range.a.s + range.length().s * uniformGenerator->sample();
            s = (s <= 1.0f ? s : 1.0f);

            auto v = range.a.v + range.length().v * uniformGenerator->sample();
            v = (v <= 1.0f ? v : 1.0f);

            return Color::hsv2rgb(Color::HSV<float>(h, s, v));
        }
    };

    class MixColorDistribution : public Math::Distribution::Unbounded<Color::RGB<float>>
    {
    private:
        std::unique_ptr<Math::Distribution::Uniform<float>> uniformGenerator;
        Color::HSV<float> a, b;

    public:
        MixColorDistribution(Color::HSV<float> a, Color::HSV<float> b) {
            auto gen = new Math::Distribution::Uniform<float>(Rt::Range<float>(0.0f, 1.0f));
            uniformGenerator = std::unique_ptr<Math::Distribution::Uniform<float>>(gen);
            this->a = a;
            this->b = b;
        }
     
        virtual Color::RGB<float> sample() override {
            auto x = uniformGenerator->sample();
            auto h = mix(a.h, b.h, x);
            auto s = mix(a.s, b.s, x);
            auto v = mix(a.v, b.v, x);
            return Color::hsv2rgb(Color::HSV<float>(h, s, v));
        }
    };

    class MixColorDistribution2 : public Math::Distribution::Unbounded<Color::RGB<float>>
    {
    private:
        std::unique_ptr<Math::Distribution::Uniform<float>> uniformGenerator;
        std::default_random_engine generator;
        struct {
            Rt::Range<float> hue1, hue2, saturation, value;
        }
        ranges;

    public:
        MixColorDistribution2(Rt::Range<float> hue1, Rt::Range<float> hue2, Rt::Range<float> saturation, Rt::Range<float> value) {
            auto gen = new Math::Distribution::Uniform<float>(Rt::Range<float>(0.0f, 1.0f));
            uniformGenerator = std::unique_ptr<Math::Distribution::Uniform<float>>(gen);
            ranges.hue1 = hue1;
            ranges.hue2 = hue2;
            ranges.saturation = saturation;
            ranges.value = value;
            generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
        }

        float random(Rt::u1 n) {
            float retval = 1.0f;
            for (auto i = 0; i < n; i++)
                retval *= uniformGenerator->sample();
            return retval;
        }

        virtual Color::RGB<float> sample() override
        {
            Rt::u1 n = 1;

            auto h = generator() % 2 ? ranges.hue1.a + ranges.hue1.length() * random(n) :
                                       ranges.hue2.a + ranges.hue2.length() * random(n);

            auto s = ranges.saturation.a + ranges.saturation.length() * random(n);
            s = (s <= 1.0f ? s : 1.0f);

            auto v = ranges.value.a + ranges.value.length() * random(n);
            v = (v <= 1.0f ? v : 1.0f);

            return Color::hsv2rgb(Color::HSV<float>(h, s, v));
        }
    };
}

#endif
