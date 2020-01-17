#pragma warning(disable:4996)
//#pragma warning(suppress: 6301) //SWIADOME UZYCIE
#include "CSolution.h"

CSolution::CSolution()
{
	i_suppliers_count = DEFAULT_ENTITY_SIZE;
	i_factories_count = DEFAULT_ENTITY_SIZE;
	i_warehouses_count = DEFAULT_ENTITY_SIZE;
	i_shops_count = DEFAULT_ENTITY_SIZE;
	i_solution_size = i_suppliers_count * i_factories_count + i_factories_count * i_warehouses_count + i_warehouses_count * i_shops_count;
	pf_file = NULL;
}

CSolution::CSolution(unsigned int iSuppliersCount, unsigned int iFactoriesCount, unsigned int iWarehousesCount, unsigned int iShopsCount, bool& bSuccess)
{
	bSuccess = true;
	i_suppliers_count = iSuppliersCount;
	i_factories_count = iFactoriesCount;
	i_warehouses_count = iWarehousesCount;
	i_shops_count = iShopsCount;
	i_solution_size = i_suppliers_count * i_factories_count + i_factories_count * i_warehouses_count + i_warehouses_count * i_shops_count;
	pf_file = NULL;

	t_solution.b_set_size(i_solution_size);
	m_delivery_qty_matrix.b_set_size(i_suppliers_count, i_factories_count);
	m_factory_qty_matrix.b_set_size(i_factories_count, i_warehouses_count);
	m_warehouse_qty_matrix.b_set_size(i_warehouses_count, i_shops_count);
	if (!bSuccess) CSolution();
}

CSolution::~CSolution()
{
}

CSolution::CSolution(const CSolution& cOther)
{
	vCopy(cOther);
}

CSolution& CSolution::operator=(const CSolution& cOther)
{
	vCopy(cOther);
	return *this;
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
	vInitTableFromMatrixes();

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

void CSolution::vInitTableFromMatrixes()
{
	int iSolutionIndex = 0;
	vInitTableTwoDim(m_delivery_qty_matrix, i_suppliers_count, i_factories_count, iSolutionIndex);
	vInitTableTwoDim(m_factory_qty_matrix, i_factories_count, i_warehouses_count, iSolutionIndex);
	vInitTableTwoDim(m_warehouse_qty_matrix, i_warehouses_count, i_shops_count, iSolutionIndex);
}

void CSolution::vInitMatrixesFromTable()
{
	int iSolutionIndex = 0;
	vInitMatrixTwoDim(m_delivery_qty_matrix, i_suppliers_count, i_factories_count, iSolutionIndex);
	vInitMatrixTwoDim(m_factory_qty_matrix, i_factories_count, i_warehouses_count, iSolutionIndex);
	vInitMatrixTwoDim(m_warehouse_qty_matrix, i_warehouses_count, i_shops_count, iSolutionIndex);
}

bool CSolution::bIdentical(CSolution& cOther)
{
	bool bSuccess = true;
	if (i_solution_size != cOther.i_solution_size) return false;
	for (int i = 0; i < i_solution_size; i++)
	{
		if (t_solution.d_get_value(i, bSuccess) != cOther.t_solution.d_get_value(i, bSuccess)) return false;
	}
	return true;
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

void CSolution::vInitTableTwoDim(CMatrix& matrixToInit, int iIndex, int jIndex, int& iSolutionIndex)
{
	bool bSuccess = true;
	for (int i = 0; i < iIndex; i++)
	{
		for (int j = 0; j < jIndex; j++)
		{
			t_solution.b_set_val(matrixToInit.d_get_val(i, j, bSuccess), iSolutionIndex);
			iSolutionIndex++;
		}
	}
}

void CSolution::vInitMatrixTwoDim(CMatrix& matrixToInit, int iIndex, int jIndex, int& iSolutionIndex)
{
	bool bSuccess = true;
	for (int i = 0; i < iIndex; i++)
	{
		for (int j = 0; j < jIndex; j++)
		{
			matrixToInit.b_set_value(t_solution.d_get_value(iSolutionIndex, bSuccess), i, j);
			iSolutionIndex++;
		}
	}
}

void CSolution::vCopy(const CSolution& cOther)
{
	i_solution_size = cOther.i_solution_size;
	i_suppliers_count = cOther.i_suppliers_count;
	i_factories_count = cOther.i_factories_count;
	i_warehouses_count = cOther.i_warehouses_count;
	i_shops_count = cOther.i_shops_count;

	t_solution = cOther.t_solution;
	m_delivery_qty_matrix = cOther.m_delivery_qty_matrix;
	m_factory_qty_matrix = cOther.m_factory_qty_matrix;
	m_warehouse_qty_matrix = cOther.m_warehouse_qty_matrix;
}
