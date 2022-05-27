#include <iostream>
#include "game.h"
#include "test_game.h"
#include "iplayer.h"
#include "player_random.h"
#include "q_player.h"
#include "my_player.h"
#include <vector>
#include <fstream>
#include <exception>

#define N_EVALUATION_GAMES 2000

using namespace std;

std::vector<int> play_games(game &g, int n_games){
    std::vector<int> wins = {0, 0, 0, 0};
    for(int i = 0; i < n_games; i++){
        g.reset();
        g.set_first(i%4); //alternate who starts the game
        g.play_game();
        int winner = g.get_winner();
        wins[winner] = wins[winner] + 1;
    } 
    return wins;
}

std::vector<int> play_learning_games(game &g, Q_player &learning_agent, int n_games){
    learning_agent.set_learning(true);
    std::vector<int> wins = play_games(g, n_games);
    learning_agent.set_learning(false);
    return wins;
}

std::vector<int> play_learning_games(game &g, my_player &learning_agent, int n_games){
    learning_agent.set_learning(true);
    std::vector<int> wins = play_games(g, n_games);
    learning_agent.set_learning(false);
    return wins;
}

std::vector<int> play_learning_games(game &g, Q_player &learning_agent_1, my_player &learning_agent_2, int n_games){
    learning_agent_1.set_learning(true);
    learning_agent_2.set_learning(true);
    std::vector<int> wins = play_games(g, n_games);
    learning_agent_1.set_learning(false);
    learning_agent_2.set_learning(false);
    return wins;
}

std::vector<int> play_learning_games(game &g, Q_player &learning_agent_1, Q_player &learning_agent_2, int n_games){
    learning_agent_1.set_learning(true);
    learning_agent_2.set_learning(true);
    std::vector<int> wins = play_games(g, n_games);
    learning_agent_1.set_learning(false);
    learning_agent_2.set_learning(false);
    return wins;
}

std::vector<int> evaluate(game &g){
    return play_games(g, N_EVALUATION_GAMES);
}

void write_results(std::vector<int> n_learning_games, std::vector<std::vector<int>> evaluations_wins){
    std::ofstream results_stream;
    results_stream.open("../results/test_2v2_non_pretrained.csv");
    if (!results_stream.is_open()) throw std::exception();
    if (n_learning_games.size() != evaluations_wins.size()) std::cout << "Something went wrong with logging" << std::endl;
    
    results_stream << "n_learning_games_played, Q_player, Random_player1, Random_player2, Random_player3" << "\n";
    for (int i = 0; i < n_learning_games.size(); i++){
        results_stream << n_learning_games[i]       << ",";
        results_stream << evaluations_wins[i][0]    << ",";
        results_stream << evaluations_wins[i][1]    << ",";
        results_stream << evaluations_wins[i][2]    << ",";
        results_stream << evaluations_wins[i][3]    << "\n";
    }
    results_stream.close();
}

