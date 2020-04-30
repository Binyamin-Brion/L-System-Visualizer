//
// Created by binybrion on 4/30/20.
//

#include "GridVAO.h"

namespace Render
{
    namespace GridSystem
    {
        void GridVAO::initialize()
        {
            if(!initializeOpenGLFunctions())
            {
                throw std::runtime_error{"Unable to initialize OpenGL functions at: " + std::string{__PRETTY_FUNCTION__}};
            }

            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            const int MAX_ABSOLUTE_VALUE = 10'000;

            const float GRID_COLOUR = 0.2f;

            // To avoid many OpenGL calls uploading line information, all of the line information is stored in one vector
            // before being uploaded at the same time. Same idea with the colours for the lies.
            std::vector<glm::vec3> gridLines;
            std::vector<glm::vec3> gridLineColours;

            // To create a grid in the scene, lines parallel to the x-axis have to be created, along with lines parallel
            // to the z-axis. Lines parallel to the x-axis is done first.

            // The lines are incremented by one as that is the length of a one cube / voxel.

            for(int i = -MAX_ABSOLUTE_VALUE; i < MAX_ABSOLUTE_VALUE; ++i)
            {
                // Make sure that the x-axis is visible- ie not covered by another line.
                if(i == 0)
                {
                    continue;
                }

                gridLines.emplace_back(i, 0.0f, -MAX_ABSOLUTE_VALUE);
                gridLines.emplace_back(i, 0.0f, MAX_ABSOLUTE_VALUE);

                gridLineColours.emplace_back(GRID_COLOUR, GRID_COLOUR, GRID_COLOUR);
                gridLineColours.emplace_back(GRID_COLOUR, GRID_COLOUR, GRID_COLOUR);
            }

            //Same idea as when creating lines parallel to the x-axis.

            for(int i = -MAX_ABSOLUTE_VALUE; i < MAX_ABSOLUTE_VALUE; ++i)
            {
                if(i == 0)
                {
                    continue;
                }

                gridLines.emplace_back(-MAX_ABSOLUTE_VALUE, 0.0f, i);
                gridLines.emplace_back(MAX_ABSOLUTE_VALUE, 0.0f, i);

                gridLineColours.emplace_back(GRID_COLOUR, GRID_COLOUR, GRID_COLOUR);
                gridLineColours.emplace_back(GRID_COLOUR, GRID_COLOUR, GRID_COLOUR);
            }

            // Upload the required rendering data into buffers.

            lineVBOs.initialize();
            lineVBOs.uploadData(gridLines);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            lineColours.initialize();
            lineColours.uploadData(gridLineColours);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glEnableVertexAttribArray(1);

            // Keep track of how many vertices to render to ensure that all grid lines are required.

            numberLinePoints += gridLines.size();
        }

        void GridVAO::render()
        {
            glBindVertexArray(vao);

            glDrawArrays(GL_LINES, 0, numberLinePoints);
        }
    }
}