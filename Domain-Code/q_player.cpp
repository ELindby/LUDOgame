#include "q_player.h"
#include <iostream>
#include <exception>
#include <vector>

Q_player::Q_player(/*Q_Table& table*/){
    Q_player(/*table,*/ default_alpha);
    //Q_player(table, default_alpha);
}

Q_player::Q_player(/*Q_Table &table,*/ double learning_rate){
    //q_table = &table;
    alpha = learning_rate;

    std::random_device rd;
    generator = std::mt19937(rd());
}

int Q_player::make_decision(){
    /*********************************************************
     * Determine which move to make with Q-learning player
     *********************************************************/
    // Determine which pieces have valid moves
    // Note: is_valid_move(piece) actually determines if the piece HAS valid moves
    // int movable_pieces[AMOUNT_OF_PIECES];
    // int valid_pieces = 0;
    std::vector<int> movable_pieces;
    //test

    for (int piece_idx = 0; piece_idx < AMOUNT_OF_PIECES; piece_idx++){
        if (is_valid_move(piece_idx)){
            movable_pieces.push_back(piece_idx);
            // movable_pieces[valid_pieces] = piece_idx;
            // valid_pieces += 1;
        }
    }

    // If amount of movable pieces <= 1, decision is forced
    if (movable_pieces.size() == 0){
        return -1;
    }
    if (movable_pieces.size() == 0){
        return ;
    }
    
    // Decide the best piece to move from movable pieces
    for (int i = 0; i < movable_pieces.size(); i++)
    {
        /* code */
    }
    
    
}

std::vector<int> Q_player::predict_positions(int candidate_piece_idx){
    /*********************************************************
     * Predict positions outcome of moving a certain (candidate) piece
     *********************************************************/
    std::vector<int> predicted_positions;
    for (int piece_idx = 0; piece_idx < AMOUNT_OF_PIECES_TOTAL; piece_idx++){
        predicted_positions.push_back(position[piece_idx]);
    }
    
    int candidate_piece_init_pos = position[candidate_piece_idx];
    int candidate_piece_res_pos;

    //If candidate piece is in starting area
    if (candidate_piece_init_pos == -1){ 
        //Move piece to starting position
        candidate_piece_res_pos = 0;
    }
    else{
        candidate_piece_res_pos =  predict_movement(candidate_piece_init_pos);
    }

    //Update positions
    predicted_positions[candidate_piece_idx] = candidate_piece_res_pos;
    predict_knockouts(candidate_piece_idx, predicted_positions);

    return predicted_positions;
}

int Q_player::predict_movement(int init_pos){
    /*********************************************************
     * Predict single position movement using Ludo rules
     *********************************************************/
    int res_pos = init_pos + dice;

    // Check if piece moves to a star (And jump if it is)
    if (res_pos == 5 || res_pos == 18 || res_pos == 31 || res_pos == 44)
        res_pos += 6;
    else if (res_pos == 11 || res_pos == 24 || res_pos == 37 )
        res_pos += 7;
    else if (res_pos == 50)
        res_pos += 6;

    // Check if piece to finishline and react accordingly 
    if (res_pos == 56) //Finish space
        res_pos = 99;
    else if (res_pos > 55) //"Past" finish space (bounces back)
        res_pos = 56 - (res_pos - 56);

    return res_pos;    
}

void Q_player::predict_knockouts(int cand_piece_idx, std::vector<int>& pred_positions){
    /*********************************************************
     * Predict which pieces will be knocked back home and modify pred_pos
     *********************************************************/
    int cand_piece_pos = pred_positions[cand_piece_idx];
    /*The piece being moved be in this position after movement 
      (can still be knocked back by other player on globe)*/
    
    int victim_count = 0;
    int victim_idx; //Only 1 piece saved, no more than one piece can be knocked back
    for (int piece_idx = AMOUNT_OF_PIECES; piece_idx < AMOUNT_OF_PIECES_TOTAL; piece_idx++){
        if (pred_positions[piece_idx] == cand_piece_pos)
            victim_count += 1;
    }

    //If position moved to is a globe and there is an opponent piece, knock moved piece home
    if (globe(cand_piece_pos) && (victim_count > 0))
        pred_positions[cand_piece_idx] = -1;
    
    //If position moved to has multiple opponent pieces, knock moved piece home
    if (victim_count > 1)
        pred_positions[cand_piece_idx] = -1;
    
    //If position moved to has one victim and is not a globe, knock it back home
    if (!globe(cand_piece_pos) && (victim_count == 1))
        pred_positions[victim_idx] = -1;
    
    return; //TODO: Optimize if statements runtime
}

bool Q_player::globe(int position){
    if((position % 13) == 0)
        return true;
    if((position % 13) == 8)
        return true;
    else
        return false;
}