int main(){
    //Create Q_table for Q_players
    Q_Table e1_Qtable;
    Q_Table e2_Qtable;
    My_Q_Table jajep_Q_table(false);

    //Create players
    Q_player      player_e1(e1_Qtable);
    Q_player      player_e2(e2_Qtable);
    my_player     player_j(jajep_Q_table, 0.2, 0.6);
    player_random player_1;
    player_random player_2;
    player_random player_3;

    //Pretrain Jajeps (Jacob floe jeppesen) (player_j)
    /*{
        //Create game
        game g(&player_j, &player_1, &player_2, &player_3);

        //Set amount of learning games between evaluations
        std::vector<int> n_learning_games_cumulative = {0, 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000}; //Note that this vector is set up to be cumulative
        for (int i = 4000; i <= 100000; i+=2000){
            n_learning_games_cumulative.push_back(i);
        }
        // Convert from cumulative to non-cumulative
        std::vector<int> n_learning_games;
        n_learning_games.push_back(n_learning_games_cumulative[0]);
        for (int i = 1; i < n_learning_games_cumulative.size(); i++){
            int stepsize_of_games = n_learning_games_cumulative[i] - n_learning_games_cumulative[i-1];
            if (stepsize_of_games < 0) throw std::exception();
            n_learning_games.push_back(stepsize_of_games);
        }

        //Perform learning games and evaluations
        std::vector<std::vector<int>> wins_pr_evaluations;
        for (int i = 0; i < n_learning_games.size(); i++){
            // Perform learning games
            play_learning_games(g, player_j, n_learning_games[i]);

            // Evaluate
            std::vector<int> evaluation_results = evaluate(g);
            wins_pr_evaluations.push_back(evaluation_results);

            // Print results to terminal
            std::cout << "Evaluation " << i << " results: " << std::endl;
            for(int j = 0; j < 4; j++) cout << "Player " << j << " won " << evaluation_results[j] << " games, aka " 
                    << 100.0*evaluation_results[j]/N_EVALUATION_GAMES << "% of games." << endl;
        }
    }*/
    
    //Pretrain my own player (player_e)
    /*{
        //Create game
        game g(&player_e_1, &player_1, &player_2, &player_3);

        //Set amount of learning games between evaluations
        std::vector<int> n_learning_games_cumulative = {0, 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000}; //Note that this vector is set up to be cumulative
        for (int i = 4000; i <= 100000; i+=2000){
            n_learning_games_cumulative.push_back(i);
        }
        // Convert from cumulative to non-cumulative
        std::vector<int> n_learning_games;
        n_learning_games.push_back(n_learning_games_cumulative[0]);
        for (int i = 1; i < n_learning_games_cumulative.size(); i++){
            int stepsize_of_games = n_learning_games_cumulative[i] - n_learning_games_cumulative[i-1];
            if (stepsize_of_games < 0) throw std::exception();
            n_learning_games.push_back(stepsize_of_games);
        }

        //Perform learning games and evaluations
        std::vector<std::vector<int>> wins_pr_evaluations;
        for (int i = 0; i < n_learning_games.size(); i++){
            // Perform learning games
            play_learning_games(g, player_e_1, n_learning_games[i]);

            // Evaluate
            std::vector<int> evaluation_results = evaluate(g);
            wins_pr_evaluations.push_back(evaluation_results);

            // Print results to terminal
            if (i % 10 == 0){
                std::cout << "Evaluation " << i << " results: " << std::endl;
                for(int j = 0; j < 4; j++)
                    cout << "Player " << j << " won " << evaluation_results[j] << " games, aka " 
                        << 100.0*evaluation_results[j]/N_EVALUATION_GAMES << "% of games." << endl;
            }
            
            
        }
    }*/

    //Create new copies of former pretrained players.
    //Q_player player_e2 = Q_player(player_e1);

    // Play and train agents against each other
    //Create game
    game g(&player_e1, &player_e2, &player_2, &player_3);

    //Set amount of learning games between evaluations
    std::vector<int> n_learning_games_cumulative = {0, 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000}; //Note that this vector is set up to be cumulative
    for (int i = 4000; i <= 20000; i+=2000){
        n_learning_games_cumulative.push_back(i);
    }

    // Convert from cumulative to non-cumulative
    std::vector<int> n_learning_games;
    n_learning_games.push_back(n_learning_games_cumulative[0]);
    for (int i = 1; i < n_learning_games_cumulative.size(); i++){
        int stepsize_of_games = n_learning_games_cumulative[i] - n_learning_games_cumulative[i-1];
        if (stepsize_of_games < 0) throw std::exception();
        n_learning_games.push_back(stepsize_of_games);
    }

    //Perform learning games and evaluations
    std::vector<std::vector<int>> wins_pr_evaluations;
    for (int i = 0; i < n_learning_games.size(); i++){
        // Perform learning games
        //play_learning_games(g, player_e1, player_j, n_learning_games[i]);
        play_learning_games(g, player_e1, player_e2, n_learning_games[i]);

        // Evaluate
        std::vector<int> evaluation_results = evaluate(g);
        wins_pr_evaluations.push_back(evaluation_results);

        // Print results to terminal
        if (i % 3 == 0){
            std::cout << "Evaluation " << i << " results: " << std::endl;
            for(int j = 0; j < 4; j++)
                cout << "Player " << j << " won " << evaluation_results[j] << " games, aka " 
                    << 100.0*evaluation_results[j]/N_EVALUATION_GAMES << "% of games." << endl;
        }
    }

    // Print/Log test results to .csv for further analysis in MATLAB
    write_results(n_learning_games_cumulative, wins_pr_evaluations);
    return 0;
}






