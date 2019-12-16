#include "CTable.h"
#define DEFAULT_SIZE 1
#define INITIAL_VAL -1

CTable::CTable()
{
	i_table_size = DEFAULT_SIZE;
	initTable(&pd_table, i_table_size);
}

CTable::CTable(int iTableSize, bool& bSuccess)
{
	i_table_size = iTableSize;
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

void CTable::vCopyMethod(const CTable& cOther)
{
	pd_table = new double[cOther.i_table_size];
	i_table_size = cOther.i_table_size;
	for (int i = 0; i < i_table_size; i++)
	{
		pd_table[i] = cOther.pd_table[i];
	}
}

CTable::CTable(const CTable& cOther)
{
	vCopyMethod(cOther);
}

CTable CTable::operator=(const CTable& cOther)
{
	delete[] pd_table;
	vCopyMethod(cOther);
	return *this;
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

double CTable::d_get_value(int index, bool& bSuccess)
{
	bSuccess = true;
	if (index < 0 || index > i_table_size)
	{
		bSuccess = false;
		return -1;
	}
	else return pd_table[index];
}
