#include "image.h"
#include <iostream>

Image::Image()
{
    images.insert("field", QImage(":/Images/read_field.png"));
    images.insert("red cell", QImage(":/Images//red_cell.png"));
    images.insert("orange cell", QImage(":/Images//orange_cell.png"));
    images.insert("blue cell", QImage(":/Images//blue_cell.png"));
    images.insert("white cell", QImage(":/Images//white_cell.png"));
}

const QImage& Image::operator[](const QString& name)
{
    return images[name];
}
