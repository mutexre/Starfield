#include <stdafx.h>
#include <Test/Starfield.h>

Starfield::Main::Main(const std::string& resourcesPath) {
    this->resourcesPath = resourcesPath;
}

void Starfield::Main::setGLOptions() {
//    glEnable(GL_DOUBLEBUFFER);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_CLAMP);

#if defined __APPLE__ && TARGET_OS_IPHONE
    glDepthRangef(0.0, 1.0);
#else
//    glDepthRange(0.5, 100.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

    glDepthRange(0.0f, 1.0f);

#if 1
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

#if 0
    #ifndef GL_POINT_SMOOTH
        #define GL_POINT_SMOOTH 0x0B10
    #endif

    //glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
#endif

    //glFrontFace(GL_CCW);

#if 0
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, -1.0);
#endif

//    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 0.0);
//    glLineWidth(2.0);

    float c = 0.0f;
    glClearColor(c, c, c, 1.0f);
    glEnable(GL_PROGRAM_POINT_SIZE);

    //wglSwapIntervalEXT(-1);
}

void Starfield::Main::initStarfieldAnimation() {
    animations.starfield = std::make_shared<Starfield::Animation<Rt::u8, float>>();
    animations.starfield->setTimeSource(vars.time);
    animations.starfield->setVelocity(vars.starfield.velocity);
    animations.starfield->setBox(starfield.model->getBox());
    animations.starfield->setOutput(vars.starfield.z);
}

void Starfield::Main::initAnimations() {
    initStarfieldAnimation();
}

template <typename T = float>
std::vector<T> convertArrayToVector(const T* array, Rt::u4 n) {
    std::vector<T> retval;
    retval.resize(n);
    memcpy(retval.data(), array, n * sizeof(T));
    return retval;
}

