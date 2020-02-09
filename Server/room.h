#include <QTcpSocket>

struct Player {
    QTcpSocket* socket;
    QString name;
    int oponId = -1;
    int id = -1;
    enum {
        NAME,
        ROOM,
        FIELD
    } status = NAME;
};
