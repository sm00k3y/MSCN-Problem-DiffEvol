// TEP_Lista9.cpp

#include <iostream>
#include <vector>
#include "CMscnProblem.h"

int main()
{
	CMscnProblem cProblem;
	double* pdSolution;
	int iSolutionSize = 0;
	double dResult = 0;
	bool bConstraintsSatisfiedSolution;

	cProblem.bReadFromFile("test2.txt");
	cProblem.bReadSolutionFromFile("solution2.txt", &pdSolution, iSolutionSize);
	bConstraintsSatisfiedSolution = cProblem.bConstraintsSatisfied(pdSolution, iSolutionSize);
	cProblem.bGetQuality(pdSolution, iSolutionSize, dResult);

	if(bConstraintsSatisfiedSolution == 0) std::cout << "bConstrainsSatisfied: FALSE" << std::endl;
	else std::cout << "bConstrainsSatisfied: TRUE" << std::endl;
	std::cout << "bGetQuality: " << dResult << std::endl;
	
	return 0;
}