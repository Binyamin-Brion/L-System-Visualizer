//
// Created by binybrion on 5/5/20.
//

#ifndef VOXEL_L_SYSTEM_VARIABLE_H
#define VOXEL_L_SYSTEM_VARIABLE_H

#include <QtCore/QString>

namespace L_System
{
    namespace DataStructures
    {
        struct Variable
        {
            QString variableName;
            QString associatedModel;
        };
    }
}

#endif //VOXEL_L_SYSTEM_VARIABLE_H
