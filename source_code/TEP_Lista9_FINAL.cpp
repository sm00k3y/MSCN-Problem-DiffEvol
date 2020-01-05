// TEP_Lista9.cpp

#include <iostream>
#include "CMscnProblem.h"
#include "CSolution.h"

int main()
{
	CMscnProblem cProblem;
	CSolution cSolution;
	double dResult = 0;
	bool bConstraintsSatisfiedSolution;

	cProblem.bReadFromFile("test2.txt");
	cSolution.bReadSolutionFromFile("solution2.txt");
	bConstraintsSatisfiedSolution = cProblem.bConstraintsSatisfied(cSolution);
	cProblem.bGetQuality(cSolution, dResult);

	if (bConstraintsSatisfiedSolution == 0) std::cout << "bConstrainsSatisfied: FALSE" << std::endl;
	else std::cout << "bConstrainsSatisfied: TRUE" << std::endl;
	std::cout << "bGetQuality: " << dResult << std::endl;

	return 0;
}