//
// Created by binybrion on 5/8/20.
//

#include "ScriptOutputWidget.h"
#include <QVBoxLayout>
#include <L_System/Interpretation/Interpreter.h>
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
                displayScriptOutput(tokens, {}, {});
            }

            void ScriptOutputWidget::showScriptOutput()
            {
                displayScriptOutput(::L_System::Execution::Executor::getRecursionResult(), ::L_System::Interpretation::Interpreter::getUnBalancedErrors(), ::L_System::Interpretation::Interpreter::getUnderFlowErrors());
            }

            // Beginning of private functions


            void ScriptOutputWidget::addUnbalancedErrorMessage(unsigned int depthLevel, ScriptOutputEntry *scriptOutputEntry, const std::vector<::L_System::Interpretation::UnbalancedError> &unbalancedErrors)
            {
                QString errorMessage = " Unbalanced error with tokens: ";
                bool addErrorMessage = false; // There may be no unbalanced error at the given depth level; thus a variable to track
                                              // of whether to actually add an error message is required.

                // There may be more than one unbalanced error for a depth, as each unbalanced token is an error.
                // Thus the errors have to be iterated over to find all of the errors associated with the given depth.
                for(const auto &i : unbalancedErrors)
                {
                   if(i.depthLevel == depthLevel)
                   {
                       for(const auto &token : i.tokens)
                       {
                           addErrorMessage = true;

                           // In an unbalanced error, a token may be either a variable or a constant, and thus a check has to be done.
                           if(token.isConstant())
                           {
                               errorMessage += token.getConstant().getConstantName() + " , ";
                           }
                           else if(token.isVariable())
                           {
                               errorMessage += token.getVariable().getVariableName() + " , ";
                           }
                           else
                           {
                               assert(false);
                           }
                       }
                   }
                }

                if(addErrorMessage)
                {
                    scriptOutputEntry->setErrorMessage(errorMessage);
                }
            }

            void ScriptOutputWidget::addUnderflowErrorMessage(unsigned int depthLevel, ScriptOutputEntry *scriptOutputEntry, const std::vector<::L_System::Interpretation::UnderflowError> &underflowErrors)
            {
                QString errorMessage = " Underflow error with tokens: ";
                bool addErrorMessage = false;   // There may be no unbalanced error at the given depth level; thus a variable to track
                                                // of whether to actually add an error message is required.

                // There may be more than one underflow error for a depth, as each unbalanced token is an error.
                // Thus the errors have to be iterated over to find all of the errors associated with the given depth.
                for(const auto &i : underflowErrors)
                {
                    // An underflow error can only happen as a result of a token popping the stack, which can only be
                    // done with a constant. Thus the token associated with an underflow error has to be a constant.
                    if(i.depthLevel == depthLevel)
                    {
                        addErrorMessage = true;

                        errorMessage += i.token.getConstant().getConstantName() + " , ";
                    }
                }

                if(addErrorMessage)
                {
                    scriptOutputEntry->setErrorMessage(errorMessage);
                }
            }

            void ScriptOutputWidget::displayScriptOutput(const std::vector<std::vector<::L_System::Execution::Token>> &tokens,
                                                         const std::vector<::L_System::Interpretation::UnbalancedError> &unbalancedErrors,
                                                         const std::vector<::L_System::Interpretation::UnderflowError> &underflowError)
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

                    addUnbalancedErrorMessage(depthResult, entry, unbalancedErrors);

                    addUnderflowErrorMessage(depthResult, entry, underflowError);
                }
            }
        }
    }
}