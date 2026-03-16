#pragma once
#include <math.h>

struct EngineOutput {
	double thrust; // in Newtons
	double torque; // in Newton-meters
	double CurrentRpm; // in revolutions per minute
};

class EngineWithPropeller {
public:
	EngineWithPropeller(
			double batteryVoltage, double motorKv, int rotationDirection,
			double propellerDiameter, double propellerPitch, double propellerMass, double propellerInertia,
			double thrustCoefficient, double torqueCoefficient,
			double initialRpm = 0);

	~EngineWithPropeller();

	EngineOutput calculateOutput(double throttle, double delta, double airDensity);

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
