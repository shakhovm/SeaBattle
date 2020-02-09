//
// Created by user on 25.07.2019.
//

#include "game.h"

bool Game::game_over(Field& field)
{
    return field.destroyed_ships == 10;
}
void Game::set_alPlayer(Player *player)
{
    alPlayer = player;
}

void Game::set_enemyPlayer(Player *player)
{
    enemyPlayer = player;
}


Player* Game::get_alPlayer()
{
    return alPlayer;
}

Player* Game::get_enemyPlayer()
{
    return enemyPlayer;
}


Field& Game::enemy_field()
{
    return enemyPlayer->field;
}


Field& Game::al_field()
{
    return alPlayer->field;
}
