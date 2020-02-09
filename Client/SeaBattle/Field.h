//
// Created by user on 03.07.2019.
//

#ifndef SEABATTLE_FIELD_H
#define SEABATTLE_FIELD_H
#include "ship.h"
#include <vector>
#include <QDebug>

typedef pair<int, int> P;


class Field {



    vector<Ship> SHIPS;

    


public:
    Ship field[10][10];
    vector<P> POSITIONS;
    vector<P> shoot_positions;
    vector<P> valid_positions;
    int destroyed_ships = 0;



    Field();
    ~Field() = default;

    void set_ship(Ship& ship);
    void shoot(P position);
    inline bool is_shot(const P& position) { return field[position.first][position.second].shot(position); }
    inline bool is_destroyed(const P& position) { return field[position.first][position.second].is_destroyed(); }
    inline bool is_ship(const P& position) { return field[position.first][position.second].is_ship; }
    inline Ship& get_ship(const P& position) { return field[position.first][position.second]; }
    bool create_field(int size);
    bool is_valid(Ship& ship);
    void generate_field();
    void print();
    void show();
    void clear();
    bool contains(const Ship& ship);
    inline bool is_empty() { return POSITIONS.empty(); }
};


#endif //SEABATTLE_FIELD_H
