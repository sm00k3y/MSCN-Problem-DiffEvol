#pragma once
#include "CProblem.h"
#include "CTimer.h"

class COptimizer
{
public:
	virtual bool bInitialize(CProblem& cProblem) = 0;
	virtual bool bRunAlgorithm() = 0;
	virtual CSolution cGetBestSolution(bool& bSuccess) { return cBestSolution; }

protected:
	CSolution cBestSolution;
};

