#pragma once

class CMscnProblem
{
public:
	CMscnProblem();
	CMscnProblem(unsigned int iSupplierCount, unsigned int iFactoryCount, unsigned int iWarehouseCount, unsigned int iShopCount);
	void vInitTabs();
	~CMscnProblem();

	void vInitMatrixes();
	void vInitMatrixesCopy(double*** tabToInit, unsigned int size1, unsigned int size2);

	void vCopyTable(double* pd_copyFrom, double* pd_copyTo, int i_copySize);

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

private:
	unsigned int i_supplier_count;
	unsigned int i_factory_count;
	unsigned int i_warehouse_count;
	unsigned int i_shop_count;

	double* pd_suppliers_contract_prizes_tab;
	double* pd_factories_contract_prizes_tab;
	double* pd_warehouses_contract_prizes_tab;
	
	double* pd_production_capacity_tab;
	double* pd_factory_capacity_tab;
	double* pd_warehouse_capacity_tab;
	double* pd_shop_capacity_tab;

	double** pd_delivery_matrix;
	double** pd_factory_matrix;
	double** pd_warehouse_matrix;
};

