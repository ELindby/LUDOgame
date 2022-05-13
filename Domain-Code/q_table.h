#ifndef Q_TABLE_H
#define Q_TABLE_H

#define N_STATES    4
#define N_ACTIONS  11

enum PIECE_STATE {HOME = 0, SAFE = 1, DANGER = 2, GOAL = 3};
enum ACTIONS     {GO_TO_GOAL, MOVE_FROM_HOME, KNOCK_OPP, KNOCK_SELF, GO_TO_STAR, 
                  GO_TO_SAFE, GO_TO_TEAMMATE, MOVE_NORMAL, MOVE_OUT_OF_RANGE, REBOUND, NOTHING};
//Possible extra actions: Move closest-to-home piece. Move out of range.
//                  {-, -, -, -, -, 
//                  -, -, -, -, -, NOTHING};

class Q_Table{
private:
    double q_table[N_STATES][N_STATES][N_STATES][N_STATES][N_ACTIONS] = {0};
    double init_q_value = 0.25;

public:
    Q_Table();

    double get_Q_value(std::vector<int> state, int action);
    void   set_Q_value(std::vector<int> state, int action, double Q_value);

};


#endif