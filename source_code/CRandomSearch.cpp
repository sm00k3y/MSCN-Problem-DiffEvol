#include "CRandomSearch.h"

CRandomSearch::CRandomSearch()
{
}

bool CRandomSearch::bInitialize(CProblem& cProblem)
{
	pc_problem = &cProblem;
	return true;
}

bool CRandomSearch::bRunAlgorithm()
{
	if (pc_problem == nullptr)
	{
		std::cout << "ALGORITHM UNINITILIZED! CANNOT PROCEED" << std::endl;
		return false;
	}

	bool bFoundSuccessful = false;
	CRandom c_random;
	bool bSuccess = true;
	CSolution c_solution(pc_problem->i_get_supplier_count(), pc_problem->i_get_factory_count(), pc_problem->i_get_warehouse_count(), pc_problem->i_get_shop_count(), bSuccess);
	CSolution c_best_solution;
	c_best_solution = c_solution;

	double dCurrentProfit = 0;
	double dBestProfit = 0;

	CTimer c_timer;
	c_timer.vStart();

	do {

		vGenerateOneSolution(*pc_problem, c_solution);

		if (pc_problem->bConstraintsSatisfied(c_solution))
		{
			iNumOfSolutionsFound++;
			pc_problem->bGetQuality(c_solution, dCurrentProfit);
			if (dCurrentProfit > dBestProfit)
			{
				dBestProfit = dCurrentProfit;
				c_best_solution = c_solution;
				cBestSolution = c_solution;
				bFoundSuccessful = true;
			}
		}

	} while (c_timer.dCurrentTime() < SECONDS_TILL_STOP);

	return bFoundSuccessful;
}

CSolution* CRandomSearch::pcGetValidSolution(CProblem& cProblem, bool& bFoundSuccessful)
{
	bFoundSuccessful = true;
	CSolution* pc_solution = new CSolution(cProblem.i_get_supplier_count(), cProblem.i_get_factory_count(), cProblem.i_get_warehouse_count(), cProblem.i_get_shop_count(), bFoundSuccessful);

	int iFallbackTries = TRIES_UNTIL_FALLBACK;
	do
	{
		vGenerateOneSolution(cProblem, *pc_solution);
		iFallbackTries--;
	} while (!cProblem.bConstraintsSatisfied(*pc_solution) || iFallbackTries==0);

	if (iFallbackTries == 0) bFoundSuccessful = false;
	return pc_solution;
}

void CRandomSearch::vGenerateOneSolution(CProblem& cProblem, CSolution& cSolution)
{
	bool bSuccess = true;
	for (int i = 0; i < cSolution.i_get_solution_size(); i++)
	{
		cRandom.vSetRandomRange(cProblem.d_get_min_val_at(i, bSuccess), cProblem.d_get_max_val_at(i, bSuccess));
		cSolution.t_get_pdSolution().b_set_val(cRandom.dGenerateNumber(), i);
	}
	cSolution.vInitMatrixesFromTable();
}
