#pragma once
#include "CMscnProblem.h"
#include "COptimizer.h"

class CRandomSearch : public COptimizer
{
public:
	CRandomSearch();

	bool bInitialize(CProblem& cProblem);
	bool bRunAlgorithm();
	CSolution* pcGetValidSolution(CProblem& cProblem, bool& bFoundSuccessful);
	void vGenerateOneSolution(CProblem& cProblem, CSolution& cSolution);

	int iGetNumOfSolutionsFound() { return iNumOfSolutionsFound; }

private:
	CRandom cRandom;
	CProblem* pc_problem = nullptr;
	int iNumOfSolutionsFound = 0;
};

