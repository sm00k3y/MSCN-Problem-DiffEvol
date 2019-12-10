#pragma once
class CTable
{
public:
	CTable();
	CTable(int iTableSize, bool bSuccess);
	~CTable();
	void initTable(double** tabToInit, int iSize);

	bool b_set_size(int iSize);
	bool b_set_val(double dVal, int iIndex);
private:
	double* pd_table;
	int i_table_size;
};

