#include "Random.h"
#include <random>

const long Random::Next(const long nMin, const long nMax)
{
	std::random_device rseed;
	std::mt19937 rng(rseed());
	std::uniform_int<long> rand(nMin, nMax);
	return rand(rng);
}
const double Random::NextDouble(double dMin, double dMax)
{
	std::random_device rseed;
	std::mt19937 rng(rseed());
	std::uniform_real<double> u(dMin, dMax);
	std::variate_generator<std::mt19937&, std::uniform_real<double>> gen(rng, u); 
	return gen();
}
