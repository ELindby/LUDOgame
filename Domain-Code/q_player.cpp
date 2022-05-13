#include "q_player.h"
#include <iostream>
#include <exception>
#include <vector>

Q_player::Q_player(Q_Table& table){
    Q_player(table, default_alpha);
}

Q_player::Q_player(Q_Table &table, double learning_rate){
    q_table = &table;
    alpha = learning_rate;

    std::random_device rd;
    generator = std::mt19937(rd());
}

int Q_player::make_decision(){
    /*********************************************************
     * Determine which move to make with Q-learning player
     *********************************************************/
    // Determine which pieces have valid moves
    /*Note: is_valid_move(piece) actually determines if the piece HAS valid moves*/
    std::vector<int> movable_pieces;
    for (int piece_idx = 0; piece_idx < AMOUNT_OF_PIECES; piece_idx++){
        if (is_valid_move(piece_idx)){
            movable_pieces.push_back(piece_idx);
        }
    }

    // If amount of movable pieces <= 1, decision is forced
    if (movable_pieces.size() == 0)
        return -1; //Debug value
    if (movable_pieces.size() == 1)
        return movable_pieces[0];
    
    // Decide the best piece to move from movable pieces
    std::vector<int> state  = get_current_state();
    double best_Q_value = -9999;
    std::vector<int> best_moves;
    std::vector<int> predicted_positions; //Initilization outside for time opt.
    for (int i = 0; i < movable_pieces.size(); i++){
        int candidate_piece     = movable_pieces[i];
        int action              = NOTHING; //default value for debug
        predicted_positions     = predict_positions(candidate_piece, action);
        //std::vector<int> state  = get_state_from_pos(predicted_positions);
        
        double Q_value          = q_table->get_Q_value(state, action);

        if (Q_value > best_Q_value){
            best_moves.clear();
            best_moves.push_back(candidate_piece);
            best_Q_value = Q_value;
        }
        else if (Q_value == best_Q_value){
            best_moves.push_back(candidate_piece);
        }
        
    }
    
    return best_moves[0];
}

std::vector<int> Q_player::predict_positions(int candidate_piece_idx, int & action){
    /*********************************************************
     * Predict positions outcome of moving a (candidate) piece. Also determine action
     * input:  Piece that will be moved
     *         Action performed. Passed by reference, also used as output.
     * output: Positions of all pieces after moving
     *********************************************************/
    std::vector<int> predicted_positions;
    for (int piece_idx = 0; piece_idx < AMOUNT_OF_PIECES_TOTAL; piece_idx++){
        predicted_positions.push_back(position[piece_idx]); //Copy from current
    }
    
    int candidate_piece_init_pos = position[candidate_piece_idx];
    int candidate_piece_res_pos;

    //If candidate piece is in starting area
    if (candidate_piece_init_pos == -1){ 
        //Move piece to starting position
        candidate_piece_res_pos = 0;
        action = MOVE_FROM_HOME;
    }
    else{
        candidate_piece_res_pos =  predict_movement(candidate_piece_init_pos, action);
    }

    //Check if piece moves out of range of opponent pieces
    if ( in_range_of_opponent(candidate_piece_init_pos) && 
        (!in_range_of_opponent(candidate_piece_res_pos)) ){
        action = MOVE_OUT_OF_RANGE;
    }

    //Check for "Team-up" / action = GO_TO_TEAMMATE
    if ((!globe(candidate_piece_res_pos)) && (!goal_zone(candidate_piece_res_pos))){ //Not a safe zone
        for (int i = 0; i < AMOUNT_OF_PIECES; i++){
            if ((candidate_piece_idx != i))//Another piece than candidate
                if (predicted_positions[i] == candidate_piece_res_pos)
                    action = GO_TO_TEAMMATE;      
        }
    }
    
    //Update positions (Including knockouts)
    predicted_positions[candidate_piece_idx] = candidate_piece_res_pos;
    predict_knockouts(candidate_piece_idx, predicted_positions, action);

    if(action == NOTHING) throw std::exception();

    return predicted_positions;
}

