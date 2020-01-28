//
// Created by user on 25.07.2019.
//

#ifndef SEABATTLE_GAME_H
#define SEABATTLE_GAME_H

#include "Field.h"
#include "player.h"

class Game {
    vector<Player> players;
    int current_player = 0;
public:
    Game() = default;
    bool game_over(Field& field);
    void change_player();
    void previous_player();
    Player& get_current_player();
    void add_player(Player player);
    Field& current_field();
    Field& enemy_field();
    void clear();

};


#endif //SEABATTLE_GAME_H
