//
// Created by binybrion on 5/8/20.
//

#ifndef VOXEL_L_SYSTEM_SCRIPTINPUT_H
#define VOXEL_L_SYSTEM_SCRIPTINPUT_H

#include "L_System/Execution/Token.h"
#include "L_System/DataStructures/Rules/Rule.h"
#include <vector>

namespace L_System
{
    /**
     * Holds the required parameters to execute a L-Script.
     */

    class ScriptInput
    {
        public:

            /**
             * Get the starting axiom for the script.
             *
             * @return script starting axiom
             */
            static ::L_System::DataStructures::Variable getAxiom();

            /**
             * Get the rules by which to execute the script.
             *
             * @return script's rules
             */
            static const std::vector<::L_System::DataStructures::Rule>& getRules();

            /**
             * Set the starting axiom for the script.
             *
             * @param inputAxiom starting axiom
             */
            static void setAxiom(::L_System::DataStructures::Variable inputAxiom);

            /**
             * Set the rules for the scriot.
             *
             * @param inputRules script's rules
             */
            static void setRules(std::vector<::L_System::DataStructures::Rule> inputRules);

        private:

            static ::L_System::DataStructures::Variable axiom;;
            static std::vector<::L_System::DataStructures::Rule> rules;
    };
}


#endif //VOXEL_L_SYSTEM_SCRIPTINPUT_H
