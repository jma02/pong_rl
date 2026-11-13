#include "pong.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: pong_rl EPISODES\n";
        return 2;
    }

    try {
        const std::string text(argv[1]);
        std::size_t end = 0;
        const auto episodes = std::stoull(text, &end);
        if (end != text.size() || episodes == 0 || episodes > 1000000) {
            throw std::invalid_argument("episodes must be between 1 and 1000000");
        }

        pong::Environment environment;
        std::mt19937 random(42);
        std::uniform_int_distribution<int> action(-1, 1);
        std::uint64_t frames = 0;
        std::uint64_t wins = 0;
        std::uint64_t losses = 0;
        const auto start = std::chrono::steady_clock::now();

        for (std::uint32_t episode = 0; episode < episodes; ++episode) {
            environment.reset(episode);
            while (true) {
                const auto result = environment.step(static_cast<pong::Action>(action(random)));
                ++frames;
                if (result.done) {
                    wins += result.reward == 1;
                    losses += result.reward == -1;
                    break;
                }
            }
        }

        const double seconds = std::chrono::duration<double>(
            std::chrono::steady_clock::now() - start).count();
        std::cout << "episodes " << episodes << ", wins " << wins << ", losses " << losses
                  << ", timeouts " << episodes - wins - losses << '\n'
                  << "frames " << frames << ", episodes/s " << episodes / seconds
                  << ", frames/s " << frames / seconds << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 2;
    }
}
