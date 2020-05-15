//
// Created by binybrion on 5/2/20.
//

#include "ScriptInfoTabWidget.h"
#include "Constants/ConstantTabContent.h"
#include "Variables/VariableTabContent.h"
#include "Rules/RuleTabContent.h"
#include "../../L_System/ScriptInput.h"
#include "ProjectSaverLoader/ProjectDetails.h"
#include "HelperFunctions/RuleLoading.h"

namespace GUI
{
    namespace L_System
    {
        ScriptInfoTabWidget::ScriptInfoTabWidget(QWidget *parent)
                                :
                                    QTabWidget{parent},
                                    constantTabContent{new ConstantTabContent{this}},
                                    ruleTabContent{new RuleTabContent{this}},
                                    variableTabContent{new VariableTabContent{this}}
        {

            addTab(variableTabContent, "Variables");
            addTab(constantTabContent, "Constants");
            addTab(ruleTabContent, "Rules");

            setupConnections();
        }

        void ScriptInfoTabWidget::addFavouriteScript(const QString &saveResultName, const std::vector<std::vector<::L_System::Execution::Token>> &executionResult)
        {
            favouriteResults.push_back(::ProjectSaverLoader::FavouriteResult{saveResultName, executionResult, unsavedUserDefinedInstances});
        }

        const ::ProjectSaverLoader::FavouriteResult &ScriptInfoTabWidget::getFavouriteScript(const QString &saveResultName) const
        {
            auto saveResultLocation = std::find_if(favouriteResults.begin(), favouriteResults.end(), [&](const ::ProjectSaverLoader::FavouriteResult &favouriteResult)
            {
                return favouriteResult.resultName == saveResultName;
            });

            return *saveResultLocation;
        }

        void ScriptInfoTabWidget::loadProject(const ::ProjectSaverLoader::ScriptInformation &scriptInformation)
        {
            // Start at a clean state.
            loadedModels.clear();

            scriptConstants.clear();

            scriptRules.clear();

            scriptVariables.clear();

            favouriteResults.clear();

            variableTabContent->clearExistingModelEntries();

            // Upload all of the associated models, taking care not to upload a model more than once (if more than one
            // variable refers to the same model). Doing so is a waste of resources and could (potentially) break the program integrity.
            for(const auto &variable : scriptInformation.variables)
            {
                if(!modelAlreadyLoaded(variable.getAssociatedModelName()))
                {
                    variableTabContent->loadNewModel(variable.getAssociatedModelName());
                }
            }

            // Create variable and constant entries.
            variableTabContent->loadVariables(scriptInformation.axiom, scriptInformation.variables);

            constantTabContent->loadEntries(scriptInformation.constants);

            // Gather all of the constant names and the variable names so that the rule tab can have the list of
            // of these names available before creating rule entries, which is a requirement when loading rule entries.
            std::vector<QString> constantNames;

            std::vector<QString> variableNames;

            for(const auto &i : scriptInformation.constants)
            {
                constantNames.push_back(i.getConstantName());
            }

            for(const auto &i : scriptInformation.variables)
            {
                variableNames.push_back(i.getVariableName());
            }

            ruleTabContent->updateAvailableConstantEntries(constantNames);

            ruleTabContent->updateAvailableVariableEntries(variableNames);

            ruleTabContent->loadRules(scriptInformation.rules);

            favouriteResults.insert(favouriteResults.end(), scriptInformation.favouriteResults.begin(), scriptInformation.favouriteResults.end());
        }

        void ScriptInfoTabWidget::removeFavouriteScript(const QString &saveResultName)
        {
            auto saveResultLocation = std::find_if(favouriteResults.begin(), favouriteResults.end(), [&](const ::ProjectSaverLoader::FavouriteResult &favouriteResult)
            {
                return favouriteResult.resultName == saveResultName;
            });

            favouriteResults.erase(saveResultLocation);
        }

