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
    typedef typename unordered_map<KEY, VALUE>::iterator iterator;
    
    // constructors and destructor
    Set();
    Set(Set<KEY, VALUE>*);
    Set(vector<KEY>, VALUE);
    ~Set() {}
    
    // adding and deleting correspondences
    void add(KEY, VALUE);
    void add(vector<KEY>, VALUE);
    
    bool remove(KEY);
    bool remove(vector<KEY>);
    void clear();
    
    // iterators
    iterator begin();
    iterator end();
    
    // attributes
    bool        contains(KEY);
    VALUE       getValue(KEY);
    VALUE       operator[](KEY);
    
    unsigned    size();
    
    void getRandomSubset(unsigned size, Set<KEY, VALUE>*);
    void getRandomKeySubset(unsigned size, vector<KEY>*);
    void getRandomValueSubset(unsigned size, vector<VALUE>*);
    
    unordered_map<KEY, VALUE>* getElements();
    
protected:
    unordered_map<KEY, VALUE> elements_;
    
    // number of stored elements
    // notice that this is not the same as elements_.size()!
    unsigned setSize;
    
    void getRandomIndices(unsigned size, unordered_map<unsigned, bool>*);
    
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
template<class KEY, class VALUE>
Set<KEY, VALUE>::Set(Set<KEY, VALUE>* set) {
    elements_ = *unordered_map<KEY, VALUE>(set->getElements());
    setSize = set->size();
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
void Set<KEY, VALUE>::getRandomSubset(unsigned size, Set<KEY, VALUE>* set) {
    
    // if size is larger than the number elements in the set, all keys are
    // returned
    if (size >= setSize) {
        
        // copy this set
        set = new Set(this);
        
    } else { // size smaller that number of correspondences
        
        unordered_map<unsigned, bool> indices = unordered_map<unsigned, bool>(size);
        
        indices = getRandomIndices(size, &indices);
        
        // iterates over all elements and puts the chosen ones in the result
        // TODO there are probably better ways to do that
        auto it = begin();
        for (int i = 0; i < setSize; i++) {
            if (indices.count(i) != 0) {
                set->add(it->first, it->second);
            }
            it++;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void Set<KEY, VALUE>::getRandomKeySubset(unsigned size, vector<KEY>* result) {
    
    // if size is larger than the number elements in the set, all keys are
    // returned
    if (size >= setSize) {
        
        // copy this set
        for (auto it = begin(); it != end(); it++) {
            result->push_back(it->first);
        }
        
    } else { // size smaller that number of correspondences
        
        unordered_map<unsigned, bool> indices = unordered_map<unsigned, bool>(size);
        
        indices = getRandomIndices(size, &indices);
        
        // iterates over all elements and puts the chosen ones in the result
        // TODO there are probably better ways to do that
        auto it = begin();
        for (int i = 0; i < setSize; i++) {
            if (indices.count(i) != 0) {
                result->push_back(it->first);
            }
            it++;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void Set<KEY, VALUE>::getRandomValueSubset(unsigned size, vector<VALUE>* result) {
    
    // if size is larger than the number elements in the set, all keys are
    // returned
    if (size >= setSize) {
        
        // copy this set
        for (auto it = begin(); it != end(); it++) {
            result->push_back(it->second);
        }
        
    } else { // size smaller that number of correspondences
        
        unordered_map<unsigned, bool> indices = unordered_map<unsigned, bool>(size);
        
        indices = getRandomIndices(size, &indices);
        
        // iterates over all elements and puts the chosen ones in the result
        // TODO there are probably better ways to do that
        auto it = begin();
        for (int i = 0; i < setSize; i++) {
            if (indices.count(i) != 0) {
                result->push_back(it->second);
            }
            it++;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
unsigned Set<KEY, VALUE>::size() {
    return setSize;
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
typename Set<KEY, VALUE>::iterator Set<KEY, VALUE>::begin() {
    return elements_.begin();
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
typename Set<KEY, VALUE>::iterator Set<KEY, VALUE>::end() {
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

///////////////////////////////////////////////////////////////////////////////
// returns the value to the key, returns null pointer if the key does not exist
template<class KEY, class VALUE>
VALUE Set<KEY, VALUE>::operator[](KEY correspondence) {
    auto find = elements_.find(correspondence);
    
    if (find != elements_.end()) {
        return find->second;
    }
    
    return nullptr;
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
unordered_map<KEY, VALUE>* Set<KEY, VALUE>::getElements() {
    return &elements_;
}


///////////////////////////////////////////////////////////////////////////////
// Protected Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// will return an empty set if the size is larger than the available indices
template<class KEY, class VALUE>
void Set<KEY, VALUE>::getRandomIndices(unsigned size, unordered_map<unsigned, bool>* indices) {
    
    // no indices will be produced, if the requested size is larger than the
    // available indices
    if (size <= setSize) {
    
        int counter = 0;
        
        // generating random unique index sequence
        // the assumption is that the number of elements is way
        // larger than the size of the subset, thats why i did not use
        // shuffle
        
        // generate random numbers until size many unique values have been created
        while (counter < size) {
            int v = rand() % setSize;
            
            if(indices->count(v) == 0) {
                indices->insert(make_pair<unsigned, bool>(v, true));
                counter++;
            }
        }
        
    }
}

#endif
