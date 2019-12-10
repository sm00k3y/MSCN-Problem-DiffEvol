#include "CMscnProblem.h"

#define DEFAULT_ANY_COUNT 1

CMscnProblem::CMscnProblem()
{
	i_supplier_count = DEFAULT_ANY_COUNT;
	i_factory_count = DEFAULT_ANY_COUNT;
	i_warehouse_count = DEFAULT_ANY_COUNT;
	i_shop_count = DEFAULT_ANY_COUNT;
	vInitTabs();
	vInitMatrixes();
}

CMscnProblem::CMscnProblem(unsigned int iSupplierCount, unsigned int iFactoryCount, unsigned int iWarehouseCount, unsigned int iShopCount)
{
	i_supplier_count = iSupplierCount;
	i_factory_count = iFactoryCount;
	i_warehouse_count = iWarehouseCount;
	i_shop_count = iShopCount;
	vInitTabs();
	vInitMatrixes();
}

void CMscnProblem::vInitTabs()
{
	pd_suppliers_contract_prizes_tab = new double[i_supplier_count];
	pd_factories_contract_prizes_tab = new double[i_factory_count];
	pd_warehouses_contract_prizes_tab = new double[i_warehouse_count];

	pd_production_capacity_tab = new double[i_supplier_count];
	pd_factory_capacity_tab = new double[i_factory_count];
	pd_warehouse_capacity_tab = new double[i_warehouse_count];
	pd_shop_capacity_tab = new double[i_shop_count];
}

CMscnProblem::~CMscnProblem()
{
	delete[] pd_suppliers_contract_prizes_tab;
	delete[] pd_factories_contract_prizes_tab;
	delete[] pd_warehouses_contract_prizes_tab;

	delete[] pd_production_capacity_tab;
	delete[] pd_factory_capacity_tab;
	delete[] pd_warehouse_capacity_tab;
	delete[] pd_shop_capacity_tab;
}

void CMscnProblem::vInitMatrixes()
{
	vInitMatrixesCopy(&pd_delivery_matrix, i_supplier_count, i_factory_count);
	vInitMatrixesCopy(&pd_factory_matrix, i_factory_count, i_warehouse_count);
	vInitMatrixesCopy(&pd_warehouse_matrix, i_warehouse_count, i_shop_count);
}

void CMscnProblem::vInitMatrixesCopy(double*** tabToInit, unsigned int size1, unsigned int size2)
{
	(*tabToInit) = new double* [size1];
	for (int i = 0; i < size1; i++)
	{
		(*tabToInit)[i] = new double[size2];
	}
}

void CMscnProblem::vCopyTable(double* pd_copyFrom, double* pd_copyTo, int i_copySize)
{
	for (int i = 0; i < i_copySize; i++)
	{
		pd_copyTo[i] = pd_copyFrom[i];
	}
}

bool CMscnProblem::b_set_supplier_count(unsigned int iSupplierCount)
{
	if (iSupplierCount < 1) return false;
	
	int iTempSize = (i_supplier_count < iSupplierCount ? i_supplier_count : iSupplierCount);
	i_supplier_count = iSupplierCount;

	double* suppliersTemp = new double[i_supplier_count];
	vCopyTable(pd_suppliers_contract_prizes_tab, suppliersTemp, iTempSize);
	delete[] pd_suppliers_contract_prizes_tab;
	pd_suppliers_contract_prizes_tab = suppliersTemp;

	double* productionCapacityTemp = new double[i_supplier_count];
	vCopyTable(pd_production_capacity_tab, productionCapacityTemp, iTempSize);
	delete[] pd_production_capacity_tab;
	pd_production_capacity_tab = productionCapacityTemp;

	for (int i = 0; i < i_supplier_count; i++)
	{
		delete[] pd_delivery_matrix[i];
	}
	delete[] pd_delivery_matrix;
	vInitMatrixesCopy(&pd_delivery_matrix, i_supplier_count, i_factory_count);

	return true;
}

bool CMscnProblem::b_set_factory_count(unsigned int iFactoryCount)
{
	return false;
}

bool CMscnProblem::b_set_warehouse_count(unsigned int iWarehouseCount)
{
	return false;
}

bool CMscnProblem::b_set_shop_count(unsigned int iShopCount)
{
	return false;
}

bool CMscnProblem::b_set_delivery_matrix_val(double dVal, int iXindex, int iYindex)
{
	if (iXindex < 0 || iXindex > i_supplier_count || iYindex < 0 || iYindex > i_factory_count) return false;
	pd_delivery_matrix[iXindex][iYindex] = dVal;
	return true;
}

bool CMscnProblem::b_set_factory_matrix_val(double dVal, int iXindex, int iYindex)
{
	if (iXindex < 0 || iXindex > i_factory_count || iYindex < 0 || iYindex > i_warehouse_count) return false;
	pd_factory_matrix[iXindex][iYindex] = dVal;
	return true;
}

bool CMscnProblem::b_set_warehouse_matrix_val(double dVal, int iXindex, int iYindex)
{
	if (iXindex < 0 || iXindex > i_warehouse_count || iYindex < 0 || iYindex > i_shop_count) return false;
	pd_warehouse_matrix[iXindex][iYindex] = dVal;
	return true;
}

bool CMscnProblem::b_set_supplier_contract_val(double dVal, int index)
{
	if (index < 0 || index > i_supplier_count) return false;
	pd_suppliers_contract_prizes_tab[index] = dVal;
	return true;
}

bool CMscnProblem::b_set_factory_contract_val(double dVal, int index)
{
	if (index < 0 || index > i_factory_count) return false;
	pd_factories_contract_prizes_tab[index] = dVal;
	return true;
}

bool CMscnProblem::b_set_warehouse_contract_val(double dVal, int index)
{
	if (index < 0 || index > i_warehouse_count) return false;
	pd_warehouses_contract_prizes_tab[index] = dVal;
	return true;
}

bool CMscnProblem::b_set_production_capacity_val(double dVal, int index)
{
	if (index < 0 || index > i_supplier_count) return false;
	pd_production_capacity_tab[index] = dVal;
	return true;
}

bool CMscnProblem::b_set_factory_capacity_val(double dVal, int index)
{
	if (index < 0 || index > i_factory_count) return false;
	pd_factory_capacity_tab[index] = dVal;
	return true;
}

bool CMscnProblem::b_set_warehouse_capacity_val(double dVal, int index)
{
	if (index < 0 || index > i_warehouse_count) return false;
	pd_warehouse_capacity_tab[index] = dVal;
	return true;
}

bool CMscnProblem::b_set_shop_capacity_val(double dVal, int index)
{
	if (index < 0 || index > i_shop_count) return false;
	pd_shop_capacity_tab[index] = dVal;
	return true;
}
