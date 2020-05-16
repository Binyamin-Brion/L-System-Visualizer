//
// Created by binybrion on 5/16/20.
//

#include <QtTest/qtestcase.h>
#include "TestStoredModels.h"
#include "Render/DataStructures/StoredModels.h"

using namespace ModelLoading;
using namespace Render::DataStructures;

static Tests::Render::TestStoredModels testStoredModels{"Test_Stored_Models"};

namespace Tests
{
    namespace Render
    {
        TestStoredModels::TestStoredModels(const QString &testClassName)
                            :
                                TestSuite{testClassName}
        {
            cubeModelTestFile = getAssetFolderLocation().toStdString() + "/cube.obj";

            sphereModelTestFile = getAssetFolderLocation().toStdString() + "/sphere.obj";
        }

        void TestStoredModels::testAddModel()
        {
            Model cubeModel{cubeModelTestFile};
            Model sphereModel{sphereModelTestFile};

            StoredModels storedModels;

            storedModels.addModel(cubeModel);
            storedModels.addModel(sphereModel);

            const ModelRange &cubeModelRange = storedModels.getModelRanges()[0];
            const ModelRange &sphereModelRange = storedModels.getModelRanges()[1];

            QVERIFY(storedModels.getModelRanges().size() == 2);

            QVERIFY(cubeModelRange.getModel().getModelFileName() == cubeModel.getModelFileName());
            QVERIFY(sphereModelRange.getModel().getModelFileName() == sphereModel.getModelFileName());

            // Test the stored cube.
            QVERIFY(cubeModelRange.getInstanceMatrixBegin() == 0);
            QVERIFY(cubeModelRange.getInstanceMatrixCount() == 0);
            QVERIFY(cubeModelRange.getUserAddedMatrixCount() == 0);
            QVERIFY(cubeModelRange.getIndiceBegin() == 0);
            QVERIFY(cubeModelRange.getIndiceCount() == 36);

            // Test the stored sphere.

            // At the beginning, with no instances stored for any models, all uploaded models' instances are considered
            // to start right at the beginning of the instances VBO.
            QVERIFY(sphereModelRange.getInstanceMatrixBegin() == 0);
            QVERIFY(sphereModelRange.getInstanceMatrixCount() == 0);
            QVERIFY(sphereModelRange.getUserAddedMatrixCount() == 0);
            QVERIFY(sphereModelRange.getIndiceBegin() == 36); // Indices start after the cube's indices.
            QVERIFY(sphereModelRange.getIndiceCount() == 276);
        }

        void TestStoredModels::testAddModelInstances()
        {
            Model cubeModel{cubeModelTestFile};
            Model sphereModel{sphereModelTestFile};

            StoredModels storedModels;

            storedModels.addModel(cubeModel);
            storedModels.addModel(sphereModel);

            const ModelRange &cubeModelRange = storedModels.getModelRanges()[0];
            const ModelRange &sphereModelRange = storedModels.getModelRanges()[1];

            storedModels.addModelInstances(cubeModel.getModelFileName(), 10, false);

            // Test cube.
            QVERIFY(cubeModelRange.getInstanceMatrixBegin() == 0);
            QVERIFY(cubeModelRange.getInstanceMatrixCount() == 10); // This changed!
            QVERIFY(cubeModelRange.getUserAddedMatrixCount() == 0);
            QVERIFY(cubeModelRange.getIndiceBegin() == 0);
            QVERIFY(cubeModelRange.getIndiceCount() == 36);

            // Test sphere.
            QVERIFY(sphereModelRange.getInstanceMatrixBegin() == 10); // This changed- the sphere instances
                                                                                                // are stored after the cube instances!
            QVERIFY(sphereModelRange.getInstanceMatrixCount() == 0);
            QVERIFY(sphereModelRange.getUserAddedMatrixCount() == 0);
            QVERIFY(sphereModelRange.getIndiceBegin() == 36); // Indices start after the cube's indices.
            QVERIFY(sphereModelRange.getIndiceCount() == 276);

            // Try adding some sphere instances.
            storedModels.addModelInstances(sphereModel.getModelFileName(), 20, false);

            QVERIFY(cubeModelRange.getInstanceMatrixBegin() == 0);
            QVERIFY(cubeModelRange.getInstanceMatrixCount() == 10);
            QVERIFY(cubeModelRange.getUserAddedMatrixCount() == 0);
            QVERIFY(cubeModelRange.getIndiceBegin() == 0);
            QVERIFY(cubeModelRange.getIndiceCount() == 36);


            // Test sphere.
            QVERIFY(sphereModelRange.getInstanceMatrixBegin() == 10); // This changed- the sphere instances
            // are stored after the cube instances!
            QVERIFY(sphereModelRange.getInstanceMatrixCount() == 20); // This changed!
            QVERIFY(sphereModelRange.getUserAddedMatrixCount() == 0);
            QVERIFY(sphereModelRange.getIndiceBegin() == 36); // Indices start after the cube's indices.
            QVERIFY(sphereModelRange.getIndiceCount() == 276);
        }

