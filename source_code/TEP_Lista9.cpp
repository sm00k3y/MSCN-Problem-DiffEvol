// TEP_Lista9.cpp

#include <iostream>
#include <vector>
void tescik(int*** tablicka)
{
    (*tablicka) = new int* [10];
    for (int i = 0; i < 10; i++)
    {
        (*tablicka)[i] = new int[10];
        for (int j = 0; j < 10; j++)
        {
            (*tablicka)[i][j] = i + j;
        }
    }
}

int main()
{
    int** tab = new int* [10];
    tescik(&tab);

    std::cout << tab[0][2] << std::endl;

    //std::cout << (6<5?2:5) << std::endl;

    std::vector<std::vector<int>> matrix(10);
    for (int i = 0; i < 10; i++)
    {
        matrix[i].resize(10);
        for (int j = 0; j < 10; j++)
        {
            matrix[i][j] = i + j;
        }
    }
    std::cout << matrix[0][2] << std::endl;

    matrix[0].resize(15);
    matrix[0][12] = 5;

    std::cout << matrix[0][2] << ", " << matrix[0][12] << std::endl;
    
}