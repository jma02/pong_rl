#include "pong.hpp"

#include <stdexcept>

namespace {

void require(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

}

int main() {
    pong::Environment environment;
    pong::Environment other;
    const auto start = environment.reset(42);
    const auto duplicate = other.reset(42);
    require(start.ball_y == duplicate.ball_y && start.ball_vx == duplicate.ball_vx &&
                start.ball_vy == duplicate.ball_vy, "same seed produced different starting states");

    auto result = environment.step(pong::Action::Up);
    const auto replay = other.step(pong::Action::Up);
    require(result.observation.player_y < start.player_y &&
                result.observation.ball_x == replay.observation.ball_x &&
                result.observation.ball_y == replay.observation.ball_y,
            "action or replay produced a different state");

    for (int frame = 1; frame < 1000 && !result.done; ++frame) {
        result = environment.step(pong::Action::Stay);
    }
    require(result.done && result.reward >= -1 && result.reward <= 1,
            "episode did not end with a valid reward");

    bool rejected = false;
    try {
        environment.step(pong::Action::Stay);
    } catch (const std::logic_error&) {
        rejected = true;
    }
    require(rejected, "stepping a finished episode was accepted");
    require(environment.reset(42).ball_y == start.ball_y, "reset was not reproducible");
}
