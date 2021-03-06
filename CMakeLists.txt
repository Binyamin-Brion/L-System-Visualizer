cmake_minimum_required(VERSION 3.15)
project(L-System-Visualizer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# If compiling from scratch, the location to the Qt installation and glm folder will have to be updated to match
# where it is installed on the machine being used for compilation

if(WIN32)
    set(CMAKE_PREFIX_PATH C:\\Qt\\Qt5.9.9\\5.9.9\\mingw53_32)

    # Assumes that Assimp has been compiled from scratch, and the result of running
    # make and then make install results in the Assimp build being placed in Program Files.
    set(ASSIMP_DIR "C:/Program Files (x86)/Assimp")

    set(GLM_FOLDER C:/glm-0.9.9.8/glm)
endif(WIN32)

if(UNIX)
    set(CMAKE_PREFIX_PATH /home/binybrion/Qt5.9.9/5.9.9/gcc_64)
    set(GLM_FOLDER /home/binybrion/ProjectSourceDependencies/glm-0.9.9.8/glm)
endif(UNIX)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

include_directories(${Qt5Widgets_INCLUDES})
include_directories(${GLM_FOLDER})

# On Windows, using the find_package for Assimp doesn't work for some reason. As a result,
# the include directory is included manually.
if(WIN32)
    include_directories(${ASSIMP_DIR}/include)
endif(WIN32)

if(UNIX)
    find_package(ASSIMP REQUIRED)
    include_directories(${ASSIMP_INCLUDES_DIR})
endif(UNIX)

add_executable(L-System-Visualizer main.cpp GUI/MainWindow.cpp GUI/MainWindow.h Render/VBO/VBOWapper.h Render/VAO/GridSystem/AxisVAO.cpp
        Render/VAO/GridSystem/AxisVAO.h Render/CommandCentre.cpp Render/CommandCentre.h Render/Shaders/ShaderManager.cpp
        Render/Shaders/ShaderManager.h GUI/OpenGL/GLWidget.cpp GUI/OpenGL/GLWidget.h Render/VAO/GridSystem/GridVAO.cpp
        Render/VAO/GridSystem/GridVAO.h Render/Camera/CameraMovement.h Render/Camera/CameraObject.cpp Render/Camera/CameraObject.h
        ModelLoading/Mesh.cpp ModelLoading/Mesh.h ModelLoading/Model.cpp
        ModelLoading/Model.h ModelLoading/Face.h Render/VAO/UserModels/ModelVAO.cpp Render/VAO/UserModels/ModelVAO.h ModelLoading/Face.cpp
        Render/DataStructures/StoredModels.cpp Render/DataStructures/StoredModels.h
        Render/DataStructures/ModelRange.h Render/DataStructures/ModelRange.cpp
        GUI/L_System/ScriptInfoTabWidget.cpp GUI/L_System/ScriptInfoTabWidget.h GUI/L_System/Variables/VariablesWidget.cpp
        GUI/L_System/Variables/VariablesWidget.h GUI/L_System/Variables/VariableTabContent.cpp GUI/L_System/Variables/VariableTabContent.h
        GUI/L_System/Variables/VariableEntry.cpp GUI/L_System/Variables/VariableEntry.h GUI/L_System/Constants/ConstantTabContent.cpp
        GUI/L_System/Constants/ConstantTabContent.h GUI/L_System/Constants/ConstantsWidget.cpp GUI/L_System/Constants/ConstantsWidget.h
        GUI/L_System/Constants/ConstantEntryInformation.cpp GUI/L_System/Constants/ConstantEntryInformation.h
        GUI/L_System/Constants/ConstantEntryDeclaration.cpp GUI/L_System/Constants/ConstantEntryDeclaration.h
        L_System/DataStructures/Constants/Constant.h L_System/DataStructures/Constants/StackOperation.h
        L_System/DataStructures/Constants/Rotation.h L_System/DataStructures/Constants/Translation.h GUI/L_System/Rules/RuleTabContent.cpp
        GUI/L_System/Rules/RuleTabContent.h  GUI/L_System/Rules/RuleWidget.cpp GUI/L_System/Rules/RuleWidget.h GUI/L_System/Rules/RuleEntry.cpp
        GUI/L_System/Rules/RuleEntry.h L_System/Execution/Executor.cpp L_System/Execution/Executor.h L_System/Execution/Token.h
        Tests/TestSuite.h Tests/TestSuite.cpp Tests/L_System/TestExecutor.cpp Tests/L_System/TestExecutor.h L_System/DataStructures/Rules/Rule.h
        L_System/DataStructures/Variable/Variable.cpp L_System/Execution/Token.cpp L_System/DataStructures/Constants/Rotation.cpp
        L_System/DataStructures/Constants/Translation.cpp L_System/DataStructures/Constants/Constant.cpp L_System/DataStructures/Rules/Rule.cpp
        L_System/Interpretation/Interpreter.cpp L_System/Interpretation/Interpreter.h L_System/Interpretation/ModelInstancePlacement.h
        Tests/L_System/TestInterpreter.cpp Tests/L_System/TestInterpreter.h GUI/L_System/Rules/RuleInformation.h GUI/Output/L_System/ScriptOutputWidget.cpp
        GUI/Output/L_System/ScriptOutputWidget.h GUI/Output/L_System/ScriptOutputEntry.cpp GUI/Output/L_System/ScriptOutputEntry.h
        L_System/ScriptInput.cpp L_System/ScriptInput.h L_System/DataStructures/Variable/Variable.h L_System/Execution/NoMatchError.h
        ProjectSaverLoader/ProjectSaver.cpp ProjectSaverLoader/ProjectSaver.h ProjectSaverLoader/ProjectDetails.cpp
        ProjectSaverLoader/ProjectDetails.h ProjectSaverLoader/FavouriteResult.h GUI/Dialogs/NewNameDialog.cpp
        GUI/Dialogs/NewNameDialog.h ProjectSaverLoader/ProjectLoader.cpp ProjectSaverLoader/ProjectLoader.h
        HelperFunctions/RuleLoading.cpp HelperFunctions/RuleLoading.h Tests/ProjectSaverLoader/TestProjectSaveLoad.cpp
        Tests/ProjectSaverLoader/TestProjectSaveLoad.h ProjectSaverLoader/ScriptInformation.h Tests/ProjectSaverLoader/TestBasicSaveLoad.cpp
        Tests/ProjectSaverLoader/TestBasicSaveLoad.h GUI/L_System/ScriptTabs.cpp GUI/L_System/ScriptTabs.h GUI/L_System/ScriptTabWidget.cpp
        GUI/L_System/ScriptTabWidget.h L_System/Interpretation/UnderflowError.h L_System/Interpretation/UnbalancedError.h
        Render/DataStructures/TransformationIdentifier.h Render/DataStructures/TransformationData.h GUI/OpenGL/RenderResultWidget.cpp
        GUI/OpenGL/RenderResultWidget.h Render/DataStructures/UserAddedModelInstances.h ProjectSaverLoader/UserDefinedInstances.h
        Tests/Render/TestStoredModels.cpp Tests/Render/TestStoredModels.h Render/Textures/TextureID.h Render/Textures/TextureManager.cpp
        Render/Textures/TextureManager.h Render/Textures/TextureBank.cpp Render/Textures/TextureBank.h Render/Textures/CompressFactor.h
        ProjectSaverLoader/ProjectExporter.cpp ProjectSaverLoader/ProjectExporter.h)

# Window and Linux have different requirements for linking Assimp and OpenGL, so OS specific
# link commands written. Common link commands are given outside of OS specific if-statements.
if(WIN32)
    # Remember that on Windows Assimp is kind of weird with the way it is included into
    # a project.
    target_link_libraries(L-System-Visualizer ${ASSIMP_DIR}/lib/libassimp.dll.a -lopenGL32)
endif(WIN32)

if(UNIX)
    target_link_libraries(L-System-Visualizer assimp -lGL)
endif(UNIX)

target_link_libraries(L-System-Visualizer ${Qt5Widgets_LIBRARIES} ${Qt5Test_LIBRARIES})