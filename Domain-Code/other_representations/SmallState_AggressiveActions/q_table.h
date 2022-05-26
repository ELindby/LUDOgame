#ifndef Q_TABLE_H
#define Q_TABLE_H
#include <iostream>
#include <exception>
#include <vector>

//#define N_STATES    4
#define N_STATES   58
#define N_ACTIONS  11

enum PIECE_STATE {HOME = 0, SAFE = 1, DANGER = 2, GOAL = 3};
enum ACTIONS     {KNOCK_OPP, GO_TO_GOAL, MOVE_FROM_HOME, MOVE_FURTHEST, MOVE_NORMAL, KNOCK_SELF, GO_TO_STAR, 
                  GO_TO_SAFE, GO_TO_TEAMMATE,  MOVE_OUT_OF_RANGE, REBOUND, NOTHING};
//Possible extra actions: Move closest-to-home piece. Move out of range.
//                  {-, -, -, -, -, 
//                  -, -, -, -, -, NOTHING};//

class Q_Table{
private:
    //double q_table[N_STATES][N_STATES][N_STATES][N_STATES][N_ACTIONS] = {0};
    double q_table[N_STATES][N_ACTIONS] = {0};
    double init_q_value = 0.0;

    //Rewards
    std::vector<double> rewards{1.0, 0.8, 0.6, 0.4, 0, -1.0, 0.3,
                                0.45, 0.5, 0.1, 0.4, -0.9};
    //std::vector<double> rewards{0.8, 0.5, 0.65, -0.9, 0.6,
    //                            0.3, 0.4, 0.1, 0.2, -0.8, -0.5};

public:
    Q_Table();

    // double get_Q_value(std::vector<int> state, int action);
    // void   set_Q_value(std::vector<int> state, int action, double Q_value);
    // double get_max_Q_value(std::vector<int> state);
    double get_Q_value(int state, int action);
    void   set_Q_value(int state, int action, double Q_value);
    double get_max_Q_value(int state);

    double get_reward(int action);

};


#endif