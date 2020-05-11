//
// Created by binybrion on 5/10/20.
//

#ifndef VOXEL_L_SYSTEM_TESTPROJECTSAVELOAD_H
#define VOXEL_L_SYSTEM_TESTPROJECTSAVELOAD_H

#include "../TestSuite.h"

namespace Tests
{
    namespace ProjectSaverLoader
    {
        /**
         *  Tests saving a script to a file and then reading it back, ensuring all script information
         *  is preserved.
         *
         * These tests are from examples on the respective Wikipedia page:
         *
         * https://en.wikipedia.org/wiki/L-system
         *
         * The code to create a script is taken from the tests that test the Script Interpreter, located
         * in TestInterpreter.
         */

        class TestProjectSaveLoad : public TestSuite
        {
            Q_OBJECT

            public:

                /**
                 *  Initializes this Test Class with the given name, allowing it to be controlled
                 *  when it is run based off of program arguments.
                 *
                 * @param testClassName name of the test class
                 */
                explicit TestProjectSaveLoad(const QString &testClassName);

            private slots:

                /**
                 * Tests saving the script from the Wikipedia algae example and then loading it back into memory.
                 */
                void testSaveLoadAlgae();

                /**
                 * Tests saving the script from the Wikipedia fractal tree example and then loading it back into memory.
                 */
                void testFractalTree();

                /**
                 * Tests saving the script from the Wikipedia Koch Curve example and then loading it back into memory.
                 */
                void testKochCurve();
        };
    }
}

#endif //VOXEL_L_SYSTEM_TESTPROJECTSAVELOAD_H
