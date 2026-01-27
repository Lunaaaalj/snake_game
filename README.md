# Snake Game

A classic Snake game implementation in C using the ncurses library for terminal-based graphics.

## Overview

This is a terminal-based Snake game where the player controls a growing snake that must eat food while avoiding collisions with walls and its own body. The game features smooth movement, collision detection, and a simple restart mechanism.

## Features

- **Terminal-based graphics** using ncurses
- **Smooth snake movement** with configurable speed (10 movements per second)
- **Collision detection** for walls and self-collision
- **Dynamic snake growth** when eating food
- **Game restart functionality** after collision
- **Keyboard controls** supporting both arrow keys and vim-style movement keys

## Dependencies

### Required Libraries

1. **ncurses** - Terminal user interface library
   - Used for rendering the game window and handling keyboard input
   - Installation:
     - Ubuntu/Debian: `sudo apt-get install libncurses5-dev libncursesw5-dev`
     - Fedora/RHEL: `sudo dnf install ncurses-devel`
     - macOS: `brew install ncurses`

2. **cvector** - Custom external vector library
   - A custom dynamic array implementation for managing snake segments
   - Note: This is referenced in `game.h` as `<vector.h>` - you need to have this library installed or available in your include path
   - The library must provide functions like: `init_void_vector`, `void_append`, `void_get`, `void_set`, `free_void_vector`

### Build Tools

- **CMake** (version 4.0 or higher)
- **C Compiler** with C99 support or later
- **Address Sanitizer** support (enabled via `-fsanitize=address`)

## Building the Game

1. Clone the repository:
   ```bash
   git clone https://github.com/Lunaaaalj/snake_game.git
   cd snake_game
   ```

2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Configure with CMake:
   ```bash
   cmake ../src
   ```

4. Build the executable:
   ```bash
   make
   ```

5. Run the game:
   ```bash
   ./snake
   ```

## How to Play

### Controls

The game supports two control schemes:

#### Arrow Keys
- **↑** (Up Arrow) - Move up
- **↓** (Down Arrow) - Move down
- **←** (Left Arrow) - Move left
- **→** (Right Arrow) - Move right

#### Vim-style Keys
- **k** - Move up
- **j** - Move down
- **h** - Move left
- **l** - Move right

#### Other Controls
- **q** - Quit the game

### Gameplay

1. **Starting the game**: The snake starts in the center of the screen with an initial length of 3 segments (1 head + 2 body segments).

2. **Movement**: The snake moves continuously in the current direction. You can change direction using the control keys, but you cannot reverse directly into yourself (e.g., if moving right, you cannot immediately move left).

3. **Eating food**: 
   - Food is represented by the `*` character
   - When the snake's head reaches the food position, the snake grows by one segment
   - New food appears at a random location after being eaten

4. **Collision**:
   - The game ends when the snake collides with:
     - The window borders
     - Its own body segments
   - Upon collision, the snake turns into `X` characters

5. **Restarting**:
   - After a collision, press **Enter** to restart the game
   - Press **q** to quit the game

### Game Window

- **Window size**: 20 rows × 35 columns
- **Snake representation**: 
  - Head: `#`
  - Body segments: `#`
  - When collided: `X`
- **Food representation**: `*`
- **Title**: "Snake" displayed at the top center of the window

## Technical Details

### Architecture

The game consists of three main files:

1. **game.h** - Header file with function declarations and data structures
2. **game.c** - Implementation of game logic functions
3. **main.c** - Main game loop and initialization

### Key Data Structures

- **`coord`**: Structure representing x,y coordinates
  ```c
  typedef struct coord {
    int x_pos;
    int y_pos;
  } coord;
  ```

- **`snk_state`**: Enum representing snake direction
  ```c
  typedef enum snk_state {
    SNK_UP, SNK_DOWN, SNK_LEFT, SNK_RIGHT, SNK_NAN
  } snk_state;
  ```

### Core Functions

- **`CheckInput()`**: Processes keyboard input and updates snake direction
- **`move_snk()`**: Updates snake position based on current direction
- **`update_scr()`**: Renders the game state to the screen
- **`snake_food_gen()`**: Generates random food positions
- **`snake_grow()`**: Adds a new segment to the snake
- **`snk_collided()`**: Detects collisions with walls or self
- **`terminate_session()`**: Cleanly exits the game

### Game Loop

The main game loop operates at 10 movements per second (100ms interval):
1. Check for keyboard input
2. Update snake position every 100ms
3. Check for food collision and grow snake if needed
4. Check for wall/self collision
5. Update screen rendering
6. Handle game over and restart logic

### Memory Management

- The game uses a dynamic vector (`void_vec`) to store snake segment positions
- Memory is properly freed and reinitialized on game restart
- Address Sanitizer is enabled during compilation to detect memory issues

## Known Issues and Limitations

### Limitations

1. **Fixed window size**: The game window is hardcoded to 20×35 characters and doesn't adapt to terminal size

2. **External dependency**: Requires the custom `cvector` library which is not included in the repository. Users must install or provide this library separately.

3. **CMake version**: Requires CMake version 4.0+, which may not be available on older systems (note: this is unusually high, CMake 4.0 doesn't exist as of 2024, likely should be 3.x)

4. **Food overlap**: Food can spawn on the snake's body, making it immediately collectible or inaccessible

5. **Movement constraints**: The snake cannot move through borders (stops at edges rather than wrapping around)

6. **No scoring system**: The game doesn't track or display the player's score

7. **No speed increase**: Game speed remains constant regardless of snake length

### Potential Issues

1. **Terminal size**: If your terminal is smaller than the game window, the rendering may be distorted

2. **Curses initialization**: The game assumes ncurses is properly installed and functional on your system

3. **Food generation**: The random food generation doesn't check for collisions with the snake body

4. **Minimum terminal size**: Ensure your terminal window is large enough to display the 20×35 game window plus borders

### Recommendations

- Use a terminal with at least 40 columns × 25 rows for optimal display
- Ensure the `cvector` library is properly installed before building
- Consider modifying `CMakeLists.txt` to use a more standard CMake version (e.g., 3.10 or higher)

## Development

### Code Style

The code is formatted using `clang-format` with LLVM conventions (as noted in `main.c`).

### Debugging

The project is compiled with Address Sanitizer (`-fsanitize=address`) to help detect:
- Memory leaks
- Buffer overflows
- Use-after-free errors
- Other memory-related issues

## License

No license information is currently provided in the repository.

## Contributing

This appears to be a personal/educational project. If you'd like to contribute, consider:
- Adding a proper `cvector` library or replacing it with a standard implementation
- Implementing a scoring system
- Adding difficulty levels with increasing speed
- Implementing terminal size detection and adaptive window sizing
- Adding color support for better visuals
- Fixing the CMake version requirement

## Acknowledgments

- Built with ncurses for terminal graphics
- Uses a custom vector implementation for dynamic array management
