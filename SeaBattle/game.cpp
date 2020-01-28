//
// Created by user on 25.07.2019.
//

#include "game.h"

bool Game::game_over(Field& field)
{
    return field.destroyed_ships == 10;
}
void Game::add_player(Player player)
{
    players.push_back(player);
}

void Game::change_player()
{
   current_player = (current_player + 1) % static_cast<int>(players.size());
}

Player& Game::get_current_player()
{
    return players[current_player];
}

Field& Game::current_field()
{
    return players[static_cast<int>(current_player)].field;
}

Field& Game::enemy_field()
{
    int s = (current_player + 1) % static_cast<int>(players.size());
    return players[s].field;
}

void Game::clear()
{
    players.clear();
    current_player = 0;
}
