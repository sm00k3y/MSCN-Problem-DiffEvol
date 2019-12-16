#include "CMatrix.h"
#define DEFAULT_MATRIX_SIZE 1
#define INITIAL_VAL -1

CMatrix::CMatrix()
{
	i_xSize = DEFAULT_MATRIX_SIZE;
	i_ySize = DEFAULT_MATRIX_SIZE;
	vInitMatrix(&pd_matrix, i_xSize, i_ySize);
}

CMatrix::CMatrix(int iSizeX, int iSizeY, bool& bSuccess)
{
	if (iSizeX < 1 || iSizeY < 1)
	{
		bSuccess = false;
		CMatrix();
	}
	else
	{
		i_xSize = iSizeX;
		i_ySize = iSizeY;
		vInitMatrix(&pd_matrix, i_xSize, i_ySize);
	}
}

void CMatrix::vInitMatrix(double*** pd_toInit, int iSizex, int iSizey)
{
	*pd_toInit = new double* [iSizex];
	for (int i = 0; i < iSizex; i++)
	{
		(*pd_toInit)[i] = new double[iSizey];
		for (int j = 0; j < iSizey; j++)
		{
			(*pd_toInit)[i][j] = INITIAL_VAL;
		}
	}
}

CMatrix::~CMatrix()
{
	vDeleteMatrix();
}

CMatrix::CMatrix(const CMatrix& cOther)
{
	vCopyConstructorHelper(cOther);
}

CMatrix CMatrix::operator=(const CMatrix& cOther)
{
	vDeleteMatrix();
	vCopyConstructorHelper(cOther);
	return *this;
}

void CMatrix::vDeleteMatrix()
{
	for (int i = 0; i < i_xSize; i++)
	{
		delete[] pd_matrix[i];
	}
	delete[] pd_matrix;
}

void CMatrix::vCopyMatrix(double** copyFromMatrix, double** copyToMatrix, int iSizeX, int iSizeY)
{
	for (int i = 0; i < iSizeX; i++)
	{
		for (int j = 0; j < iSizeY; j++)
		{
			copyToMatrix[i][j] = copyFromMatrix[i][j];
		}
	}
}

void CMatrix::vCopyConstructorHelper(const CMatrix& cOther)
{
	i_xSize = cOther.i_xSize;
	i_ySize = cOther.i_ySize;
	pd_matrix = new double* [i_xSize];
	for (int i = 0; i < i_xSize; i++)
	{
		pd_matrix[i] = new double[i_ySize];
	}
	vCopyMatrix(cOther.pd_matrix, pd_matrix, i_xSize, i_ySize);
}

bool CMatrix::b_set_size(int iSizeX, int iSizeY)
{
	if (iSizeX < 1 || iSizeY < 0) return false;
	int minXsize = (i_xSize < iSizeX ? i_xSize : iSizeX);
	int minYsize = (i_ySize < iSizeY ? i_ySize : iSizeY);
	double** pdTempMatrix = new double* [iSizeX];
	vInitMatrix(&pdTempMatrix, iSizeX, iSizeY);
	vCopyMatrix(pd_matrix, pdTempMatrix, minXsize, minYsize);
	vDeleteMatrix();						//deleting pd_matrix
	pd_matrix = pdTempMatrix;
	i_xSize = iSizeX;
	i_ySize = iSizeY;
	return true;
}

bool CMatrix::b_set_value(double dVal, int iXindex, int iYindex)
{
	if (iXindex < 0 || iXindex >= i_xSize || iYindex<0 || iYindex>=i_ySize || dVal < 0) return false;
	pd_matrix[iXindex][iYindex] = dVal;
	return true;
}

double CMatrix::d_get_val(int i_xIndex, int i_yIndex, bool& bSuccess)
{
	bSuccess = true;
	if (i_xIndex < 0 || i_xIndex > i_xSize || i_yIndex < 0 || i_yIndex > i_ySize)
	{
		bSuccess = false;
		return -1;
	}
	else return pd_matrix[i_xIndex][i_yIndex];
}

double CMatrix::d_get_whole_delivery_cost(double* pdSolution, int& iSolutionIndex, bool& bSuccess)
{
	bSuccess = true;
	if (iSolutionIndex < 0)
	{
		bSuccess = false;
		return -1;
	}
	double dRetVal = 0;
	for (int i = 0; i < i_xSize; i++)
	{
		for (int j = 0; j < i_ySize; j++)
		{
			dRetVal += (pd_matrix[i][j] * pdSolution[iSolutionIndex]);
			iSolutionIndex++;
		}
	}
	return dRetVal;
}
