//
// Created by binybrion on 5/7/20.
//

#include <ext.hpp>
#include "Interpreter.h"
#include "../Execution/Executor.h"
#include <math.h>

namespace L_System
{
    namespace Interpretation
    {
        std::vector<std::vector<ModelInstancePlacement>> Interpreter::modelInstances;
        std::vector<Execution::Token> Interpreter::tokenStack;

        glm::mat4 Interpreter::transformationMatrix;

        std::vector<UnbalancedError> Interpreter::unbalancedErrors;
        std::vector<UnderflowError> Interpreter::underflowErrors;

        const std::vector<std::vector<ModelInstancePlacement>> &Interpreter::getResult()
        {
            return modelInstances;
        }

        const std::vector<UnbalancedError> &Interpreter::getUnBalancedErrors()
        {
            return unbalancedErrors;
        }

        const std::vector<UnderflowError> &Interpreter::getUnderFlowErrors()
        {
            return underflowErrors;
        }

        void Interpreter::interpret()
        {
            runInterpretation(Execution::Executor::getRecursionResult());
        }

        void Interpreter::interpretExistingResult(const std::vector<std::vector<Execution::Token>> &previousExecutionResult)
        {
            runInterpretation(previousExecutionResult);
        }

        void Interpreter::clearPreviousResult()
        {
            modelInstances.clear();

            unbalancedErrors.clear();

            underflowErrors.clear();

            transformationMatrix = glm::mat4{1.0f};
        }

        void Interpreter::calculateTransformation()
        {
            // Go through all of the tokens in the stack and apply their transformations.
            for(const auto &token : tokenStack)
            {
                if(token.getConstant().getTranslation().isEnabled())
                {
                    transformationMatrix = glm::translate(transformationMatrix, token.getConstant().getTranslation().getTranslation());
                }
                else if(token.getConstant().getRotation().isEnabled())
                {
                    transformationMatrix = glm::rotate(transformationMatrix,
                                                       glm::radians(token.getConstant().getRotation().getAngle()),
                                                       token.getConstant().getRotation().getRotation());
                }
                else
                {
                    // A constant can only be a translation or a rotation. It is an error if this branch is taken.
                    assert(false);
                }
            }
        }

        void Interpreter::handleToken(const Execution::Token &token, unsigned int depthLevel)
        {
            switch(token.getConstant().getStackOperation())
            {
                case DataStructures::StackOperation::Pop:

                    if(tokenStack.empty())
                    {
                        underflowErrors.push_back(UnderflowError{depthLevel, token});

                        // Temporarily push this token so that the calculation function applies this constant's
                        // transformation.
                        tokenStack.push_back(token);

                        calculateTransformation();

                        tokenStack.pop_back();
                    }
                    else
                    {
                        tokenStack.pop_back();

                        tokenStack.push_back(token);

                        calculateTransformation();

                        // Pop the preceding token in the stack, to match the requested stack operation of this constant.
                        tokenStack.pop_back();
                    }

                    break;

                case DataStructures::StackOperation::Push:
                    tokenStack.push_back(token);
                    calculateTransformation();
                    break;
            }
        }

        void Interpreter::runInterpretation(const std::vector<std::vector<Execution::Token>> &executionResult)
        {
            clearPreviousResult();

            unsigned int currentDepthLevel = 0;

            for(const auto &depthResult : executionResult)
            {
                // Add a list of instance matrices for this current depth level
                modelInstances.emplace_back();

                for(const auto &token : depthResult)
                {
                    // If the token is a constant, modify the token stack and calculate the new transformation matrix
                    // for the next encountered variable
                    if(token.isConstant())
                    {
                        // The matrix needs to be reset in order to prevent a previous constant transformation
                        // from compounding with this constant's transformation more than once
                        transformationMatrix = glm::mat4x4{1.0};

                        handleToken(token, currentDepthLevel);
                    }
                    else if(token.isVariable())
                    {
                        // Add the associated model with the most transformation matrix, which represents all of the transformations
                        // preceding th variable.
                        modelInstances.back().push_back(ModelInstancePlacement{token.getVariable().getVariableName(), token.getVariable().getAssociatedModelName(), transformationMatrix});
                    }
                    else
                    {
                        // A token is either a constant or a variable- this branch should never be reached.
                        assert(false);
                    }
                }

                if(!tokenStack.empty())
                {
                    unbalancedErrors.push_back(UnbalancedError{currentDepthLevel, {tokenStack.begin(), tokenStack.end()}});
                }

                // Clear everything for the next depth level.
                tokenStack.clear();

                transformationMatrix = glm::mat4x4{1.0};

                currentDepthLevel += 1;
            }
        }
    }
}