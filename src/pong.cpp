#include "pong.hpp"

#include <algorithm>
#include <cmath>
#include <random>
#include <stdexcept>

namespace pong {
namespace {

constexpr float paddle_x = 0.9f;
constexpr float paddle_half_height = 0.18f;
constexpr float paddle_speed = 0.04f;
constexpr float opponent_speed = 0.02f;
constexpr float ball_speed = 0.035f;
constexpr float bounce_spin = 0.12f;
constexpr std::size_t max_frames = 1000;

}

Observation Environment::reset(std::uint32_t seed) {
    std::mt19937 random(seed);
    state_ = {};
    state_.ball_y = (static_cast<int>(random() % 101) - 50) / 100.0f;
    state_.ball_vx = random() % 2 ? ball_speed : -ball_speed;
    state_.ball_vy = (static_cast<int>(random() % 31) - 15) / 1000.0f;
    frames_ = 0;
    done_ = false;
    return state_;
}

Transition Environment::step(Action action) {
    if (done_) {
        throw std::logic_error("reset before stepping");
    }

    const float paddle_limit = 1.0f - paddle_half_height;
    state_.player_y = std::clamp(
        state_.player_y + paddle_speed * static_cast<int>(action), -paddle_limit, paddle_limit);
    const float target = state_.ball_x > 0 ? state_.ball_y : 0.0f;
    state_.opponent_y = std::clamp(
        state_.opponent_y + std::clamp(target - state_.opponent_y, -opponent_speed, opponent_speed),
        -paddle_limit, paddle_limit);

    state_.ball_x += state_.ball_vx;
    state_.ball_y += state_.ball_vy;
    if (state_.ball_y < -1.0f || state_.ball_y > 1.0f) {
        state_.ball_y = std::clamp(state_.ball_y, -1.0f, 1.0f);
        state_.ball_vy = -state_.ball_vy;
    }

    int reward = 0;
    if (state_.ball_vx < 0 && state_.ball_x <= -paddle_x) {
        const float offset = state_.ball_y - state_.player_y;
        if (std::abs(offset) > paddle_half_height) {
            reward = -1;
        } else {
            state_.ball_x = -paddle_x;
            state_.ball_vx = -state_.ball_vx;
            state_.ball_vy = bounce_spin * offset;
        }
    } else if (state_.ball_vx > 0 && state_.ball_x >= paddle_x) {
        const float offset = state_.ball_y - state_.opponent_y;
        if (std::abs(offset) > paddle_half_height) {
            reward = 1;
        } else {
            state_.ball_x = paddle_x;
            state_.ball_vx = -state_.ball_vx;
            state_.ball_vy = bounce_spin * offset;
        }
    }

    ++frames_;
    done_ = reward != 0 || frames_ >= max_frames;
    return {state_, reward, done_};
}

}
