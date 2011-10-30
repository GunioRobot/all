const float o2_in_air_perc = 0.20946f;
const float n2_in_air_perc = 0.78084f;

//http://en.wikipedia.org/wiki/Atmospheric_pressure
const float p0 = 101325; // Pa
const float pa2atm_ratio = 1.f / p0;
const float L = 0.0065f; // K/m
const float T0 = 288.15f; // K
const float g = 9.80655f; // m/s
const float M = 0.0289644f; // kg/mol

const float water_density = 1000; // density of water Kg/m3
const float water_molar_mass = 18.01528f;

const float molar_mass_n2 = 28.0134f;

//http://en.wikipedia.org/wiki/Gas_constant
const float R = 8.3144621f; // J/(mol * k)

//Features
// Oxygen intake @ pressure
// Dissolved nitrogen @ pressure

#include <cmath>

// must be in open air
float atmospheres_at_height(float h) {
	float exp = (g * M) / (R * L);
	float val = 1.f - ((L * h) / T0);
	float p = powf(val, exp);
	return p;
}

// underwater only
float water_pressure_at(float depth) {
	float p = 1.f + (water_density * (g * depth) * pa2atm_ratio);
	return p;
}

float pa_to_atm(float pa) { return (pa / p0); }
float atm_to_pa(float atm) { return (atm * p0); }

float oxygen_pp_at(float atm) { return (atm * o2_in_air_perc); }
float soxygen_pp_at(float atm, float saturated_vapor_atm) { return ((atm - saturated_vapor_atm) * o2_in_air_perc); }
float nitrogen_pp_at(float atm) { return (atm * n2_in_air_perc); }

// human_weight(kg) * percent_weight_blood / blood_density (kg / m^3)
float blood_in_body(float human_weight) { return (human_weight * 0.08f / 1.06f); }

//http://www.ccmtutorials.com/rs/oxygen/page02.htm

#include <cstdio>

int main() {
	float depth = 20.f;
	float atm = water_pressure_at(depth);
	float npp = nitrogen_pp_at(atm);

	printf("Atmospheres @ depth of %i: %f (N2 partial pressure: %f)\n", (int)depth, atm, npp);

	/////////

	printf("Litres blood in 70kg male: %f\n", blood_in_body(70.0));

	return 0;
}
