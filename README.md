# pong_rl

A small headless C++17 Pong environment for trying out RL loops. It runs entirely in memory: no Atari ROM, graphics, game process or JSON logs.

Build with Bazelisk (pinned in `.bazelversion`):

```sh
bazel test //...
bazel build //:pong_rl
./bazel-bin/pong_rl 10000
```

For throughput measurements, build with `bazel build -c opt //:pong_rl` before running the binary.

`pong::Environment::reset(seed)` returns the initial `Observation`. `step(Action)` moves the left paddle one frame and returns an observation, reward and `done` flag. The right paddle follows the ball imperfectly. Coordinates stay in `[-1, 1]`; the first missed paddle ends the point with reward `+1` for the agent or `-1` against it. An episode also ends without a point after 1000 frames. Call `reset` before stepping again.

The binary runs a seeded random policy and reports episode and frame throughput. It is a harness, not a trained agent or an emulator of Atari Pong.
