//
// Created by binybrion on 5/8/20.
//

#include "ScriptOutputWidget.h"
#include <QVBoxLayout>
#include "../../../L_System/Execution/Executor.h"
#include "ScriptOutputEntry.h"

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

            // Beginning of public slots

            void ScriptOutputWidget::showScriptOutput()
            {
                removePreviousResult();

                for(unsigned int depthResult = 0; depthResult < ::L_System::Execution::Executor::getRecursionResult().size(); ++depthResult)
                {
                    ScriptOutputEntry *entry = new ScriptOutputEntry{this};

                    entries.push_back(entry);

                    entry->setDepthResult(depthResult);

                    layout->addWidget(entry);

                    for(const auto &variableResult : ::L_System::Execution::Executor::getRecursionResult()[depthResult])
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

            // Beginning of private functions

            void ScriptOutputWidget::removePreviousResult()
            {
                for(auto &i : entries)
                {
                    layout->removeWidget(i);

                    delete i;
                }

                entries.clear();
            }
        }
    }
}