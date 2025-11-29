# Mini Factory

Spritesheet used: https://sierrassets.itch.io/automation-tileset

Rename `automation spritesheet.png` to `automation_spritesheet.png` and save to new directory `assets/`.

## Layout Format

Create new file `layout.txt` under `config/`. To add a new tile, sepecify the tile ID and position on separate lines using the below format:

```
<TILE_ID> <X position> <Y position>
```

**Note**: Empty lines are permitted.

## Building

### Command Line

```sh
mkdir build
cd build
cmake -SDL2_DIR=/path/to/SDL2/cmake -SDL2_Image_DIR=/path/to/SDL2_image/cmake ..
cmake --build .
```

### VS Code with CMake extension

In `settings.json`:

```json
{
    "cmake.configureSettings": {
        "SDL2_DIR": "/path/to/SDL2/cmake",
        "SDL2_Image_DIR": "/path/to/SDL2_image/cmake"
    }
}
```

## Usage

On Windows, copy `SDL2.dll` into the same directory (i.e. `build/`) as `Mini_Factory.exe`.
