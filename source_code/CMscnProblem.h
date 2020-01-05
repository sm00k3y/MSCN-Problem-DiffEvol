#pragma once
#include "CMatrix.h"
#include "CTable.h"
#include "CSolution.h"
#include <iostream>

class CMscnProblem
{
public:
	CMscnProblem();
	CMscnProblem(unsigned int iSupplierCount, unsigned int iFactoryCount, unsigned int iWarehouseCount, unsigned int iShopCount, bool bSuccess);
	~CMscnProblem();

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

	bool b_set_min_supply_matrix_val(double dVal, int iXindex, int iYindex);
	bool b_set_max_supply_matrix_val(double dVal, int iXindex, int iYindex);
	bool b_set_min_factory_matrix_val(double dVal, int iXindex, int iYindex);
	bool b_set_max_factory_matrix_val(double dVal, int iXindex, int iYindex);
	bool b_set_min_warehouse_matrix_val(double dVal, int iXindex, int iYindex);
	bool b_set_max_warehouse_matrix_val(double dVal, int iXindex, int iYindex);

	double d_get_min_supply_matrix_val(int iXindex, int iYindex, bool& bSuccess);
	double d_get_max_supply_matrix_val(int iXindex, int iYindex, bool& bSuccess);
	double d_get_min_factory_matrix_val(int iXindex, int iYindex, bool& bSuccess);
	double d_get_max_factory_matrix_val(int iXindex, int iYindex, bool& bSuccess);
	double d_get_min_warehouse_matrix_val(int iXindex, int iYindex, bool& bSuccess);
	double d_get_max_warehouse_matrix_val(int iXindex, int iYindex, bool& bSuccess);

	bool bGetQuality(CSolution& s_solution, double& result);
	bool bConstraintsSatisfied(CSolution& s_solution);
	bool bCheckSolutionSize(CSolution& s_solution);
	bool bCheckSolutionForNegativeNumbers(CSolution& s_solution);
	bool bCheckCapacityOverload(CSolution& s_solution);						// metoda do 4 pierwszych p-punktow tabelki
	bool bCheckInsufficientAmount(CSolution& s_solution);					// metoda do 2 ostatnich p-punktow tabelki
	bool bCheckMinMaxOutOfRage(CSolution& s_solution);						// metoda na sprawdzenie przedzialow min/maksowych

	bool bWriteToFIle(std::string sFileName);
	bool bReadFromFile(std::string sFileName);

	//bool bReadSolutionFromFile(std::string sSolutionFileName, double** pdSolution, int& iSolutionSize);

private:
	void vInitTabs();
	void vInitMatrixes();

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

	CMatrix m_min_supply_values;
	CMatrix m_max_supply_values;
	CMatrix m_min_factories_values;
	CMatrix m_max_factories_values;
	CMatrix m_min_warehouse_values;
	CMatrix m_max_warehouse_values;

	//CSolution s_solution;

	FILE *pf_file;
};




// -------------------------------------- INLINE FUNCTIONS BODIES -------------------------------------

inline bool CMscnProblem::b_set_delivery_matrix_val(double dVal, int iXindex, int iYindex)
{
	return m_delivery_matrix.b_set_value(dVal, iXindex, iYindex);
}

inline bool CMscnProblem::b_set_factory_matrix_val(double dVal, int iXindex, int iYindex)
{
	return m_factory_matrix.b_set_value(dVal, iXindex, iYindex);
}

inline bool CMscnProblem::b_set_warehouse_matrix_val(double dVal, int iXindex, int iYindex)
{
	return m_warehouse_matrix.b_set_value(dVal, iXindex, iYindex);
}

inline bool CMscnProblem::b_set_supplier_contract_val(double dVal, int index)
{
	return t_suppliers_contract_prizes_tab.b_set_val(dVal, index);
}

inline bool CMscnProblem::b_set_factory_contract_val(double dVal, int index)
{
	return t_factories_contract_prizes_tab.b_set_val(dVal, index);
}

inline bool CMscnProblem::b_set_warehouse_contract_val(double dVal, int index)
{
	return t_warehouses_contract_prizes_tab.b_set_val(dVal, index);
}

inline bool CMscnProblem::b_set_production_capacity_val(double dVal, int index)
{
	return t_production_capacity_tab.b_set_val(dVal, index);
}

inline bool CMscnProblem::b_set_factory_capacity_val(double dVal, int index)
{
	return t_factory_capacity_tab.b_set_val(dVal, index);
}

inline bool CMscnProblem::b_set_warehouse_capacity_val(double dVal, int index)
{
	return t_warehouse_capacity_tab.b_set_val(dVal, index);
}

inline bool CMscnProblem::b_set_shop_capacity_val(double dVal, int index)
{
	return t_shop_capacity_tab.b_set_val(dVal, index);
}

inline bool CMscnProblem::b_set_shop_profit_val(double dVal, int index)
{
	return t_shop_profit_tab.b_set_val(dVal, index);
}

inline bool CMscnProblem::b_set_min_supply_matrix_val(double dVal, int iXindex, int iYindex)
{
	return m_min_supply_values.b_set_value(dVal, iXindex, iYindex);
}

inline bool CMscnProblem::b_set_max_supply_matrix_val(double dVal, int iXindex, int iYindex)
{
	return m_max_supply_values.b_set_value(dVal, iXindex, iYindex);
}

inline bool CMscnProblem::b_set_min_factory_matrix_val(double dVal, int iXindex, int iYindex)
{
	return m_min_factories_values.b_set_value(dVal, iXindex, iYindex);
}

inline bool CMscnProblem::b_set_max_factory_matrix_val(double dVal, int iXindex, int iYindex)
{
	return m_max_factories_values.b_set_value(dVal, iXindex, iYindex);
}

inline bool CMscnProblem::b_set_min_warehouse_matrix_val(double dVal, int iXindex, int iYindex)
{
	return m_min_warehouse_values.b_set_value(dVal, iXindex, iYindex);
}

inline bool CMscnProblem::b_set_max_warehouse_matrix_val(double dVal, int iXindex, int iYindex)
{
	return m_max_warehouse_values.b_set_value(dVal, iXindex, iYindex);
}

inline double CMscnProblem::d_get_min_supply_matrix_val(int iXindex, int iYindex, bool& bSuccess)
{
	return m_min_supply_values.d_get_val(iXindex, iYindex, bSuccess);
}

inline double CMscnProblem::d_get_max_supply_matrix_val(int iXindex, int iYindex, bool& bSuccess)
{
	return m_max_supply_values.d_get_val(iXindex, iYindex, bSuccess);
}

inline double CMscnProblem::d_get_min_factory_matrix_val(int iXindex, int iYindex, bool& bSuccess)
{
	return m_min_factories_values.d_get_val(iXindex, iYindex, bSuccess);
}

inline double CMscnProblem::d_get_max_factory_matrix_val(int iXindex, int iYindex, bool& bSuccess)
{
	return m_max_factories_values.d_get_val(iXindex, iYindex, bSuccess);
}

inline double CMscnProblem::d_get_min_warehouse_matrix_val(int iXindex, int iYindex, bool& bSuccess)
{
	return m_min_warehouse_values.d_get_val(iXindex, iYindex, bSuccess);
}

inline double CMscnProblem::d_get_max_warehouse_matrix_val(int iXindex, int iYindex, bool& bSuccess)
{
	return m_max_warehouse_values.d_get_val(iXindex, iYindex, bSuccess);
}
