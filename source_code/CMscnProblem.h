#pragma once
#include "CMatrix.h"
#include "CTable.h"

class CMscnProblem
{
public:
	CMscnProblem();
	CMscnProblem(unsigned int iSupplierCount, unsigned int iFactoryCount, unsigned int iWarehouseCount, unsigned int iShopCount, bool bSuccess);
	~CMscnProblem();
	void vInitTabs();
	void vInitMatrixes();

	bool b_set_supplier_count(unsigned int iSupplierCount);
	bool b_set_factory_count(unsigned int iFactoryCount);
	bool b_set_warehouse_count(unsigned int iWarehouseCount);
	bool b_set_shop_count(unsigned int iShopCount);

	bool b_set_delivery_matrix_val(double dVal, int iXindex, int iYindex);
	bool b_set_factory_matrix_val(double dVal, int iXindex, int iYindex);
	bool b_set_warehouse_matrix_val(double dVal, int iXindex, int iYindex);

	bool b_set_supplier_contract_val(double dVal, int index);
	bool b_set_factory_contract_val(double dVal, int index);
	bool b_set_warehouse_contract_val(double dVal, int index);

	bool b_set_production_capacity_val(double dVal, int index);
	bool b_set_factory_capacity_val(double dVal, int index);
	bool b_set_warehouse_capacity_val(double dVal, int index);
	bool b_set_shop_capacity_val(double dVal, int index);

	bool b_set_shop_profit_val(double dVal, int index);

private:
	unsigned int i_supplier_count;
	unsigned int i_factory_count;
	unsigned int i_warehouse_count;
	unsigned int i_shop_count;

	CTable t_suppliers_contract_prizes_tab;
	CTable t_factories_contract_prizes_tab;
	CTable t_warehouses_contract_prizes_tab;

	CTable t_production_capacity_tab;
	CTable t_factory_capacity_tab;
	CTable t_warehouse_capacity_tab;
	CTable t_shop_capacity_tab;

	CTable t_shop_profit_tab;

	CMatrix m_delivery_matrix;
	CMatrix m_factory_matrix;
	CMatrix m_warehouse_matrix;
};