        void TestStoredModels::testAddModelUserInstances()
        {
            // Remember- the user added instances are included within the instance matrix count!

            Model cubeModel{cubeModelTestFile};
            Model sphereModel{sphereModelTestFile};

            StoredModels storedModels;

            storedModels.addModel(cubeModel);
            storedModels.addModel(sphereModel);

            const ModelRange &cubeModelRange = storedModels.getModelRanges()[0];
            const ModelRange &sphereModelRange = storedModels.getModelRanges()[1];

            storedModels.addModelInstances(cubeModel.getModelFileName(), 10, true);

            // Test cube.
            QVERIFY(cubeModelRange.getInstanceMatrixBegin() == 0);
            QVERIFY(cubeModelRange.getInstanceMatrixCount() == 10);
            QVERIFY(cubeModelRange.getUserAddedMatrixCount() == 10); // This changed!
            QVERIFY(cubeModelRange.getIndiceBegin() == 0);
            QVERIFY(cubeModelRange.getIndiceCount() == 36);


            // Test sphere.
            QVERIFY(sphereModelRange.getInstanceMatrixBegin() == 10); // This changed- the sphere instances
                                                                                                // are stored after the cube (not user)  instances!
            QVERIFY(sphereModelRange.getInstanceMatrixCount() == 0);
            QVERIFY(sphereModelRange.getUserAddedMatrixCount() == 0);
            QVERIFY(sphereModelRange.getIndiceBegin() == 36); // Indices start after the cube's indices.
            QVERIFY(sphereModelRange.getIndiceCount() == 276);

            // Try adding some sphere instances.
            storedModels.addModelInstances(sphereModel.getModelFileName(), 20, true);

            QVERIFY(cubeModelRange.getInstanceMatrixBegin() == 0);
            QVERIFY(cubeModelRange.getInstanceMatrixCount() == 10);
            QVERIFY(cubeModelRange.getUserAddedMatrixCount() == 10);
            QVERIFY(cubeModelRange.getIndiceBegin() == 0);
            QVERIFY(cubeModelRange.getIndiceCount() == 36);


            // Test sphere.
            QVERIFY(sphereModelRange.getInstanceMatrixBegin() == 10); // This changed- the sphere instances
                                                                                                // are stored after the cube (not user) instances!
            QVERIFY(sphereModelRange.getInstanceMatrixCount() == 20);
            QVERIFY(sphereModelRange.getUserAddedMatrixCount() == 20); // This changed!
            QVERIFY(sphereModelRange.getIndiceBegin() == 36); // Indices start after the cube's indices.
            QVERIFY(sphereModelRange.getIndiceCount() == 276);
        }

