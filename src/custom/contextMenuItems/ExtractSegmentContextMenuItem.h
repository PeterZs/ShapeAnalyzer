//
//  ExtractSegmentContextMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__ExtractSegmentContextMenuItem__
#define __ShapeAnalyzer__ExtractSegmentContextMenuItem__

#include <string>

#include "../../util/HashMap.h"

#include "CustomContextMenuItem.h"

#include "../../domain/laplaceBeltrami/LaplaceBeltramiOperator.h"

#include "../../view/ShapeAnalyzerInterface.h"

#include "../Factory.h"

#include <QMessageBox>

using namespace std;

class ExtractSegmentContextMenuItem : public CustomContextMenuItem {
public:
    ExtractSegmentContextMenuItem() {}
    
    virtual void onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent);
};

#endif /* defined(__ShapeAnalyzer__ExtractSegmentContextMenuItem__) */
