//
// Created by binybrion on 5/8/20.
//

#ifndef VOXEL_L_SYSTEM_RULEINFORMATION_H
#define VOXEL_L_SYSTEM_RULEINFORMATION_H

#include <QtCore/QString>
#include <vector>

namespace GUI
{
    namespace L_System
    {
        /**
         * Stores the required information to create a Rule Data structure. See ScriptInfoTabWidget.cpp for example usage.
         */

        struct RuleInformation
        {
            QString startingRuleName;
            std::vector<QString> productionNames;
            unsigned int probability;
        };
    }
}

#endif //VOXEL_L_SYSTEM_RULEINFORMATION_H
