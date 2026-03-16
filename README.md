# Engine with Propeller GDExtension

GDExtension for Godot 4.5 that exposes two C++ utility classes for simple propulsion and atmosphere calculations:

- `AirDensity` for estimating air density from altitude, temperature, pressure, and humidity.
- `EngineWithPropeller` for estimating thrust, torque, and RPM response of a motor-propeller pair.

The extension is implemented with `godot-cpp`, built with SCons, and loaded through `enginewithpropeller.gdextension`.

## What This Project Provides

This repository is a native C++ extension, not an editor plugin. It does not add a `plugin.cfg` or custom editor dock. Instead, it registers runtime classes that are available from GDScript, C#, and other Godot scripting APIs that can access GDExtension classes.

Registered classes:

- `AirDensity`
- `EngineWithPropeller`

Both classes inherit from `RefCounted`, so they can be created directly from script with `.new()`.

## Godot Compatibility

This project currently targets Godot `4.5`.

Relevant files:

- `test_project/enginewithpropeller/enginewithpropeller.gdextension`
- `test_project/project.godot`

If you want to target another Godot 4.x version, use the setup tool or update the generated configuration consistently across the repository.

## Repository Layout

- `src/` - C++ source for the extension classes and registration entry point.
- `godot-cpp/` - upstream bindings submodule.
- `test_project/` - small Godot project used to load and test the extension.
- `doc_classes/` - XML documentation sources for editor class reference generation.
- `tools/` - helper scripts for renaming, version changes, building, and docs generation.
- `setup.py` - interactive project setup and build helper.

## Exposed API

### AirDensity

Purpose:

- Store atmospheric conditions.
- Recalculate density after altitude changes.
- Expose the current state back to Godot.

Available methods in Godot:

- `setup(startAltitude, startTemperature, startPressure, humidity)`
- `update_altitude(newAltitude)`
- `get_density()`
- `get_altitude()`
- `get_temperature()`
- `get_pressure()`
- `get_humidity()`

Parameter conventions:

- altitude in meters
- temperature in degrees Celsius
- pressure in hPa
- humidity in range `0.0 .. 1.0`
- density result in `kg/m^3`

### EngineWithPropeller

Purpose:

- Store motor and propeller parameters.
- Smooth RPM toward a throttle-dependent target.
- Return estimated thrust, torque, and RPM.

Available methods in Godot:

- `setup(batteryVoltage, motorKv, rotationDirection, propellerDiameter, propellerPitch, propellerMass, propellerInertia, thrustCoefficient, torqueCoefficient, initialRpm := 0.0)`
- `calculate_output(throttle, delta, airDensity)`
- `change_response_rate(responseRateConst, responseRateDevisor)`

Parameter conventions:

- battery voltage in volts
- motor Kv in RPM per volt
- `rotationDirection` as `1` or `-1`
- propeller diameter and pitch in meters
- propeller mass in kilograms
- propeller inertia in `kg*m^2`
- thrust in newtons
- torque in newton-meters
- RPM in revolutions per minute

Return value of `calculate_output`:

```gdscript
{
    "thrust": float,
    "torque": float,
    "rpm": float
}
```

## Example Usage In GDScript

```gdscript
extends Node

func _ready() -> void:
	var air_density := AirDensity.new()
	air_density.setup(0.0, 20.0, 1013.25, 0.40)

	var engine := EngineWithPropeller.new()
	engine.setup(
		14.8,    # batteryVoltage
		920.0,   # motorKv
		1,       # rotationDirection
		0.254,   # propellerDiameter
		0.1143,  # propellerPitch
		0.020,   # propellerMass
		0.00012, # propellerInertia
		0.11,    # thrustCoefficient
		0.04,    # torqueCoefficient
		0.0      # initialRpm
	)

	var output := engine.calculate_output(0.65, 1.0 / 60.0, air_density.get_density())
	print(output)
```

## Building

### Requirements

- Python 3
- SCons
- a working C++ toolchain for your platform
- initialized `godot-cpp` submodule

Initialize submodules if needed:

```bash
git submodule update --init --recursive
```

Install SCons if it is not already available:

```bash
python3 -m pip install scons
```

### Option 1: Interactive Setup Tool

Run:

```bash
python3 setup.py
```

Available actions:

1. Change target Godot version
2. Change build profile
3. Rename plugin
4. Compile debug build
5. Generate missing XML docs

### Option 2: Build With SCons Directly

Typical debug build on Linux:

```bash
scons platform=linux target=template_debug
```

Typical debug build on Windows:

```bash
scons platform=windows target=template_debug
```

Typical debug build on macOS:

```bash
scons platform=macos target=template_debug
```

Build outputs are written into `bin/<platform>/` and then copied into:

```text
test_project/enginewithpropeller/bin/<platform>/
```

## Loading In Godot

The test project already contains the extension manifest:

- `test_project/enginewithpropeller/enginewithpropeller.gdextension`

Godot will load the native library when the project starts, as long as the matching binary for your platform exists in the expected `bin` directory.

## Documentation Generation

The project supports Godot XML class documentation generation through the files in `doc_classes/`.

You can generate missing XML files with:

```bash
python3 setup.py
```

Then choose option `5`.

If your target/editor combination supports it, those docs are compiled into generated source and can be surfaced in the Godot editor.

## Current State

What is already in place:

- GDExtension entry point and class registration
- Linux debug binary present in `bin/linux/`
- test project configured to target Godot 4.5

What still needs cleanup:

- `test_project/main.gd` is still template/demo code and does not yet demonstrate `AirDensity` or `EngineWithPropeller`
- public-facing docs and examples should be kept in sync with the native API as it evolves

## Development Notes

- Class registration happens in `src/register_types.cpp`.
- Godot method names are defined in `_bind_methods()` in each class implementation.
- The extension initializes at `MODULE_INITIALIZATION_LEVEL_SCENE`.
- `EngineWithPropeller` uses a simple exponential blend toward target RPM, not a full motor/ESC/propeller physics model.
- `AirDensity` currently updates density by recalculating temperature and pressure from stored ISA-derived baseline values.

## License

See `LICENSE`.
