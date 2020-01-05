#pragma once
class CMatrix
{
public:
	CMatrix();
	CMatrix(int iSizeX, int iSizeY, bool& bSuccess);
	~CMatrix();

	CMatrix(const CMatrix& cOther);
	CMatrix operator=(const CMatrix& cOther);

	bool b_set_size(int iSizeX, int iSizeY);
	bool b_set_value(double dVal, int i_xIndex, int i_yIndex);

	double d_get_val(int i_xIndex, int i_yIndex, bool& bSuccess);

	double d_multiply_matrix(const CMatrix& m_other_matrix, bool& bSuccess);
	double d_get_first_dim_sum(int iSecDimNumber, bool& bSuccess);
	double d_get_second_dim_sum(int iFirstDimNumber, bool& bSuccess);

private:
	void vInitMatrix(double*** pd_toInit, int iSizex, int iSizey);
	void vDeleteMatrix();
	void vCopyMatrix(double** copyFromMatrix, double** copyToMatrix, int iSizeX, int iSizeY);
	void vCopyConstructorHelper(const CMatrix& cOther);

	double** pd_matrix;
	int i_xSize;
	int i_ySize;
};

