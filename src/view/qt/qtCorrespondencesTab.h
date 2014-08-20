//
//  qtCorrespondencesTab.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 19.08.14.
//
//

#ifndef ShapeAnalyzer_qtCorrespondencesTab_h
#define ShapeAnalyzer_qtCorrespondencesTab_h

#include "qtTab.h"

class qtCorrespondencesTab : public qtTab {
public:
    //called after a new correspondence was added
    virtual void onCorrespondenceAdd(Correspondence* correspondence) = 0;
    virtual void onCorrespondenceDelete(Correspondence* correspondence) = 0;
    virtual void onCorrespondenceEdit(Correspondence* correspondence) = 0;
    virtual void onCorrespondenceSelect(Correspondence* correspondence) = 0;
    virtual void onCorrespondencesClear() = 0;
};

#endif
