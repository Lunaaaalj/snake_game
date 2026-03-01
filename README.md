# Snake Game

A classic Snake game implementation in C using the ncurses library for terminal-based graphics.

## Overview

This is a terminal-based Snake game where the player controls a growing snake that must eat food while avoiding collisions with walls and its own body. The game features smooth movement, collision detection, and a simple restart mechanism.

## Features

- **Terminal-based graphics** using ncurses
- **Smooth snake movement** with configurable speed (10 movements per second)
- **Collision detection** for walls and self-collision
- **Dynamic snake growth** when eating food
- **Scoring system** with live score display and persistent high score tracking
- **High score persistence** stored in `files/high_score.txt`, survives game restarts
- **Game restart functionality** after collision
- **Keyboard controls** supporting both arrow keys and vim-style movement keys
- **Terminal resize support** — the game window re-centers automatically on resize
- **Minimum terminal size enforcement** at startup and on resize events

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

- **CMake** (version 3.15 or higher)
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
   - The current score is shown; if a new high score was reached, "New Highest Score" is displayed

5. **Restarting**:
   - After a collision, press **Enter** to restart the game
   - Press **q** to quit the game

### Game Window

- **Window size**: 20 rows × 35 columns (centered in the terminal, adapts on resize)
- **Snake representation**: 
  - Head: `#`
  - Body segments: `#`
  - When collided: `X`
- **Food representation**: `*`
- **Score display**: shown at the bottom center of the window during play
- **Title**: "Snake" displayed at the top center of the window

## Technical Details

### Architecture

The game consists of four main files:

1. **game.h** - Header file with function declarations and data structures
2. **game.c** - Implementation of game logic functions
3. **main.c** - Main game loop and initialization
4. **files/high_score.txt** - Persistent storage for the high score

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

- **`CheckInput()`**: Processes keyboard input and updates the requested snake direction; also handles terminal resize events
- **`move_snk()`**: Updates snake position based on current direction
- **`update_scr()`**: Renders the game state (snake, food, score) to the screen
- **`snake_food_gen()`**: Generates random food positions, guaranteed not to overlap the snake body
- **`snake_grow()`**: Adds a new segment to the snake
- **`snk_collided()`**: Detects collisions with walls or self
- **`handle_resize()`**: Handles terminal resize events — re-centers the game window and returns `false` if the terminal is now too small
- **`get_high_score()`**: Reads the high score from `files/high_score.txt`
- **`write_high_score()`**: Writes the current high score to `files/high_score.txt`
- **`disp_hscore()`**: Displays "New Highest Score" at the bottom of the window after a collision
- **`terminate_session()`**: Cleanly exits the game

### Game Loop

The main game loop operates at 10 movements per second (100ms interval):
1. Check for keyboard input (including resize events)
2. Update snake position every 100ms
3. Check for food collision and grow snake + increment score if needed
4. Update and persist high score if beaten
5. Check for wall/self collision
6. Update screen rendering (score shown at the bottom)
7. Handle game over and restart logic

### Memory Management

- The game uses a dynamic vector (`void_vec`) to store snake segment positions
- Memory is properly freed and reinitialized on game restart
- Address Sanitizer is enabled during compilation to detect memory issues

## Known Issues and Limitations

### Limitations

1. **Fixed window size**: The game window is hardcoded to 20×35 characters; the window is re-centered on resize but its dimensions do not change

2. **External dependency**: Requires the custom `cvector` library which is not included in the repository. Users must install or provide this library separately.

3. **No speed increase**: Game speed remains constant regardless of snake length

### Potential Issues

1. **Terminal size**: If your terminal is smaller than the minimum required size (37 columns × 22 rows), the game will exit with an error message at startup or during a resize event.

2. **Curses initialization**: The game assumes ncurses is properly installed and functional on your system

3. **Minimum terminal size**: Ensure your terminal window is at least 37 columns × 22 rows to display the game

### Recommendations

- Use a terminal with at least 37 columns × 22 rows for optimal display
- Ensure the `cvector` library is properly installed before building

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
- Implementing difficulty levels with increasing speed
- Implementing terminal size detection and adaptive window sizing
- Adding color support for better visuals

## Release Notes

### v1.1.0 — Score & High Score System (2026-03-01)

- **Scoring**: The player now earns one point for each food item eaten. The current score is displayed at the bottom center of the game window during play.
- **High score persistence**: The highest score achieved is stored in `files/high_score.txt` and persists across sessions. The file is updated immediately when the current score exceeds the stored high score.
- **New Highest Score notification**: When a collision ends the game and the player has beaten the previous high score, a "New Highest Score" message replaces the score line at the bottom of the window.
- **Food collision fix**: `snake_food_gen()` now re-rolls the food position if it overlaps any segment of the snake, ensuring food always spawns on a free cell.

### v1.0.0 — Initial Release

- Classic Snake gameplay in a 20×35 terminal window using ncurses
- Arrow key and vim-style (h/j/k/l) controls
- Self-collision and wall-collision detection
- Dynamic snake growth on eating food
- Terminal resize handling — game window re-centers automatically; exits gracefully if the terminal becomes too small (minimum 37 × 22)
- Minimum terminal size validation at startup
- Address Sanitizer enabled in debug builds

## Acknowledgments

- Built with ncurses for terminal graphics
- Uses a custom vector implementation for dynamic array management