int Q_player::predict_movement(int init_pos, int & action){
    /*********************************************************
     * Predict position of  a single pieces movement using Ludo rules. Also determine action
     * input: init_pos: Initial position of piece
     *          action: Action performed. Passed by reference, used as output.
     * output: res_pos: Post movement predicted position of piece
     *********************************************************/
    int res_pos = init_pos + dice;
    action = MOVE_NORMAL;

    // Check if piece moves to safety
    if ( globe(res_pos) || ( (!goal_zone(init_pos))&&goal_zone(res_pos) ) ){
        action = GO_TO_SAFE;
    }

    // Check if piece moves to a star (And jump if it is)
    if (res_pos == 5 || res_pos == 18 || res_pos == 31 || res_pos == 44){
        res_pos += 6;
        action = GO_TO_STAR;
    }
    else if (res_pos == 11 || res_pos == 24 || res_pos == 37 ){
        res_pos += 7;
        action = GO_TO_STAR;
    }
    else if (res_pos == 50){
        res_pos += 6;
        action = GO_TO_STAR;
    }

    // Check if piece moves to finishline and react accordingly 
    if (res_pos == 56){ //Finish space
        res_pos = 99;
        action = GO_TO_GOAL;
    }
    else if (res_pos > 55){ //"Past" finish space (bounces back)
        res_pos = 56 - (res_pos - 56);
        action = REBOUND;
    }

    return res_pos;    
}

void Q_player::predict_knockouts(int cand_piece_idx, std::vector<int>& pred_positions, int& action){
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
    if (globe(cand_piece_pos) && (victim_count > 0)){
        pred_positions[cand_piece_idx] = -1;
        action = KNOCK_SELF;
    }
    
    //If position moved to has multiple opponent pieces, knock moved piece home
    if (victim_count > 1){
        pred_positions[cand_piece_idx] = -1;
        action = KNOCK_SELF;
    }
    
    //If position moved to has one victim and is not a globe, knock it back home
    if (!globe(cand_piece_pos) && (victim_count == 1)){
        pred_positions[victim_idx] = -1;
        action = KNOCK_OPP;
    }
    
    return; //TODO: Optimize if statements runtime
}

std::vector<int> Q_player::get_current_state(){
    std::vector<int> current_positions;// = position;

    for (int i = 0; i < AMOUNT_OF_PIECES_TOTAL; i++){
        current_positions.push_back(position[i]);
    }

    return get_state_from_pos(current_positions);
}

std::vector<int> Q_player::get_state_from_pos(std::vector<int> positions){
    /**************************************************************
     * Calculate the state (q-table idx) from (predicted) positions
     **************************************************************/
    // Count amount of pieces in different piece states (for sorted result)
    int n_HOME = 0, n_SAFE = 0, n_DANGER = 0, n_GOAL = 0;
    for (int i = 0; i < AMOUNT_OF_PIECES; i++){
        if ( positions[i] == HOME_POSITION )
            n_HOME++;
        else if ( globe(positions[i]) || (51 <= positions[i] && positions[i] <= 55) )
            n_SAFE++;
        else if ( positions[i] == GOAL_POSITION )
            n_GOAL++;
        else
            n_DANGER++;
    }

    //Place piece states into state in a sorted manner
    std::vector<int> state;
    for (int i = 0; i < n_HOME; i++)
        state.push_back(HOME);
    for (int i = 0; i < n_SAFE; i++)
        state.push_back(SAFE);
    for (int i = 0; i < n_DANGER; i++)
        state.push_back(DANGER);
    for (int i = 0; i < n_GOAL; i++)
        state.push_back(GOAL);
    

    if(state.size() != AMOUNT_OF_PIECES) throw std::exception();
    
    return state;
}



bool Q_player::globe(int tile_position){
    //Check if position input is a globe
    if((tile_position % 13) == 0)
        return true;
    if((tile_position % 13) == 8)
        return true;
    else
        return false;
}

bool Q_player::goal_zone(int tile_position){
    //Check if position input is in the goal zone (5 fields before goal, 51-55)
    if ((tile_position >= 51) && (tile_position <= 55))
        return true;
    else
        return false;
}

bool Q_player::in_range_of_opponent(int piece_position){
    //Check if player piece is in range (6 tiles) of an opponent piece
    if (globe(piece_position) || goal_zone(piece_position))
        return false;

    for (int i = AMOUNT_OF_PIECES; i < AMOUNT_OF_PIECES_TOTAL; i++){
        if (position[i] != -1){ //Not in home
            int dist = piece_position - position[i];
            if ( (1 <= dist) && (dist <= 6) ){
                return true;
            }
            //TODO: If opponent piece is behind, but its path goes to goalzone and not to piece
            //TODO: Check if piece is on star and an opponent is in range of the prev. star
        }
    }

    return false;
}

