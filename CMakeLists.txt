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

        math/Matrix.cpp
        math/Random.cpp
        fitness/Fitness.cpp
        evolution/Selection.cpp
        evolution/Crossover.cpp
        evolution/Mutation.cpp
        evolution/GeneticAlgorithm.cpp
        statistics/Calculation.cpp
        tests/JSONTest.cpp
        configuration/ExperimentConfig.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain nlohmann_json::nlohmann_json)

include(CTest)
include(Catch)
catch_discover_tests(tests)

if(NOT BUILD_TESTS_ONLY)
add_executable(EvolutionaryNeuralAgents
        main.cpp

        app/FixedTimestep.cpp
        app/FixedTimestep.h

        configuration/Params.h

        math/Vec2.h
        math/Matrix.cpp
        math/Matrix.h
        math/Random.cpp
        math/Random.h

        neural/Layer.cpp
        neural/Layer.h
        neural/NeuralNetwork.cpp
        neural/NeuralNetwork.h

        sensors/RaySensor.cpp
        sensors/RaySensor.h
        sensors/ScanResult.h

        world/World.cpp
        world/World.h
        world/Agent.cpp
        world/Agent.h
        world/AgentAction.h
        world/SimStatus.h
        world/Target.cpp
        world/Target.h
        world/Obstacle.cpp
        world/Obstacle.h
        world/obstacles/CircleObstacle.cpp
        world/obstacles/CircleObstacle.h
        world/obstacles/RectObstacle.cpp
        world/obstacles/RectObstacle.h

        simulation/Simulation.cpp
        simulation/Simulation.h
        simulation/Episode.cpp
        simulation/Episode.h
        simulation/Population.cpp
        simulation/Population.h
        simulation/SceneBuilder.cpp
        simulation/SceneBuilder.h

        rendering/Renderer.cpp
        rendering/Renderer.h
        rendering/Viewport.cpp
        rendering/Viewport.h
        rendering/Drawable.cpp
        rendering/Drawable.h
        rendering/DrawParams.h
        rendering/Color.h
        rendering/ColorUtil.h

        fitness/Fitness.h
        fitness/Fitness.cpp

        evolution/GeneticAlgorithm.cpp
        evolution/GeneticAlgorithm.h
        evolution/Selection.cpp
        evolution/Selection.h
        evolution/Crossover.cpp
        evolution/Crossover.h
        evolution/Mutation.cpp
        evolution/Mutation.h

        app/Trainer.cpp
        app/Trainer.h
        app/Program.cpp
        app/Program.h
        evolution/Individual.h
        evolution/GAParams.h
        gui/UILayer.cpp
        gui/UILayer.h
        statistics/Statistics.cpp
        statistics/Statistics.h
        statistics/GenerationStatistics.h
        statistics/Calculation.cpp
        statistics/Calculation.h
        statistics/Storage.cpp
        statistics/Storage.h
        statistics/Export.cpp
        statistics/Export.h
        statistics/ConsoleReporter.cpp
        statistics/ConsoleReporter.h
        app/HeadlessRunner.cpp
        app/HeadlessRunner.h
        configuration/ExperimentConfig.h
        configuration/ExperimentConfig.cpp
        evolution/ParamController.cpp
        evolution/ParamController.h
        gui/panels/Panel.cpp
        gui/panels/Panel.h
        gui/Layout.cpp
        gui/Layout.h
        gui/UIData.h
        gui/panels/PlaceholderPanel.cpp
        gui/panels/PlaceholderPanel.h
        gui/panels/TransportPanel.cpp
        gui/panels/TransportPanel.h
        gui/panels/StatisticsPanel.cpp
        gui/panels/StatisticsPanel.h
        gui/panels/ParametersPanel.cpp
        gui/panels/ParametersPanel.h
        gui/panels/StatusPanel.cpp
        gui/panels/StatusPanel.h
)

# Putanja do assets/ (fontovi) ugradjena u program; radi iz CLion-a bez kopiranja fajlova.
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
