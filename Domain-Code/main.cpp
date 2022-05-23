#include <iostream>
#include "game.h"
#include "test_game.h"
#include "iplayer.h"
#include "player_random.h"
#include "q_player.h"
#include <vector>

using namespace std;

std::vector<int> play_games(game &g, int n_games, bool learning){
    std::vector<int> wins = {0, 0, 0, 0};
    for(int i = 0; i < n_games; i++){
        g.reset();
        g.set_first(i%4); //alternate who starts the game
        g.play_game();
        wins[g.get_winner()]++;
    } 
}

int main(){
    //Run the unit tests
    /*test_game tester;
    tester.run_all_tests();*/

    //Create Q_table for Q_players
    Q_Table p0_Qtable;

    //Create players
    Q_player      player_0(p0_Qtable);
    player_random player_1;
    player_random player_2;
    player_random player_3;

    //Play a game of Ludo
    game g(&player_0, &player_1, &player_2, &player_3);
    g.play_game();
    cout << "Player " << g.get_winner() << " won the game!" << endl << endl;

    //Play many games of Ludo
    int wins[] = {0, 0, 0, 0};
    int n_games             = 3000000;
    int n_games_pr_batch    = 10000;
    int batches             = n_games / n_games_pr_batch;
    for (int batch = 0; batch < batches; batch++)
    {
        int batch_wins[] = {0, 0, 0, 0};
        for(int i = 0; i < n_games_pr_batch; i++)
        {
            g.reset();
            g.set_first(i%4); //alternate who starts the game
            g.play_game();
            batch_wins[g.get_winner()]++;
            wins[g.get_winner()]++;
        }
        std::cout << "Batch " << batch << " wins: " << std::endl;
        for(int i = 0; i < 4; i++)
            cout << "Player " << i << " won " << batch_wins[i] << " games, aka " 
                 << 100.0*batch_wins[i]/n_games_pr_batch << "% of games." << endl;
    }
    
    std::cout << "--- Total wins: " << std::endl;
    for(int i = 0; i < 4; i++)
        cout << "Player " << i << " won " << wins[i] << " games, aka " 
             << 100.0*wins[i]/n_games << "% of games." << endl;

    cout << "States (position) max Q values:" << endl;
    for (int i = 0; i < 58; i++)
    {
        cout << "State(pos) " << i << " has max Qval: " << p0_Qtable.get_max_Q_value(i) << endl;
    }

    cout << "Number of times actions have been taken:" << endl;
    for (int i = 0; i < N_ACTIONS; i++)
    {
        cout << "Action " << i << " has been chosen: " << player_0.actions_taken[i] << "times."<< endl;
    }
    

    // int wins[] = {0, 0, 0, 0};
    // int n_games             = 50000;
    // int n_games_pr_batch    = 500;
    // for(int i = 0; i < n_games; i++)
    // {
    //     g.reset();
    //     g.set_first(i%4); //alternate who starts the game
    //     g.play_game();
    //     wins[g.get_winner()]++;
    // }
    // for(int i = 0; i < 4; i++)
    //     cout << "Player " << i << " won " << wins[i] << " games, aka " 
    //          << 100.0*wins[i]/n_games << "% of games." << endl;


    cout << "End of main" << endl;
    return 0;
}




