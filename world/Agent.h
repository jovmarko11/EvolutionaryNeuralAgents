//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_AGENT_H
#define EVOLUTIONARYNEURALAGENTS_AGENT_H

#include <vector>

#include "AgentAction.h"
#include "SimStatus.h"

#include "../configuration/Params.h"
#include "../math/Vec2.h"
#include "../neural/NeuralNetwork.h"
#include "../rendering/Drawable.h"
#include "../rendering/DrawParams.h"
#include "../sensors/RaySensor.h"

class World;

/*
 * Agent je politika + fizika, i nista vise.
 *
 * Statistiku epizode (broj koraka, predjeni put, napredak ka cilju,
 * ishod) vodi Episode. Agent zna samo da li je jos uvek ziv i zasto
 * je prestao da bude — sve ostalo Episode izvodi iz povratne vrednosti
 * step() i iz getPosition().
 */
class Agent : public Drawable {
private:
    static constexpr int numOfSensors = Params::numOfSensors;
    static constexpr double fieldOfView = Params::fieldOfView;

    // Rezultat pokusaja pomeranja: status i pozicija na kojoj se agent
    // stvarno zaustavio (kod sudara to je poslednja bezbedna tacka).
    struct MoveResult {
        SimStatus status;
        Vec2 position;
    };

    // --- pocetno stanje, cuva se radi reset() ---
    Vec2 startPosition;
    Vec2 startHeading;

    // --- fizicko stanje ---
    Vec2 position;
    Vec2 velocity;
    Vec2 heading;
    float radius;

    DrawParams drawParams;

    // --- mozak i senzori ---
    NeuralNetwork brain;

    // senzori
    std::vector<double> sensorAngles;
    std::vector<RaySensor> sensors;

    // Poslednji sken, cuva se samo radi vizualizacije.
    std::vector<ScanResult> lastScan;

    // --- stanje zivota ---
    bool alive;
    SimStatus lastStatus;   // zasto je agent prestao da bude ziv

    void initializeSensors();
    void updateSensorDirections();

    std::vector<ScanResult> scanEnvironment(const World& world) const;

    Matrix encodeInputs(const std::vector<ScanResult>& scanResults, const World& world) const;
    AgentAction decodeOutput(const Matrix& output) const;
    void applyAction(const AgentAction& action);

    MoveResult resolveMovement(const World& world, const Vec2& from, const Vec2& to) const;

    void drawBody(sf::RenderWindow& window) const;
    void drawSensors(sf::RenderWindow& window) const;

public:
    Agent(
        Vec2 position,
        Vec2 heading,
        float radius = Params::agentRadius,
        DrawParams drawParams = DrawParams{Params::AgentColor, 0.f}
    );

    // Jedan korak simulacije: skeniraj -> odluci -> pomeri se.
    // Nad mrtvim agentom je no-op i vraca razlog smrti.
    SimStatus step(const World& world);

    // Vraca agenta u pocetno stanje. Ne dira mozak — genom je jedino
    // sto prezivljava epizodu, i njega menja iskljucivo GA.
    void reset();

    Vec2 getPosition() const;
    Vec2 getHeading() const;
    double getSpeed() const;

    bool isAlive() const;
    SimStatus getLastStatus() const;

    const NeuralNetwork& getBrain() const;
    NeuralNetwork& getBrain();

    void draw(sf::RenderWindow& window) const override;
};

inline Vec2 Agent::getPosition() const { return position; }
inline Vec2 Agent::getHeading() const { return heading; }
inline double Agent::getSpeed() const { return velocity.length(); }

inline bool Agent::isAlive() const { return alive; }
inline SimStatus Agent::getLastStatus() const { return lastStatus; }

inline const NeuralNetwork& Agent::getBrain() const { return brain; }
inline NeuralNetwork& Agent::getBrain() { return brain; }

#endif // EVOLUTIONARYNEURALAGENTS_AGENT_H