        void TestStoredModels::testAddModel_UserInstances()
        {
            // Remember- the user added instances are included within the instance matrix count!

            Model cubeModel{cubeModelTestFile};
            Model sphereModel{sphereModelTestFile};

            StoredModels storedModels;

            storedModels.addModel(cubeModel);
            storedModels.addModel(sphereModel);

            const ModelRange &cubeModelRange = storedModels.getModelRanges()[0];
            const ModelRange &sphereModelRange = storedModels.getModelRanges()[1];

            storedModels.addModelInstances(cubeModel.getModelFileName(), 10, false);
            storedModels.addModelInstances(cubeModel.getModelFileName(), 10, true);

            // The instance matrix count will be higher than the user instance matrix count because both of the above calls
            // affect the instance matrix count, but only the second affects the user instance matrix count.

            // Test cube.
            QVERIFY(cubeModelRange.getInstanceMatrixBegin() == 0);
            QVERIFY(cubeModelRange.getInstanceMatrixCount() == 20);
            QVERIFY(cubeModelRange.getUserAddedMatrixCount() == 10); // This changed!
            QVERIFY(cubeModelRange.getIndiceBegin() == 0);
            QVERIFY(cubeModelRange.getIndiceCount() == 36);


            // Test sphere.
            QVERIFY(sphereModelRange.getInstanceMatrixBegin() == 20); // This changed- the sphere instances
                                                                                                // are stored after the cube (not user) instances!
            QVERIFY(sphereModelRange.getInstanceMatrixCount() == 0);
            QVERIFY(sphereModelRange.getUserAddedMatrixCount() == 0);
            QVERIFY(sphereModelRange.getIndiceBegin() == 36); // Indices start after the cube's indices.
            QVERIFY(sphereModelRange.getIndiceCount() == 276);

            // Try adding some sphere instances.
            storedModels.addModelInstances(sphereModel.getModelFileName(), 20, false);
            storedModels.addModelInstances(sphereModel.getModelFileName(), 20, true);

            // Test cube.
            QVERIFY(cubeModelRange.getInstanceMatrixBegin() == 0);
            QVERIFY(cubeModelRange.getInstanceMatrixCount() == 20);
            QVERIFY(cubeModelRange.getUserAddedMatrixCount() == 10);
            QVERIFY(cubeModelRange.getIndiceBegin() == 0);
            QVERIFY(cubeModelRange.getIndiceCount() == 36);


            // Test sphere.
            QVERIFY(sphereModelRange.getInstanceMatrixBegin() == 20); // This changed- the sphere instances
                                                                                                // are stored after the cube (not user) instances!
            QVERIFY(sphereModelRange.getInstanceMatrixCount() == 40);
            QVERIFY(sphereModelRange.getUserAddedMatrixCount() == 20); // This changed!
            QVERIFY(sphereModelRange.getIndiceBegin() == 36); // Indices start after the cube's indices.
            QVERIFY(sphereModelRange.getIndiceCount() == 276);
        }

        void TestStoredModels::testRemoveModel_NoUserInstances()
        {
            Model cubeModel{cubeModelTestFile};
            Model sphereModel{sphereModelTestFile};

            StoredModels storedModels;

            storedModels.addModel(cubeModel);
            storedModels.addModel(sphereModel);

            const ModelRange &cubeModelRange = storedModels.getModelRanges()[0];
            const ModelRange &sphereModelRange = storedModels.getModelRanges()[1];

            storedModels.addModelInstances(cubeModel.getModelFileName(), 10, false);
            storedModels.addModelInstances(sphereModel.getModelFileName(), 20, false);

            storedModels.removeModel(cubeModel.getModelFileName());

            QVERIFY(storedModels.getModelRanges().size() == 2);
            QVERIFY(storedModels.getModelRanges()[0].getModel().getModelFileName() == cubeModel.getModelFileName());

            // Test the stored cube- it should have the same values as when it was just created in terms of instance matrices.
            QVERIFY(cubeModelRange.getInstanceMatrixBegin() == 0);
            QVERIFY(cubeModelRange.getInstanceMatrixCount() == 0);
            QVERIFY(cubeModelRange.getUserAddedMatrixCount() == 0);
            QVERIFY(cubeModelRange.getIndiceBegin() == 0);
            QVERIFY(cubeModelRange.getIndiceCount() == 36);

            // Test the stored sphere- only the location indicating the start of its instance matrices should change.
            QVERIFY(sphereModelRange.getInstanceMatrixBegin() == 0);
            QVERIFY(sphereModelRange.getInstanceMatrixCount() == 20);
            QVERIFY(sphereModelRange.getUserAddedMatrixCount() == 0);
            QVERIFY(sphereModelRange.getIndiceBegin() == 36);
            QVERIFY(sphereModelRange.getIndiceCount() == 276);

        }

