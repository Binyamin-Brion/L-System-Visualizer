cmake_minimum_required(VERSION 3.15)
project(Voxel_L_System)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# If compiling from scratch, the location to the Qt installation will have to be updated to match where it is installed
# on the machine being used for compilation

if(WIN32)
    set(CMAKE_PREFIX_PATH C:\\Qt\\Qt5.9.8\\5.9.8\\mingw53_32)
endif(WIN32)

if(UNIX)
    set(CMAKE_PREFIX_PATH /home/binybrion/Qt5.9.9/5.9.9/gcc_64)
endif(UNIX)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

include_directories(${Qt5Widgets_INCLUDES})

set(GLM_FOLDER /home/binybrion/ProjectSourceDependencies/glm-0.9.9.8/glm)
include_directories(${GLM_FOLDER})

find_package(ASSIMP REQUIRED)

include_directories(${ASSIMP_INCLUDE_DIR})

add_executable(Voxel_L_System main.cpp GUI/MainWindow.cpp GUI/MainWindow.h Render/VBO/VBOWapper.h Render/VAO/GridSystem/AxisVAO.cpp
        Render/VAO/GridSystem/AxisVAO.h Render/CommandCentre.cpp Render/CommandCentre.h Render/Shaders/ShaderManager.cpp
        Render/Shaders/ShaderManager.h GUI/OpenGL/GLWidget.cpp GUI/OpenGL/GLWidget.h Render/VAO/GridSystem/GridVAO.cpp
        Render/VAO/GridSystem/GridVAO.h Render/Camera/CameraMovement.h Render/Camera/CameraObject.cpp Render/Camera/CameraObject.h
        ModelLoading/Mesh.cpp ModelLoading/Mesh.h ModelLoading/Model.cpp
        ModelLoading/Model.h ModelLoading/Face.h Render/VAO/UserModels/ModelVAO.cpp Render/VAO/UserModels/ModelVAO.h ModelLoading/Face.cpp
        Render/DataStructures/RecursionTree.cpp Render/DataStructures/RecursionTree.h Render/DataStructures/RecursionModelRange.cpp
        Render/DataStructures/RecursionModelRange.h Render/DataStructures/StoredModels.cpp Render/DataStructures/StoredModels.h
        Render/DataStructures/ModelRange.h Render/DataStructures/ModelRange.cpp Render/DataStructures/RemoveModelRecursionResult.h
        GUI/L_System/ScriptInfoTabWidget.cpp GUI/L_System/ScriptInfoTabWidget.h GUI/L_System/Variables/VariablesWidget.cpp
        GUI/L_System/Variables/VariablesWidget.h GUI/L_System/Variables/VariableTabContent.cpp GUI/L_System/Variables/VariableTabContent.h
        GUI/L_System/Variables/VariableEntry.cpp GUI/L_System/Variables/VariableEntry.h GUI/L_System/Constants/ConstantTabContent.cpp
        GUI/L_System/Constants/ConstantTabContent.h GUI/L_System/Constants/ConstantsWidget.cpp GUI/L_System/Constants/ConstantsWidget.h
        GUI/L_System/Constants/ConstantEntryInformation.cpp GUI/L_System/Constants/ConstantEntryInformation.h
        GUI/L_System/Constants/ConstantEntryDeclaration.cpp GUI/L_System/Constants/ConstantEntryDeclaration.h
        GUI/L_System/Axiom/AxiomTabContent.cpp GUI/L_System/Axiom/AxiomTabContent.h L_System/DataStructures/Variable.h
        L_System/DataStructures/Constants/Constant.h L_System/DataStructures/Constants/StackOperation.h
        L_System/DataStructures/Constants/Rotation.h L_System/DataStructures/Constants/Translation.h GUI/L_System/Rules/RuleTabContent.cpp
        GUI/L_System/Rules/RuleTabContent.h  GUI/L_System/Rules/RuleWidget.cpp GUI/L_System/Rules/RuleWidget.h GUI/L_System/Rules/RuleEntry.cpp
        GUI/L_System/Rules/RuleEntry.h L_System/Execution/Executor.cpp L_System/Execution/Executor.h L_System/Execution/Token.h
        Tests/TestSuite.h Tests/TestSuite.cpp Tests/L_System/TestExecutor.cpp Tests/L_System/TestExecutor.h L_System/DataStructures/Rules/Rule.h L_System/DataStructures/Rules/SuccessorToken.h L_System/DataStructures/Rules/SuccessToken.cpp L_System/DataStructures/Variable.cpp L_System/Execution/Token.cpp L_System/DataStructures/Constants/Rotation.cpp L_System/DataStructures/Constants/Translation.cpp L_System/DataStructures/Constants/Constant.cpp L_System/DataStructures/Rules/Rule.cpp)

target_link_libraries(Voxel_L_System assimp ${Qt5Widgets_LIBRARIES} ${Qt5Test_LIBRARIES} -lGL)