//
//  EigenfunctionCustomMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__EigenfunctionCustomMenuItem__
#define __ShapeAnalyzer__EigenfunctionCustomMenuItem__

#include <string>

#include "CustomContextMenuItem.h"

#include "../../domain/LaplaceBeltramiOperator.h"
#include "../../domain/attributes/ScalarPointAttribute.h"
#include "../../domain/coloring/ScalarPointColoring.h"

#include "../Factory.h"

#include <qinputdialog.h>

using namespace std;

class EigenfunctionCustomMenuItem : public CustomContextMenuItem {
public:
    virtual void onClick(Shape* shape, QWidget* parent);
    
    static string getIdentifier() {
        return "eigenfunction";
    }
    
    static CustomContextMenuItem* create() {
        return new EigenfunctionCustomMenuItem();
    }
    
private:
    EigenfunctionCustomMenuItem() {}
};

#endif /* defined(__ShapeAnalyzer__EigenfunctionCustomMenuItem__) */
