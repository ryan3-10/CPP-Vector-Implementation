#ifndef VECTOR_H
#define VECTOR_H
#include <cstddef>
#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
class Vector {
enum {CHUNK = 10};
size_t capacity; // Size of the current array allocation (total number of
//ints, in use or not)
size_t n_elems; // Number of int spaces currently in use, starting from
//position 0
T* data_ptr; // Pointer to the heap array
void grow();
public:
// Object Mgt.
Vector();
Vector(const Vector& v); // Copy constructor
Vector<T>& operator=(const Vector<T>& v); // Copy assignment operator
~Vector();

// Accessors
T front() const; // Return the int in position 0, if any
T back() const; // Return last element (position n_elems-1)
T at(size_t pos) const; // Return element in position "pos" (0-
//based)
size_t size() const; // Return n_elems
bool empty() const; // Return n_elems == 0
// Mutators
T& operator[](size_t pos); // Same as at but no bounds checking
void push_back(T item); // Append a new element at the end of the
//array

void pop_back(); // --n_elems (nothing else to do; returns
//nothing)
void erase(size_t pos); // Remove item in position pos and shuffles
//following items left
void insert(size_t pos, T item); // Shuffle items right to make room for a
//new element
void clear(); // n_elems = 0 (nothing else to do; keep
//the current capacity)
// Iterators
T* begin(); // Return a pointer to 1st element, or
//nullptr if n_elems == 0
T* end(); // Return a pointer to 1 past last element,
//or nullptr if n_elems == 0
// Comparators
bool operator==(const Vector<T>& v) const;
bool operator!=(const Vector<T>& v) const;
};

template <typename T>
Vector<T>::Vector() {
    capacity = CHUNK;
    data_ptr = new T[capacity];
    n_elems = 0;
}

template <typename T>
Vector<T>::Vector(const Vector& v) {
    //Copy member data
    capacity = v.capacity;
    n_elems = v.n_elems;
    data_ptr = new T[capacity];

    //Copy elements 
    for (int i = 0; i < n_elems; ++i) {
        data_ptr[i] = v.data_ptr[i];
    }
}

template <typename T>
Vector<T>::~Vector() {
    delete[] data_ptr;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& v) {
    if (this != &v) { 
        delete[] data_ptr; 

        //Copy member data
        capacity = v.capacity;
        n_elems = v.n_elems;
        data_ptr = new T[capacity];

        //Copy elements 
        for (int i = 0; i < n_elems; ++i) {
            data_ptr[i] = v.data_ptr[i];
        }
    }

    return *this;
}

template <typename T>
void Vector<T>::grow() {
    //Save old data
    T* tempDataHolder = new T[capacity];
    
    for (int i = 0; i < n_elems; ++i) {
        tempDataHolder[i] = data_ptr[i];
    }

    //Delete old data and allocate new memory
    delete[] data_ptr;
    capacity *= 1.6;
    data_ptr = new T[capacity];

    //Copy old data back into data_ptr
    for (int i = 0; i < n_elems; ++i) {
        data_ptr[i] = tempDataHolder[i];
    }

    delete[] tempDataHolder;
}

template <typename T>
T Vector<T>::front() const {
    if (n_elems == 0) {
        throw range_error("Error: This array is empty");
    }

    return data_ptr[0];
}

template <typename T>
T Vector<T>::back() const {
    if (n_elems == 0) {
        throw range_error("Error: This array is empty");
    }

    return data_ptr[n_elems - 1];
}

template <typename T>
void Vector<T>::push_back(T item) {
    //Check if capacity needs increase
    if (n_elems == capacity) {
        grow();
    }
    //Append new item
    ++n_elems;
    data_ptr[n_elems-1] = item;
}

template <typename T>
T Vector<T>::at(size_t pos) const {
    if (pos >= n_elems) {
        throw range_error("Error: Position out of range");
    }
    
    return data_ptr[pos];
}

template <typename T>
size_t Vector<T>::size() const {
    return n_elems;
}

template <typename T>
T& Vector<T>::operator[](size_t pos) {
    if (pos >= n_elems) {
        throw range_error("Error: Position out of range");
    }

    return data_ptr[pos];
}

template <typename T>
bool Vector<T>::empty() const {
    return n_elems == 0;
}

template <typename T>
void Vector<T>::pop_back() {
    if (n_elems == 0) {
        throw range_error("Error: This array is empty");
    }
    --n_elems;
}

template <typename T>
void Vector<T>::erase(size_t pos) {
    T* tempDataHolder = new T[capacity];
    size_t i;

    if (n_elems == 0) {
        throw range_error("Error: This array is empty");
    }

    //Load tempDataHolder with old array
    for (i = 0; i < pos; ++i) {
        tempDataHolder[i] = data_ptr[i];
    }

    for (i = pos + 1; i < n_elems; ++i) {
        tempDataHolder[i - 1] = data_ptr[i];
    }

    delete[] data_ptr;

    //Load new array with tempDataHolder
    data_ptr = new T[capacity];
    --n_elems;

    for (i = 0; i < n_elems; ++i) {
        data_ptr[i] = tempDataHolder[i];
    }

   delete[] tempDataHolder;
}

template <typename T>
void Vector<T>::insert(size_t pos, T item) {
    T* tempDataHolder = new T[capacity];
    size_t i;

    //Check Capacity
    if (n_elems == capacity) {
        grow();
    }

    //Add elements before the new insert
    for (i = 0; i < pos; ++i) {
        tempDataHolder[i] = data_ptr[i];
    }

    //Add new insert
    tempDataHolder[pos] = item;

    //Add elements after the new insert
    for (i = pos; i < n_elems; ++i) {
        tempDataHolder[i + 1] = data_ptr[i];
    }

    delete[] data_ptr;

    //Load new array with tempDataHolder
    data_ptr = new T[capacity];
    ++n_elems;

    for (i = 0; i < n_elems; ++i) {
        data_ptr[i] = tempDataHolder[i];
    }

    delete[] tempDataHolder;
}

template <typename T>
void Vector<T>::clear() {
    n_elems = 0;
}

template <typename T>
T* Vector<T>::begin() {
    if (n_elems == 0) {
        return nullptr;
    }

    return data_ptr;
}

template <typename T>
T* Vector<T>::end() {
    if (n_elems == 0) {
        return nullptr;
    }

    return &data_ptr[n_elems];
}

template <typename T>
bool Vector<T>::operator==(const Vector<T>& v) const {
    int i;

    for (i = 0; i < n_elems; ++i) {
        if (this->data_ptr[i] != v.data_ptr[i]) {
            return false;
        }
    }

    return this->n_elems == v.n_elems;
}

template <typename T>
bool Vector<T>::operator!=(const Vector<T>& v) const {
    return !(*this == v);
}

#endif
