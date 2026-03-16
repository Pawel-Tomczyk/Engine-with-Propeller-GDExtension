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

EngineWithPropeller::EngineWithPropeller() {
	this->batteryVoltage = 0.0;
	this->motorKv = 0.0;
	this->rotationDirection = 0.0;
	this->propellerDiameter = 0.0;
	this->propellerPitch = 0.0;
	this->propellerMass = 0.0;
	this->propellerInertia = 0.0;
	this->thrustCoefficient = 0.0;
	this->torqueCoefficient = 0.0;
	this->currentRpm = 0.0;

	this->responseRate = 0.0;
}

void EngineWithPropeller::setup(double batteryVoltage, double motorKv, int rotationDirection,
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

void EngineWithPropeller::_bind_methods() {
	// Rejestrujemy setup()
	godot::ClassDB::bind_method(godot::D_METHOD("setup", "batteryVoltage", "motorKv", "rotationDirection", "propellerDiameter", "propellerPitch", "propellerMass", "propellerInertia", "thrustCoefficient", "torqueCoefficient", "initialRpm"), &EngineWithPropeller::setup, DEFVAL(0.0));

	// Rejestrujemy główne obliczenia
	godot::ClassDB::bind_method(godot::D_METHOD("calculate_output", "throttle", "delta", "airDensity"), &EngineWithPropeller::calculateOutput);

	// Rejestrujemy zmianę inercji
	godot::ClassDB::bind_method(godot::D_METHOD("change_response_rate", "responseRateConst", "responseRateDevisor"), &EngineWithPropeller::changeResponseRate);
}

godot::Dictionary EngineWithPropeller::calculateOutput(double throttle, double delta, double airDensity) {
	double targetRpm = throttle * this->motorKv * this->batteryVoltage; // Calculate target RPM based on throttle, motor Kv, and battery voltage

	double blendFactor = 1.0 - std::exp(-responseRate * delta);
	currentRpm += (targetRpm - currentRpm) * blendFactor;

	double rps = currentRpm / 60.0;

	double thrust = thrustCoefficient * airDensity * std::pow(rps, 2.0) * std::pow(propellerDiameter, 4.0);
	double torque = torqueCoefficient * airDensity * std::pow(rps, 2.0) * std::pow(propellerDiameter, 5.0) * rotationDirection;

	godot::Dictionary output;
	output["thrust"] = thrust;
	output["torque"] = torque;
	output["rpm"] = currentRpm;
	return output;
}

void EngineWithPropeller::changeResponseRate(double responseRateConst, double responseRateDevisor) {
	this->responseRateConst = responseRateConst;
	this->responseRateDevisor = responseRateDevisor;
	this->responseRate = responseRateConst / (propellerInertia + responseRateDevisor); // Recalculate response rate based on new values
}
