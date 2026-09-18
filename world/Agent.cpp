//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#include "Agent.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numbers>
#include <stdexcept>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "World.h"
#include "../rendering/ColorUtil.h"


Agent::Agent(
    Vec2 position,
    Vec2 heading,
    float radius,
    DrawParams drawParams
)
    : startPosition(position),
      position(position),
      radius(radius),
      drawParams(drawParams),
      brain(Params::networkLayerSizes)
{
    if (heading.lengthSquared() <= 1e-12) {
        throw std::invalid_argument(
            "Agent::Agent: heading mora biti razlicit od nula vektora."
        );
    }

    startHeading = heading.normalized();

    initializeSensors();
    reset();
}


void Agent::reset() {
    position = startPosition;
    heading = startHeading;
    velocity = Vec2();

    updateSensorDirections();
    lastScan.assign(sensors.size(), ScanResult{SimStatus::Ongoing, 1.0});

    alive = true;
    lastStatus = SimStatus::Ongoing;
}


SimStatus Agent::step(const World& world) {
    if (!alive) {
        return lastStatus;
    }

    // 1. Skeniramo okolinu pomocu svih senzora
    lastScan = scanEnvironment(world);

    // 2. Pretvaramo rezultate senzora u ulaz za neuronsku mrezu
    const Matrix inputs = encodeInputs(lastScan, world);

    // 3. Neuronska mreza donosi odluku
    const Matrix output = brain.forward(inputs);

    // 4. Pretvaramo izlaz mreze u konkretnu akciju
    const AgentAction action = decodeOutput(output);

    // 5. Primena akcije: rotacija headinga i nova brzina
    applyAction(action);

    // 6. Pokusaj pomeranja, sa proverom kolizije duz celog pomeraja
    const Vec2 desiredPosition = position + velocity * Params::timeStep;
    const MoveResult move = resolveMovement(world, position, desiredPosition);

    position = move.position;
    lastStatus = move.status;

    // Svaki status osim Ongoing je terminalan: sudar, izlazak iz sveta
    // ili dolazak do cilja. Koji je od njih — Episode izvodi iz ove
    // povratne vrednosti.
    if (move.status != SimStatus::Ongoing) {
        alive = false;
    }

    return move.status;
}


// ---------------------------------------------------------------------------
// Percepcija
// ---------------------------------------------------------------------------

std::vector<ScanResult> Agent::scanEnvironment(const World& world) const {
    std::vector<ScanResult> result;
    result.reserve(sensors.size());

    for (const auto& sensor : sensors) {
        result.push_back(sensor.scan(world, position));
    }

    return result;
}


Matrix Agent::encodeInputs(
    const std::vector<ScanResult>& scanResults,
    const World& world
) const {

    Matrix input(static_cast<std::size_t>(Params::inputSize), 1);

    std::size_t index = 0;

    for (const auto& result : scanResults) {
        input(index++, 0) = 1.0 - result.distance;
    }

    const Vec2 toTarget = world.getTarget().getPosition() - position;

    const double angleToGoal = std::atan2(heading.cross(toTarget), heading.dot(toTarget));


    const double initialDistance = (world.getTarget().getPosition() - startPosition).length();
    const double distanceToGoal = (initialDistance > 1e-9)
        ? std::min(1.0, toTarget.length() / initialDistance)
        : 0.0;

    const double normalizedSpeed = std::min(1.0, velocity.length() / Params::maxSpeed);

    input(index++, 0) = std::sin(angleToGoal);
    input(index++, 0) = std::cos(angleToGoal);
    input(index++, 0) = distanceToGoal;
    input(index++, 0) = normalizedSpeed;

    assert(index == static_cast<std::size_t>(Params::inputSize));

    return input;
}


// ---------------------------------------------------------------------------
// Odluka i akcija
// ---------------------------------------------------------------------------

AgentAction Agent::decodeOutput(const Matrix& output) const {

    const double turnOutput = std::clamp(output(0, 0), -1.0, 1.0);
    const double speedOutput = std::clamp(output(1, 0), -1.0, 1.0);

    const double turnAngle = turnOutput * Params::maxTurnRate * Params::timeStep;

    const double speed = 0.5 * (speedOutput + 1.0) * Params::maxSpeed;

    return AgentAction{
        turnAngle,
        speed
    };
}


void Agent::applyAction(const AgentAction& action) {
    heading = heading.rotated(action.turnAngle).normalized();
    velocity = heading * action.speed;

    updateSensorDirections();
}


// ---------------------------------------------------------------------------
// Kolizija
// ---------------------------------------------------------------------------

Agent::MoveResult Agent::resolveMovement(
    const World& world,
    const Vec2& from,
    const Vec2& to
) const {
    const Vec2 delta = to - from;
    const double distance = delta.length();

    // Pomeraj se deli na podkorake tako da nijedan nije duzi od
    // maxCollisionStep — inace agent moze da preskoci tanku prepreku.
    const int substeps = std::max(
        1,
        static_cast<int>(std::ceil(distance / Params::maxCollisionStep))
    );

    Vec2 lastSafe = from;

    for (int i = 1; i <= substeps; ++i) {
        const double t = static_cast<double>(i) / static_cast<double>(substeps);
        const Vec2 sample = from + delta * t;

        const SimStatus status = world.inRange(sample, static_cast<double>(radius));

        if (status == SimStatus::Blocked || status == SimStatus::OutOfBounds) {
            return MoveResult{status, lastSafe};
        }

        if (status == SimStatus::ReachedTarget) {
            return MoveResult{SimStatus::ReachedTarget, sample};
        }

        lastSafe = sample;
    }

    return MoveResult{SimStatus::Ongoing, to};
}


// ---------------------------------------------------------------------------
// Senzori
// ---------------------------------------------------------------------------

void Agent::initializeSensors() {
    sensorAngles.clear();
    sensorAngles.reserve(numOfSensors);

    if constexpr (numOfSensors <= 0) {
        return;
    }
    else if constexpr (numOfSensors == 1) {
        sensorAngles.push_back(0.0);
    }
    else {
        const double step = fieldOfView / static_cast<double>(numOfSensors - 1);

        for (int i = 0; i < numOfSensors; ++i) {
            sensorAngles.push_back(-fieldOfView / 2.0 + i * step);
        }
    }

    sensors.clear();
    sensors.reserve(sensorAngles.size());

    for (std::size_t i = 0; i < sensorAngles.size(); ++i) {
        sensors.emplace_back(Vec2(1.0, 0.0));
    }
}


void Agent::updateSensorDirections() {
    for (std::size_t i = 0; i < sensors.size(); ++i) {
        sensors[i].setDirection(heading.rotated(sensorAngles[i]));
    }
}


// ---------------------------------------------------------------------------
// Vizualizacija
// ---------------------------------------------------------------------------

void Agent::draw(sf::RenderWindow& window) const {
    drawSensors(window);
    drawBody(window);
}


void Agent::drawBody(sf::RenderWindow& window) const {
    sf::CircleShape shape(radius);

    shape.setOrigin(sf::Vector2f(radius, radius));
    shape.setPosition(sf::Vector2f(
        static_cast<float>(position.x),
        static_cast<float>(position.y)
    ));

    shape.setFillColor(toSFML(drawParams.color));

    window.draw(shape);
}


void Agent::drawSensors(sf::RenderWindow& window) const {
    for (std::size_t i = 0; i < sensors.size(); ++i) {
        const double normalizedLength =
            (i < lastScan.size()) ? lastScan[i].distance : 1.0;

        sensors[i].draw(window, position, normalizedLength);
    }
}