        void TestStoredModels::testRemoveModelUserInstances()
        {
            Model cubeModel{cubeModelTestFile};
            Model sphereModel{sphereModelTestFile};

            StoredModels storedModels;

            storedModels.addModel(cubeModel);
            storedModels.addModel(sphereModel);

            const ModelRange &cubeModelRange = storedModels.getModelRanges()[0];
            const ModelRange &sphereModelRange = storedModels.getModelRanges()[1];

            storedModels.addModelInstances(cubeModel.getModelFileName(), 10, true);
            storedModels.addModelInstances(sphereModel.getModelFileName(), 20, true);

            storedModels.removeModel(cubeModel.getModelFileName());

            QVERIFY(storedModels.getModelRanges().size() == 2);
            QVERIFY(storedModels.getModelRanges()[0].getModel().getModelFileName() == cubeModel.getModelFileName());

            // Test the stored cube- it should have the same values as when it was just created in terms of instance matrices.
            QVERIFY(cubeModelRange.getInstanceMatrixBegin() == 0);
            QVERIFY(cubeModelRange.getInstanceMatrixCount() == 0);
            QVERIFY(cubeModelRange.getUserAddedMatrixCount() == 0);
            QVERIFY(cubeModelRange.getIndiceBegin() == 0);
            QVERIFY(cubeModelRange.getIndiceCount() == 36);

            // Test the stored sphere- only the location indicating the start of its instance matrices should change.
            QVERIFY(sphereModelRange.getInstanceMatrixBegin() == 0);
            QVERIFY(sphereModelRange.getInstanceMatrixCount() == 20);
            QVERIFY(sphereModelRange.getUserAddedMatrixCount() == 20);
            QVERIFY(sphereModelRange.getIndiceBegin() == 36);
            QVERIFY(sphereModelRange.getIndiceCount() == 276);
        }

        void TestStoredModels::testRemoveSpecificInstance_NotUser()
        {
            Model cubeModel{cubeModelTestFile};
            Model sphereModel{sphereModelTestFile};

            StoredModels storedModels;

            storedModels.addModel(cubeModel);
            storedModels.addModel(sphereModel);

            const ModelRange &cubeModelRange = storedModels.getModelRanges()[0];
            const ModelRange &sphereModelRange = storedModels.getModelRanges()[1];

            storedModels.addModelInstances(cubeModel.getModelFileName(), 10, false);
            storedModels.addModelInstances(sphereModel.getModelFileName(), 20, false);

            storedModels.removeInstance(0); // This is one of the cube's instances.

            QVERIFY(storedModels.getModelRanges().size() == 2);
            QVERIFY(storedModels.getModelRanges()[0].getModel().getModelFileName() == cubeModel.getModelFileName());

            // Test the stored cube- it should have the same values as when it was just created in terms of instance matrices.
            QVERIFY(cubeModelRange.getInstanceMatrixBegin() == 0);
            QVERIFY(cubeModelRange.getInstanceMatrixCount() == 9); // There are only 9 instances now!
            QVERIFY(cubeModelRange.getUserAddedMatrixCount() == 0);
            QVERIFY(cubeModelRange.getIndiceBegin() == 0);
            QVERIFY(cubeModelRange.getIndiceCount() == 36);

            // Test the stored sphere- only the location indicating the start of its instance matrices should change.
            QVERIFY(sphereModelRange.getInstanceMatrixBegin() == 9); // The offset of the sphere's instances
                                                                                              // is also reduced by 1.
            QVERIFY(sphereModelRange.getInstanceMatrixCount() == 20);
            QVERIFY(sphereModelRange.getUserAddedMatrixCount() == 0);
            QVERIFY(sphereModelRange.getIndiceBegin() == 36);
            QVERIFY(sphereModelRange.getIndiceCount() == 276);
        }

