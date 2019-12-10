// TEP_Lista9.cpp

#include <iostream>

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

    std::cout << (6<5?2:5) << std::endl;
    
}