#pragma once
class CMatrix
{
public:
	CMatrix();
	CMatrix(int iSizeX, int iSizeY, bool bSuccess);
	void vInitMatrix(double*** pd_toInit, int iSizex, int iSizey);
	~CMatrix();
	void vDeleteMatrix();

	bool b_set_size(int iSizeX, int iSizeY);

	bool b_set_value(double dVal, int i_xIndex, int i_yIndex);

private:
	double** pd_matrix;
	int i_xSize;
	int i_ySize;
};

