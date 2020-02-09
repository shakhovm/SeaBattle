//
// Created by user on 30.06.2019.
//

#ifndef SEABATTLE_SHIP_H
#define SEABATTLE_SHIP_H

#include <iostream>
#include <vector>
#include <set>

using namespace std;
typedef pair<int, int> P;

bool valid_position(P &position);
class Ship
{


    bool destroyed = false;
    void set_closePositions();


public:
    vector<P> shoot_positions;
    set<P> close_positions;
    int length = 0;
    P first_position;
    P last_position;
    bool is_ship = true;
    vector<P> fields;
    Ship();// = default;
    Ship(P first_position, P last_position, bool is_ship = true);
    ~Ship() = default;
    int size();
    void print();
    bool shot(P position);
    void shoot(P position);
    bool contains(P position);
    bool operator==(const Ship& ship);
    bool operator!=(const Ship& ship);
    bool is_destroyed();

};



#endif //SEABATTLE_SHIP_H
