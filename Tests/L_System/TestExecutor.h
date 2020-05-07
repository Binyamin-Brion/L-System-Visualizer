//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_TESTEXECUTOR_H
#define VOXEL_L_SYSTEM_TESTEXECUTOR_H

#include "../TestSuite.h"

namespace L_System
{
    namespace Execution
    {
        class Executor;
    }
}

namespace Tests
{
    namespace L_System
    {
        class TestExecutor : public TestSuite
        {
            Q_OBJECT

            public:
                explicit TestExecutor(const QString testClassName);

            private slots:

                void testFractalBinaryTree();

            private:
                std::vector<QString> getRecursionResults(const ::L_System::Execution::Executor &executor);
        };
    }
}

#endif //VOXEL_L_SYSTEM_TESTEXECUTOR_H
