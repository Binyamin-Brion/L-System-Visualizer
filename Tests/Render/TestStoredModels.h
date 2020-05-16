//
// Created by binybrion on 5/16/20.
//

#ifndef VOXEL_L_SYSTEM_TESTSTOREDMODELS_H
#define VOXEL_L_SYSTEM_TESTSTOREDMODELS_H

#include "Tests/TestSuite.h"

namespace Tests
{
    namespace Render
    {
        /**
         * Tests that the StoredModel data structure correctly handles additions and removals of both models and those
         * models' instances.
         */

        class TestStoredModels : public TestSuite
        {
            Q_OBJECT

            public:

                /**
                 *  Initializes this Test Class with the given name, allowing it to be controlled
                 *  when it is run based off of program arguments.
                 *
                 * @param testClassName name of the test class
                 */
                explicit TestStoredModels(const QString &testClassName);

            private slots:

                /**
                 * Tests if two models are correctly added to the StoredModels.
                 */
                void testAddModel();

                /**
                 * Tests if script generated instances of two models are correctly added to the StoredModels.
                 */
                void testAddModelInstances();

                /**
                 * Tests if instances of user-added instances are correctly added to the StoredModels.
                 */
                void testAddModelUserInstances();

                /**
                 * Tests if both script-generated instances and user-added instances are added to the StoredModels.
                 */
                void testAddModel_UserInstances();

                /**
                 * Tests the removals of all script generated instances of models from StoredModels.
                 */
                void testRemoveModel_NoUserInstances();

                /**
                 * Tests the removal of all user-added instances of models from StoredModels.
                 */
                void testRemoveModelUserInstances();

                /**
                 * Tests the removal of a specific instance of a script generated instance frm StoredModels.
                 */
                void testRemoveSpecificInstance_NotUser();

                /**
                 * Tests the removal of a specific instance of a user-added instance from StoredModels.
                 */
                void testRemoveSpecificInstance_User();

                void testGetModelName();

            private:

                /**
                 * Gets the location of the assets folder, holding the model file that are used in the above tests.
                 *
                 * @return location to the asset folder
                 */
                QString getAssetFolderLocation() const;

                std::string cubeModelTestFile;
                std::string sphereModelTestFile;
        };
    }
}

#endif //VOXEL_L_SYSTEM_TESTSTOREDMODELS_H
