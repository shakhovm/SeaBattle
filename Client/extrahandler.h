#ifndef EXTRAHANDLER_H
#define EXTRAHANDLER_H
#include <vector>
#include <QPoint>

class ExtraHandler
{
   static bool closePointPredicate(QPoint p1, QPoint p2);

public:

    static void sortPointsArray(std::vector<QPoint>& v);


};

#endif // EXTRAHANDLER_H
