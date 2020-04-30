//
// Created by binybrion on 4/30/20.
//

#include "AxisVAO.h"

namespace Render
{
    namespace GridSystem
    {
        void AxisVAO::initialize()
        {
            if(!initializeOpenGLFunctions())
            {
                throw std::runtime_error{"Unable to initialize OpenGL functions at: " + std::string{__PRETTY_FUNCTION__}};
            }

            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            const float MAX_ABSOLUTE_VALUE = 10'000;
            const float MAX_COLOUR_VALUE = 1.0f;
            const float MIN_COLOUR_VALUE = 0.4;

            // Store the positions of the axis. Each axis is divided into two lines- a negative portion and a positive portion-
            // this allows the axis to have a dark colour indicating the negative portion and a lighter colour for the positive portion.

            const std::vector<glm::vec3> xAxis
            {
                // Negative portion
                glm::vec3{-MAX_ABSOLUTE_VALUE, 0.f, 0.f},
                glm::vec3{0, 0.f, 0.f},

                // Positive portion
                glm::vec3{0, 0.f, 0.f},
                glm::vec3{MAX_ABSOLUTE_VALUE, 0.f, 0.f},
            };

            const std::vector<glm::vec3> yAxis
            {
                // Negative portion
                glm::vec3{0.f, -MAX_ABSOLUTE_VALUE, 0.f},
                glm::vec3{0, 0, 0.f},

                // Positive portion
                glm::vec3{0, 0, 0.f},
                glm::vec3{0.f, MAX_ABSOLUTE_VALUE, 0.f},
            };

            const std::vector<glm::vec3> zAxis
            {
                // Negative portion
                glm::vec3{0, 0.f, -MAX_ABSOLUTE_VALUE},
                glm::vec3{0, 0.f, 0},

                // Positive portion
                glm::vec3{0, 0.f, 0},
                glm::vec3{0, 0.f, MAX_ABSOLUTE_VALUE}
            };

            // Store the colours of each vertex for the respective axis.

            const std::vector<glm::vec3> xAxisColour
            {
                glm::vec3{MIN_COLOUR_VALUE, 0.f, 0.f},
                glm::vec3{MIN_COLOUR_VALUE, 0.f, 0.f},

                glm::vec3{MAX_COLOUR_VALUE, 0.f, 0.f},
                glm::vec3{MAX_COLOUR_VALUE, 0.f, 0.f}
            };

            const std::vector<glm::vec3> yAxisColour
            {
                    glm::vec3{0.0f, MIN_COLOUR_VALUE, 0.f},
                    glm::vec3{0.0f, MIN_COLOUR_VALUE, 0.f},

                    glm::vec3{0.0f, MAX_COLOUR_VALUE, 0.f},
                    glm::vec3{0.0f, MAX_COLOUR_VALUE, 0.f}
            };

            const std::vector<glm::vec3> zAxisColour
            {
                    glm::vec3{0.0f, 0.f, MIN_COLOUR_VALUE},
                    glm::vec3{0.0f, 0.f, MIN_COLOUR_VALUE},

                    glm::vec3{0.0f, 0.f, MAX_COLOUR_VALUE},
                    glm::vec3{0.0f, 0.f, MAX_COLOUR_VALUE}
            };

            // Upload axis rendering information to buffers for rendering.

            axisVBO.initialize();
            axisVBO.uploadDataAppend(xAxis);
            axisVBO.uploadDataAppend(yAxis);
            axisVBO.uploadDataAppend(zAxis);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            axisColourVBO.initialize();
            axisColourVBO.uploadDataAppend(xAxisColour);
            axisColourVBO.uploadDataAppend(yAxisColour);
            axisColourVBO.uploadDataAppend(zAxisColour);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glEnableVertexAttribArray(1);

            // Keep track of how many vertices are required to be rendering to render all of the axis.

            numberLinePoints += xAxis.size();
            numberLinePoints += yAxis.size();
            numberLinePoints += zAxis.size();
        }

        void AxisVAO::render()
        {
            glBindVertexArray(vao);

            glDrawArrays(GL_LINES, 0, numberLinePoints);
        }
    }
}
