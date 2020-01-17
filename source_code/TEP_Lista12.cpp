// TEP_Lista11.cpp

#include <iostream>
#include "CMscnProblem.h"
#include "CSolution.h"
#include "CRandomSearch.h"
#include "CDiffEvol.h"

void vVirtualClassTest()
{
	//----------------------------Tworzenie obiektow---------------------------------
	bool bSuccess = true;
	double dResult;
	CSolution cBestSolution;

	CProblem* pc_problem = new CMscnProblem(3, 2, 4, 2, bSuccess);
	pc_problem->vGenerateInstance(200);

	CDiffEvol cDiffEvol(DEFAULT_POPULATION_SIZE);
	CRandomSearch cRandomSearch;

	//---------------------------- Ewolucja roznicowa -------------------------------

	COptimizer* pc_optimizer;
	
	pc_optimizer = &cDiffEvol;

	pc_optimizer->bInitialize(*pc_problem);

	std::cout << "INITIAL BEST SOLUTION FROM VECTOR: " << std::endl;
	cBestSolution = pc_optimizer->cGetBestSolution(bSuccess);
	std::cout << "bConstrainsSatisfied: " << std::boolalpha << pc_problem->bConstraintsSatisfied(cBestSolution) << std::endl;
	pc_problem->bGetQuality(cBestSolution, dResult);
	std::cout << "bGetQuality: " << dResult << "\n" << std::endl;

	pc_optimizer->bRunAlgorithm();

	cBestSolution = pc_optimizer->cGetBestSolution(bSuccess);

	if (bSuccess)
	{
		std::cout << "BEST SOLUTION AFTER RUNNING AN ALGORITHM" << std::endl;
		std::cout << "bConstrainsSatisfied: " << std::boolalpha << pc_problem->bConstraintsSatisfied(cBestSolution) << std::endl;
		pc_problem->bGetQuality(cBestSolution, dResult);
		std::cout << "bGetQuality: " << dResult << std::endl;
		//std::cout << "Number of solutions found: " << iNumOfSolutionsFound << std::endl;
	}
	else
	{
		std::cout << "Not even one solution found..." << std::endl;
	}

	//--------------------------------- Random search --------------------------------
	
	pc_optimizer = &cRandomSearch;

	pc_optimizer->bInitialize(*pc_problem);
	pc_optimizer->bRunAlgorithm();

	cBestSolution = pc_optimizer->cGetBestSolution(bSuccess);

	if (bSuccess)
	{
		std::cout << "\n\nBEST SOLUTION AFTER RUNNING RANDOM_SEARCH" << std::endl;
		std::cout << "bConstrainsSatisfied: " << std::boolalpha << pc_problem->bConstraintsSatisfied(cBestSolution) << std::endl;
		pc_problem->bGetQuality(cBestSolution, dResult);
		std::cout << "bGetQuality: " << dResult << std::endl;;
		std::cout << "Number of solutions found: " << cRandomSearch.iGetNumOfSolutionsFound() << std::endl;
	}
	else
	{
		std::cout << "Not even one solution found..." << std::endl;
	}

	delete pc_problem;
}

void vFixingSolutionTest()
{
	bool bSuccess = true;
	double dResult;

	CMscnProblem c_problem(3, 2, 4, 2, bSuccess);
	c_problem.vGenerateInstance(200);

	CRandomSearch cRandomSearch;
	CSolution cSolution(c_problem.i_get_supplier_count(), c_problem.i_get_factory_count(), c_problem.i_get_warehouse_count(), c_problem. i_get_shop_count(), bSuccess);
	cRandomSearch.vGenerateOneSolution(c_problem, cSolution);

	std::cout << "RANDOM SOLUTION TEST" << std::endl;
	c_problem.bGetQuality(cSolution, dResult);
	std::cout << "bConstrainsSatisfied: " << std::boolalpha << c_problem.bConstraintsSatisfied(cSolution) << std::endl;
	std::cout << "bGetQuality: " << dResult << std::endl;
}

int main()
{
	vVirtualClassTest();
	//vFixingSolutionTest();

	return 0;
}