#ifndef ShapeListItem_H
#define ShapeListItem_H

#include <QString>
#include <QListWidgetItem>

#include "Shape.h"

using namespace std;

class ShapeListItem : public QListWidgetItem {
public:
    ShapeListItem(const QString &text, Shape* shape, QListWidget *view = 0, int type = Type) : QListWidgetItem(text, view, type), shape_(shape) {};
    
    Shape* getShape() {
        return shape_;
    }
private:
    Shape* shape_;
};

#endif