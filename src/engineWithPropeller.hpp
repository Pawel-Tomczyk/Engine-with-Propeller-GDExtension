#pragma once
#include <math.h>
#include <godot_cpp/classes/ref_counted.hpp> // For RefCounted base class
#include <godot_cpp/variant/dictionary.hpp> // For returning multiple values in a structured way

struct EngineOutput {
	double thrust; // in Newtons
	double torque; // in Newton-meters
	double CurrentRpm; // in revolutions per minute
};

class EngineWithPropeller : public godot::RefCounted {
	GDCLASS(EngineWithPropeller, RefCounted) // Macro to enable Godot's class system and reflection
protected:
	static void _bind_methods(); // Method to bind C++ methods to Godot's scripting system

public:
	EngineWithPropeller(); // Default constructor, allows for later initialization with setup method

	EngineWithPropeller(
			double batteryVoltage, double motorKv, int rotationDirection,
			double propellerDiameter, double propellerPitch, double propellerMass, double propellerInertia,
			double thrustCoefficient, double torqueCoefficient,
			double initialRpm = 0);

	void setup(double batteryVoltage, double motorKv, int rotationDirection,
			double propellerDiameter, double propellerPitch, double propellerMass, double propellerInertia,
			double thrustCoefficient, double torqueCoefficient, double initialRpm = 0.0);

	~EngineWithPropeller() = default; // Default destructor, no special cleanup needed

	godot::Dictionary calculateOutput(double throttle, double delta, double airDensity);

	void changeResponseRate(double responseRateConst, double responseRateDevisor);

private:
	double responseRateConst = 10;
	double responseRateDevisor = 0.0001; // To prevent division by zero in case of zero inertia
	double responseRate; // Calculated based on inertia, will be updated in constructor

	double batteryVoltage; // in Volts
	double motorKv; // in RPM/V

	int rotationDirection; // 1 for clockwise, -1 for counterclockwise
	double propellerDiameter; // in meters
	double propellerPitch; // in meters
	double propellerMass; // in kg
	double propellerInertia; // in kg·m^2

	double thrustCoefficient; // dimensionless
	double torqueCoefficient; // dimensionless

	double currentRpm;
};
