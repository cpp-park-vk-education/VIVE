#include "entity.hpp"

bool Entity::isDead() const
{
    if (!alive_) {
        animation_->changeAnimation('d');
        return true;
    }

    return false;
}

bool Entity::isInDamageRadius(const PhysicalObjectShPtr target) const
{
    if (target->doesExist())
    {
        sf::Vector2f executor_pos = getCenter();
        sf::Vector2f target_pos = target->getCenter();

        float distance = std::sqrt(std::pow(executor_pos.x - target_pos.x, 2) +
                                std::pow(executor_pos.y - target_pos.y, 2));

        return distance <= damage_radius_;
    }
    else
    {
        return false;
    }
}

Entity::Entity(const sf::Vector2f size, const sf::Vector2f position)
    : MovableObject(size, position)
{
}

Entity::~Entity()
{
}

int Entity::getHP() const
{
    return hp_;
}

int Entity::getHPMax() const
{
    return hp_max_;
}

void Entity::spawn()
{
    alive_ = true;
    hp_ = hp_max_;
}

void Entity::updateHP()
{
    int new_hp = hp_ - damage_taken_;
    if (new_hp <= 0)
    {
        hp_ = 0;
        alive_ = false;
        hitbox_.remove();
    }
    else
    {
        hp_ = new_hp;
    }
    damage_taken_ = 0;
}

void Entity::updateDamageTaken(const unsigned int damage)
{
    if (!isDead() && doesExist())
    {
        damage_taken_ += damage;
    }
}

void Entity::attack(EntityShPtr target)
{
    if (isInDamageRadius(target))
    {
        target->updateDamageTaken(damage_);
    }
}
