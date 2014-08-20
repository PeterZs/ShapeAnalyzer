//
//  HashMap.h
//  ShapeAnalyzer
//
//  HashMap is a wrapper class for unordered_map.
//  It manages an unordered_map with some extra features,
//  a random subset of the map can be chosen and the exact number
//  of items in the map is maintained.
//
//  Created by Zorah on 28.07.14.
//
//


#ifndef ShapeAnalyzer_HashMap_h
#define ShapeAnalyzer_HashMap_h

#include <unordered_map>

using namespace std;

template <class KEY, class VALUE>

class HashMap {
public:
    typedef typename unordered_map<KEY, VALUE>::iterator iterator;
    
    // constructors and destructor
    HashMap();
    HashMap(HashMap<KEY, VALUE>&);
    HashMap(vector<KEY>&, VALUE);
    ~HashMap() {}
    
    // adding and deleting elements
    void add(KEY, VALUE);
    void add(vector<KEY>&, VALUE);
    
    bool remove(KEY);
    bool remove(vector<KEY>&);
    void clear();
    
    // iterators
    iterator begin();
    iterator end();
    
    // attributes
    bool        contains(KEY);
    VALUE       operator[](KEY);
    
    unsigned    size();
    
    void getRandomSubset(unsigned size, HashMap<KEY, VALUE>&);
    void getRandomKeySubset(unsigned size, vector<KEY>&);
    void getRandomValueSubset(unsigned size, vector<VALUE>&);
    
    unordered_map<KEY, VALUE>& getEntries();
    
protected:
    unordered_map<KEY, VALUE> entries_;
    
    void getRandomIndices(unsigned size, unordered_map<unsigned, bool>*);
    
};

///////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
HashMap<KEY, VALUE>::HashMap() {
    entries_ = unordered_map<KEY, VALUE>(0);
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
HashMap<KEY, VALUE>::HashMap(HashMap<KEY, VALUE>& map) {
    entries_ = unordered_map<KEY, VALUE>(map->getEntries());
}


///////////////////////////////////////////////////////////////////////////////
// all keys are added with the same value
template<class KEY, class VALUE>
HashMap<KEY, VALUE>::HashMap(vector<KEY>& keys, VALUE value) {
    entries_ = unordered_map<KEY, VALUE>(keys.size());
}


///////////////////////////////////////////////////////////////////////////////
// Adding and Deleting Correspondences
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::add(KEY key, VALUE value) {
    entries_.insert(make_pair<KEY, VALUE>(key, value));
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::add(vector<KEY>& keys, VALUE value) {
    // reserve enough memory for new keys
    entries_.rehash(entries_.size() + keys.size());
    
    for (auto it = keys.begin(); it != keys.end(); it++) {
        entries_.insert(make_pair<KEY, VALUE>(&it, value));
    }
}

///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
bool HashMap<KEY, VALUE>::remove(KEY key) {
    return entries_.erase(key) != 0;
}

///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
bool HashMap<KEY, VALUE>::remove(vector<KEY>& keys) {
    bool success = true;
    for (auto it = keys.begin(); it != keys.end(); it++) {
        if (!remove(*it)) {
            success = false;
        }
    }
    
    return success;
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::clear() {
    entries_.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Getters
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::getRandomSubset(unsigned size, HashMap<KEY, VALUE>& map) {
    
    // if size is larger than the number elements in the map, all keys are
    // returned
    if (size >= this->size()) {
        
        // copy this map
        map = *this;
        
    } else { // size smaller that number of entries
        
        unordered_map<unsigned, bool> indices = unordered_map<unsigned, bool>(size);
        
        indices = getRandomIndices(size, &indices);
        
        // iterates over all elements and puts the chosen ones in the result
        // TODO there are probably better ways to do that
        auto it = begin();
        for (int i = 0; i < entries_.size(); i++) {
            if (indices.count(i) != 0) {
                map->add(it->first, it->second);
            }
            it++;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::getRandomKeySubset(unsigned size, vector<KEY>& result) {
    
    // if size is larger than the number elements in the map, all keys are
    // returned
    if (size >= this->size()) {
        
        // copy this map
        for (auto it = begin(); it != end(); it++) {
            result.push_back(it->first);
        }
        
    } else { // size smaller that number of entries
        
        unordered_map<unsigned, bool> indices = unordered_map<unsigned, bool>(size);
        
        indices = getRandomIndices(size, &indices);
        
        // iterates over all elements and puts the chosen ones in the result
        // TODO there are probably better ways to do that
        auto it = begin();
        for (int i = 0; i < this->size; i++) {
            if (indices.count(i) != 0) {
                result->push_back(it->first);
            }
            it++;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::getRandomValueSubset(unsigned size, vector<VALUE>& result) {
    
    // if size is larger than the number elements in the map, all keys are
    // returned
    if (size >= this->size()) {
        
        // copy this map
        for (auto it = begin(); it != end(); it++) {
            result.push_back(it->second);
        }
        
    } else { // size smaller that number of entries
        
        unordered_map<unsigned, bool> indices = unordered_map<unsigned, bool>(size);
        
        indices = getRandomIndices(size, &indices);
        
        // iterates over all elements and puts the chosen ones in the result
        // TODO there are probably better ways to do that
        auto it = begin();
        for (int i = 0; i < this->size(); i++) {
            if (indices.count(i) != 0) {
                result->push_back(it->second);
            }
            it++;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
unsigned HashMap<KEY, VALUE>::size() {
    return entries_.size();
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
typename HashMap<KEY, VALUE>::iterator HashMap<KEY, VALUE>::begin() {
    return entries_.begin();
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
typename HashMap<KEY, VALUE>::iterator HashMap<KEY, VALUE>::end() {
    return entries_.end();
}


///////////////////////////////////////////////////////////////////////////////
// returns true if the input was found in the map, false otherwise
template<class KEY, class VALUE>
bool HashMap<KEY, VALUE>::contains(KEY key) {
    return entries_.find(key) != entries_.end();
}



///////////////////////////////////////////////////////////////////////////////
// returns the value to the key, returns null pointer if the key does not exist
template<class KEY, class VALUE>
VALUE HashMap<KEY, VALUE>::operator[](KEY key) {
    auto find = entries_.find(key);
    
    return find->second;
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
unordered_map<KEY, VALUE>& HashMap<KEY, VALUE>::getEntries() {
    return entries_;
}


///////////////////////////////////////////////////////////////////////////////
// Protected Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// will return an empty map if the size is larger than the available indices
template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::getRandomIndices(unsigned size, unordered_map<unsigned, bool>* indices) {
    
    // no indices will be produced, if the requested size is larger than the
    // available indices
    if (size <= this->size()) {
    
        int counter = 0;
        
        // generating random unique index sequence
        // the assumption is that the number of elements is way
        // larger than the size of the subset, thats why i did not use
        // shuffle
        
        // generate random numbers until size many unique values have been created
        while (counter < size) {
            int v = rand() % this->size();
            
            if(indices->count(v) == 0) {
                indices->insert(make_pair<unsigned, bool>(v, true));
                counter++;
            }
        }
        
    }
}

#endif
