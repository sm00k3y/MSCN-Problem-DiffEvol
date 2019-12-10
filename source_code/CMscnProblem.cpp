#include "CMscnProblem.h"

#define DEFAULT_ANY_COUNT 1
#define INITIAL_VAL -1

CMscnProblem::CMscnProblem()
{
	i_supplier_count = DEFAULT_ANY_COUNT;
	i_factory_count = DEFAULT_ANY_COUNT;
	i_warehouse_count = DEFAULT_ANY_COUNT;
	i_shop_count = DEFAULT_ANY_COUNT;
	vInitTabs();
	vInitMatrixes();
}

CMscnProblem::CMscnProblem(unsigned int iSupplierCount, unsigned int iFactoryCount, unsigned int iWarehouseCount, unsigned int iShopCount, bool bSuccess)
{
	if (iSupplierCount < 1 || iFactoryCount < 1 || iWarehouseCount < 1 || iShopCount < 1)
	{
		bSuccess = false;
		CMscnProblem();
	}
	else 
	{
		i_supplier_count = iSupplierCount;
		i_factory_count = iFactoryCount;
		i_warehouse_count = iWarehouseCount;
		i_shop_count = iShopCount;
		vInitTabs();
		vInitMatrixes();
		bSuccess = true;
	}
}

CMscnProblem::~CMscnProblem()
{
}

void CMscnProblem::vInitTabs()
{
	t_suppliers_contract_prizes_tab = CTable(i_supplier_count, true);
	t_factories_contract_prizes_tab = CTable(i_factory_count, true);
	t_warehouses_contract_prizes_tab = CTable(i_warehouse_count, true);

	t_production_capacity_tab = CTable(i_supplier_count, true);
	t_factory_capacity_tab = CTable(i_factory_count, true);
	t_warehouse_capacity_tab = CTable(i_warehouse_count, true);
	t_shop_capacity_tab = CTable(i_shop_count, true);

	t_shop_profit_tab = CTable(i_shop_count, true);
}

void CMscnProblem::vInitMatrixes()
{
	m_delivery_matrix = CMatrix(i_supplier_count, i_factory_count, true);
	m_factory_matrix = CMatrix(i_factory_count, i_warehouse_count, true);
	m_warehouse_matrix = CMatrix(i_warehouse_count, i_shop_count, true);
}

bool CMscnProblem::b_set_supplier_count(unsigned int iSupplierCount)
{
	if (iSupplierCount < 1) return false;
	
	i_supplier_count = iSupplierCount;

	t_suppliers_contract_prizes_tab.b_set_size(i_supplier_count);
	t_production_capacity_tab.b_set_size(i_supplier_count);
	m_delivery_matrix.b_set_size(i_supplier_count, i_factory_count);

	return true;
}

bool CMscnProblem::b_set_factory_count(unsigned int iFactoryCount)
{
	if (iFactoryCount < 1) return false;

	i_factory_count = iFactoryCount;

	t_factories_contract_prizes_tab.b_set_size(i_factory_count);
	t_factory_capacity_tab.b_set_size(i_factory_count);
	m_factory_matrix.b_set_size(i_factory_count, i_warehouse_count);
	m_delivery_matrix.b_set_size(i_supplier_count, i_factory_count);

	return true;
}

bool CMscnProblem::b_set_warehouse_count(unsigned int iWarehouseCount)
{
	if (iWarehouseCount < 1) return false;

	i_warehouse_count = iWarehouseCount;

	t_warehouses_contract_prizes_tab.b_set_size(i_warehouse_count);
	t_warehouse_capacity_tab.b_set_size(i_warehouse_count);
	m_warehouse_matrix.b_set_size(i_warehouse_count, i_shop_count);
	m_factory_matrix.b_set_size(i_factory_count, i_warehouse_count);

	return true;
}

bool CMscnProblem::b_set_shop_count(unsigned int iShopCount)
{
	if (iShopCount < 1) return false;

	i_shop_count = iShopCount;

	t_shop_capacity_tab.b_set_size(i_shop_count);
	t_shop_profit_tab.b_set_size(i_shop_count);
	m_warehouse_matrix.b_set_size(i_warehouse_count, i_shop_count);

	return true;
}

bool CMscnProblem::b_set_delivery_matrix_val(double dVal, int iXindex, int iYindex)
{
	return m_delivery_matrix.b_set_value(dVal, iXindex, iYindex);
}

bool CMscnProblem::b_set_factory_matrix_val(double dVal, int iXindex, int iYindex)
{
	return m_factory_matrix.b_set_value(dVal, iXindex, iYindex);
}

bool CMscnProblem::b_set_warehouse_matrix_val(double dVal, int iXindex, int iYindex)
{
	return m_warehouse_matrix.b_set_value(dVal, iXindex, iYindex);
}

bool CMscnProblem::b_set_supplier_contract_val(double dVal, int index)
{
	return t_suppliers_contract_prizes_tab.b_set_val(dVal, index);
}

bool CMscnProblem::b_set_factory_contract_val(double dVal, int index)
{
	return t_factories_contract_prizes_tab.b_set_val(dVal, index);
}

bool CMscnProblem::b_set_warehouse_contract_val(double dVal, int index)
{
	return t_warehouses_contract_prizes_tab.b_set_val(dVal, index);
}

bool CMscnProblem::b_set_production_capacity_val(double dVal, int index)
{
	return t_production_capacity_tab.b_set_val(dVal, index);
}

bool CMscnProblem::b_set_factory_capacity_val(double dVal, int index)
{
	return t_factory_capacity_tab.b_set_val(dVal, index);
}

bool CMscnProblem::b_set_warehouse_capacity_val(double dVal, int index)
{
	return t_warehouse_capacity_tab.b_set_val(dVal, index);
}

bool CMscnProblem::b_set_shop_capacity_val(double dVal, int index)
{
	return t_shop_capacity_tab.b_set_val(dVal, index);
}

bool CMscnProblem::b_set_shop_profit_val(double dVal, int index)
{
	return t_shop_profit_tab.b_set_val(dVal, index);
}
