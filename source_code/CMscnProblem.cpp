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
	vInitTabs();
	vInitMatrixes();
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
		vInitTabs();
		vInitMatrixes();
		pf_file = NULL;
		bSuccess = true;
	}
}

CMscnProblem::~CMscnProblem()
{
	delete pf_file;
}

void CMscnProblem::vInitTabs()
{
	bool bMethodSuccessful = true;
	t_suppliers_contract_prizes_tab = CTable(i_supplier_count, bMethodSuccessful);
	t_factories_contract_prizes_tab = CTable(i_factory_count, bMethodSuccessful);
	t_warehouses_contract_prizes_tab = CTable(i_warehouse_count, bMethodSuccessful);

	t_production_capacity_tab = CTable(i_supplier_count, bMethodSuccessful);
	t_factory_capacity_tab = CTable(i_factory_count, bMethodSuccessful);
	t_warehouse_capacity_tab = CTable(i_warehouse_count, bMethodSuccessful);
	t_shop_capacity_tab = CTable(i_shop_count, bMethodSuccessful);

	t_shop_profit_tab = CTable(i_shop_count, bMethodSuccessful);
}

void CMscnProblem::vInitMatrixes()
{
	bool bMethodSuccessful = true;
	m_delivery_matrix = CMatrix(i_supplier_count, i_factory_count, bMethodSuccessful);
	m_factory_matrix = CMatrix(i_factory_count, i_warehouse_count, bMethodSuccessful);
	m_warehouse_matrix = CMatrix(i_warehouse_count, i_shop_count, bMethodSuccessful);

	m_min_supply_values = CMatrix(i_supplier_count, i_factory_count, bMethodSuccessful);
	m_max_supply_values = CMatrix(i_supplier_count, i_factory_count, bMethodSuccessful);
	m_min_factories_values = CMatrix(i_factory_count, i_warehouse_count, bMethodSuccessful);
	m_max_factories_values = CMatrix(i_factory_count, i_warehouse_count, bMethodSuccessful);
	m_min_warehouse_values = CMatrix(i_warehouse_count, i_shop_count, bMethodSuccessful);
	m_max_warehouse_values = CMatrix(i_warehouse_count, i_shop_count, bMethodSuccessful);
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

bool CMscnProblem::bGetQuality(double* pdSolution, int iSolutionSize, double& result)
{
	if (!bConstraintsSatisfied(pdSolution, iSolutionSize)) return false;

	result = 0;
	int i_solution_index = 0;
	double d_delivery_cost = 0;
	double d_contracts_cost = 0;
	double d_income = 0;
	double d_temp_product_qty;
	bool bMethodSuccessful = true;

	//d_delivery_cost += m_delivery_matrix.d_get_whole_delivery_cost(pdSolution, i_solution_index, bMethodSuccessful);
	//d_delivery_cost += m_factory_matrix.d_get_whole_delivery_cost(pdSolution, i_solution_index, bMethodSuccessful);
	//d_delivery_cost += m_warehouse_matrix.d_get_whole_delivery_cost(pdSolution, i_solution_index, bMethodSuccessful);
	
	for (int i = 0; i < i_supplier_count; i++)
	{
		d_temp_product_qty = 0;
		for (int j = 0; j < i_factory_count; j++)
		{
			d_delivery_cost += (m_delivery_matrix.d_get_val(i, j, bMethodSuccessful) * pdSolution[i_solution_index]);
			d_temp_product_qty += pdSolution[i_solution_index];
			i_solution_index++;
		}
		if (d_temp_product_qty > 0) d_contracts_cost += t_suppliers_contract_prizes_tab.d_get_value(i, bMethodSuccessful);
	}

	for (int i = 0; i < i_factory_count; i++)
	{
		d_temp_product_qty = 0;
		for (int j = 0; j < i_warehouse_count; j++)
		{
			d_delivery_cost += (m_factory_matrix.d_get_val(i, j, bMethodSuccessful) * pdSolution[i_solution_index]);
			d_temp_product_qty += pdSolution[i_solution_index];
			i_solution_index++;
		}
		if (d_temp_product_qty > 0) d_contracts_cost += t_factories_contract_prizes_tab.d_get_value(i, bMethodSuccessful);
	}

	for (int i = 0; i < i_warehouse_count; i++)
	{
		d_temp_product_qty = 0;
		for (int j = 0; j < i_shop_count; j++)
		{
			d_delivery_cost += (m_warehouse_matrix.d_get_val(i, j, bMethodSuccessful) * pdSolution[i_solution_index]);
			d_temp_product_qty += pdSolution[i_solution_index];
			d_income += pdSolution[i_solution_index] * t_shop_profit_tab.d_get_value(j, bMethodSuccessful);
			i_solution_index++;
		}
		if (d_temp_product_qty > 0) d_contracts_cost += t_warehouses_contract_prizes_tab.d_get_value(i, bMethodSuccessful);
	}

	result = d_income - d_delivery_cost - d_contracts_cost;
	return true;
}

bool CMscnProblem::bConstraintsSatisfied(double* pdSolution, int iSolutionSize)
{
	if (pdSolution == NULL || bCheckSolutionForNegativeNumbers(pdSolution, iSolutionSize) || bCheckSolutionSize(iSolutionSize) 
		|| bCheckCapacityOverload(pdSolution) || bCheckInsufficientAmount(pdSolution) || bCheckMinMaxOutOfRage(pdSolution)) return false;
	else return true;
}

bool CMscnProblem::bCheckSolutionSize(int iSolutionSize)
{
	return iSolutionSize != (i_supplier_count * i_factory_count + i_factory_count * i_warehouse_count + i_warehouse_count * i_shop_count);
}

bool CMscnProblem::bCheckSolutionForNegativeNumbers(double* pdSolution, int iSolutionSize)
{
	for (int i = 0; i < iSolutionSize; i++)
	{
		if (pdSolution[i] < 0) return true;
	}
	return false;
}

bool CMscnProblem::bCheckCapacityOverload(double* pdSolution)	// metoda do 4 pierwszych p-punktow tabelki
{
	int i_solution_index = 0;
	double d_product_qty;
	bool bMethodSuccessful = true;

	for (int i = 0; i < i_supplier_count; i++)
	{
		d_product_qty = 0;
		for (int j = 0; j < i_factory_count; j++)
		{
			d_product_qty += pdSolution[i_solution_index];
			i_solution_index++;
		}
		if (d_product_qty > t_production_capacity_tab.d_get_value(i, bMethodSuccessful)) return true;
	}

	for (int i = 0; i < i_factory_count; i++)
	{
		d_product_qty = 0;
		for (int j = 0; j < i_warehouse_count; j++)
		{
			d_product_qty += pdSolution[i_solution_index];
			i_solution_index++;
		}
		if (d_product_qty > t_factory_capacity_tab.d_get_value(i, bMethodSuccessful)) return true;
	}

	int i_second_solution_index = i_solution_index;

	for (int i = 0; i < i_warehouse_count; i++)
	{
		d_product_qty = 0;
		for (int j = 0; j < i_shop_count; j++)
		{
			d_product_qty += pdSolution[i_solution_index];
			i_solution_index++;
		}
		if (d_product_qty > t_warehouse_capacity_tab.d_get_value(i, bMethodSuccessful)) return true;
	}

	int i_temp_solution_index = i_second_solution_index;

	for (int i = 0; i < i_shop_count; i++)
	{
		i_second_solution_index = i_temp_solution_index;
		d_product_qty = 0;
		for (int j = 0; j < i_warehouse_count; j++)
		{
			d_product_qty += pdSolution[i_second_solution_index];
			i_second_solution_index += i_shop_count;
		}
		if (d_product_qty > t_shop_capacity_tab.d_get_value(i, bMethodSuccessful)) return true;
		i_temp_solution_index++;
	}

	return false;
}

bool CMscnProblem::bCheckInsufficientAmount(double* pdSolution)	// metoda do 2 ostatnich p-punktow tabelki
{
	int i_solution_index = 0;
	int i_temp_solution_index = 0;
	int i_warehouse_solution_index = i_supplier_count * i_factory_count;
	double d_sum_TO_factory = 0;
	double d_sum_FROM_factory = 0;

	for (int i = 0; i < i_factory_count; i++)
	{
		i_solution_index = i_temp_solution_index;
		d_sum_TO_factory = 0;
		d_sum_FROM_factory = 0;

		for (int j = 0; j < i_supplier_count; j++)
		{
			d_sum_TO_factory += pdSolution[i_solution_index];
			i_solution_index += i_factory_count;
		}
		i_temp_solution_index++;


		for (int k = 0; k < i_warehouse_count; k++)
		{
			d_sum_FROM_factory += pdSolution[i_warehouse_solution_index];
			i_warehouse_solution_index++;
		}

		if (d_sum_TO_factory < d_sum_FROM_factory) return true;
	}

	i_solution_index = i_supplier_count * i_factory_count;
	i_temp_solution_index = i_solution_index;
	int i_shop_solution_index = i_solution_index + (i_factory_count * i_warehouse_count);

	for (int i = 0; i < i_warehouse_count; i++)
	{
		i_solution_index = i_temp_solution_index;
		d_sum_TO_factory = 0;
		d_sum_FROM_factory = 0;

		for (int j = 0; j < i_factory_count; j++)
		{
			d_sum_TO_factory += pdSolution[i_solution_index];
			i_solution_index += i_warehouse_count;
		}
		i_temp_solution_index++;


		for (int k = 0; k < i_shop_count; k++)
		{
			d_sum_FROM_factory += pdSolution[i_shop_solution_index];
			i_shop_solution_index++;
		}

		if (d_sum_TO_factory < d_sum_FROM_factory) return true;
	}

	return false;
}

bool CMscnProblem::bCheckMinMaxOutOfRage(double* pdSolution)
{
	int i_solution_index = 0;
	bool bMethodSuccessful = true;
	for (int i = 0; i < i_supplier_count; i++)
	{
		for (int j = 0; j < i_factory_count; j++)
		{
			if (pdSolution[i_solution_index] < m_min_supply_values.d_get_val(i, j, bMethodSuccessful)
				|| pdSolution[i_solution_index] > m_max_supply_values.d_get_val(i, j, bMethodSuccessful)) return true;
			i_solution_index++;
		}
	}

	for (int i = 0; i < i_factory_count; i++)
	{
		for (int j = 0; j < i_warehouse_count; j++)
		{
			if (pdSolution[i_solution_index] < m_min_factories_values.d_get_val(i, j, bMethodSuccessful)
				|| pdSolution[i_solution_index] > m_max_factories_values.d_get_val(i, j, bMethodSuccessful)) return true;
			i_solution_index++;
		}
	}

	for (int i = 0; i < i_warehouse_count; i++)
	{
		for (int j = 0; j < i_shop_count; j++)
		{
			if (pdSolution[i_solution_index] < m_min_warehouse_values.d_get_val(i, j, bMethodSuccessful)
				|| pdSolution[i_solution_index] > m_max_warehouse_values.d_get_val(i, j, bMethodSuccessful)) return true;
			i_solution_index++;
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

bool CMscnProblem::bReadSolutionFromFile(std::string sSolutionFileName, double** pdSolution, int& iSolutionSize)
{
	if (pf_file != NULL) fclose(pf_file);
	pf_file = fopen(sSolutionFileName.c_str(), "r");
	if (pf_file == NULL) return false;

	char cLine[MAX_LINE_LEN];
	int is_supplier_count = 0;
	int is_factories_count = 0;
	int is_warehouses_count = 0;
	int is_shop_count = 0;
	double* pd_solution;
	int i_solution_size = 0;
	int i_temp_solution_index = 0;
	double dTempVal = 0;

	fscanf(pf_file, "%s ", cLine);
	fscanf(pf_file, "%i", &is_supplier_count);
	fscanf(pf_file, "%s ", cLine);
	fscanf(pf_file, "%i", &is_factories_count);
	fscanf(pf_file, "%s ", cLine);
	fscanf(pf_file, "%i", &is_warehouses_count);
	fscanf(pf_file, "%s ", cLine);
	fscanf(pf_file, "%i", &is_shop_count);
	
	i_solution_size = is_supplier_count * is_factories_count + is_factories_count * is_warehouses_count + is_warehouses_count * is_shop_count;
	pd_solution = new double[i_solution_size];
	
	fscanf(pf_file, "%s ", cLine);
	for (int i = 0; i < is_supplier_count; i++)
	{
		for (int j = 0; j < is_factories_count; j++)
		{
			fscanf(pf_file, "%lf", &dTempVal);
			pd_solution[i_temp_solution_index] = dTempVal;
			i_temp_solution_index++;
		}
	}

	fscanf(pf_file, "%s ", cLine);
	for (int i = 0; i < is_factories_count; i++)
	{
		for (int j = 0; j < is_warehouses_count; j++)
		{
			fscanf(pf_file, "%lf", &dTempVal);
			pd_solution[i_temp_solution_index] = dTempVal;
			i_temp_solution_index++;
		}
	}

	fscanf(pf_file, "%s ", cLine);
	for (int i = 0; i < is_warehouses_count; i++)
	{
		for (int j = 0; j < is_shop_count; j++)
		{
			fscanf(pf_file, "%lf", &dTempVal);
			pd_solution[i_temp_solution_index] = dTempVal;
			i_temp_solution_index++;
		}
	}

	(*pdSolution) = pd_solution;
	iSolutionSize = i_solution_size;

	fclose(pf_file);
	pf_file = NULL;
	return true;
}
