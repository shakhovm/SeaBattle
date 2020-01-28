//
// Created by user on 03.07.2019.
//

#include "Field.h"
#include <random>



vector<P> all_positions(P first_position, P last_position, int size)
{
    vector<P> positions;
    if(first_position.first == last_position.second)
    {

        for(int i = 0; i < size; ++i)
        {
            positions.emplace_back(P(first_position.first, first_position.first + i));
        }
    } else
    {
        for(int i = 0; i < size; ++i)
        {
            positions.emplace_back(P(first_position.first + i, first_position.first));
        }
    }
    return positions;
}

Field::Field()
{
    clear();
}

Field::~Field()
{
}

void Field::set_ship(Ship& ship)
{
    for(int i = 0; i < ship.size(); ++i)
    {


        field[ship.fields[i].first][ship.fields[i].second] = ship;
        POSITIONS.push_back(ship.fields[i]);
    }
}


void Field::shoot(P position)
{
    Ship ship = field[position.first][position.second];
    for(int i = 0; i < ship.size(); ++i)
    {
        field[ship.fields[i].first][ship.fields[i].second].shoot(position);
    }
    if(field[position.first][position.second].is_destroyed())
    {
        ++destroyed_ships;
        for(auto& close_position: ship.close_positions)
        {
            if(!is_shot(close_position))
            {
                shoot(close_position);
            }
        }
    }
}

bool Field::is_valid(Ship& ship)
{
    if(ship.size() > 4 || ship.size() <= 0)
    {
        return false;
    }
    for(auto& sh: ship.fields)
    {
        for(int i = -1; i < 2; ++i)
        {

            if(!(sh.first + i < 0 || sh.first + i >= 10))
            {
                for(int j = -1; j < 2; ++j)
                {

                    if(sh.second + j >= 0 &&  sh.second + j < 10)
                    {

                        if(is_ship(P(sh.first + i, sh.second + j)))
                        {

                            return false;
                        }
                    }



                }
            }

        }
    }

    return true;

}

bool Field::create_field(int size)
{
    //std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    //std::uniform_int_distribution<int> distribution(0, 9);
    //int row = distribution(generator);
    //int col = distribution(generator);
    int row = qrand() % 10;
    int col = qrand() % 10;
    vector<Ship> all_ships;
    vector<pair<P, P>> positions = {pair<P, P>(P(row, col), P(row + size - 1, col)),
                                    pair<P, P>(P(row - size + 1, col), P(row, col)),
                                    pair<P, P>(P(row, col), P(row, col + size - 1)),
                                    pair<P, P>(P(row, col - size + 1), P(row, col))};
    for(auto& position: positions)
    {
        if(valid_position(position.first) && valid_position(position.second))
        {
            Ship ship(position.first, position.second);

            if(is_valid(ship))
            {
                all_ships.push_back(ship);
            }
        }
    }
    if(all_ships.empty())
    {
        return false;
    }

    int choice = qrand() % all_ships.size();

    set_ship(all_ships[choice]);
    return true;
}


void Field::generate_field()
{
    vector<int> sizes = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    for(auto& size: sizes)
    {
        while(!create_field(size));
    }
}


void Field::show()
{



}

void Field::clear()
{
    field = new Ship*[10];
    for(int i = 0; i < 10; ++i)
    {

        field[i] = new Ship[10];

        for(int j = 0; j < 10; ++j)
        {
            field[i][j] = Ship(P(i, j), P(i, j), false);


        }

    }
    destroyed_ships = 0;
    POSITIONS.clear();
}

bool Field::contains(const Ship &ship)
{
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            if(field[i][j] == ship)
            {
                return true;
            }
        }
    }
    return false;
}

