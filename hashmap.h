#ifndef HASHMAP_H
#define HASHMAP_H
#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <cstdlib>

const unsigned int table_size = 111;

 //Dispersion function
struct KeyHashFunc {
    unsigned long operator()(const int& k) const
    {
        return k % table_size;
    }
};


// Class for every item for the HashMap bucket
template <typename K, typename V>
class HashMapItem {
private:
    K key;
    V value;
    HashMapItem *next;
public:
    HashMapItem(const K &k, const V &v) : key(k), value(v), next(NULL) { }
    K getKey() const {
        return key;
    }
    V getValue() const {
        return value;
    }
    void setValue(V val) {
        value = val;
    }
    HashMapItem *getNext() const {
        return next;
    }
    void setNext(HashMapItem *n) {
        next = n;
    }
};

template <typename K, typename V, typename F = KeyHashFunc>
class HashMap {
private:
    HashMapItem <K, V> **map;
    F hash_function;
public:
    // Constructor which initializes the map
    HashMap() {
        map = new HashMapItem<K, V> *[table_size]();
    }
    // Copy constructor
    HashMap(const HashMap &m) {
    delete[] this->map;
    this->map = new HashMapItem<K, V> *[table_size]();
        for (int i = 0; i < table_size; i++) {
            this->map[i] = m.map[i];
            try {
                if (this->map[i] == NULL) {
                    throw 0;
                }
            }
            catch (...) {
            std::cout << "The map didn't copy correctly!\n\n" << std::endl;
            }
        }
    }
    //Destructor
    ~HashMap() {
        for (unsigned int i = 0; i < table_size; ++i) {
            HashMapItem<K, V> *current = map[i];
            while (current != NULL) {
                HashMapItem<K, V> *previous = current;
                current = current->getNext();
                delete previous;
            }
            map[i] = NULL;
        }
        delete [] map;
    }
    //Overloading [], a method similar to getElem()
    V operator[](const K &key) {
        unsigned long hash_value = hash_function(key);
        HashMapItem <K, V> *item = map[hash_value];
        while (item != NULL) {

            if (item->getKey() == key) {
                return item->getValue();
            }
            item = item->getNext();
        }
    }
    //It returns the same value as the overloaded [] operation,
    //but it also checks whether the element exists by returning a boolean
    bool getElem (const K &key, V &value) {
        unsigned long hash_value = hash_function(key);
        HashMapItem <K, V> *item = map[hash_value];
        while (item != NULL) {
            if (item->getKey() == key) {
                value = item->getValue();
                return true;
            }
            item = item->getNext();
        }
        return false;
    }
    //A method to return the list created from
    //the hash collision on a certain key
    std::vector <V> getList (const K &key) {
        unsigned long hash_value = hash_function(key);
        HashMapItem <K, V> *item = map[hash_value];
        std::vector <V> array;
        while (item != NULL) {
            if (item->getKey() == key) {
                array.push_back(item->getValue());
            }
            item = item->getNext();
        }
        return array;
    }
    //Returns the number of distinct keys in the HashMap
    int getNumberOfBuckets () {
        unsigned int counter = 0;
        for (unsigned int i = 0; i < table_size; i++) {
            HashMapItem <K, V> *item = map[i];
            if (item != NULL) {
                counter++;
            }
        }
        return counter;
    }
    //Inserts a new item in the HashMap, and if it collides,
    //it inserts said item as a part of the list
    //of the respective item in the HashMap
    void put (const K &key, const V &value) {
        unsigned long hash_value = hash_function(key);
        HashMapItem <K, V> *item = map[hash_value];

        if (item == NULL) {
            item = new HashMapItem <K, V> (key, value);
            map[hash_value] = item;
            return;
        }
        else {
            HashMapItem <K, V> *aux = item;
            HashMapItem <K, V> *p = new HashMapItem <K, V> (key, value);
            p->setNext(aux);
            map[hash_value] = p;
        }
    }
    //Removes the first item from the key given as parameter
    void remove (const K &key) {
        unsigned long hash_value = hash_function(key);
        HashMapItem<K, V> *previous = NULL;
        HashMapItem<K, V> *item = map[hash_value];

        while (item != NULL && item->getKey() != key) {
            previous = item;
            item = item->getNext();
        }
        if (item == NULL) {
            return;
        }
        else {
            if (previous == NULL) {
                map[hash_value] = item->getNext();
            }
            else {
                previous->setNext(item->getNext());
            }
            delete item;
        }
    }
    //Overloaded = operation
    HashMap& operator= (const HashMap& m) {
        delete[] this->map;
        this->map = new HashMapItem<K, V> *[table_size]();
        for (int i = 0; i < table_size; i++) {
            this->map[i] = m.map[i];
        }
        return *this;
    }
    //Overloaded print function
    friend std::ostream &operator<< (std::ostream &out, HashMap& m) {
        for (unsigned int i = 0; i < table_size; i++) {
            bool ok = 0;
            HashMapItem <K, V> *aux = m.map[i];
            if (aux != NULL) {
                out << "The key and values for position " << i << " are:\n" << aux->getKey();
                while (aux != NULL) {
                    out << " " << aux->getValue();
                    aux = aux->getNext();
                    ok = 1;
                }
            }
            if (ok == 1) {
                 out <<std::endl;
            }
        }
        return out;
    }
};

#endif // HASHMAP_H

