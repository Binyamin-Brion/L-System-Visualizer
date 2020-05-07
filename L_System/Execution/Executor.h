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
        /**
         * Executes a L-Script, expanding the output to the specified depth level.
         */

        class Executor
        {
            public:

                /**
                 * Executes the script to the passed in depth level.
                 *
                 * Both axiom and the rules must have been set using the appropriate functions for any valid result.
                 *
                 * Note: the previous execution result is discarded (if there is one) when calling this function.
                 *
                 * @param recursionDepth depth to which to execute the script
                 */
                static void execute(unsigned int recursionDepth);

                /**
                 * Get the result of running the script.
                 *
                 * @return script result
                 */
                static const std::vector<std::vector<Token>>& getRecursionResult();

                /**
                 * Set the axiom of the script. This represents level 0 (depth) of the result.
                 *
                 * @param axiom script's axiom
                 */
                static void setAxiom(const DataStructures::Variable &axiom);

                /**
                 * Sets the rules of the script. These are used to expand the script's output to the desired depth level.
                 *
                 * @param rules vector of Rules to expand the script's output
                 */
                static void setRules(const std::vector<DataStructures::Rule> &rules);

            private:

                // This is set in setAxiom(), but a copy needs to be stored so that the execute()
                // function can reset the object to a default state to rerun the script.
                static Token startingVariable;
                static std::vector<DataStructures::Rule> executionRules;
                static std::vector<std::vector<Token>> recursionResult;
        };
    }
}

#endif //VOXEL_L_SYSTEM_EXECUTOR_H
