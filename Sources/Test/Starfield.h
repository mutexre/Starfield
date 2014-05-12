#ifndef header_b6445c47c07a
#define header_b6445c47c07a

#include <mutex>
#include <stdlib.h>
#include <math.h>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <Starfield/Starfield.h>
#include <Test/Bootstrap/Transforms.h>
#include <Test/Bootstrap/ProgramsSources.h>
#include <Test/Bootstrap/Programs.h>
#include <Test/Bootstrap/Matrices.h>
#include <Test/Bootstrap/Materials.h>

using boost::multi_index::sequenced;
using boost::multi_index::hashed_unique;
using boost::multi_index::const_mem_fun;

namespace Starfield
{
    class Main
    {
    public:
        std::unique_ptr<ProgramSrc> programSrc;
        std::unique_ptr<Programs> programs;
        std::unique_ptr<Transforms<>> transforms;
        std::unique_ptr<Matrices> matrices;
        std::unique_ptr<Materials> materials;

        struct {
            std::shared_ptr<Starfield::Model> model;
            std::shared_ptr<Starfield::Object> object;
            std::shared_ptr<Starfield::Animation<Rt::u8, float>> animation;
            std::shared_ptr<Starfield::Cpu2GpuProjection> cpu2gpu;
            std::shared_ptr<SG::BindData> binder;
        }
        starfield;

        std::shared_ptr<SG::View> view;
        std::shared_ptr<SG::Objects> objectsGroup;
        std::shared_ptr<SG::ObjectsOrder> order;

        std::mutex sceneMutex;
        std::string resourcesPath;

        struct {
            struct {
                React::ScalarPtr<Rt::u4> numberOfStars;
                React::ScalarPtr<Rt::Range<Math::Vector<float>>> volume;
                React::ScalarPtr<Starfield::Model::SizeDistribution> sizeDistribution;
                React::ScalarPtr<Starfield::Model::SpatialDistribution> spatialDistribution;
                React::ScalarPtr<Starfield::Model::ColorDistribution> colorDistribution;
                React::ScalarPtr<std::vector<Starfield::Star>> stars;

                React::ScalarPtr<bool> visibility;
                React::ScalarPtr<float> z;
                React::ScalarPtr<float> velocity;
                React::ScalarPtr<float> scale;

                React::ScalarPtr<std::set<std::string>> disabledArrays;

                struct {
                    std::shared_ptr<React::RangeInfo<Math::Vector<float>>> rangeInfo;
                    std::shared_ptr<React::MathVector::GetZ<float>> getAZ, getLengthZ;
                }
                transforms;

                struct {
                    std::shared_ptr<React::Box<float>> z, scale, minZ, zSize;
                }
                box;
            }
            starfield;

            React::MatrixPtr<float> objectsScale;
            React::MatrixPtr<float> objectsRotation;
            React::MatrixPtr<float> objectsTranslate;
            React::MatrixPtr<float> pov;
            React::ScalarPtr<float> aspectRatio;
            React::ScalarPtr<Rt::u8> time;

            struct {
                std::shared_ptr<React::Box<float>> aspectRatio;
            }
            box;
        }
        vars;

        struct {
            std::shared_ptr<Starfield::Animation<Rt::u8, float>> starfield;
        }
        animations;

    private:
        void initStarfieldAnimation();
        void initAnimations();

    public:
        Main(const std::string& resourcesPath);

        void setGLOptions();
        void onGlReady();

        void prepareScene(Rt::u8 t);
        void drawScene();
        void syncData(bool updateStarsData);

        void updateStarScale(float w, float h);
        void reshape(float w, float h);

        void keyDown(Rt::Option<UI::Key>, const std::string& characters = "", void* event = nullptr);
        void keyUp(UI::Key, void* event);

        void scroll(float x, float y, float dx, float dy, void* event);
        void move(float x, float y, float dx, float dy, void* event);
    };
}

#endif
