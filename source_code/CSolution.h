#pragma once
#include "CMatrix.h"
#include "CTable.h"
#include <iostream>

class CSolution
{
public:
	CSolution();
	~CSolution();

	int i_get_solution_size() { return i_solution_size; }
	int i_get_suppliers_count() { return i_suppliers_count; }
	int i_get_factories_count() { return i_factories_count; }
	int i_get_warehouses_count() { return i_warehouses_count; }
	int i_get_shops_count() { return i_shops_count; }
	CTable t_get_pdSolution() { return t_solution; }
	CMatrix m_get_delivery_qty_matrix() { return m_delivery_qty_matrix; }
	CMatrix m_get_factory_qty_matrix() { return m_factory_qty_matrix; }
	CMatrix m_get_warehouse_qty_matrix() { return m_warehouse_qty_matrix; }

	bool bReadSolutionFromFile(std::string sFileName);
	bool bSumOfFactoriesProductsGreaterThanZero(int iSupplierNumber, bool& bSuccess);
	bool bSumOfWarehousesProductsGraterThanzero(int iFactoryNumber, bool& bSuccess);
	bool bSumOfShopsProductsGreaterThanZero(int iWarehouseNumber, bool& bSuccess);

private:
	bool bSetSolutionMatrix(CMatrix& matrixToSet, int iIndex, int jIndex, int& iSolutionIndex);

	unsigned int i_solution_size;
	unsigned int i_suppliers_count;
	unsigned int i_factories_count;
	unsigned int i_warehouses_count;
	unsigned int i_shops_count;

	CTable t_solution;
	CMatrix m_delivery_qty_matrix;
	CMatrix m_factory_qty_matrix;
	CMatrix m_warehouse_qty_matrix;

	FILE* pf_file;
};

