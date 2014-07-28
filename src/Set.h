//
//  Set.h
//  ShapeAnalyzer
//
//  Set manages an unordered_map with some extra features,
//  a random subset of the set can be chosen and the exact number
//  of items in the set is maintained.
//
//  Created by Zorah on 28.07.14.
//
//

#ifndef ShapeAnalyzer_Set_h
#define ShapeAnalyzer_Set_h

#include <unordered_map>

using namespace std;

// TODO rename later, has nothing to do with correspondences anymore

template <class KEY, class VALUE>
class Set {
    
public:
    // constructors and destructor
    Set();
    Set(vector<KEY*>, vector<VALUE*>);
    ~Set() {}
    
    // adding and deleting correspondences
    void add(KEY*, VALUE);
    void add(pair<KEY*, VALUE>);
    void add(vector<KEY*>, VALUE);
    void add(vector<pair<KEY*, VALUE> >);
    
    bool remove(KEY*);
    bool remove(vector<KEY*>);
    
    // iterators
    typename unordered_map<KEY*, VALUE>::iterator begin();
    typename unordered_map<KEY*, VALUE>::iterator end();
    
    // attributes
    bool        isContained(KEY*);
    VALUE       getValue(KEY*);
    unsigned    size();
    
    vector<KEY*> getRandomSubset(int size);
    
protected:
    unordered_map<KEY*, VALUE> correspondences_;
    // number of stored correspondences
    // notice that this is not the same as correspondences_.size()!
    unsigned                setSize;
    
};

///////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
Set<KEY, VALUE>::Set() {
    correspondences_ = unordered_map<KEY*, VALUE>(0);
    setSize = 0;
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
Set<KEY, VALUE>::Set(vector<KEY*> correspondences, VALUE value) {
    correspondences_ = unordered_map<KEY*, VALUE>(correspondences.size());
    setSize = correspondences.size();
    
    addCorrespondences(correspondences, value);
}


///////////////////////////////////////////////////////////////////////////////
// Adding and Deleting Correspondences
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void Set<KEY, VALUE>::addCorrespondence(KEY* correspondence, VALUE value) {
    correspondences_.insert(make_pair<KEY*, bool>(correspondence, value));
    setSize++;
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void Set<KEY, VALUE>::addCorrespondences(vector<KEY*> correspondences, VALUE value) {
    // reserve enough memory for new correspondences
    correspondences_.rehash(setSize + correspondences.size());
    
    for (auto it = correspondences.begin(); it != correspondences.end(); it++) {
        correspondences_.insert(make_pair<KEY*, VALUE>(&it, value));
    }
    
    setSize += correspondences.size();
}

///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
bool Set<KEY, VALUE>::deleteCorrespondence(KEY* correspondence) {
    if (correspondences_.find(correspondence) != correspondences_.end()) {
        correspondences_.erase(correspondence);
        setSize--;
        
        return true;
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
bool Set<KEY, VALUE>::deleteCorrespondences(vector<KEY*> correspondences) {
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
template<class KEY, class VALUE>
vector<KEY*> Set<KEY, VALUE>::getRandomSubset(int size) {
    // create result vector
    vector<KEY*> result = vector<KEY*>(size);
    
    // if size is larger than the number elements in the set, all keys are
    // returned
    if (size >= setSize) {
        
        auto it = correspondences_.begin();
        for (int i = 0; i < correspondences_.size(); i++) {
            result.push_back(it->first);
            it++;
        }
        
    } else { // size smaller that number of correspondences
        
        unordered_map<unsigned, bool> indices = unordered_map<unsigned, bool>(size);
        
        int counter = 0;
        
        // generating random unique index sequence
        // the assumption is that the number of elements is way
        // larger than the size of the subset, thats why i did not use
        // shuffle
        
        // generate random numbers until size many unique values have been created
        while (counter < size) {
            int v = rand() % setSize;
            
            if(indices.count(v) == 0) {
                indices.insert(make_pair<unsigned, bool>(v, true));
                counter++;
            }
        }
        
        // iterates over all elements and puts the chosen ones in the vector
        // TODO there are probably better ways to do that
        auto it = begin();
        for (int i = 0; i < setSize; i++) {
            if (indices.count(i) != 0) {
                result.push_back(it->first);
            }
            it++;
        }
    }
    
    return result;
}

///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
unsigned Set<KEY, VALUE>::size() {
    return setSize;
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
typename unordered_map<KEY*, VALUE>::iterator Set<KEY, VALUE>::begin() {
    return correspondences_.begin();
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
typename unordered_map<KEY*, VALUE>::iterator Set<KEY, VALUE>::end() {
    return correspondences_.end();
}


///////////////////////////////////////////////////////////////////////////////
// returns true if the input was found in the set, false otherwise
template<class KEY, class VALUE>
bool Set<KEY, VALUE>::isContained(KEY* correspondence) {
    if (correspondences_.find(correspondence) != correspondences_.end()) {
        return true;
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////////
// returns the value to the key, returns null pointer if the key does not exist
template<class KEY, class VALUE>
VALUE Set<KEY, VALUE>::getValue(KEY* correspondence) {
    auto find = correspondences_.find(correspondence);
    
    if (find != correspondences_.end()) {
        return find->second;
    }
    
    return null;
}

#endif
