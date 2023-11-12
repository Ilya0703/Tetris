
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;


const int size_x = 10;
const int size_y = 20;

class Figurs {
private:
    int figures[7][4] = {
            {3,1,5,7} , // I
    {4,2,5,7}, // S
    {5,3,4,6}, // Z
    {5,3,4,7}, // T
    {5,3,2,7}, // L
    {5,3,7,6}, // J
    {4,3,2,5} };//O
public:
    Figurs() {
    }
    int get_number_figure() {
//        srand(time(NULL));
//        rand()%7;
//        return rand() % 7;
    random_device rd;   // non-deterministic generator
    mt19937 gen(rd());  // to seed mersenne twister.
    uniform_int_distribution<> dist(0,6);
    return dist(gen);
    }

     vector<int> get_figure() {
        int ind = get_number_figure();
        vector<int>a;
        for (int i = 0;i < 4;i++)
            a.push_back(figures[ind][i]);
        return a;
    }
};


class tetris {
protected:
    vector<std::vector<int>> matrix;
    vector<std::vector<int>> matrix_of_figure_x;
    vector<std::vector<int>> matrix_of_figure_y;
    int center[2];//0-x,1-y;

    bool flag_finish_game=0;
    bool flag_restart=0;
    bool flag_cur_matrix = 0;//0-y,1-x
    bool flag_finish = 0;
    bool flag_need_new_figure = 0;
private:
    void transp() {
        matrix_of_figure_y=vector<vector<int> >(matrix_of_figure_x[0].size(),vector<int>(matrix_of_figure_x.size()));

        for (int i = 0; i < matrix_of_figure_x.size(); i++) {
            for (int j = 0; j < matrix_of_figure_x[i].size(); j++) {
                matrix_of_figure_y[j][i] = matrix_of_figure_x[i][j];
            }
        }
    }
public:
    tetris() {
        matrix = vector<vector<int>>(size_y, vector<int>(size_x, 0));
        matrix = add_figure();
    }

    tetris(std::vector<std::vector<int>> m, string act) {
        matrix = m;
        flag_need_new_figure = 0;
    }

    std::vector<std::vector<int>> update_matrix_copy(std::vector<std::vector<int>>matrix_copy) {
        for (int i = 0; i < size_y; i++)
            for (int j = 0; j < size_x; j++)
                if (matrix_copy[i][j] == 2)
                    matrix_copy[i][j] = 1;
        return matrix_copy;
    }

    void update_matrix() {
        for (int i = 0; i < size_y; i++)
            for (int j = 0; j < size_x; j++)
                if (matrix[i][j] == 2)
                    matrix[i][j] = 1;
    }

    void finish_game() {
        flag_finish_game=1;
    }

    bool get_flag_finish_game(){
        return flag_finish_game;
    }

    vector<std::vector<int>> add_figure() {
        Figurs a;
        vector<int> new_figure;
        new_figure = a.get_figure();
        matrix_of_figure_x = vector<vector<int>>(4, vector<int>(2));
        for (int i = 0; i < 4; i++) {
            int x = new_figure[i] / 2;
            int y = new_figure[i] % 2;
            if(i==0){
                center[0]=y;
                center[1]=x;
            }
            matrix_of_figure_x[x][y] = 1;
        }
        transp();
        for (int i = 0; i < 4; i++) {
            for (int j = 4; j < 6; j++) {
                if (matrix_of_figure_x[i][j - 4] == 1) {
                    if (matrix[i][j] == 0)
                        matrix[i][j] = 2;
                    else
                        finish_game();
                }
            }
        }
        center[0]+=4;
        return matrix;
    }

    std::vector<std::vector<int>> down() {
        std::vector<std::vector<int>> matrix_copy = matrix;
        for (int i = size_y - 1; i >= 0; i--)
            for (int j = 0; j < size_x; j++) {
                if (matrix[i][j] == 2 and i < size_y - 1 and matrix[i + 1][j] != 1) {
                    if(matrix[i + 1][j] == 0 ) {
                        matrix[i][j] = 0;
                        matrix[i + 1][j] = 2;
                    }
                }
                else if (matrix[i][j] == 2 and i == size_y - 1) {
                    flag_need_new_figure = 1;
                    matrix_copy = update_matrix_copy(matrix_copy);
                    return matrix_copy;

                }else if (matrix[i][j] == 2 and matrix[i + 1][j] == 1) {
                    flag_need_new_figure = 1;
                    matrix_copy = update_matrix_copy(matrix_copy);
                    return matrix_copy;
                }
            }
        center[1]+=1;
        return matrix;
    }

