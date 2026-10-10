cmake_minimum_required(VERSION 3.20)

project(EvolutionaryNeuralAgents)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS_ONLY "Build only the tests target, skip the SFML app" OFF)

include(FetchContent)

if(NOT BUILD_TESTS_ONLY)
    find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

    FetchContent_Declare(
            imgui
            GIT_REPOSITORY https://github.com/ocornut/imgui.git
            GIT_TAG        v1.91.9
    )
    FetchContent_MakeAvailable(imgui)

    set(IMGUI_DIR ${imgui_SOURCE_DIR})

    # SFML je vec pronadjen iznad (i radi kao shared lib preko Homebrew-a).
    # imgui-sfml bi inace sam ponovo zvao find_package(SFML...) i pri tom
    # pogresno pretpostavio staticki build kad BUILD_SHARED_LIBS nije ON --
    # to mu ovde iskljucujemo, pa koristi vec postojece SFML:: target-e.
    set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "Don't let imgui-sfml call find_package(SFML) itself" FORCE)

    FetchContent_Declare(
            imgui-sfml
            GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
            GIT_TAG        master
    )
    FetchContent_MakeAvailable(imgui-sfml)

    # ImPlot (grafici). Nema svoj CMakeLists.txt, pa FetchContent samo preuzme
    # izvore, a staticku biblioteku pravimo ovde. Linkovan je na ImGui-SFML da bi
    # dobio isti imgui include i isti IMGUI_USER_CONFIG define kao ostatak.
    FetchContent_Declare(
            implot
            GIT_REPOSITORY https://github.com/epezent/implot.git
            GIT_TAG        v1.0
    )
    FetchContent_MakeAvailable(implot)

    add_library(implot STATIC
            ${implot_SOURCE_DIR}/implot.cpp
            ${implot_SOURCE_DIR}/implot_items.cpp
            ${implot_SOURCE_DIR}/implot_demo.cpp
    )
    target_include_directories(implot SYSTEM PUBLIC ${implot_SOURCE_DIR})
    target_link_libraries(implot PUBLIC ImGui-SFML::ImGui-SFML)
endif()

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
        GIT_TAG         v3.7.1
)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(tests
        tests/VecTest.cpp
        tests/MatrixTest.cpp
        tests/FitnessTest.cpp
        tests/GeneticAlgorithmTest.cpp
        tests/CalculationTest.cpp

        src/core/math/Matrix.cpp
        src/core/math/Random.cpp
        src/learning/fitness/Fitness.cpp
        src/learning/evolution/Selection.cpp
        src/learning/evolution/Crossover.cpp
        src/learning/evolution/Mutation.cpp
        src/learning/evolution/GeneticAlgorithm.cpp
        src/learning/statistics/Calculation.cpp
        tests/JSONTest.cpp
        src/app/config/ExperimentConfig.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain nlohmann_json::nlohmann_json)
target_include_directories(tests PRIVATE src)

include(CTest)
include(Catch)
catch_discover_tests(tests)

