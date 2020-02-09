//
// Created by user on 25.07.2019.
//

#ifndef SEABATTLE_GAME_H
#define SEABATTLE_GAME_H

#include "Field.h"
#include "player.h"

class Game {
    Player* alPlayer;
    Player* enemyPlayer;
    int current_player = 0;
public:
    Game(): alPlayer(nullptr), enemyPlayer(nullptr){}
    bool game_over(Field& field);
    Player* get_alPlayer();
    Player* get_enemyPlayer();
    void set_alPlayer(Player* player);
    void set_enemyPlayer(Player* player);
    Field& enemy_field();
    Field& al_field();


};


#endif //SEABATTLE_GAME_H
