#include "CRandom.h"

CRandom::CRandom()
{
	generator = std::mt19937(random_device());
	vSetRandomRange(DEFAULT_RANDOM_MIN_VAL, DEFAULT_RANDOM_MAX_VAL);
}

CRandom::CRandom(int iSeed)
{
	generator = std::mt19937(iSeed);
	vSetRandomRange(DEFAULT_RANDOM_MIN_VAL, DEFAULT_RANDOM_MAX_VAL);
}

CRandom& CRandom::vSetRandomRange(double dMinVal, double dMaxVal)
{
	random_generator = std::uniform_real_distribution<double>(dMinVal, dMaxVal);
	return *this;
}

double CRandom::dGenerateNumber()
{
	return random_generator(generator);
}

int CRandom::iGenerateNumber()
{
	return (int)random_generator(generator);
}
