#include "CTable.h"
#define DEFAULT_SIZE 1
#define INITIAL_VAL -1

CTable::CTable()
{
	i_table_size = DEFAULT_SIZE;
	pd_table = new double[i_table_size];
}

CTable::CTable(int iTableSize, bool bSuccess)
{
	if (i_table_size < 1)
	{
		bSuccess = false;
		CTable();
	}
	else
	{
		i_table_size = iTableSize;
		initTable(&pd_table, i_table_size);
	}
}

CTable::~CTable()
{
	delete[] pd_table;
}

void CTable::initTable(double** tabToInit, int iSize)
{
	(*tabToInit) = new double[iSize];
	for (int i = 0; i < iSize; i++)
	{
		(*tabToInit)[i] = INITIAL_VAL;
	}
}

bool CTable::b_set_size(int iSize)
{
	if (iSize < 1) return false;
	int minSize = (i_table_size < iSize ? i_table_size : iSize);
	double* tempTab = new double[iSize];
	initTable(&tempTab, iSize);
	for (int i = 0; i < minSize; i++)
	{
		tempTab[i] = pd_table[i];
	}
	delete[] pd_table;
	pd_table = tempTab;
	i_table_size = iSize;
	return true;
}

bool CTable::b_set_val(double dVal, int iIndex)
{
	if (iIndex<0 || iIndex > i_table_size || dVal < 0) return false;
	pd_table[iIndex] = dVal;
	return true;
}
