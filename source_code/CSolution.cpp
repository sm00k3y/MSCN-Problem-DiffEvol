#pragma warning(disable:4996)
//#pragma warning(suppress: 6301) //SWIADOME UZYCIE
#include "CSolution.h"
#define DEFAULT_SOLUTION_ENT_SIZE 1
#define MAX_LINE_LEN 2000

CSolution::CSolution()
{
	i_suppliers_count = DEFAULT_SOLUTION_ENT_SIZE;
	i_factories_count = DEFAULT_SOLUTION_ENT_SIZE;
	i_warehouses_count = DEFAULT_SOLUTION_ENT_SIZE;
	i_shops_count = DEFAULT_SOLUTION_ENT_SIZE;
	i_solution_size = i_suppliers_count * i_factories_count + i_factories_count * i_warehouses_count + i_warehouses_count * i_shops_count;
	pf_file = NULL;
}

CSolution::~CSolution()
{
}

bool CSolution::bReadSolutionFromFile(std::string sFileName)
{
	if (pf_file != NULL) fclose(pf_file);
	pf_file = fopen(sFileName.c_str(), "r");
	if (pf_file == NULL) return false;

	char cLine[MAX_LINE_LEN];
	double dTempVal = 0;

	fscanf(pf_file, "%s ", cLine);
	fscanf(pf_file, "%i", &i_suppliers_count);
	fscanf(pf_file, "%s ", cLine);
	fscanf(pf_file, "%i", &i_factories_count);
	fscanf(pf_file, "%s ", cLine);
	fscanf(pf_file, "%i", &i_warehouses_count);
	fscanf(pf_file, "%s ", cLine);
	fscanf(pf_file, "%i", &i_shops_count);

	i_solution_size = i_suppliers_count * i_factories_count + i_factories_count * i_warehouses_count + i_warehouses_count * i_shops_count;

	t_solution.b_set_size(i_solution_size);
	m_delivery_qty_matrix.b_set_size(i_suppliers_count, i_factories_count);
	m_factory_qty_matrix.b_set_size(i_factories_count, i_warehouses_count);
	m_warehouse_qty_matrix.b_set_size(i_warehouses_count, i_shops_count);

	int iTempSolutionIndex = 0;
	bSetSolutionMatrix(m_delivery_qty_matrix, i_suppliers_count, i_factories_count, iTempSolutionIndex);
	bSetSolutionMatrix(m_factory_qty_matrix, i_factories_count, i_warehouses_count, iTempSolutionIndex);
	bSetSolutionMatrix(m_warehouse_qty_matrix, i_warehouses_count, i_shops_count, iTempSolutionIndex);

	fclose(pf_file);
	pf_file = NULL;
	return true;
}

bool CSolution::bSumOfFactoriesProductsGreaterThanZero(int iSupplierNumber, bool& bSuccess)
{
	return m_delivery_qty_matrix.d_get_second_dim_sum(iSupplierNumber, bSuccess) > 0;
}

bool CSolution::bSumOfWarehousesProductsGraterThanzero(int iFactoryNumber, bool& bSuccess)
{
	return m_factory_qty_matrix.d_get_second_dim_sum(iFactoryNumber, bSuccess) > 0;
}

bool CSolution::bSumOfShopsProductsGreaterThanZero(int iWarehouseNumber, bool& bSuccess)
{
	return m_warehouse_qty_matrix.d_get_second_dim_sum(iWarehouseNumber, bSuccess) > 0;
}

bool CSolution::bSetSolutionMatrix(CMatrix& matrixToSet, int iIndex, int jIndex, int& iSolutionIndex)
{
	char cLine[MAX_LINE_LEN];
	double dTempVal = 0;
	fscanf(pf_file, "%s ", cLine);
	for (int i = 0; i < iIndex; i++)
	{
		for (int j = 0; j < jIndex; j++)
		{
			fscanf(pf_file, "%lf", &dTempVal);
			matrixToSet.b_set_value(dTempVal, i, j);
			t_solution.b_set_val(dTempVal, iSolutionIndex);
			iSolutionIndex++;
		}
	}
	return true;
}
