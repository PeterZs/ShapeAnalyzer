//
//  CorrespondenceSet.h
//  ShapeAnalyzer
//
//  Created by Zorah on 09.07.14.
//
//

#ifndef ShapeAnalyzer_CorrespondenceSet_h
#define ShapeAnalyzer_CorrespondenceSet_h

#include "CorrespondenceData.h"

#include <unordered_map>
#include <stdlib.h>

using namespace std;

template <class T>
class CorrespondenceSet {
public:
    CorrespondenceSet();
    CorrespondenceSet(vector<T*>, bool = false);
    ~CorrespondenceSet() {}
    
    void addCorrespondence(T*, bool = false);
    void addCorrespondences(vector<T*>, bool = false);
    
    bool deleteCorrespondence(T*);
    bool deleteCorrespondences(vector<T*>);
    
    bool isContained(T *);
    
    unsigned size();
    
    vector<T*> getRandomSubset(int size);
    
protected:
    // the bool indicates whether or not a correspondence has a
    // vtkActor
    unordered_map<T*, bool> correspondences_;
    // number of stored correspondences
    // notice that this is not the same as correspondences_.size()!
    unsigned                numberOfCorrespondences;
    
};


#endif
