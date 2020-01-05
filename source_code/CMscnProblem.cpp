#pragma warning (disable : 4996)
#include "CMscnProblem.h"

#define DEFAULT_ANY_COUNT 1
#define INITIAL_VAL -1
#define MAX_LINE_LEN 1000

CMscnProblem::CMscnProblem()
{
	i_supplier_count = DEFAULT_ANY_COUNT;
	i_factory_count = DEFAULT_ANY_COUNT;
	i_warehouse_count = DEFAULT_ANY_COUNT;
	i_shop_count = DEFAULT_ANY_COUNT;
	pf_file = NULL;
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
		pf_file = NULL;
		bSuccess = true;
	}
}

CMscnProblem::~CMscnProblem()
{
	delete pf_file;
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

bool CMscnProblem::bGetQuality(CSolution& s_solution, double& result)
{
	//if (!bConstraintsSatisfied(s_solution)) return false; // sprawdzac czy nie

	double d_delivery_cost = 0;
	double d_contracts_cost = 0;
	double d_income = 0;
	bool bMethodSuccessful = true;

	d_delivery_cost += m_delivery_matrix.d_multiply_matrix(s_solution.m_get_delivery_qty_matrix(), bMethodSuccessful);
	d_delivery_cost += m_factory_matrix.d_multiply_matrix(s_solution.m_get_factory_qty_matrix(), bMethodSuccessful);
	d_delivery_cost += m_warehouse_matrix.d_multiply_matrix(s_solution.m_get_warehouse_qty_matrix(), bMethodSuccessful);
	
	for (int i = 0; i < i_supplier_count; i++)
	{
		if (s_solution.bSumOfFactoriesProductsGreaterThanZero(i, bMethodSuccessful)) d_contracts_cost += t_suppliers_contract_prizes_tab.d_get_value(i, bMethodSuccessful);
	}

	for (int i = 0; i < i_factory_count; i++)
	{
		if (s_solution.bSumOfWarehousesProductsGraterThanzero(i, bMethodSuccessful)) d_contracts_cost += t_factories_contract_prizes_tab.d_get_value(i, bMethodSuccessful);
	}

	for (int i = 0; i < i_warehouse_count; i++)
	{
		for (int j = 0; j < i_shop_count; j++)
		{
			d_income += s_solution.m_get_warehouse_qty_matrix().d_get_val(i,j,bMethodSuccessful) * t_shop_profit_tab.d_get_value(j, bMethodSuccessful);
		}
		if (s_solution.bSumOfShopsProductsGreaterThanZero(i, bMethodSuccessful)) d_contracts_cost += t_warehouses_contract_prizes_tab.d_get_value(i, bMethodSuccessful);
	}

	result = d_income - d_delivery_cost - d_contracts_cost;
	return true;
}

bool CMscnProblem::bConstraintsSatisfied(CSolution& s_solution)
{
	if (bCheckSolutionSize(s_solution) || bCheckSolutionForNegativeNumbers(s_solution) || bCheckCapacityOverload(s_solution) || bCheckInsufficientAmount(s_solution) || bCheckMinMaxOutOfRage(s_solution)) return false;
	else return true;
}

bool CMscnProblem::bCheckSolutionSize(CSolution& s_solution)
{
	if(s_solution.i_get_solution_size() != (i_supplier_count * i_factory_count + i_factory_count * i_warehouse_count + i_warehouse_count * i_shop_count)) return true;
	if (s_solution.i_get_suppliers_count() != i_supplier_count || s_solution.i_get_factories_count() != i_factory_count || s_solution.i_get_warehouses_count() != i_warehouse_count || s_solution.i_get_shops_count() != i_shop_count) return true;
	
	return false;
}

bool CMscnProblem::bCheckSolutionForNegativeNumbers(CSolution& s_solution)
{
	bool bMethodSucessful = true;
	for (int i = 0; i < s_solution.i_get_solution_size(); i++)
	{
		if (s_solution.t_get_pdSolution().d_get_value(i, bMethodSucessful) < 0) return true;
	}
	return false;
}

bool CMscnProblem::bCheckCapacityOverload(CSolution& s_solution)	// metoda do 4 pierwszych p-punktow tabelki
{
	int i_solution_index = 0;
	double d_product_qty;
	bool bMethodSuccessful = true;

	for (int i = 0; i < i_supplier_count; i++)
	{
		d_product_qty = s_solution.m_get_delivery_qty_matrix().d_get_second_dim_sum(i, bMethodSuccessful);
		if (d_product_qty > t_production_capacity_tab.d_get_value(i, bMethodSuccessful)) return true;
	}

	for (int i = 0; i < i_factory_count; i++)
	{
		d_product_qty = s_solution.m_get_factory_qty_matrix().d_get_second_dim_sum(i, bMethodSuccessful);
		if (d_product_qty > t_factory_capacity_tab.d_get_value(i, bMethodSuccessful)) return true;
	}

	for (int i = 0; i < i_warehouse_count; i++)
	{
		d_product_qty = s_solution.m_get_warehouse_qty_matrix().d_get_second_dim_sum(i, bMethodSuccessful);
		if (d_product_qty > t_warehouse_capacity_tab.d_get_value(i, bMethodSuccessful)) return true;
	}

	for (int i = 0; i < i_shop_count; i++)
	{
		d_product_qty = s_solution.m_get_warehouse_qty_matrix().d_get_first_dim_sum(i, bMethodSuccessful);
		if (d_product_qty > t_shop_capacity_tab.d_get_value(i, bMethodSuccessful)) return true;
	}

	return false;
}

bool CMscnProblem::bCheckInsufficientAmount(CSolution& s_solution)	// metoda do 2 ostatnich p-punktow tabelki
{
	bool bMethodSucessful = true;
	double d_sum_TO_entity = 0;
	double d_sum_FROM_entity = 0;

	for (int i = 0; i < i_factory_count; i++)
	{
		d_sum_TO_entity = s_solution.m_get_delivery_qty_matrix().d_get_first_dim_sum(i, bMethodSucessful);
		d_sum_FROM_entity = s_solution.m_get_factory_qty_matrix().d_get_second_dim_sum(i, bMethodSucessful);
		if (d_sum_TO_entity < d_sum_FROM_entity) return true;
	}

	for (int i = 0; i < i_warehouse_count; i++)
	{
		d_sum_TO_entity = s_solution.m_get_factory_qty_matrix().d_get_first_dim_sum(i, bMethodSucessful);
		d_sum_FROM_entity = s_solution.m_get_warehouse_qty_matrix().d_get_second_dim_sum(i, bMethodSucessful);
		if (d_sum_TO_entity < d_sum_FROM_entity) return true;
	}

	return false;
}

bool CMscnProblem::bCheckMinMaxOutOfRage(CSolution& s_solution)
{
	bool bMethodSuccessful = true;
	for (int i = 0; i < i_supplier_count; i++)
	{
		for (int j = 0; j < i_factory_count; j++)
		{
			if (s_solution.m_get_delivery_qty_matrix().d_get_val(i, j, bMethodSuccessful) < m_min_supply_values.d_get_val(i, j, bMethodSuccessful) || s_solution.m_get_delivery_qty_matrix().d_get_val(i, j, bMethodSuccessful) > m_max_supply_values.d_get_val(i, j, bMethodSuccessful)) return true;
		}
	}

	for (int i = 0; i < i_factory_count; i++)
	{
		for (int j = 0; j < i_warehouse_count; j++)
		{
			if (s_solution.m_get_factory_qty_matrix().d_get_val(i, j, bMethodSuccessful) < m_min_factories_values.d_get_val(i, j, bMethodSuccessful) || s_solution.m_get_factory_qty_matrix().d_get_val(i, j, bMethodSuccessful) > m_max_factories_values.d_get_val(i, j, bMethodSuccessful)) return true;
		}
	}

	for (int i = 0; i < i_warehouse_count; i++)
	{
		for (int j = 0; j < i_shop_count; j++)
		{
			if (s_solution.m_get_warehouse_qty_matrix().d_get_val(i, j, bMethodSuccessful) < m_min_warehouse_values.d_get_val(i, j, bMethodSuccessful) || s_solution.m_get_warehouse_qty_matrix().d_get_val(i, j, bMethodSuccessful) > m_max_warehouse_values.d_get_val(i, j, bMethodSuccessful)) return true;
		}
	}

	return false;
}

bool CMscnProblem::bWriteToFIle(std::string sFileName)
{
	if (pf_file != NULL) fclose(pf_file);
	pf_file = fopen(sFileName.c_str(), "w+");
	if (pf_file == NULL) return false;
	bool bMethodSuccessful = true;
	double d_temp_val;

	fprintf(pf_file, "D %u", i_supplier_count);
	fprintf(pf_file, "\nF %u", i_factory_count);
	fprintf(pf_file, "\nM %u", i_warehouse_count);
	fprintf(pf_file, "\nS %u", i_shop_count);

	fprintf(pf_file, "\nsd\n");
	for (int i = 0; i < i_supplier_count; i++)
	{
		d_temp_val = t_production_capacity_tab.d_get_value(i, bMethodSuccessful);
		fprintf(pf_file, "%g ", d_temp_val);
	}

	fprintf(pf_file, "\nsf\n");
	for (int i = 0; i < i_factory_count; i++)
	{
		d_temp_val = t_factory_capacity_tab.d_get_value(i, bMethodSuccessful);
		fprintf(pf_file, "%g ", d_temp_val);
	}

	fprintf(pf_file, "\nsm\n");
	for (int i = 0; i < i_warehouse_count; i++)
	{
		d_temp_val = t_warehouse_capacity_tab.d_get_value(i, bMethodSuccessful);
		fprintf(pf_file, "%g ", d_temp_val);
	}

	fprintf(pf_file, "\nss\n");
	for (int i = 0; i < i_shop_count; i++)
	{
		d_temp_val = t_shop_capacity_tab.d_get_value(i, bMethodSuccessful);
		fprintf(pf_file, "%g ", d_temp_val);
	}

	fprintf(pf_file, "\ncd\n");
	for (int i = 0; i < i_supplier_count; i++)
	{
		for (int j = 0; j < i_factory_count; j++)
		{
			d_temp_val = m_delivery_matrix.d_get_val(i, j, bMethodSuccessful);
			fprintf(pf_file, "%g ", d_temp_val);
		}
		fprintf(pf_file, "\n");
	}

	fprintf(pf_file, "cf\n");
	for (int i = 0; i < i_factory_count; i++)
	{
		for (int j = 0; j < i_warehouse_count; j++)
		{
			d_temp_val = m_factory_matrix.d_get_val(i, j, bMethodSuccessful);
			fprintf(pf_file, "%g ", d_temp_val);
		}
		fprintf(pf_file, "\n");
	}

	fprintf(pf_file, "cm\n");
	for (int i = 0; i < i_warehouse_count; i++)
	{
		for (int j = 0; j < i_shop_count; j++)
		{
			d_temp_val = m_warehouse_matrix.d_get_val(i, j, bMethodSuccessful);
			fprintf(pf_file, "%g ", d_temp_val);
		}
		fprintf(pf_file, "\n");
	}

	fprintf(pf_file, "ud\n");
	for (int i = 0; i < i_supplier_count; i++)
	{
		d_temp_val = t_suppliers_contract_prizes_tab.d_get_value(i, bMethodSuccessful);
		fprintf(pf_file, "%g ", d_temp_val);
	}

	fprintf(pf_file, "\nuf\n");
	for (int i = 0; i < i_factory_count; i++)
	{
		d_temp_val = t_factories_contract_prizes_tab.d_get_value(i, bMethodSuccessful);
		fprintf(pf_file, "%g ", d_temp_val);
	}

	fprintf(pf_file, "\num\n");
	for (int i = 0; i < i_warehouse_count; i++)
	{
		d_temp_val = t_warehouses_contract_prizes_tab.d_get_value(i, bMethodSuccessful);
		fprintf(pf_file, "%g ", d_temp_val);
	}

	fprintf(pf_file, "\np\n");
	for (int i = 0; i < i_shop_count; i++)
	{
		d_temp_val = t_shop_profit_tab.d_get_value(i, bMethodSuccessful);
		fprintf(pf_file, "%g ", d_temp_val);
	}

	double d_temp_val2;

	fprintf(pf_file, "\nxdminmax\n");
	for (int i = 0; i < i_supplier_count; i++)
	{
		for (int j = 0; j < i_factory_count; j++)
		{
			d_temp_val = m_min_supply_values.d_get_val(i, j, bMethodSuccessful);
			d_temp_val2 = m_max_supply_values.d_get_val(i, j, bMethodSuccessful);
			fprintf(pf_file, "%g %g ", d_temp_val, d_temp_val2);
		}
		fprintf(pf_file, "\n");
	}

	fprintf(pf_file, "xfminmax\n");
	for (int i = 0; i < i_factory_count; i++)
	{
		for (int j = 0; j < i_warehouse_count; j++)
		{
			d_temp_val = m_min_factories_values.d_get_val(i, j, bMethodSuccessful);
			d_temp_val2 = m_max_factories_values.d_get_val(i, j, bMethodSuccessful);
			fprintf(pf_file, "%g %g ", d_temp_val, d_temp_val2);
		}
		fprintf(pf_file, "\n");
	}

	fprintf(pf_file, "xmminmax\n");
	for (int i = 0; i < i_warehouse_count; i++)
	{
		for (int j = 0; j < i_shop_count; j++)
		{
			d_temp_val = m_min_warehouse_values.d_get_val(i, j, bMethodSuccessful);
			d_temp_val2 = m_max_warehouse_values.d_get_val(i, j, bMethodSuccessful);
			fprintf(pf_file, "%g %g ", d_temp_val, d_temp_val2);
		}
		fprintf(pf_file, "\n");
	}

	fclose(pf_file);
	pf_file = NULL;
	return true;
}

bool CMscnProblem::bReadFromFile(std::string sFileName)
{
	if (pf_file != NULL) fclose(pf_file);
	pf_file = fopen(sFileName.c_str(), "r");
	if (pf_file == NULL) return false;

	char cLine[MAX_LINE_LEN];
	int iTempVal = 0;
	double dTempVal = 0;
	double dTempVal2 = 0;

	fscanf(pf_file, "%s ", cLine);
	fscanf(pf_file, "%i", &i_supplier_count);
	t_suppliers_contract_prizes_tab.b_set_size(i_supplier_count);
	t_production_capacity_tab.b_set_size(i_supplier_count);

	fscanf(pf_file, "%s ", cLine);
	fscanf(pf_file, "%i", &i_factory_count);
	t_factories_contract_prizes_tab.b_set_size(i_factory_count);
	t_factory_capacity_tab.b_set_size(i_factory_count);
	m_delivery_matrix.b_set_size(i_supplier_count, i_factory_count);

	fscanf(pf_file, "%s ", cLine);
	fscanf(pf_file, "%i", &i_warehouse_count);
	t_warehouses_contract_prizes_tab.b_set_size(i_warehouse_count);
	t_warehouse_capacity_tab.b_set_size(i_warehouse_count);
	m_factory_matrix.b_set_size(i_factory_count, i_warehouse_count);

	fscanf(pf_file, "%s ", cLine);
	fscanf(pf_file, "%i", &i_shop_count);
	t_shop_capacity_tab.b_set_size(i_shop_count);
	t_shop_profit_tab.b_set_size(i_shop_count);
	m_warehouse_matrix.b_set_size(i_warehouse_count, i_shop_count);

	m_min_supply_values.b_set_size(i_supplier_count, i_factory_count);
	m_max_supply_values.b_set_size(i_supplier_count, i_factory_count);
	m_min_factories_values.b_set_size(i_factory_count, i_warehouse_count);
	m_max_factories_values.b_set_size(i_factory_count, i_warehouse_count);
	m_min_warehouse_values.b_set_size(i_warehouse_count, i_shop_count);
	m_max_warehouse_values.b_set_size(i_warehouse_count, i_shop_count);

	fscanf(pf_file, "%s ", cLine);
	for (int i = 0; i < i_supplier_count; i++)
	{
		fscanf(pf_file, "%lf", &dTempVal);
		t_production_capacity_tab.b_set_val(dTempVal, i);
	}

	fscanf(pf_file, "%s ", cLine);
	for (int i = 0; i < i_factory_count; i++)
	{
		fscanf(pf_file, "%lf", &dTempVal);
		t_factory_capacity_tab.b_set_val(dTempVal, i);
	}

	fscanf(pf_file, "%s ", cLine);
	for (int i = 0; i < i_warehouse_count; i++)
	{
		fscanf(pf_file, "%lf", &dTempVal);
		t_warehouse_capacity_tab.b_set_val(dTempVal, i);
	}

	fscanf(pf_file, "%s ", cLine);
	for (int i = 0; i < i_shop_count; i++)
	{
		fscanf(pf_file, "%lf", &dTempVal);
		t_shop_capacity_tab.b_set_val(dTempVal, i);
	}

	fscanf(pf_file, "%s ", cLine);		// cd
	for (int i = 0; i < i_supplier_count; i++)
	{
		for (int j = 0; j < i_factory_count; j++)
		{
			fscanf(pf_file, "%lf", &dTempVal);
			m_delivery_matrix.b_set_value(dTempVal, i, j);
		}
	}

	fscanf(pf_file, "%s ", cLine);		// cf
	for (int i = 0; i < i_factory_count; i++)
	{
		for (int j = 0; j < i_warehouse_count; j++)
		{
			fscanf(pf_file, "%lf", &dTempVal);
			m_factory_matrix.b_set_value(dTempVal, i, j);
		}
	}

	fscanf(pf_file, "%s ", cLine);		// cm
	for (int i = 0; i < i_warehouse_count; i++)
	{
		for (int j = 0; j < i_shop_count; j++)
		{
			fscanf(pf_file, "%lf", &dTempVal);
			m_warehouse_matrix.b_set_value(dTempVal, i, j);
		}
	}

	fscanf(pf_file, "%s ", cLine);
	for (int i = 0; i < i_supplier_count; i++)
	{
		fscanf(pf_file, "%lf", &dTempVal);
		t_suppliers_contract_prizes_tab.b_set_val(dTempVal, i);
	}

	fscanf(pf_file, "%s ", cLine);
	for (int i = 0; i < i_factory_count; i++)
	{
		fscanf(pf_file, "%lf", &dTempVal);
		t_factories_contract_prizes_tab.b_set_val(dTempVal, i);
	}

	fscanf(pf_file, "%s ", cLine);
	for (int i = 0; i < i_warehouse_count; i++)
	{
		fscanf(pf_file, "%lf", &dTempVal);
		t_warehouses_contract_prizes_tab.b_set_val(dTempVal, i);
	}

	fscanf(pf_file, "%s ", cLine);		// shop profit p
	for (int i = 0; i < i_shop_count; i++)
	{
		fscanf(pf_file, "%lf", &dTempVal);
		t_shop_profit_tab.b_set_val(dTempVal, i);
	}

	fscanf(pf_file, "%s ", cLine);		// xdminmax
	for (int i = 0; i < i_supplier_count; i++)
	{
		for (int j = 0; j < i_factory_count; j++)
		{
			fscanf(pf_file, "%lf %lf", &dTempVal, &dTempVal2);
			m_min_supply_values.b_set_value(dTempVal, i, j);
			m_max_supply_values.b_set_value(dTempVal2, i, j);
		}
	}

	fscanf(pf_file, "%s ", cLine);		// xfminmax
	for (int i = 0; i < i_factory_count; i++)
	{
		for (int j = 0; j < i_warehouse_count; j++)
		{
			fscanf(pf_file, "%lf %lf", &dTempVal, &dTempVal2);
			m_min_factories_values.b_set_value(dTempVal, i, j);
			m_max_factories_values.b_set_value(dTempVal2, i, j);
		}
	}

	fscanf(pf_file, "%s ", cLine);		// xmminmax
	for (int i = 0; i < i_warehouse_count; i++)
	{
		for (int j = 0; j < i_shop_count; j++)
		{
			fscanf(pf_file, "%lf %lf", &dTempVal, &dTempVal2);
			m_min_warehouse_values.b_set_value(dTempVal, i, j);
			m_max_warehouse_values.b_set_value(dTempVal2, i, j);
		}
	}

	fclose(pf_file);
	pf_file = NULL;
	return true;
}
