# Click Buffer - Geode Mod

Expands the click window in Geometry Dash for more forgiving inputs.

## Features

- **Buffer Frames**: Add extra frames before/after the perfect click frame
  - Default: 2 frames (makes 1-frame perfect → 3-frame perfect)
  - Configurable: 0-10 frames
- **Buffer Modes**:
  - `before` - Clicks slightly early still count
  - `after` - Clicks slightly late still count  
  - `both` - Both early and late clicks count (default)
- **Per-level toggle** via mod menu

## How It Works

The mod hooks `PlayerObject::pushButton` and `releaseButton` to intercept click inputs. It maintains a buffer of recent clicks with frame timestamps. When a click happens, it checks if there's a buffered click within the configured window that should register instead.

## Settings

| Setting | Default | Description |
|---------|---------|-------------|
| Enabled | `true` | Master toggle |
| Buffer Frames | `2` | Extra frames (0 = vanilla) |
| Buffer Mode | `both` | `before`, `after`, or `both` |
| Visual Indicator | `false` | Show buffer status |

## Building

```bash
# Set Geode SDK path if not in default location
export GEODE_SDK=/path/to/geode/sdk

# Build
./build.sh
```

## Installation

1. Build the mod (produces `.geode` file)
2. Drag `.geode` file into Geode mod loader
3. Enable in mod menu
4. Configure buffer frames in settings

## How Buffer Frames Work

| Buffer Frames | Effect |
|---------------|--------|
| 0 | Vanilla (1-frame perfect) |
| 1 | 2-frame perfect |
| 2 | 3-frame perfect (default) |
| 3 | 4-frame perfect |
| ... | ... |

The buffer applies symmetrically in "both" mode: ±N frames around the perfect frame.

## Technical Details

- Hooks `PlayerObject::pushButton` and `releaseButton`
- Maintains circular buffer of recent clicks with frame timestamps
- Checks buffer on each click for valid buffered inputs
- Clears buffer on level reset/death
- Minimal performance impact
