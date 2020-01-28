#include <utility>

#include "player.h"
#include <qrandom.h>
#include <QDebug>


Player::Player(string name, bool is_computer)
{
    this->name = name;
    this->is_computer = is_computer;

}

Player::~Player()
{

}

void Player::shoot(Field& field, P position)
{
    field.shoot(position);
    if(field.is_ship(position) && field.is_destroyed(position))
    {
        if((length += 1) < 10)
        {
            max_length = LENGTH[length];
        }
    }
}

bool Player::random_shoot(Field& field)
{
    vector<P> potential_positions;
    if(last_shoot != nullptr)
    {


        vector<P> all_positions = {P(last_shoot->first + 1, last_shoot->second),
                                   P(last_shoot->first - 1, last_shoot->second),
                                   P(last_shoot->first, last_shoot->second + 1),
                                   P(last_shoot->first, last_shoot->second - 1)};
        for(auto& position: all_positions)
        {
            if(valid_position(position) && !field.is_shot(position))
            {

                potential_positions.push_back(position);
            }
        }
        if(potential_positions.empty())
        {
            last_shoot = first_shoot;
            return random_shoot(field);
        }
        P choice = potential_positions[qrand() % potential_positions.size()];

        field.shoot(choice);

        if(field.is_ship(choice))
        {
            if(field.is_destroyed(choice))
            {
                last_shoot = nullptr;
                first_shoot = nullptr;
            }
            else {
                last_shoot = new P(choice);
            }
        }

        return field.is_ship(choice);

    }
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {

            vector<P> pos;
            P position(i, j);
            int k1 = i;
            while(k1 < 10 && !field.is_shot(P(k1, j)))
            {

                pos.push_back(P(k1++, j));
            }
            int k2 = i - 1;
            while(k2 >= 0 && !field.is_shot(P(k2, j)))
            {

                pos.push_back(P(k2--, j));
            }

            if(pos.size() >= max_length)
            {
                potential_positions.push_back(position);
                continue;
            }
            int k3 = j;
            while(k3 < 10 && !field.is_shot(P(i, k3)))
            {
                pos.push_back(P(i, k3++));
            }
            int k4 = j - 1;
            while(k4 >= 0 && !field.is_shot(P(i, k4)))
            {
                pos.push_back(P(i, k4--));
            }
            if(pos.size() >= max_length)
            {
                potential_positions.push_back(position);
                continue;
            }

        }
    }
    P position = potential_positions[qrand() % potential_positions.size()];
    field.shoot(position);
    if(field.is_ship(position) && !field.is_destroyed(position))
    {
        last_shoot = new P(position);
        first_shoot = last_shoot;
    }

    return field.is_ship(position);

}

void Player::set_field(Field& field)
{
    this->field = field;
}

void Player::add_rivalField(Field field)
{
    this->rival_field = field;
}
