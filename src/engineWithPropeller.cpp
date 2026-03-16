#include "engineWithPropeller.hpp"

EngineWithPropeller::EngineWithPropeller(double batteryVoltage, double motorKv, int rotationDirection,
		double propellerDiameter, double propellerPitch, double propellerMass, double propellerInertia,
		double thrustCoefficient, double torqueCoefficient,
		double initialRpm) {
	this->batteryVoltage = batteryVoltage;
	this->motorKv = motorKv;
	this->rotationDirection = rotationDirection;
	this->propellerDiameter = propellerDiameter;
	this->propellerPitch = propellerPitch;
	this->propellerMass = propellerMass;
	this->propellerInertia = propellerInertia;
	this->thrustCoefficient = thrustCoefficient;
	this->torqueCoefficient = torqueCoefficient;
	this->currentRpm = initialRpm;

	this->responseRate = responseRateConst / (propellerInertia + responseRateDevisor); // Calculate response rate based on inertia
}

EngineOutput EngineWithPropeller::calculateOutput(double throttle, double delta, double airDensity) {
	double targetRpm = throttle * this->motorKv * this->batteryVoltage; // Calculate target RPM based on throttle, motor Kv, and battery voltage

	double blendFactor = 1.0 - std::exp(-responseRate * delta);
	currentRpm += (targetRpm - currentRpm) * blendFactor;

	double rps = currentRpm / 60.0;

	double thrust = thrustCoefficient * airDensity * std::pow(rps, 2.0) * std::pow(propellerDiameter, 4.0);
	double torque = torqueCoefficient * airDensity * std::pow(rps, 2.0) * std::pow(propellerDiameter, 5.0) * rotationDirection;

	return { thrust, torque, currentRpm };
}

void EngineWithPropeller::changeResponseRate(double responseRateConst, double responseRateDevisor) {
	this->responseRateConst = responseRateConst;
	this->responseRateDevisor = responseRateDevisor;
	this->responseRate = responseRateConst / (propellerInertia + responseRateDevisor); // Recalculate response rate based on new values
}
