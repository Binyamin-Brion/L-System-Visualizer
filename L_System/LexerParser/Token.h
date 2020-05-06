//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_TOKEN_H
#define VOXEL_L_SYSTEM_TOKEN_H

#include <QtCore/QString>

namespace L_System
{
    namespace LexerParser
    {
        struct Token
        {
            QString startRule;
            std::pair<bool, QString> probability;
            QString endRule;
        };
    }
}

#endif //VOXEL_L_SYSTEM_TOKEN_H
