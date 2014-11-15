//
//  ColorEigenfunctionCustomMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__ColorEigenfunctionCustomMenuItem__
#define __ShapeAnalyzer__ColorEigenfunctionCustomMenuItem__

#include <string>

#include "CustomContextMenuItem.h"

#include "../../domain/LaplaceBeltramiOperator.h"
#include "../../domain/attributes/ScalarPointAttribute.h"
#include "../../domain/coloring/ScalarPointColoring.h"

#include "../Factory.h"

#include <qinputdialog.h>

using namespace std;

class ColorEigenfunctionCustomMenuItem : public CustomContextMenuItem {
public:
    virtual void onClick(Shape* shape, QWidget* parent);
    
    static string getIdentifier() {
        return "color_eigenfunction";
    }
    
    static CustomContextMenuItem* create() {
        return new ColorEigenfunctionCustomMenuItem();
    }
    
private:
    ColorEigenfunctionCustomMenuItem() {}
};

#endif /* defined(__ShapeAnalyzer__ColorEigenfunctionCustomMenuItem__) */
