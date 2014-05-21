#ifndef ShapeListItem_H
#define ShapeListItem_H

#include <vtkSmartPointer.h>

#include <QString>
#include <QListWidgetItem>

#include "vtkShape.h"

using namespace std;

class ShapeListItem : public QListWidgetItem {
public:
    ShapeListItem(
                  const QString                     &text,
                        vtkSmartPointer<vtkShape>   shape,
                        QListWidget                 *view   = 0,
                        int                         type    = Type
    ) : QListWidgetItem(text, view, type), shape_(shape) {};
    
    vtkSmartPointer<vtkShape> getShape() {
        return shape_;
    }
    
    void setName(char* name) {
        setText(*(new QString(name)));
    }
private:
    vtkSmartPointer<vtkShape>shape_;
};

#endif