    vector<std::vector<int>> left() {
        vector<std::vector<int>> matrix_copy = matrix;
        for (int i = 0; i < size_y; i++)
            for (int j = 0; j < size_x; j++) {
                if (matrix[i][j] == 2 and j == 0)
                    return matrix_copy;
                else if (matrix[i][j] == 2 and matrix[i][j - 1] == 0) {
                    matrix[i][j] = 0;
                    matrix[i][j - 1] = 2;
                } else if (matrix[i][j] == 2 and matrix[i][j - 1] == 1) {
                    return matrix_copy;
                }
            }
        center[0]-=1;
        return matrix;
    }

    vector<std::vector<int>> right() {
        vector<std::vector<int>> matrix_copy = matrix;
        for (int i = 0; i < size_y; i++)
            for (int j = size_x-1; j >= 0; j--) {
                if (matrix[i][j] == 2 and j == size_x - 1)
                    return matrix_copy;
                else if (matrix[i][j] == 2 and matrix[i][j + 1] == 0) {
                    matrix[i][j] = 0;
                    matrix[i][j + 1] = 2;
                } else if (matrix[i][j] == 2 and matrix[i][j + 1] == 1) {
                    return matrix_copy;
                }
            }
        center[0]+=1;
        return matrix;
    }

    void delete_str(int i) {
        for (int j = i; j > 0; j--)
            for (int k = 0; k < size_x; k++)
                matrix[j][k] = matrix[j - 1][k];
        for (int k = 0; k < size_x; k++)
            matrix[0][k] = 0;
    }

    vector<std::vector<int>> delete_zapoln() {
        for (int i = 0; i < size_y; i++) {
            bool flag = 1;
            for (int j = 0; j < size_x; j++) {
                if (matrix[i][j] == 0 or matrix[i][j] == 2)
                    flag = 0;
            }
            if (flag) {
                delete_str(i);
            }
        }
        return matrix;
    }

    vector<std::vector<int>> rotate() {
        vector<vector<int>> matrix_copy(matrix[0].size(), vector<int>(matrix.size(), 0));
        matrix_copy=matrix;
        vector<vector<int>> rotatedMatrix(matrix.size(), vector<int>(matrix[0].size(), 0));
        for (int i = 0; i < matrix.size() and !flag_restart; i++) {
            for (int j = 0; j < matrix[0].size() and !flag_restart; j++) {
                if (matrix[i][j] == 2) {
                    int offsetX = i - center[1];
                    int offsetY = j - center[0];

                    int rotatedX = center[0] - offsetX;
                    int rotatedY = center[1] + offsetY;

                    if (rotatedX < 0 ){
                        matrix=right();
                        flag_restart=1;
//                        rotate();
                        }
                    else if(rotatedX >= rotatedMatrix[0].size()){
                        matrix=left();
                        flag_restart=1;
//                        rotate();
                    }
                    else if(rotatedY < 0){
                        matrix=down();
                        flag_restart=1;
//                        rotate();
                    }
                    else if(rotatedY >= rotatedMatrix.size())
                    {
                        for (int i = 0; i < matrix.size(); i++)
                            for (int j = 0; j < matrix[0].size(); j++) {
                                matrix[i][j] = 0;
                                matrix[i-1][j]=2;
                            }
                        center[1]-=1;
                        flag_restart=1;
//                        rotate();
                    }
                    else {
                        if(matrix[rotatedY][rotatedX]!=1)
                            rotatedMatrix[rotatedY][rotatedX] = matrix[i][j];
                        else
                            return matrix_copy;
                    }
                } else {
                    if (rotatedMatrix[i][j]!=2)
                        rotatedMatrix[i][j] = matrix[i][j];
                }
            }
        }
        if(!flag_restart) {
            matrix = rotatedMatrix;
            return matrix;
        }
        else
            return matrix_copy;
    }

    void reset(){
        flag_finish_game=0;
        matrix = vector<vector<int>>(size_y, vector<int>(size_x, 0));
        matrix = add_figure();
    }

    void preprocess_action_str(string act) {
        if (act == "down") {
            matrix = down();
            matrix=delete_zapoln();
            if (flag_need_new_figure)
                matrix = add_figure();
            flag_need_new_figure=0;
        }
        if (act == "left") {
            matrix = left();
            matrix=delete_zapoln();
        }
        if (act == "right") {
            matrix = right();
            matrix=delete_zapoln();
        }
        if (act == "reset") {
            reset();
        }
        if (act == "rotate") {
            matrix = rotate();
            if(flag_restart==1) {
                while (flag_restart == 1) {
                    flag_restart = 0;
                    matrix = rotate();
                }
            }
            matrix=delete_zapoln();
        }
    }

    vector<std::vector<int>> get_matrix() {
        return matrix;
    }

    void show() {
        for (int i = 0; i < size_y; i++) {
            for (int j = 0; j < size_x; j++) {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};

