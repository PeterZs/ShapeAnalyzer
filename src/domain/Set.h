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

template <class KEY, class VALUE>
class Set {
    
public:
    // constructors and destructor
    Set();
    Set(vector<KEY>, VALUE);
    ~Set() {}
    
    // adding and deleting correspondences
    void add(KEY, VALUE);
    void add(vector<KEY>, VALUE);
    
    bool remove(KEY);
    bool remove(vector<KEY>);
    void clear();
    
    // iterators
    typename unordered_map<KEY, VALUE>::iterator begin();
    typename unordered_map<KEY, VALUE>::iterator end();
    
    // attributes
    bool        contains(KEY);
    VALUE       getValue(KEY);
    unsigned    size();
    
    vector<KEY> getRandomSubset(int size);
    
protected:
    unordered_map<KEY, VALUE> elements_;
    
    // number of stored elements
    // notice that this is not the same as elements_.size()!
    unsigned setSize;
    
};

///////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
Set<KEY, VALUE>::Set() {
    elements_ = unordered_map<KEY, VALUE>(0);
    setSize = 0;
}


///////////////////////////////////////////////////////////////////////////////
// all keys are added with the same value
template<class KEY, class VALUE>
Set<KEY, VALUE>::Set(vector<KEY> correspondences, VALUE value) {
    elements_ = unordered_map<KEY, VALUE>(correspondences.size());
    setSize = correspondences.size();
    
    addCorrespondences(correspondences, value);
}


///////////////////////////////////////////////////////////////////////////////
// Adding and Deleting Correspondences
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void Set<KEY, VALUE>::add(KEY correspondence, VALUE value) {
    elements_.insert(make_pair<KEY, VALUE>(correspondence, value));
    setSize++;
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void Set<KEY, VALUE>::add(vector<KEY> correspondences, VALUE value) {
    // reserve enough memory for new correspondences
    elements_.rehash(setSize + correspondences.size());
    
    for (auto it = correspondences.begin(); it != correspondences.end(); it++) {
        elements_.insert(make_pair<KEY, VALUE>(&it, value));
    }
    
    setSize += correspondences.size();
}

///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
bool Set<KEY, VALUE>::remove(KEY correspondence) {
    if (elements_.find(correspondence) != elements_.end()) {
        elements_.erase(correspondence);
        setSize--;
        
        return true;
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
bool Set<KEY, VALUE>::remove(vector<KEY> correspondences) {
    bool success = true;
    for (auto it = correspondences.begin(); it != correspondences.end(); it++) {
        if (!deleteCorrespondences(&it)) {
            success = false;
        }
    }
    
    return success;
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void Set<KEY, VALUE>::clear() {
    elements_.clear();
    setSize = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Getters
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
vector<KEY> Set<KEY, VALUE>::getRandomSubset(int size) {
    // create result vector
    vector<KEY> result = vector<KEY>(size);
    
    // if size is larger than the number elements in the set, all keys are
    // returned
    if (size >= setSize) {
        
        auto it = elements_.begin();
        for (int i = 0; i < elements_.size(); i++) {
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
typename unordered_map<KEY, VALUE>::iterator Set<KEY, VALUE>::begin() {
    return elements_.begin();
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
typename unordered_map<KEY, VALUE>::iterator Set<KEY, VALUE>::end() {
    return elements_.end();
}


///////////////////////////////////////////////////////////////////////////////
// returns true if the input was found in the set, false otherwise
template<class KEY, class VALUE>
bool Set<KEY, VALUE>::contains(KEY correspondence) {
    if (elements_.find(correspondence) != elements_.end()) {
        return true;
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////////
// returns the value to the key, returns null pointer if the key does not exist
template<class KEY, class VALUE>
VALUE Set<KEY, VALUE>::getValue(KEY correspondence) {
    auto find = elements_.find(correspondence);
    
    if (find != elements_.end()) {
        return find->second;
    }
    
    return nullptr;
}

#endif
