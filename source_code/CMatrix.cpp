#include "CMatrix.h"
#define DEFAULT_MATRIX_SIZE 1
#define INITIAL_VAL -1

CMatrix::CMatrix()
{
	i_xSize = DEFAULT_MATRIX_SIZE;
	i_ySize = DEFAULT_MATRIX_SIZE;
	vInitMatrix(&pd_matrix, i_xSize, i_ySize);
}

CMatrix::CMatrix(int iSizeX, int iSizeY, bool bSuccess)
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

void CMatrix::vDeleteMatrix()
{
	for (int i = 0; i < i_xSize; i++)
	{
		delete[] pd_matrix[i];
	}
	delete[] pd_matrix;
}

bool CMatrix::b_set_size(int iSizeX, int iSizeY)
{
	if (iSizeX < 1 || iSizeY < 0) return false;
	int minXsize = (i_xSize < iSizeX ? i_xSize : iSizeX);
	int minYsize = (i_ySize < iSizeY ? i_ySize : iSizeY);
	double** pdTempMatrix = new double* [iSizeX];
	vInitMatrix(&pdTempMatrix, iSizeX, iSizeY);
	for (int i = 0; i < minXsize; i++)
	{
		for (int j = 0; j < minYsize; j++)
		{
			pdTempMatrix[i][j] = pd_matrix[i][j];
		}
	}
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
