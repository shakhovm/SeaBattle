
#include "ship.h"
#include <QDebug>

bool valid_position(P& position)
{
    return (0 <= position.first && position.first < 10) &&
           (0 <= position.second && position.second < 10);
}


Ship::Ship() = default;

void Ship::set_closePositions()
{
    for(int i = 0; i < size(); ++i)
    {
        for(int j = -1; j < 2; ++j)
        {
            for(int k = -1; k < 2; ++k)
            {
                P position(fields[i].first+j, fields[i].second + k);
                if(valid_position(position) && !contains(position))
                {
                    close_positions.insert(position);
                }
            }
        }
    }
}

Ship::Ship(P first_position, P last_position, bool is_ship)
{
    this->first_position = first_position;
    this->last_position = last_position;
    if(!is_ship)
    {
        this->is_ship = false;
        length = 1;


        fields.emplace_back(first_position);

    }
    else
    {

        if(first_position == last_position)
        {

            length = 1;

            fields.emplace_back(first_position.first, first_position.second);

        }
        else
        {
            if(first_position.first == last_position.first)
            {

                length = abs(first_position.second - last_position.second) + 1;
                ;
                for(int i = 0; i < length; ++i)
                {
                    fields.push_back(P(first_position.first, first_position.second + i));
                }
            }
            else
            {
                length = abs(first_position.first - last_position.first) + 1;

                for(int i = 0; i < length; ++i)
                {
                    fields.emplace_back(first_position.first + i, first_position.second);
                }
            }
        }
    }
    set_closePositions();
}

void Ship::shoot(P position)
{

    shoot_positions.push_back(position);
    if(is_ship && (shoot_positions.size() == length))
    {
        destroyed = true;
    }
}

 bool Ship::contains(P position)
{
    for(int i = 0; i < length; ++i)
    {
        if(fields[i] == position)
        {return true;}
    }
    return false;
}



bool Ship::shot(P position)
{
    for(auto& x: shoot_positions)
    {
        if(x == position)
        {
            return true;
        }
    }
    return false;
}

Ship::~Ship(){}

int Ship::size()
{
    return length;
}

bool Ship::is_destroyed()
{
    return destroyed;
}

bool Ship::operator==(const Ship& ship)
{
    return (fields[0] == ship.fields[0]) &&
           (fields[length - 1] == ship.fields[ship.length - 1]);
}

bool Ship::operator!=(const Ship &ship)
{
    return !operator==(ship);
}

void Ship::print()
{
    cout << '[';
    for(int i = 0; i < length; ++i)
    {
        cout << '(' << fields[i].first << ", " << fields[i].second << ')' << ", ";
    }
    cout << ']';
}
