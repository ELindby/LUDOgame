#ifndef Q_PLAYER_H
#define Q_PLAYER_H

#include "iplayer.h"
#include "random"
#include "q_table.h"

//Misc. defines to avoid magic numbers
#define AMOUNT_OF_PIECES        4
#define AMOUNT_OF_PIECES_TOTAL 16

class Q_player : public iplayer
{
private:
    //Q_Table* q_table;

    // Variable parameters
    double alpha         = 0.1;
    double default_alpha = 0.1;

    // Settings
    bool learning = true;

    // Random Generator
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;
public:
    // Constructors
    Q_player(/*Q_Table& table*/);
    Q_player(/*Q_Table& table, */double learning_rate);

    // Public functions
    
private:
    // Private functions
    int make_decision();
    std::vector<int> predict_positions(int piece_idx);
    int predict_movement(int init_pos);
    void predict_knockouts(int piece_idx, std::vector<int>& pred_positions);
    int get_state_from_pos(std::vector<int> positions);

    bool globe(int position);
};


#endif // Q_PLAYER_H