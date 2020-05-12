//
// Created by binybrion on 5/9/20.
//

#include "ProjectSaver.h"
#include "ProjectDetails.h"

namespace ProjectSaverLoader
{
    void ProjectSaver::saveProject(const QString &saveLocation, const ProjectDetails &projectDetails)
    {
        file.setFileName(saveLocation);

        if(!file.open(QIODevice::WriteOnly))
        {
            throw std::runtime_error{"Unable to create a file at the location for saving: " + saveLocation.toStdString()};
        }

        for(const auto &i : projectDetails.getScripts())
        {
            QTextStream writeStream{&file};

            // Mark the beginning of a new script. An explicit flush is required to ensure that this
            // text is written to the file before the script contents.
            writeStream << "BEGIN SCRIPT\n\n";

            writeStream.flush();

            saveName(i.scriptName);
            // Write the variables BEFORE the axiom. This is required due to how the project file is loaded.
            saveVariables(i.variables);
            saveAxiom(i.axiom);
            saveConstants(i.constants);
            saveRules(i.rules);
            saveRequestedScriptOutputs(i.favouriteResults);

            // Mark the end of the read in script. The explicit flush is required to ensure that this text
            // is written to the file before the script contents.
            writeStream << "END SCRIPT\n\n";

            writeStream.flush();
        }

        file.close();
    }

    // Beginning of private functions

    QString ProjectSaver::convertStackOperationString(::L_System::DataStructures::StackOperation stackOperation)
    {
        switch(stackOperation)
        {
            case ::L_System::DataStructures::StackOperation::Pop:
                return QString{"Pop"};

            case ::L_System::DataStructures::StackOperation::Push:
                return QString{"Push"};
        }
    }

    QString ProjectSaver::convertVectorString(const glm::vec3 &vector)
    {
        return QString::number(vector.x) + " , " + QString::number(vector.y) + " , " + QString::number(vector.z);
    }

    void ProjectSaver::saveAxiom(const ::L_System::DataStructures::Variable &axiom)
    {
        QTextStream writeStream{&file};

        writeStream << "Axiom -- " << axiom.getVariableName() << "\n\n";
    }

    void ProjectSaver::saveConstants(const std::vector<::L_System::DataStructures::Constant> &constants)
    {
        QTextStream writeStream{&file};

        writeStream << "Constants: \n\n";

        for(const auto &i : constants)
        {
            writeStream << i.getConstantName() << " -- " << convertStackOperationString(i.getStackOperation()) << " -- ";

            if(i.getRotation().isEnabled())
            {
                writeStream << "Rotation -- " + convertVectorString(i.getRotation().getRotation()) + " -- Angle -- " + QString::number(i.getRotation().getAngle()) + '\n';
            }

            if(i.getTranslation().isEnabled())
            {
                writeStream << "Translation -- " + convertVectorString(i.getTranslation().getTranslation()) + '\n';
            }

            writeStream << '\n';
        }
    }

    void ProjectSaver::saveName(const QString &scriptName)
    {
        QTextStream writeStream{&file};

        writeStream << "Script_name -- " << scriptName << "\n\n";
    }

    void ProjectSaver::saveRules(const std::vector<::L_System::DataStructures::Rule> &rules)
    {
        QTextStream writeStream{&file};

        writeStream << "Rules: \n\n";

        // Since a rule cannot be on one line due to potentially many successor tokens, a text (END RULE)
        // is required to make sure that there is a known boundary between subsequent rules when reading the project file.

        for(const auto &i : rules)
        {
            writeStream << "Predecessor_name -- " << i.getPredecessor().getVariableName() + '\n';

            for(const auto &j : i.getSuccessorTokens())
            {
                if(j.isConstant())
                {
                    writeStream << "Successor_token -- Constant -- " << j.getConstant().getConstantName() + '\n';
                }

                if(j.isVariable())
                {
                    writeStream << "Successor_token -- Variable -- " << j.getVariable().getVariableName() + '\n';
                }
            }

            writeStream << "\nEND RULE\n\n";
        }
    }

    void ProjectSaver::saveRequestedScriptOutputs(const std::vector<FavouriteResult> &favouriteResults)
    {
        QTextStream writeStream{&file};

        writeStream << "Favourite_results: \n\n";

        for(const auto &i : favouriteResults)
        {
            writeStream << "Result_name -- " << i.resultName + "\n\n";

            for(unsigned int j = 0; j < i.executionResult.size(); ++j)
            {
                writeStream << "Recursion_depth -- " << j << + "\n\n";

                for(const auto &k : i.executionResult[j])
                {
                    if(k.isConstant())
                    {
                        writeStream << "Successor_token -- (Constant) -- " << k.getConstant().getConstantName() + '\n';
                    }

                    if(k.isVariable())
                    {
                        writeStream << "Successor_token -- (Variable) -- " << k.getVariable().getVariableName() + '\n';
                    }
                }

                writeStream << '\n';
            }

            writeStream << "\nEND FAVOURITE RESULT\n\n";
        }
    }

    void ProjectSaver::saveVariables(const std::vector<::L_System::DataStructures::Variable> &variables)
    {
        QTextStream writeStream{&file};

        writeStream << '\n';

        writeStream << "Variables: \n\n";

        for(const auto &i : variables)
        {
            writeStream << "Variable_name -- " << i.getVariableName() + " -- Associated_model_name: -- "  + i.getAssociatedModelName() + '\n';
        }

        writeStream << '\n';
    }
}