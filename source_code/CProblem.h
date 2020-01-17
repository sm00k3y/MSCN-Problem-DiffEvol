#pragma once
#include "CSolution.h"
class CProblem
{
public:
	virtual bool bConstraintsSatisfied(CSolution& s_solution) = 0;
	virtual bool bGetQuality(CSolution& s_solution, double& result) = 0;
	virtual void vGenerateInstance(int iSeed) = 0;

	virtual bool bWriteToFIle(std::string sFileName) = 0;
	virtual bool bReadFromFile(std::string sFileName) = 0;

	virtual double d_get_min_val_at(int iIndex, bool& bSuccess) = 0;
	virtual double d_get_max_val_at(int iIndex, bool& bSuccess) = 0;
	virtual int i_get_supplier_count() = 0;
	virtual int i_get_factory_count() = 0;
	virtual int i_get_warehouse_count() = 0;
	virtual int i_get_shop_count() = 0;
};

