//
//  CorrespondenceSet.h
//  ShapeAnalyzer
//
//  Set that manages an unordered_map with some extra features,
//  a random subset of the set can be chosen and the exact number
//  of items in the set is maintained.
//  Each element is associated with an boolean value.
//
//  Created by Zorah on 09.07.14.
//
//

#ifndef ShapeAnalyzer_CorrespondenceSet_h
#define ShapeAnalyzer_CorrespondenceSet_h

#include <unordered_map>

using namespace std;

// TODO rename later, has nothing to do with correspondences anymore

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
    
    typename unordered_map<T*, bool>::iterator begin();
    typename unordered_map<T*, bool>::iterator end();
    
    bool isContained(T *);
    bool getValue(T *);
    
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

///////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
template<class T>
CorrespondenceSet<T>::CorrespondenceSet() {
    correspondences_ = unordered_map<T*, bool>(0);
    numberOfCorrespondences = 0;
}


///////////////////////////////////////////////////////////////////////////////
// the boolean parameter indicates if the added correspondences are visible in
// the gui / have a vtkActor object, the standard is false
template<class T>
CorrespondenceSet<T>::CorrespondenceSet(vector<T*> correspondences, bool haveActor) {
    correspondences_ = unordered_map<T*, bool>(correspondences.size());
    numberOfCorrespondences = correspondences.size();
    
    addCorrespondences(correspondences, haveActor);
}


///////////////////////////////////////////////////////////////////////////////
// Adding and Deleting Correspondences
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// the boolean parameter indicates if the added correspondence is visible in
// the gui / have a vtkActor object, the standard is false
template<class T>
void CorrespondenceSet<T>::addCorrespondence(T* correspondence, bool hasActor) {
    correspondences_.insert(make_pair<T*, bool>(correspondence, hasActor));
    numberOfCorrespondences++;
}


///////////////////////////////////////////////////////////////////////////////
// the boolean parameter indicates if the added correspondences are visible in
// the gui / have a vtkActor object, the standard is false
template<class T>
void CorrespondenceSet<T>::addCorrespondences(vector<T*> correspondences, bool haveActor) {
    // reserve enough memory for new correspondences
    correspondences_.rehash(numberOfCorrespondences + correspondences.size());
    
    for (auto it = correspondences.begin(); it != correspondences.end(); it++) {
        correspondences_.insert(make_pair<T*, bool>(&it, haveActor));
    }
    
    numberOfCorrespondences += correspondences.size();
}

///////////////////////////////////////////////////////////////////////////////
// deletes correspondence from set, this will have no effect on the gui
// e.g. if the correspondence has an vtkActor it has to be deleted separatly
// returns true if the correspondence was in the set beforehand
// false otherwise
template<class T>
bool CorrespondenceSet<T>::deleteCorrespondence(T* correspondence) {
    if (correspondences_.find(correspondence) != correspondences_.end()) {
        correspondences_.erase(correspondence);
        numberOfCorrespondences--;
        
        return true;
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////////
// deletes correspondences from set, this will have no effect on the gui
// e.g. if the correspondence has an vtkActor it has to be deleted separatly
// returns true if all correspondences were in the set beforehand
// false otherwise
template<class T>
bool CorrespondenceSet<T>::deleteCorrespondences(vector<T*> correspondences) {
    bool success = true;
    for (auto it = correspondences.begin(); it != correspondences.end(); it++) {
        if (!deleteCorrespondences(&it)) {
            success = false;
        }
    }
    
    return success;
}


///////////////////////////////////////////////////////////////////////////////
// Getters
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
template<class T>
vector<T*> CorrespondenceSet<T>::getRandomSubset(int size) {
    // create result vector
    vector<T*> result = vector<T*>(size);
    
    // if size is larger than the number of correspondences available, all
    // correspondences will be returned
    if (size >= numberOfCorrespondences) {
        
        auto it = correspondences_.begin();
        for (int i = 0; i < correspondences_.size(); i++) {
            result.push_back(it->first);
            it++;
        }
        
    } else { // size smaller that number of correspondences
        
        unordered_map<unsigned, bool> indices = unordered_map<unsigned, bool>(size);
        
        int counter = 0;
        
        // generating random unique index sequence
        // the assumption is that the number of correspondences is way
        // larger than the size of the subset, thats why i did not use
        // shuffle
        
        // generate random numbers until size many unique values have been created
        while (counter < size) {
            int v = rand() % numberOfCorrespondences;
            
            if(indices.count(v) == 0) {
                indices.insert(make_pair<unsigned, bool>(v, true));
                counter++;
            }
        }
        
        // iterates over all correspondences and puts the chosen ones in the vector
        // TODO there are probably better ways to do that
        auto it = begin();
        for (int i = 0; i < numberOfCorrespondences; i++) {
            if (indices.count(i) != 0) {
                result.push_back(it->first);
            }
            it++;
        }
    }
    
    return result;
}

///////////////////////////////////////////////////////////////////////////////
template<class T>
unsigned CorrespondenceSet<T>::size() {
    return numberOfCorrespondences;
}


///////////////////////////////////////////////////////////////////////////////
template<class T>
typename unordered_map<T*, bool>::iterator CorrespondenceSet<T>::begin() {
    return correspondences_.begin();
}


///////////////////////////////////////////////////////////////////////////////
template<class T>
typename unordered_map<T*, bool>::iterator CorrespondenceSet<T>::end() {
    return correspondences_.end();
}


///////////////////////////////////////////////////////////////////////////////
// returns true if the input was found in the set, false otherwise
template<class T>
bool CorrespondenceSet<T>::isContained(T* correspondence) {
    if (correspondences_.find(correspondence) != correspondences_.end()) {
        return true;
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////////
// returns the value to the key T* (true or false)
// also returns false when the key was not found
template<class T>
bool CorrespondenceSet<T>::getValue(T* correspondence) {
    auto find = correspondences_.find(correspondence);
    
    if (find != correspondences_.end()) {
        return find->second;
    }
    
    return false;
}


#endif
