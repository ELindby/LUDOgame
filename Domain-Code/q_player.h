#ifndef Q_PLAYER_H
#define Q_PLAYER_H

#include "iplayer.h"
#include "random"
#include "q_table.h"

//Misc. defines to avoid magic numbers
#define AMOUNT_OF_PIECES        4
#define AMOUNT_OF_PIECES_TOTAL 16
#define HOME_POSITION          -1
#define GOAL_POSITION          99

class Q_player : public iplayer
{
private:
    Q_Table* q_table;

    // Variable parameters
    double alpha         = 0.3;
    double default_alpha = 0.1;
    double gamma         = 0.8; //Discount factor on future rewards

    // Settings
    bool learning = false;

    // Random Generator
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;
public:
    // Constructors
    //Q_player();
    Q_player(Q_Table& table);
    Q_player(Q_Table& table, double learning_rate);
    // Copy constructor
    Q_player(const Q_player& p);

    // Public functions
    void set_learning(bool _learning);

    //Public variables
    int actions_taken[N_ACTIONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
private: // Private functions
    int make_decision();
    std::vector<int> predict_positions(int piece_idx, int& action);
    int predict_movement(int init_pos, int & action);
    void predict_knockouts(int piece_idx, std::vector<int>& pred_positions, int& action);

    //Get state
    // std::vector<int> get_current_state();
    // std::vector<int> get_state_from_pos(std::vector<int> positions);//
    int get_current_state(int candidate_piece);
    int get_state_from_pos(int candidate_piece, std::vector<int> positions);

    //Check tiles
    bool globe(int tile_position);
    bool goal_zone(int tile_position);
    bool in_range_of_opponent(int piece_position);
};


#endif // Q_PLAYER_H