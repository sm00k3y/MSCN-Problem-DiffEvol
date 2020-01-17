#include "CDiffEvol.h"

CDiffEvol::CDiffEvol(int populationSize)
{
	iPopulationSize = populationSize;
	dCrossProb = CROSS_PROB;
	dDiffWeight = DIFF_WEIGHT;
	cRandomReal.vSetRandomRange(0, 1);
}

CDiffEvol::~CDiffEvol()
{
	for (int i = 0; i < v_population.size(); i++)
	{
		delete v_population.at(i);
	}
	v_population.clear();
}

void CDiffEvol::vSetConstans(double crossProb, double diffWeight)
{
	dCrossProb = crossProb;
	dDiffWeight = diffWeight;
}

bool CDiffEvol::bInitialize(CProblem& cProblem)
{
	pc_problem = &cProblem;
	if (!bInitPopulation(iPopulationSize))
	{
		std::cout << "WARNING! POPULATION INITIALIZATION TIMEOUT!\n" << std::endl;
		return false;
	}
	else
	{
		std::cout << "POPULATION INITIALIZED\n" << std::endl;
	}
	return true;
}

bool CDiffEvol::bInitPopulation(int iPopulationSize)
{
	CRandomSearch cRandomSearch;
	bool bSuccess = true;

	for (int i = 0; i < iPopulationSize; i++)
	{
		CSolution* pc_solution = cRandomSearch.pcGetValidSolution(*pc_problem, bSuccess);
		if (bSuccess) v_population.push_back(pc_solution);
		else return false;
	}

	cRandomInt.vSetRandomRange(0, v_population.size());

	return true;
}

bool CDiffEvol::bRunAlgorithm()
{
	if (pc_problem == nullptr)
	{
		std::cout << "ALGORITHM UNINITILIZED! CANNOT PROCEED" << std::endl;
		return false;
	}

	double dTempVal;
	double dTempQuality;
	bool bSuccess = true;
	CSolution *ind, *baseInd, *addInd0, *addInd1;

	for (int j = 0; j < DEFAULT_NUMBER_OF_ITERATIONS; j++)
	{
		for (int i = 0; i < v_population.size(); i++)
		{
			ind = v_population.at(i);
			baseInd = pcGetRandomInd();
			addInd0 = pcGetRandomInd();
			addInd1 = pcGetRandomInd();

			if (bIndividualsAreDifferent(ind, baseInd, addInd0, addInd1))
			{
				CSolution* newInd = new CSolution(*ind);
				for (int geneOffset = 0; geneOffset < ind->i_get_solution_size(); geneOffset++)
				{
					if (cRandomReal.dGenerateNumber() < dCrossProb)
					{
						dTempVal = baseInd->t_get_pdSolution().d_get_value(geneOffset, bSuccess) + dDiffWeight * (addInd0->t_get_pdSolution().d_get_value(geneOffset, bSuccess) - addInd1->t_get_pdSolution().d_get_value(geneOffset, bSuccess));

						if (dTempVal < pc_problem->d_get_min_val_at(geneOffset, bSuccess))
						{
							dTempVal = pc_problem->d_get_min_val_at(geneOffset, bSuccess);
						}

						if (dTempVal > pc_problem->d_get_max_val_at(geneOffset, bSuccess))
						{
							dTempVal = pc_problem->d_get_max_val_at(geneOffset, bSuccess);
						}

						newInd->t_get_pdSolution().b_set_val(dTempVal, geneOffset);
					}
				}
				newInd->vInitMatrixesFromTable();
				pc_problem->bGetQuality(*newInd, dTempQuality);
				pc_problem->bGetQuality(*ind, dTempVal);
				if (dTempQuality >= dTempVal && pc_problem->bConstraintsSatisfied(*newInd))
				{
					//delete v_population.at(i);
					delete ind;
					v_population.erase(v_population.begin() + i);
					v_population.insert(v_population.begin() + i, newInd);
				}
				else
				{
					delete newInd;
				}
			}
		}
	}
	return bSuccess;
}

CSolution CDiffEvol::cGetBestSolution(bool& bSuccess)
{
	int index = 0;
	double dBestQuality = 0;
	double dTempQuality = 0;

	for (int i = 0; i < v_population.size(); i++)
	{
		pc_problem->bGetQuality(*v_population.at(i), dTempQuality);
		if (dTempQuality > dBestQuality)
		{
			dBestQuality = dTempQuality;
			index = i;
		}
	}

	return *v_population.at(index);
}

CSolution* CDiffEvol::pcGetRandomInd()
{
	return v_population.at((int)cRandomInt.dGenerateNumber());
}

bool CDiffEvol::bIndividualsAreDifferent(CSolution* ind, CSolution* baseInd, CSolution* addInd0, CSolution* addInd1)
{
	if (ind == baseInd || ind == addInd0 || ind == addInd1 || baseInd == addInd0 || baseInd == addInd1 || addInd0 == addInd1) return false;
	if (ind->bIdentical(*baseInd) || ind->bIdentical(*addInd0) || ind->bIdentical(*addInd1) || baseInd->bIdentical(*addInd0) || baseInd->bIdentical(*addInd1) || addInd0->bIdentical(*addInd1)) return false;
	return true;
}
