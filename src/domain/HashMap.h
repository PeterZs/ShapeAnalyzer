//
//  HashMap.h
//  ShapeAnalyzer
//
//  HashMap is a wrapper class for unordered_map.
//  It manages an unordered_map with some extra features:
//  E.g. a contains function and a random subset of the map can be generated.
//
//  Created by Zorah on 28.07.14.
//
//


#ifndef ShapeAnalyzer_HashMap_h
#define ShapeAnalyzer_HashMap_h

#include <unordered_map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>


using namespace std;

template <class KEY, class VALUE>

class HashMap {
public:
    typedef typename unordered_map<KEY, VALUE>::iterator iterator;
    
    // constructors and destructor
    HashMap();
    HashMap(int n);
    HashMap(HashMap<KEY, VALUE>&);
    HashMap(vector<KEY>&, VALUE);
    ~HashMap() {}
    
    // adding and deleting elements
    void add(KEY, VALUE);
    void add(vector<KEY>&, VALUE);
    void add(vector<pair<KEY, VALUE> >&);
    
    bool remove(KEY);
    bool remove(vector<KEY>&);
    void clear();
    
    // iterators
    iterator begin();
    iterator end();
    
    // attributes
    bool        containsKey(KEY);
    VALUE&      operator[](KEY); // this expression is assignable since it is a reference to the value
    
    unsigned    size();
    
    void getRandomSample(unsigned size, HashMap<KEY, VALUE>&);
    void getRandomSampleKeys(unsigned size, vector<KEY>&);
    void getRandomSampleValues(unsigned size, vector<VALUE>&);
    
    unordered_map<KEY, VALUE>& getEntries();
    
    void getValues(vector<VALUE>&);
    void getKeys(vector<KEY>&);
    
private:
    unordered_map<KEY, VALUE> entries_;
    
};

///////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
HashMap<KEY, VALUE>::HashMap() {
    entries_ = unordered_map<KEY, VALUE>(100);
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
HashMap<KEY, VALUE>::HashMap(int n) {
    entries_ = unordered_map<KEY, VALUE>(n);
}

///////////////////////////////////////////////////////////////////////////////
// copy contructor
template<class KEY, class VALUE>
HashMap<KEY, VALUE>::HashMap(HashMap<KEY, VALUE>& map) {
    entries_ = unordered_map<KEY, VALUE>(map.getEntries());
}


///////////////////////////////////////////////////////////////////////////////
// all keys are added with the same value
template<class KEY, class VALUE>
HashMap<KEY, VALUE>::HashMap(vector<KEY>& keys, VALUE value) {
    entries_ = unordered_map<KEY, VALUE>(keys.size());
    for(int i = 0; i < keys.size(); i++) {
        entries_.insert(make_pair(keys[i], value));
    }
}


///////////////////////////////////////////////////////////////////////////////
// Adding and Deleting Correspondences
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::add(KEY key, VALUE value) {
    entries_.insert(make_pair(key, value));
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::add(vector<KEY>& keys, VALUE value) {
    // reserve enough memory for new keys
    entries_.rehash(entries_.size() + keys.size());
    
    for (auto it = keys.begin(); it != keys.end(); it++) {
        entries_.insert(make_pair<KEY, VALUE>(*it, value));
    }
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::add(vector<pair<KEY, VALUE> >& entries) {
    // reserve enough memory for new keys
    entries_.rehash(entries_.size() + entries.size());
    
    for (auto it = entries.begin(); it != entries.end(); it++) {
        entries_.insert(*it);
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
bool HashMap<KEY, VALUE>::containsKey(KEY key) {
    return entries_.find(key) != entries_.end();
}



///////////////////////////////////////////////////////////////////////////////
// returns the value to the key. Circumvent the condition that key is not contained in map by a check via the function contains
template<class KEY, class VALUE>
VALUE& HashMap<KEY, VALUE>::operator[](KEY key) {
    auto value = entries_.find(key);
    
    return value->second;
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
unordered_map<KEY, VALUE>& HashMap<KEY, VALUE>::getEntries() {
    return entries_;
}


template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::getValues(vector<VALUE>& values) {
    for(HashMap<KEY, VALUE>::iterator it = entries_.begin(); it != entries_.end(); it++) {
        values.push_back(it->second);
    }
}

template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::getKeys(vector<KEY>& keys) {
    for(HashMap<KEY, VALUE>::iterator it = entries_.begin(); it != entries_.end(); it++) {
        keys.push_back(it->first);
    }
}

///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::getRandomSample(unsigned size, HashMap<KEY, VALUE>& sampleMap) {
    
    // if size is larger than the number elements in the map, all keys are
    // returned
    if (size >= this->size()) {
        
        // copy this map
        for (auto it = begin(); it != end(); it++) {
            sampleMap.add(it->first, it->second);
        }
        
    } else { // size smaller that number of entries
        vector<unsigned int> permutation(entries_.size());
        for(int i = 0; i < entries_.size(); i++) {
            permutation[i] = i;
        }
        random_shuffle(permutation.begin(), permutation.end());
        
        // obtain an ordered sequence of all keys
        vector<KEY> keys;
        getKeys(keys);
        
        // write size keys of random order into subsetKeys
        for (int i = 0; i < size; i++) {
            sampleMap.add(keys[permutation[i]], entries_[keys[permutation[i]]]);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::getRandomSampleKeys(unsigned size, vector<KEY>& sampleKeys) {
    
    // if size is larger than the number elements in the map, all keys are
    // returned
    if (size >= this->size()) {
        
        // copy this map
        for (auto it = begin(); it != end(); it++) {
            sampleKeys.push_back(it->first);
        }
        
    } else { // size smaller that number of entries
        
        vector<unsigned int> permutation(entries_.size());
        for(int i = 0; i < entries_.size(); i++) {
            permutation[i] = i;
        }
        random_shuffle(permutation.begin(), permutation.end());
        
        // obtain an ordered sequence of all keys
        vector<KEY> keys;
        getKeys(keys);
        
        // write size keys of random order into subsetKeys
        for (int i = 0; i < size; i++) {
            sampleKeys.push_back(keys[permutation[i]]);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
void HashMap<KEY, VALUE>::getRandomSampleValues(unsigned size, vector<VALUE>& sampleValues) {
    
    // if size is larger than the number elements in the map, all keys are
    // returned
    if (size >= this->size()) {
        
        // copy this map
        for (auto it = begin(); it != end(); it++) {
            sampleValues.push_back(it->second);
        }
        
    } else { // size smaller that number of entries
        vector<unsigned int> permutation(entries_.size());
        for(int i = 0; i < entries_.size(); i++) {
            permutation[i] = i;
        }
        random_shuffle(permutation.begin(), permutation.end());
        
        // obtain an ordered sequence of all keys
        vector<KEY> keys;
        getKeys(keys);
        
        // write size keys of random order into subsetKeys
        for (int i = 0; i < size; i++) {
            sampleValues.push_back(entries_[keys[permutation[i]]]);
        }
    }
}


#endif
