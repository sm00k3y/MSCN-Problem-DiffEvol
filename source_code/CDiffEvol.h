#pragma once
#include "Constants.h"
#include "CSolution.h"
#include "CRandomSearch.h"
#include "COptimizer.h"

class CDiffEvol : public COptimizer
{
public:
	CDiffEvol(int populationSize);
	~CDiffEvol();
	void vSetConstans(double crossProb, double diffWeight);
	bool bInitialize(CProblem& cProblem);
	bool bRunAlgorithm();
	CSolution cGetBestSolution(bool& bSuccess);

private:
	bool bInitPopulation(int iPopulationSize);
	CSolution* pcGetRandomInd();
	bool bIndividualsAreDifferent(CSolution* ind, CSolution* baseInd, CSolution* addInd0, CSolution* addInd1);

	std::vector<CSolution*> v_population;
	CProblem* pc_problem = nullptr;

	int iPopulationSize;
	double dCrossProb;
	double dDiffWeight;

	CRandom cRandomInt;
	CRandom cRandomReal;
};

