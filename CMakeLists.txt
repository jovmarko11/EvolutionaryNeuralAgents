cmake_minimum_required(VERSION 3.20)

project(EvolutionaryNeuralAgents)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

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
)

target_link_libraries(EvolutionaryNeuralAgents
        PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)
