#pragma once
#include "CRandom.h"
#include "Constants.h"

class CTable
{
public:
	CTable();
	CTable(int iTableSize, bool& bSuccess);
	~CTable();

	CTable(const CTable& cOther);
	CTable& operator=(const CTable& cOther);

	bool b_set_size(int iSize);
	bool b_set_val(double dVal, int iIndex);
	double d_get_value(int index, bool& bSuccess);

	bool b_randomize_values(CRandom& cRandom);

private:
	void initTable(double** tabToInit, int iSize);
	void vCopyMethod(const CTable& cOther);

	double* pd_table;
	int i_table_size;
};

