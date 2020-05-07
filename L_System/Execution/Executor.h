//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_EXECUTOR_H
#define VOXEL_L_SYSTEM_EXECUTOR_H

#include <QString>
#include <vector>

#include "L_System/DataStructures/Constants/Constant.h"
#include "../DataStructures/Variable.h"
#include "Token.h"

namespace L_System
{
    namespace Execution
    {
        class Executor
        {
            public:

                void execute(unsigned int recursionDepth);
                const std::vector<std::vector<Token>>& getRecursionResult() const;
                void setAxiom(const DataStructures::Variable &axiom);
                void setRules(const std::vector<DataStructures::Rule> &rules);

            private:

                void executeRecursively(unsigned int timesToExecute);

                unsigned int numberTimesExecuted = 0;

                std::vector<DataStructures::Rule> rules;
                std::vector<std::vector<Token>> recursionResult;
        };
    }
}

#endif //VOXEL_L_SYSTEM_EXECUTOR_H
