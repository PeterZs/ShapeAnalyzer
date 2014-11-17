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
#include "../../domain/correspondences/Correspondence.h"

class qtCorrespondencesTab : public qtTab {
public:
    virtual ~qtCorrespondencesTab() {}
    //called after a new correspondence was added
    //virtual void onCorrespondencesAddAll(vector<CorrespondenceData*>& correspondences) = 0;
    virtual void onCorrespondenceAdd(CorrespondenceData* correspondence) = 0;
    virtual void onCorrespondenceDelete(CorrespondenceData* correspondence) = 0;
    virtual void onCorrespondenceEdit(Correspondence* correspondence) = 0;
    virtual void onCorrespondenceSelect(Correspondence* correspondence) = 0;
    virtual void onPointCorrespondencesClear() = 0;
    virtual void onFaceCorrespondencesClear() = 0;
};

#endif
