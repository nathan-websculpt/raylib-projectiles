#### NOTE: This has only been tested on Linux (on wide monitors) - requires raylib

# Compile
```

g++ -std=c++23 src/main.cpp src/core/gear_config.cpp src/core/launcher.cpp src/core/base_projectile.cpp src/core/dodge_ball.cpp src/core/projectile_two.cpp -o main -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

// release build targeting C++23 with warnings and optimizations
g++ -std=c++23 -O2 -march=native -flto -Wall -Wextra -Wpedantic src/main.cpp src/core/gear_config.cpp src/core/launcher.cpp src/core/base_projectile.cpp src/core/dodge_ball.cpp src/core/projectile_two.cpp -o main -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11


./main
```

### Notes

You can edit `assets/configs/gear.json` while the game is running - once it is saved - it will hot-reload the configurations