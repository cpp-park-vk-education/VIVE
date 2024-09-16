#pragma once

#include "Connection.hpp"
#include "ISerializable.hpp"

class LevelManager;
using LevelManagerUPtr = std::unique_ptr<LevelManager>;

class GameCalculator;
using GameCalculatorUPtr = std::unique_ptr<GameCalculator>;

class GameCalculator
{
private:
    //LevelManagerUPtr curr_state_;
    //ISerializableShPtr serializer_;

public:
    //GameCalculator(ISerializableShPtr serializer) : serializer_(serializer)
    //{
    //}

    GameCalculator() = default;

    Message checkCollision(const Message& mess);
};