        void TestStoredModels::testRemoveSpecificInstance_User()
        {
            Model cubeModel{cubeModelTestFile};
            Model sphereModel{sphereModelTestFile};

            StoredModels storedModels;

            storedModels.addModel(cubeModel);
            storedModels.addModel(sphereModel);

            const ModelRange &cubeModelRange = storedModels.getModelRanges()[0];
            const ModelRange &sphereModelRange = storedModels.getModelRanges()[1];

            storedModels.addModelInstances(cubeModel.getModelFileName(), 10, true);
            storedModels.addModelInstances(sphereModel.getModelFileName(), 20, true);

            storedModels.removeInstance(11); // This is one of the cube's instances.

            QVERIFY(storedModels.getModelRanges().size() == 2);
            QVERIFY(storedModels.getModelRanges()[0].getModel().getModelFileName() == cubeModel.getModelFileName());

            // Test the stored cube- it should have the same values as when it was just created in terms of instance matrices.
            QVERIFY(cubeModelRange.getInstanceMatrixBegin() == 0);
            QVERIFY(cubeModelRange.getInstanceMatrixCount() == 10);
            QVERIFY(cubeModelRange.getUserAddedMatrixCount() == 10);
            QVERIFY(cubeModelRange.getIndiceBegin() == 0);
            QVERIFY(cubeModelRange.getIndiceCount() == 36);

            // Test the stored sphere- only the location indicating the start of its instance matrices should change.
            QVERIFY(storedModels.getModelRanges()[1].getInstanceMatrixBegin() == 10); // The offset of the sphere's instances
                                                                                               // is not affected as the cube's instances are not removed.
            QVERIFY(sphereModelRange.getInstanceMatrixCount() == 19); // This value has changed by 1.
            QVERIFY(sphereModelRange.getUserAddedMatrixCount() == 19); // This value has changed by 1.
            QVERIFY(sphereModelRange.getIndiceBegin() == 36);
            QVERIFY(sphereModelRange.getIndiceCount() == 276);
        }

        void TestStoredModels::testGetModelName()
        {
            Model cubeModel{cubeModelTestFile};
            Model sphereModel{sphereModelTestFile};

            StoredModels storedModels;

            storedModels.addModel(cubeModel);
            storedModels.addModel(sphereModel);

            storedModels.addModelInstances(cubeModel.getModelFileName(), 10, true);
            storedModels.addModelInstances(sphereModel.getModelFileName(), 20, true);

            QVERIFY(storedModels.getModelName(0) == cubeModel.getModelFileName());
            QVERIFY(storedModels.getModelName(11) == sphereModel.getModelFileName());
        }

        QString TestStoredModels::getAssetFolderLocation() const
        {
            auto currentDirectory = QDir::current();

            if(!currentDirectory.cdUp())
            {
                throw std::runtime_error{"Unable to cd-up from the executable folder!"};
            }

            if(!currentDirectory.cd("Assets"))
            {
                throw std::runtime_error{"Unable to find the \"Assets\" folder!!"};
            }

            return currentDirectory.path();
        }
    }
}
