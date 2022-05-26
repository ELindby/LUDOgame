#include "q_table.h"
// #include <vector>
// #include <exception>


Q_Table::Q_Table(){ 
    // for(int s1 = 0; s1 < N_STATES; s1++)
    //     for(int s2 = 0; s2 < N_STATES; s2++)
    //         for(int s3 = 0; s3 < N_STATES; s3++)
    //             for(int s4 = 0; s4 < N_STATES; s4++)
    //                 for (int a = 0; a < N_ACTIONS; a++)
    //                     q_table[s1][s2][s3][s4][a] = init_q_value;
    for(int s1 = 0; s1 < N_STATES; s1++)
        for (int a = 0; a < N_ACTIONS; a++)
            q_table[s1][a] = init_q_value;
    
    std::cout << "Q_Table constructor finished." << std::endl;
}

//double Q_Table::get_Q_value(std::vector<int> state, int action){
//     if(state.size() != 4) throw std::exception();
//     return q_table[state[0]][state[1]][state[2]][state[3]][action];
// }

double Q_Table::get_Q_value(int state, int action){
    return q_table[state][action];
}

// void Q_Table::set_Q_value(std::vector<int> state, int action, double Q_value){
//     if(state.size() != 4) throw std::exception();
//     q_table[state[0]][state[1]][state[2]][state[3]][action] = Q_value;
// }

void Q_Table::set_Q_value(int state, int action, double Q_value){
    q_table[state][action] = Q_value;
}

double Q_Table::get_reward(int action){
    return rewards[action];
}

// double Q_Table::get_max_Q_value(std::vector<int> state){
//     double max_Q_value = -9999;
//     for (int a = 0; a < N_ACTIONS; a++){
//         double Q_value = q_table[state[0]][state[1]][state[2]][state[3]][a];
//         if (Q_value > max_Q_value){
//             max_Q_value = Q_value;
//         }
//     }
//     return max_Q_value;
// }

double Q_Table::get_max_Q_value(int state){
    double max_Q_value = -9999;
    for (int a = 0; a < N_ACTIONS; a++){
        double Q_value = q_table[state][a];
        if (Q_value > max_Q_value){
            max_Q_value = Q_value;
        }
    }
    return max_Q_value;
}