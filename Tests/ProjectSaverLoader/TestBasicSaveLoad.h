//
// Created by binybrion on 5/11/20.
//

#ifndef VOXEL_L_SYSTEM_TESTBASICSAVELOAD_H
#define VOXEL_L_SYSTEM_TESTBASICSAVELOAD_H

#include <Tests/TestSuite.h>

namespace Tests
{
    namespace ProjectSaverLoader
    {
        /**
         *  Tests saving and loading parts of a script (such as only bookmarked favourite results),
         *  rather than testing saving and loading all parts of a script such as the tests in TestProjectSaveLoad.
         */

        class TestBasicSaveLoad : public TestSuite
        {
                Q_OBJECT

            public:

                /**
                 *  Initializes this Test Class with the given name, allowing it to be controlled
                 *  when it is run based off of program arguments.
                 *
                 * @param testClassName name of the test class
                 */
                explicit TestBasicSaveLoad(const QString &testClassName);

            private slots:

                /**
                 * Tests saving and loading only the favourite (bookmarked) results of a script.
                 */
                void testSaveFavouriteResult();
        };
    }
}

#endif //VOXEL_L_SYSTEM_TESTBASICSAVELOAD_H
