#include <iostream>
class Level
{
public:
    int levelTube[10] = {7, 7, 7, 9, 9, 9, 11, 11, 11, 14};
    int level;
    int **combination;
    int leftMoves[10] = {-1, -1, -1, -1, 22, 21, 30, 30, 32, 45};
    // Default constructor
    Level() : level(1)
    {
        combination = new int *[levelTube[level - 1] - 2];
        for (int i = 0; i < (levelTube[level - 1] - 2); i++)
        {
            combination[i] = new int[4];
        }
        for (int i = 0; i < (levelTube[level - 1] - 2); i++)
        {
            for (int j = 0; j < 4; j++)
            {
                combination[i][j] = 0;
            }
        }
    }
    Level(int l)
    {
        level = l;
        combination = new int *[levelTube[level - 1] - 2];
        for (int i = 0; i < 4; i++)
        {
            combination[i] = new int[4];
        }
    }
    int **set_level(int l)
    {
        level = l;
        combination = new int *[levelTube[level - 1] - 2];
        for (int i = 0; i < (levelTube[level - 1] - 2); i++)
        {
            combination[i] = new int[4];
        }
        for (int i = 0; i < (levelTube[level - 1] - 2); i++)
        {
            for (int j = 0; j < 4; j++)
            {
                combination[i][j] = 0;
            }
        }
        setCombination();
        return combination;
    }
    void setCombination()
    {
        if (level == 1)
        {
            int values[levelTube[level - 1] - 2][4] = {{8, 5, 4, 6}, {5, 8, 4, 6}, {6, 1, 5, 1}, {5, 6, 1, 4}, {8, 8, 1, 4}};
            for (int i = 0; i < (levelTube[level - 1] - 2); i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    combination[i][j] = values[i][j];
                }
            }
        }
        else if (level == 2)
        {
            int values[levelTube[level - 1] - 2][4] = {{8, 2, 1, 2}, {5, 5, 1, 2}, {7, 7, 5, 2}, {8, 7, 1, 5}, {8, 1, 8, 7}};
            for (int i = 0; i < (levelTube[level - 1] - 2); i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    combination[i][j] = values[i][j];
                }
            }
        }
        else if (level == 3)
        {
            int values[levelTube[level - 1] - 2][4] = {{5, 7, 7, 0}, {6, 5, 1, 6}, {6, 5, 6, 0}, {1, 1, 0, 5}, {0, 7, 1, 7}};
            for (int i = 0; i < (levelTube[level - 1] - 2); i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    combination[i][j] = values[i][j];
                }
            }
        }
        else if (level == 4)
        {
            int values[levelTube[level - 1] - 2][4] = {{0, 10, 7, 4}, {6, 6, 1, 6}, {0, 10, 9, 10}, {9, 4, 6, 1}, {4, 4, 7, 10}, {1, 0, 0, 7}, {7, 9, 9, 1}};
            for (int i = 0; i < (levelTube[level - 1] - 2); i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    combination[i][j] = values[i][j];
                }
            }
        }
        else if (level == 5)
        {
            int values[levelTube[level - 1] - 2][4] = {{10, 7, 6, 0}, {7, 9, 9, 6}, {4, 0, 4, 0}, {6, 7, 9, 4}, {10, 1, 9, 4}, {7, 10, 10, 1}, {1, 0, 1, 6}};
            for (int i = 0; i < (levelTube[level - 1] - 2); i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    combination[i][j] = values[i][j];
                }
            }
        }
        else if (level == 6)
        {
            int values[levelTube[level - 1] - 2][4] = {{0, 6, 6, 6}, {4, 4, 7, 9}, {0, 1, 4, 6}, {1, 10, 0, 9}, {1, 10, 7, 10}, {4, 7, 9, 7}, {10, 1, 0, 9}};
            for (int i = 0; i < (levelTube[level - 1] - 2); i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    combination[i][j] = values[i][j];
                }
            }
        }
        else if (level == 7)
        {
            int values[levelTube[level - 1] - 2][4] = {{10, 5, 9, 0}, {8, 7, 4, 10}, {0, 8, 4, 7}, {9, 10, 8, 1}, {5, 9, 5, 6}, {6, 7, 1, 5}, {6, 7, 1, 6}, {9, 10, 0, 8}, {1, 0, 4, 4}};
            for (int i = 0; i < (levelTube[level - 1] - 2); i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    combination[i][j] = values[i][j];
                }
            }
        }
        else if (level == 8)
        {
            int values[levelTube[level - 1] - 2][4] = {{9, 5, 4, 8}, {8, 0, 1, 4}, {7, 8, 10, 6}, {6, 9, 1, 0}, {7, 7, 9, 0}, {10, 10, 0, 5}, {9, 4, 4, 1}, {8, 7, 6, 5}, {10, 1, 5, 6}};
            for (int i = 0; i < (levelTube[level - 1] - 2); i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    combination[i][j] = values[i][j];
                }
            }
        }
        else if (level == 9)
        {
            int values[levelTube[level - 1] - 2][4] = {{10, 5, 9, 0}, {1, 6, 6, 6}, {7, 9, 5, 9}, {4, 7, 0, 10}, {10, 7, 5, 4}, {4, 7, 8, 6}, {0, 1, 6, 9}, {1, 8, 10, 8}, {1, 4, 0, 8}};
            for (int i = 0; i < (levelTube[level - 1] - 2); i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    combination[i][j] = values[i][j];
                }
            }
        }
        else if (level == 10)
        {
            int values[levelTube[level - 1] - 2][4] = {{1, 0, 2, 8}, {3, 7, 9, 9}, {3, 10, 4, 4}, {1, 11, 6, 0}, {2, 8, 5, 7}, {6, 10, 7, 0}, {2, 1, 7, 3}, {5, 11, 1, 2}, {10, 9, 0, 8}, {6, 4, 10, 9}, {6, 11, 5, 3}, {11, 4, 5, 8}};
            for (int i = 0; i < (levelTube[level - 1] - 2); i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    combination[i][j] = values[i][j];
                }
            }
        }
    }
    int get_noTubes()
    {
        return levelTube[level - 1];
    }
    int get_leftMoves()
    {
        return leftMoves[level - 1];
    }
    ~Level()
    {
        for (int i = 0; i < (levelTube[level - 1] - 2); i++)
        {
            delete[] combination[i];
        }
        delete[] combination;
    }
};