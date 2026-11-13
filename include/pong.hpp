#pragma once

#include <cstddef>
#include <cstdint>

namespace pong {

enum class Action { Up = -1, Stay = 0, Down = 1 };

struct Observation {
    float ball_x = 0;
    float ball_y = 0;
    float ball_vx = 0;
    float ball_vy = 0;
    float player_y = 0;
    float opponent_y = 0;
};

struct Transition {
    Observation observation;
    int reward = 0;
    bool done = false;
};

class Environment {
public:
    Observation reset(std::uint32_t seed = 0);
    Transition step(Action action);

private:
    Observation state_;
    std::size_t frames_ = 0;
    bool done_ = true;
};

}