void Starfield::Main::onGlReady()
{
    programSrc = std::unique_ptr<ProgramSrc>(new ProgramSrc(resourcesPath));
    programs = std::unique_ptr<Programs>(new Programs(programSrc.get()));
    matrices = std::unique_ptr<Matrices>(new Matrices);
    transforms = std::unique_ptr<Transforms<>>(new Transforms<>);
    materials = std::unique_ptr<Materials>(new Materials());

// SceneGraph Objects
    view = std::make_shared<SG::View>();
    objectsGroup = std::make_shared<SG::Objects>();
    order = std::make_shared<SG::ObjectsOrder>();

// Generic Vars
    vars.time = React::makeScalarPtr<Rt::u8>();
    vars.time->setName("vars.time");

    vars.aspectRatio = React::makeScalarPtr<float>();
    vars.aspectRatio->setName("vars.aspectRatio");

// Starfield Vars
    vars.starfield.stars = React::makeScalarPtr<std::vector<Starfield::Star>>();
    vars.starfield.stars->setName("vars.starfield.stars");

    vars.starfield.numberOfStars = React::makeScalarPtr<Rt::u4>(128 * 1024);
    vars.starfield.numberOfStars->setName("vars.starfield.numberOfStars");

    vars.starfield.z = React::makeScalarPtr(0.0f);
    vars.starfield.z->setName("vars.starfield.z");

    vars.starfield.velocity = React::makeScalarPtr(0.1f /* 1000000.0f*/);
    vars.starfield.velocity->setName("vars.starfield.velocity");

    vars.starfield.volume = React::makeScalarPtr(Rt::Range<Math::Vector<float>>(Math::Vector<float>(-1.0f, -1.0f, 0.0f), Math::Vector<float>(2.0f, 2.0f, 1.0f)));
    vars.starfield.volume->setName("vars.starfield.volume");

    vars.starfield.sizeDistribution = React::makeScalarPtr(std::shared_ptr<Math::Distribution::Bounded<float>>(new Math::Distribution::Uniform<float>(Rt::Range<float>(1.0f, 2.0f))));
    vars.starfield.sizeDistribution->setName("vars.starfield.sizeDistribution");

    vars.starfield.spatialDistribution = React::makeScalarPtr(std::shared_ptr<Math::Distribution::Bounded<Math::Vector<float>>>(new SpatialDistribution(vars.starfield.volume->value())));
    vars.starfield.spatialDistribution->setName("vars.starfield.spatialDistribution");

    float range = 0.1f;
    Rt::Range<float> colorDistributionRanges[] = {
        Rt::Range<float>(0.0f - range / 2.0f, range),
        Rt::Range<float>(0.66f - range / 2.0f, range)
    };

    vars.starfield.colorDistribution = React::makeScalarPtr(std::shared_ptr<Math::Distribution::Unbounded<Color::RGB<float>>>(new MixColorDistribution2(colorDistributionRanges[0],
                                                                                                                      colorDistributionRanges[1],
                                                                                                                      Rt::Range<float>(0.0f, 0.35f),
                                                                                                                      Rt::Range<float>(0.7f, 0.3f))));
    vars.starfield.colorDistribution->setName("vars.starfield.colorDistribution");

    vars.starfield.visibility = React::makeScalarPtr(true);
    vars.starfield.visibility->setName("visibility.starfield");

    vars.starfield.scale = React::makeScalarPtr();
    vars.starfield.scale->setName("vars.starfield.scale");

    vars.starfield.disabledArrays = React::makeScalarPtr<std::set<std::string>>();
    vars.starfield.disabledArrays->setName("vars.starfield.disabledArrays");

// Transformations
// build transforms tree
    transforms->starfield->addInput(matrices->starfield);

    starfield.model = std::make_shared<Starfield::Model>();
    starfield.model->setNumberOfStars(vars.starfield.numberOfStars);
    starfield.model->setBox(vars.starfield.volume);
    starfield.model->setSizeDistribution(vars.starfield.sizeDistribution);
    starfield.model->setSpatialDistribution(vars.starfield.spatialDistribution);
    starfield.model->setColorDistribution(vars.starfield.colorDistribution);
    starfield.model->setStars(vars.starfield.stars);

    vars.starfield.object = React::makeScalarPtr(std::make_shared<SG::Object>(/*programs->starfield, materials->starfield*/));

    starfield.animation = std::make_shared<Starfield::Animation<Rt::u8, float>>();
    starfield.animation->setBox(vars.starfield.volume);
    starfield.animation->setTimeSource(vars.time);
    starfield.animation->setVelocity(vars.starfield.velocity);
    starfield.animation->setOutput(vars.starfield.z);

    starfield.cpu2gpu = std::make_shared<Starfield::Cpu2GpuProjection>();
    starfield.cpu2gpu->setStars(vars.starfield.stars);
    starfield.cpu2gpu->setProgram(programs->starfield);
    starfield.cpu2gpu->setMaterial(materials->starfield);
    starfield.cpu2gpu->setObject(vars.starfield.object);
//    starfield.cpu2gpu->setData(vars.starfield.object->data);
//    starfield.cpu2gpu->setDrawCall(vars.starfield.object->drawCall);

/*    starfield.binder = std::make_shared<SG::BindData>();
    starfield.binder->setData(vars.starfield.object->data);
    starfield.binder->setProgram(programs->starfield);
    starfield.binder->setDisabledArrays(vars.starfield.disabledArrays);
    starfield.binder->setDataBinding(vars.starfield.object->dataBinding);*/

//    starfield.object->enableDepthTest(false);

// RangeInfo
    vars.starfield.transforms.rangeInfo = std::make_shared<React::RangeInfo<Math::Vector<float>>>();
    vars.starfield.transforms.rangeInfo->setRange(vars.starfield.volume);
    vars.starfield.transforms.rangeInfo->setA(React::makeScalarPtr<Math::Vector<float>>());
    vars.starfield.transforms.rangeInfo->getA()->setName("vars.starfield.transforms.rangeInfo.a");
    vars.starfield.transforms.rangeInfo->setB(React::makeScalarPtr<Math::Vector<float>>());
    vars.starfield.transforms.rangeInfo->getB()->setName("vars.starfield.transforms.rangeInfo.b");
    vars.starfield.transforms.rangeInfo->setLength(React::makeScalarPtr<Math::Vector<float>>());
    vars.starfield.transforms.rangeInfo->getLength()->setName("vars.starfield.transforms.rangeInfo.length");

    vars.starfield.transforms.getAZ = std::make_shared<React::MathVector::GetZ<float>>();
    vars.starfield.transforms.getAZ->setInput(vars.starfield.transforms.rangeInfo->getA());
    vars.starfield.transforms.getAZ->setOutput(React::makeScalarPtr<float>());
    vars.starfield.transforms.getAZ->getOutput()->setName("vars.starfield.transforms.getAZ.output");

    vars.starfield.transforms.getLengthZ = std::make_shared<React::MathVector::GetZ<float>>();
    vars.starfield.transforms.getLengthZ->setInput(vars.starfield.transforms.rangeInfo->getLength());
    vars.starfield.transforms.getLengthZ->setOutput(React::makeScalarPtr<float>());
    vars.starfield.transforms.getLengthZ->getOutput()->setName("vars.starfield.transforms.getAZ.output");

    vars.box.aspectRatio = std::make_shared<React::Box<float>>();
    vars.box.aspectRatio->setInput(vars.aspectRatio);
    vars.box.aspectRatio->setOutput(React::makeMatrix<float>(1));
    vars.box.aspectRatio->getOutput()->setName("vars.box.aspectRatio.output");

    vars.starfield.box.z = std::make_shared<React::Box<float>>();
    vars.starfield.box.z->setInput(vars.starfield.z);
    vars.starfield.box.z->setOutput(React::makeMatrix<float>(1));
    vars.starfield.box.z->getOutput()->setName("vars.starfield.box.z.output");

    vars.starfield.box.scale = std::make_shared<React::Box<float>>();
    vars.starfield.box.scale->setInput(vars.starfield.z);
    vars.starfield.box.scale->setOutput(React::makeMatrix<float>(1));
    vars.starfield.box.scale->getOutput()->setName("vars.starfield.box.scale.output");

    vars.starfield.box.minZ = std::make_shared<React::Box<float>>();
    vars.starfield.box.minZ->setInput(vars.starfield.transforms.getAZ->getOutput());
    vars.starfield.box.minZ->setOutput(React::makeMatrix<float>(1));
    vars.starfield.box.minZ->getOutput()->setName("vars.starfield.box.minZ.output");

    vars.starfield.box.zSize = std::make_shared<React::Box<float>>();
    vars.starfield.box.zSize->setInput(vars.starfield.transforms.getLengthZ->getOutput());
    vars.starfield.box.zSize->setOutput(React::makeMatrix<float>(1));
    vars.starfield.box.zSize->getOutput()->setName("vars.starfield.box.zSize.output");

// -----------------

    order->add(vars.starfield.object);
    objectsGroup->add(vars.starfield.object);

    view->setObjects(objectsGroup);
    view->setOrder(order);

    view->setVisibility(vars.starfield.object, vars.starfield.visibility);

    matrices->perspective->set(Math::PerspectiveProjection<float>(M_PI / 4.0f, 0.01f, 300.0f));
    matrices->starfield->set(Math::Scale<float>(1.0f, 1.0f, -1.0f));

// Vars Bindings
    view->bind("aspectRatio", vars.box.aspectRatio->getOutput());
    view->bind(vars.starfield.object, "projection", matrices->perspective);
    view->bind(vars.starfield.object, "transform", transforms->starfield->getOutput());
    view->bind(vars.starfield.object, "z", vars.starfield.box.z->getOutput());
    view->bind(vars.starfield.object, "scale", vars.starfield.box.scale->getOutput());
    view->bind(vars.starfield.object, "minZ", vars.starfield.box.minZ->getOutput());
    view->bind(vars.starfield.object, "zSize", vars.starfield.box.zSize->getOutput());

    initAnimations();
}
