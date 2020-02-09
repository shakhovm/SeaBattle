#include "extrahandler.h"


bool ExtraHandler::closePointPredicate(QPoint p1, QPoint p2)
{
    if(p1.x() == p2.x()) {
        return abs(p1.y() - p2.y()) == 1;
    }

    if(p1.y() == p2.y()) {
        return abs(p1.x() - p2.x()) == 1;
    }
    return false;
}

void ExtraHandler::sortPointsArray(std::vector<QPoint> &v)
{
    std::vector<std::vector<QPoint>> vec;
    for(auto& x: v)
    {
        for(auto& vect: vec)
        {
            for(auto& elem: vect)
            {
                if (closePointPredicate(elem, x))
                {
                    vect.push_back(x);
                    goto Label;
                }
            }

        }

        vec.emplace_back(1, x);
        Label:
            continue;
    }

    v.clear();
    for (auto& vect: vec)
    {
        std::sort(vect.begin(), vect.end(),
                  [](QPoint p1, QPoint p2){return p1.y() < p2.y() || p1.x() < p2.x();});


        for (auto& x: vect)
        {

            v.push_back(x);
        }
    }
}
