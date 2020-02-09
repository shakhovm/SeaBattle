//
// Created by user on 25.07.2019.
//

#ifndef SEABATTLE_PLAYER_H
#define SEABATTLE_PLAYER_H


#include "Field.h"

class Player {
    int length = 0;
    int max_length = 4;
    int LENGTH[10] = {4, 3, 3, 2, 2, 2, 1, 1, 1 ,1};
    P* first_shoot = nullptr;
    P* last_shoot = nullptr;
public:


    Field rival_field;
    Field field;
    string name;
    bool is_computer;
    Player(string name, bool is_computer=false);
    ~Player();
    void shoot(Field& field, P position);
    bool random_shoot(Field& field);
    void set_field(Field& field);

    void add_rivalField(Field field);
};


#endif //SEABATTLE_PLAYER_H
