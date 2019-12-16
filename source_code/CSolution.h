#pragma once
#include "CMatrix.h"
class CSolution
{
public:
	CSolution(int iSuppliersCount, int iFactoriesCount, int iWarehousesCount, int iShopsCount);
	~CSolution();

	bool bInitSolution(double* pdSolution);
private:
	double** pd_delivery_matrix;
	double** pd_factory_matrix;
	double** pd_warehouse_matrix;

	int i_suppliers_count;
	int i_factories_count;
	int i_warehouses_count;
	int i_shops_count;
};

