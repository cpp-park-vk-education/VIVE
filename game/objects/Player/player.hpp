#pragma once

#include "entity.hpp"

#include <string>
#include <memory>
#include <iostream>

class Player : public Entity
{
protected:
    std::string name_;

    int exp_;
    int exp_max_;
    int curr_lvl_;
    int coins_count_;

    // Initions
    void initSprite() override;
    void initPhysics() override;
    void initStats() override;

public:
    Player(const sf::Vector2f size, const sf::Vector2f position);
    virtual ~Player();

    // Getters
    int getExp();
    int getExpMax();
    int getCurrLvl();
    int getCoinsCount();
    virtual bool doesExist() const override;

    // Updates
    void updateExp(const uint16_t exp);
    void updateCurrLvl();
    void updateCoinsCount(const uint16_t coins_count);

    // Object overrides
    void update(const sf::Event &event, const float delta_time) override;
    void update(const sf::Event &event, Entity *target, const float delta_time);

    // MovableObject overrides
    void updateMovement(const float delta_time) override;

    // Entity overrides
    virtual void updateAttack(const sf::Event &event, Entity *target, const float delta_time);
    virtual void updateHP(const unsigned int damage);
};

using PlayerShPtr = std::shared_ptr<Player>;