if(NOT BUILD_TESTS_ONLY)
add_executable(EvolutionaryNeuralAgents
        src/main.cpp

        src/app/FixedTimestep.cpp
        src/app/FixedTimestep.h

        src/app/config/Params.h
        src/app/config/TrainingParams.h

        src/core/math/Vec2.h
        src/core/math/Matrix.cpp
        src/core/math/Matrix.h
        src/core/math/Random.cpp
        src/core/math/Random.h

        src/core/neural/Layer.cpp
        src/core/neural/Layer.h
        src/core/neural/NeuralNetwork.cpp
        src/core/neural/NeuralNetwork.h

        src/model/sensors/RaySensor.cpp
        src/model/sensors/RaySensor.h
        src/model/sensors/ScanResult.h

        src/model/world/World.cpp
        src/model/world/World.h
        src/model/world/Agent.cpp
        src/model/world/Agent.h
        src/model/world/AgentAction.h
        src/model/world/SimStatus.h
        src/model/world/Target.cpp
        src/model/world/Target.h
        src/model/world/Obstacle.cpp
        src/model/world/Obstacle.h
        src/model/world/obstacles/CircleObstacle.cpp
        src/model/world/obstacles/CircleObstacle.h
        src/model/world/obstacles/RectObstacle.cpp
        src/model/world/obstacles/RectObstacle.h

        src/model/simulation/Simulation.cpp
        src/model/simulation/Simulation.h
        src/model/simulation/Episode.cpp
        src/model/simulation/Episode.h
        src/model/simulation/Population.cpp
        src/model/simulation/Population.h
        src/model/simulation/SceneBuilder.cpp
        src/model/simulation/SceneBuilder.h

        src/ui/rendering/Renderer.cpp
        src/ui/rendering/Renderer.h
        src/ui/rendering/Viewport.cpp
        src/ui/rendering/Viewport.h
        src/ui/rendering/Drawable.cpp
        src/ui/rendering/Drawable.h
        src/ui/rendering/DrawParams.h
        src/ui/rendering/Color.h
        src/ui/rendering/ColorUtil.h

        src/learning/fitness/Fitness.h
        src/learning/fitness/Fitness.cpp

        src/learning/evolution/GeneticAlgorithm.cpp
        src/learning/evolution/GeneticAlgorithm.h
        src/learning/evolution/Selection.cpp
        src/learning/evolution/Selection.h
        src/learning/evolution/Crossover.cpp
        src/learning/evolution/Crossover.h
        src/learning/evolution/Mutation.cpp
        src/learning/evolution/Mutation.h

        src/app/Trainer.cpp
        src/app/Trainer.h
        src/app/Program.cpp
        src/app/Program.h
        src/learning/evolution/Individual.h
        src/learning/evolution/GAParams.h
        src/ui/gui/UILayer.cpp
        src/ui/gui/UILayer.h
        src/learning/statistics/Statistics.cpp
        src/learning/statistics/Statistics.h
        src/learning/statistics/GenerationStatistics.h
        src/learning/statistics/Calculation.cpp
        src/learning/statistics/Calculation.h
        src/learning/statistics/Storage.cpp
        src/learning/statistics/Storage.h
        src/learning/statistics/Export.cpp
        src/learning/statistics/Export.h
        src/learning/statistics/ConsoleReporter.cpp
        src/learning/statistics/ConsoleReporter.h
        src/app/HeadlessRunner.cpp
        src/app/HeadlessRunner.h
        src/app/config/ExperimentConfig.h
        src/app/config/ExperimentConfig.cpp
        src/learning/evolution/ParamController.cpp
        src/learning/evolution/ParamController.h
        src/ui/gui/panels/Panel.cpp
        src/ui/gui/panels/Panel.h
        src/ui/gui/Layout.cpp
        src/ui/gui/Layout.h
        src/ui/gui/UIData.h
        src/ui/gui/panels/PlaceholderPanel.cpp
        src/ui/gui/panels/PlaceholderPanel.h
        src/ui/gui/panels/TransportPanel.cpp
        src/ui/gui/panels/TransportPanel.h
        src/ui/gui/panels/StatisticsPanel.cpp
        src/ui/gui/panels/StatisticsPanel.h
        src/ui/gui/panels/ParametersPanel.cpp
        src/ui/gui/panels/ParametersPanel.h
        src/ui/gui/panels/StatusPanel.cpp
        src/ui/gui/panels/StatusPanel.h
)

# Putanja do assets/ (fontovi) ugradjena u program; radi iz CLion-a bez kopiranja fajlova.
target_include_directories(EvolutionaryNeuralAgents PRIVATE src)
target_compile_definitions(EvolutionaryNeuralAgents PRIVATE
        ENA_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")

target_link_libraries(EvolutionaryNeuralAgents
        PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        nlohmann_json::nlohmann_json
        ImGui-SFML::ImGui-SFML
        implot
)
endif()
