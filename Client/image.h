#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <QMap>


class Image
{
    QMap<QString, QImage> images;
public:
    Image();

    QImage& operator[](const QString& name);

};

#endif // IMAGE_H
