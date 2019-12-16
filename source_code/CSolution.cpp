#include "CSolution.h"

CSolution::CSolution(int iSuppliersCount, int iFactoriesCount, int iWarehousesCount, int iShopsCount)
{
	i_suppliers_count = iSuppliersCount;
	i_factories_count = iFactoriesCount;
	i_warehouses_count = iWarehousesCount;
	i_shops_count = iShopsCount;
}

CSolution::~CSolution()
{
	for (int i = 0; i < i_suppliers_count; i++)
	{
		delete[] pd_delivery_matrix[i];
	}
	delete[] pd_delivery_matrix;
}

bool CSolution::bInitSolution(double* pdSolution)
{
	for (int i = 0; i < i_suppliers_count; i++)
	{
		pd_delivery_matrix[i] = pdSolution;
	}
	return true;
}