//
// Created by binybrion on 5/8/20.
//

#include "ScriptOutputWidget.h"
#include <QVBoxLayout>
#include "../../../L_System/Execution/Executor.h"
#include "ScriptOutputEntry.h"
#include "L_System/Execution/Token.h"

namespace GUI
{
    namespace Output
    {
        namespace L_System
        {
            ScriptOutputWidget::ScriptOutputWidget(QWidget *parent)
                    :
                        QWidget{parent},
                        layout{new QVBoxLayout{this}}
            {
                setLayout(layout);


            }

            void ScriptOutputWidget::removePreviousResult()
            {
                for(auto &i : entries)
                {
                    layout->removeWidget(i);

                    delete i;
                }

                entries.clear();
            }

            // Beginning of public slots

            void ScriptOutputWidget::showSavedScriptOutput(const std::vector<std::vector<::L_System::Execution::Token>> &tokens)
            {
                displayScriptOutput(tokens);
            }

            void ScriptOutputWidget::showScriptOutput()
            {
                displayScriptOutput(::L_System::Execution::Executor::getRecursionResult());
            }

            // Beginning of private functions

            void ScriptOutputWidget::displayScriptOutput(const std::vector<std::vector<::L_System::Execution::Token>> &tokens)
            {
                removePreviousResult();

                for(unsigned int depthResult = 0; depthResult < tokens.size(); ++depthResult)
                {
                    // Create an entry for the current depth result.
                    ScriptOutputEntry *entry = new ScriptOutputEntry{this};

                    entries.push_back(entry);

                    entry->setDepthResult(depthResult);

                    layout->addWidget(entry);

                    // Show the (text equivalent) result of the current depth.
                    for(const auto &variableResult : tokens[depthResult])
                    {
                        if(variableResult.isVariable())
                        {
                            entry->appendResultText(variableResult.getVariable().getVariableName());
                        }
                        else if(variableResult.isConstant())
                        {
                            entry->appendResultText(variableResult.getConstant().getConstantName());
                        }
                        else
                        {
                            assert(false);
                        }
                    }
                }
            }
        }
    }
}