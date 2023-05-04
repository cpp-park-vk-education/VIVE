#pragma once

#include "movable_object.hpp"

enum TYPE
{
    COIN,
    EXP
};

class Particle : public MovableObject
{
protected:
    TYPE type_;

public:
    Particle(const sf::Vector2f size, const sf::Vector2f position);
    virtual ~Particle();

    void create();
    void pop();

    // IObject overrides
    void update() override;
    void render() override;

    // PhysicalObject overrides
    void setPosition(const float x, const float y) override;
    sf::Vector2f getPosition() override;

    // MovableObject overrides
    void move(sf::Vector2f displacement) override;
    void updateMovement(const float delta_time) override;
};