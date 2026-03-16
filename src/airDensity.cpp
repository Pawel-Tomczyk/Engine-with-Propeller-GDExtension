#include "airDensity.hpp"

AirDensity::AirDensity(double startAltitude, double startTemperature, double startPressure, double humidity) {
	this->altitude = startAltitude;
	this->temperature = startTemperature + 273.15; // Convert to Kelvin
	this->pressure = startPressure * 100; // Convert hPa to Pa
	this->humidity = humidity;

	// ISA standard conditions at sea level
	this->temperature0 = this->temperature + (L * this->altitude);
	this->pressure0 = this->pressure * pow((this->temperature0 / this->temperature), (g / (L * Rd)));

	this->density = isa_calculateDensity(); // Initial density calculation based on provided conditions
}

double AirDensity::getDensity() const {
	return this->density;
}

void AirDensity::updateAltitude(double newAltitude) {
	this->altitude = newAltitude;
	this->temperature = isa_calculateTemperature();
	this->pressure = isa_calculatePressure();
	this->density = isa_calculateDensity();
}

double AirDensity::getAltitude() const {
	return this->altitude;
}

double AirDensity::getTemperature() const {
	return this->temperature - 273.15; // Return in Celsius
}

double AirDensity::getPressure() const {
	return this->pressure / 100; // Return in hPa
}

double AirDensity::getHumidity() const {
	return this->humidity;
}

double AirDensity::isa_calculateTemperature() {
	return this->temperature0 - L * this->altitude;
}

double AirDensity::isa_calculatePressure() {
	return this->pressure0 * pow((this->temperature / this->temperature0), (g / (L * Rd)));
}

double AirDensity::isa_calculateDensity() {
	double tempCelsius = this->temperature - 273.15; // for magnus formula
	double saturationVaporPressure = 6.1078 * pow(10.0, (7.5 * tempCelsius) / (tempCelsius + 237.3)); // in hPa

	double actualVaporPressure = saturationVaporPressure * this->humidity; // in Pa - it works because in formula we devide devide by 100, and because of that hPa to Pa conversion is not needed here (we used 100 in hPa to negate devision by 100 in formula)

	double vaporDensity = actualVaporPressure / (Rv * this->temperature);
	double dryAirDensity = (this->pressure - actualVaporPressure) / (Rd * this->temperature); // Convert hPa to Pa for calculation
	return dryAirDensity + vaporDensity; // Total air density
}
