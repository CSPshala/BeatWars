#ifndef RANDOM_H
#define RANDOM_H

class Random
{
public:
	static const long Next(const long nMin, const long nMax);
	static const double NextDouble(double dMin, double dMax);
};

#endif