        void ScriptInfoTabWidget::saveFavouriteScript(const QString &favouriteResultName)
        {
            auto saveResultLocation = std::find_if(favouriteResults.begin(), favouriteResults.end(), [&](const ::ProjectSaverLoader::FavouriteResult &favouriteResult)
            {
                return favouriteResult.resultName == favouriteResultName;
            });

            // The unsaved user added instances are now part of the current favourite result. Thus when the project is saved,
            // these instances will be saved as well.
            saveResultLocation->userDefinedInstances = unsavedUserDefinedInstances;
        }

        void ScriptInfoTabWidget::saveProject(const QString &scriptName, ::ProjectSaverLoader::ProjectDetails &projectDetails)
        {
            accumulateScriptInformation();

            projectDetails.addScriptInformation(scriptName,
                                                scriptAxiom,
                                                scriptConstants,
                                                scriptRules,
                                                scriptVariables,
                                                favouriteResults);
        }

        void ScriptInfoTabWidget::setScriptInput()
        {
            accumulateScriptInformation();

            // Remember: for the execution of the script, only the axiom and the rules have to be given to the executor!
            // The constants and variables are indirectly stored in the uploaded rules.

            ::L_System::ScriptInput::setAxiom(scriptAxiom);

            ::L_System::ScriptInput::setRules(scriptRules);
        }

        void ScriptInfoTabWidget::setModelInstances(const std::vector<::ProjectSaverLoader::UserDefinedInstances> &matrices)
        {
            unsavedUserDefinedInstances = matrices;
        }

        // Beginning of private functions

        void ScriptInfoTabWidget::accumulateScriptInformation()
        {
            if(!variableTabContent->hasValidAxiom())
            {
                throw std::runtime_error{"No valid axiom has been selected\n"};
            }

            // Remove the previous accumulation.
            scriptConstants.clear();
            scriptRules.clear();
            scriptVariables.clear();

            scriptAxiom = variableTabContent->getAxiom();

            // Copies of these structures are needed to generate the rules (using the required data structures)
            // for the scripts without constantly regenerating these temporarily data structures
            scriptVariables = variableTabContent->getVariablesTokens();

            scriptConstants = constantTabContent->getConstantsTokens();

            // Generate the rules composed of the Execution data structures
            for(const auto &i : ruleTabContent->getRuleInformations())
            {
                // If a rule has a probability of 0, it will never execute and thus should not be included in the set of rules.
                if(i.probability == 0)
                {
                    continue;
                }

                std::vector<::L_System::Execution::Token> predecessorTokens;

                for(const auto &production : i.productionNames)
                {
                    predecessorTokens.emplace_back(::HelperFunctions::findEquivalentSuccessorTokens(production, scriptConstants, scriptVariables));
                }

                scriptRules.emplace_back(::HelperFunctions::findEquivalentVariable(i.startingRuleName, scriptVariables), predecessorTokens, i.probability);
            }
        }

        bool ScriptInfoTabWidget::modelAlreadyLoaded(const QString &modelFileLocation) const
        {
            return std::find(loadedModels.begin(), loadedModels.end(), modelFileLocation) != loadedModels.end();
        }

        void ScriptInfoTabWidget::setupConnections()
        {
            connect(variableTabContent, &VariableTabContent::modelLoaded, [this](const ::ModelLoading::Model &model) { emit modelLoaded(model); });

            // Update the rules variable so that it is known the names of constants that can be used; update the variable (tab) so that it is known what variable names
            // cannot be used due to constants already having that name.
            connect(constantTabContent, SIGNAL(constantsChangedValidity(std::vector<QString>)), ruleTabContent, SLOT(updateAvailableConstantEntries(const std::vector<QString>&)));

            connect(constantTabContent, SIGNAL(constantsChangedValidity(std::vector<QString>)), variableTabContent, SLOT(setConstantNames(const std::vector<QString>&)));

            // Update the rules variable so that it is known the names of variables that can be used; update the constant (tab) so that it is known what constant names
            // cannot be used due to variables already having that name.
            connect(variableTabContent, SIGNAL(variablesChangedValidity(std::vector<QString>)), ruleTabContent, SLOT(updateAvailableVariableEntries(const std::vector<QString>&)));

            connect(variableTabContent, SIGNAL(variablesChangedValidity(std::vector<QString>)), constantTabContent, SLOT(setVariableNames(const std::vector<QString>&)));
        }
    }
}
