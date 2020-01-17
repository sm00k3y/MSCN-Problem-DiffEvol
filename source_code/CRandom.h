#pragma once
#include "Constants.h"
#include <random>

class CRandom
{
public:
	CRandom();
	CRandom(int iSeed);

	CRandom& vSetRandomRange(double dMinVal, double dMaxVal);

	double dGenerateNumber();
	int iGenerateNumber();

private:
	std::random_device random_device;				// will be used to obtain a seed for the random number engine
	std::mt19937 generator;							// mersenne_twister_engine seeded with random_device creates a sequence of numbers
	std::uniform_real_distribution<double> random_generator;	// distributes random double from certain range
};

