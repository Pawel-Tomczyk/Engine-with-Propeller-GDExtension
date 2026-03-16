#pragma once
#include <cmath>
#include <godot_cpp/classes/ref_counted.hpp> // For RefCounted base class
#include <godot_cpp/variant/dictionary.hpp> // For returning multiple values in a structured way

class AirDensity : public godot::RefCounted {
	GDCLASS(AirDensity, RefCounted); // Macro to enable Godot's class system and reflection
protected:
	static void _bind_methods(); // Method to bind C++ methods to Godot's scripting system

public:
	// Constructor to initialize the air density calculator with starting conditions:
	// startAltitude in meters, startTemperature in Celsius, startPressure in hecto Pascals, humidity in 0..1
	AirDensity();
	AirDensity(double startAltitude, double startTemperature, double startPressure, double humidity);

	void setup(double startAltitude, double startTemperature, double startPressure, double humidity);

	~AirDensity() = default;

	double getDensity() const; // Return the current air density based on the stored conditions
	// TODO: implementacja metody getDensity, która będzie obliczać gęstość powietrza na podstawie aktualnych warunków (altitude, temperature, pressure, humidity) bez zmiany stanu przechowywanego w obiekcie. Może to być przydatne do szybkiego obliczania gęstości dla różnych warunków bez konieczności aktualizowania stanu obiektu.
	// double getDensity(double altitude, double temperature, double pressure, double humidity); // Overloaded method to calculate density based on provided conditions without changing the stored state

	void updateAltitude(double newAltitude);

	// Simple getters for overloaded getDensity method
	double getAltitude() const; // Return the current altitude in meters
	double getTemperature() const; // Return the current temperature in Celsius
	double getPressure() const; // Return the current pressure in hPa
	double getHumidity() const; // Return the current humidity in percentage

private:
	// Constants for ISA calculations
	static constexpr double L = 0.0065; // Temperature lapse rate in K/m
	static constexpr double Rd = 287.058; // Specific gas constant for dry air in J/(kg·K)
	static constexpr double Rv = 461.495; // Specific gas constant for water vapor in J/(kg·K)
	static constexpr double g = 9.80665; // Gravitational acceleration in m/s^2

	double density; // in kg/m^3
	double altitude; // in meters
	double temperature; // in Kelvin
	double pressure; // in Pa
	double humidity; // in percentage

	double temperature0; // Temperature at sea level (ISA standard)
	double pressure0; // Pressure at sea level (ISA standard)

	double isa_calculateTemperature();
	double isa_calculatePressure();

	double isa_calculateDensity();
};
