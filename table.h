/*************************************************************/
/*                                                           */
/*  Starter file for a2                                      */
/*                                                           */
/*  Author1 Name: <name here>                                */
/*      - class/function list/main author or main checker    */
/*  Author2 Name: <name here>                                */
/*      - class/function list/main author or main checker    */
/*                                                           */
/*************************************************************/

#include <string>
#include <utility>
#include <functional>
#include <iostream>
#include "mylist.h"



template <class TYPE>
class Table {
public:
    Table() {}
    virtual bool update(const std::string& key, const TYPE& value) = 0;
    virtual bool remove(const std::string& key) = 0;
    virtual bool find(const std::string& key, TYPE& value) = 0;
    virtual int numRecords() const = 0;
    virtual bool isEmpty() const = 0;
    virtual int capacity() const = 0;
    virtual ~Table() {}
};

template <class TYPE>
class SimpleTable :public Table<TYPE> {

    struct Record {
        TYPE data_;
        std::string key_;
        Record(const std::string& key, const TYPE& data) {
            key_ = key;
            data_ = data;
        }

    };

    Record** records_;   //the table
    int capacity_;       //capacity of the array



public:
    SimpleTable(int capacity);
    SimpleTable(const SimpleTable& rhs);
    SimpleTable(SimpleTable&& rhs);
    virtual bool update(const std::string& key, const TYPE& value);
    virtual bool remove(const std::string& key);
    virtual bool find(const std::string& key, TYPE& value);
    virtual const SimpleTable& operator=(const SimpleTable& rhs);
    virtual const SimpleTable& operator=(SimpleTable&& rhs);
    virtual ~SimpleTable();
    virtual bool isEmpty() const { return numRecords() == 0; }
    virtual int numRecords() const;
    virtual int capacity() const { return capacity_; }

};

template <class TYPE>
int SimpleTable<TYPE>::numRecords() const {
    int rc = 0;  // 1 
    for (int i = 0; records_[i] != nullptr; i++) {   //1 + (n+1) + n 
        rc++;   // n
    }
    return rc; // 1

}
// = 1 + 1 (n+1) + n + n
// = 3n + 3
//  O(n) = 3n + 3 



template <class TYPE>
SimpleTable<TYPE>::SimpleTable(int capacity) : Table<TYPE>() {
    records_ = new Record * [capacity + 1];
    capacity_ = capacity;
    for (int i = 0; i < capacity_ + 1; i++) {
        records_[i] = nullptr;
    }
}

template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& rhs) {
    records_ = new Record * [rhs.capacity_ + 1]; // 2 
    capacity_ = rhs.capacity_; // 1
    for (int i = 0; i < capacity_ + 1; i++) {  // 1 + 2(n+ 1) + n 
        records_[i] = nullptr; // n
    }
    for (int i = 0; i < rhs.numRecords(); i++) {  // 1 + 3(n + 1 ) + n
        update(rhs.records_[i]->key_, rhs.records_[i]->data_); // n(12n + 14)
    }
}
// = 2 + 1 + 1 + 2(n + 1) + n + n + 1 +3(n +1) + n + n(12n + 14)
// = 2 + 1 + 1 + 2n + 2 + n + n + 1 + 3n + 3 + n + 12n^2 + 14n
// = 10 + 22n + 12n^2
//  O(n^2)
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& rhs) {
    capacity_ = rhs.capacity_; // 1 
    records_ = rhs.records_; //1 
    rhs.records_ = nullptr; //1 
    rhs.capacity_ = 0; //1 
}
//= 1 + 1 + 1 +1 
//=4
//O(0)

template <class TYPE>
bool SimpleTable<TYPE>::update(const std::string& key, const TYPE& value) {
    int idx = -1;  // 1 
    int sz = numRecords();  // 2 
    bool rc = true;  // 1 
    for (int i = 0; i < sz; i++) {     // 1 + (n+1) + n
        if (records_[i]->key_ == key) {  // n 
            idx = i;
        }
    }
    if (idx == -1) { // 1
        if (sz < capacity_) {  // 1 
            records_[numRecords()] = new Record(key, value);   // 1 
            for (int i = numRecords() - 1; i > 0 && records_[i]->key_ < records_[i - 1]->key_; i--) {  // 2  + 4(n + 1) + n 
                Record* tmp = records_[i];     // n
                records_[i] = records_[i - 1];  // n
                records_[i - 1] = tmp;          //2(n) 
            }
        }
        else {
            rc = false;
        }
    }
    else {
        records_[idx]->data_ = value;
    }
    return rc;   //1 
// = 1 + 2 + 1 + 1 + (n+1) + n + n + 1 + 1 + 1 + 2 + 4(n+1) + n + n + 2n
// = 12n + 14;
}



template <class TYPE>
bool SimpleTable<TYPE>::remove(const std::string& key) {
    int idx = -1;  // 1 
    for (int i = 0; records_[i] != nullptr; i++) {  // 1 + (n+1) + n
        if (records_[i]->key_ == key) {    // n 
            idx = i;
        }
    }
    if (idx != -1) {  // 1 
        delete records_[idx];   // 1 

        for (int i = idx; i < numRecords() - 1; i++) {   // 1 + 2(n + 1) + n 
            records_[i] = records_[i + 1];    // 2n 
        }
        records_[capacity_ - 1] = nullptr; // 1 
        return true; //1 
    }
    else {
        return false; // 1 
    }
}
// If not found
// = 1 + 1 + (n+1) + n + n + 1 + 1
// = 3n + 4
// = O(n) = 3n + 5
// If found 
// = 1 + 1 + (n + 1) + n + n + 1 + 1 + 1 + 2(n+1) + n + 2n + 1 + 1
// = 8n + 10 
// = O(n) = 8n + 10
template <class TYPE>
bool SimpleTable<TYPE>::find(const std::string& key, TYPE& value) {
    int idx = -1; // 1 
    for (int i = 0; i < numRecords(); i++) {  // 1 + 2(n + 1 ) + n
        if (records_[i]->key_ == key) {  // n
            idx = i;
        }
    }
    if (idx == -1)
        return false;
    else {
        value = records_[idx]->data_;  // 1 
        return true; // 1 
    }
}

// = 1 + 1 + 2(n+1) + n + n + 1 + 1 
// = 4n + 6
// O(n) = 4n + 6

template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& rhs) {
    if (this != &rhs) { //1
        if (records_) { // 1
            while (numRecords() != 0) { // 2n + 1
                remove(records_[0]->key_); // n
            }
            delete[] records_; // 1 
        }
        records_ = new Record * [rhs.capacity_ + 1]; // 2 
        capacity_ = rhs.capacity_; // 1
        for (int i = 0; i < capacity_; i++) {  // 1 + (n+1) + n 
            records_[i] = nullptr; // n 
        }
        for (int i = 0; i < rhs.numRecords(); i++) { // 1 + 2(n+1) + 1 
            update(rhs.records_[i]->key_, rhs.records_[i]->data_);  //n(12n + 14)
        }

    }
    return *this; // 1 
}
// = 1 + 1 + 2n + 1 + n + 1 + 2 + 1+ 1 + (n+1) + n + n + 1 + 2(n +1 ) +1 + n(12n + 14)
// = 11 + 22n + 12n^2
// O(n^2)
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& rhs) {
    if (records_) { // 1 
        while (numRecords() != 0) { // 2(n + 1)
            remove(records_[0]->key_); // n(8n + 10)
        }
        delete[] records_; // 1
    }
    records_ = rhs.records_; //1 
    capacity_ = rhs.capacity_; //1 
    rhs.records_ = nullptr;// 1
    rhs.capacity_ = 0; // 1

    return *this; // 1
}
// = 1 + 2n + 2 + 8n^2 + 10n + 1 + 1 + 1 + 1 +1 +1
// = 9 + 12n + 8n^2
//  O(n^2)
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable() {
    if (records_) { // 1 
        int sz = numRecords(); // 2 
        for (int i = 0; i < sz; i++) {  // 1 + (n+1)  + n 
            remove(records_[0]->key_); //  n(8n + 10) 
        }
        delete[] records_; // 1 
    }
}
// = 1 + 2 + 1 + (n + 1) + n + 8n^2 + 10n  +1 
// = 6 + 10n + 8n^2
// = O(n^2)
template <class TYPE>
class ChainingTable :public Table<TYPE> {
    struct Record {
        DList<TYPE> data_;
        Record() {


        }
        Record(const std::string& key, const TYPE& data) {

            data_.push_back(data, key);
        }

    };

    Record** records_;   //the table
    size_t capacity_;       //capacity of the array

public:
    ChainingTable(size_t maxExpected);
    ChainingTable(const ChainingTable& other);
    ChainingTable(ChainingTable&& other);
    double loadFactor() const;
    virtual bool update(const std::string& key, const TYPE& value);
    virtual bool remove(const std::string& key);
    virtual bool find(const std::string& key, TYPE& value);
    virtual const ChainingTable& operator=(const ChainingTable& other);
    virtual const ChainingTable& operator=(ChainingTable&& other);
    virtual ~ChainingTable();
    virtual bool isEmpty() const;
    virtual int numRecords() const;
    virtual int capacity() const;

};
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(size_t capacity) : Table<TYPE>() {
    records_ = new Record * [capacity + 1];

    capacity_ = capacity;
    for (int i = 0; i < capacity + 1; i++) {
        records_[i] = nullptr;
    }


}

template <class TYPE>
ChainingTable<TYPE>::ChainingTable(const ChainingTable<TYPE>& other) {
    records_ = new Record * [other.capacity_ + 1];
    capacity_ = other.capacity_;

    for (int i = 0; i < capacity_ + 1; i++) {

        if (other.records_[i]) {
            records_[i] = new Record();
            records_[i]->data_ = other.records_[i]->data_;

        }
        else {
            records_[i] = nullptr;

        }

    }

}
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(ChainingTable<TYPE>&& other) {
    records_ = new Record * [other.capacity_ + 1];
    capacity_ = other.capacity_;

    for (int i = 0; i < capacity_ + 1; i++) {

        if (other.records_[i]) {
            records_[i] = new Record();
            records_[i]->data_ = other.records_[i]->data_;

        }
        else {
            records_[i] = nullptr;

        }

    }
    delete other.records_;


}
template <class TYPE>
bool ChainingTable<TYPE>::update(const std::string& key, const TYPE& value) {
    std::hash<std::string> hashFunction;
    size_t hash = hashFunction(key);
    size_t index = hash % capacity_;
    if (records_[index]) {

        auto KV = records_[index]->data_.searchKey(key);
        if (KV != records_[index]->data_.end()) {

            *KV = value;
        }
        else {
            records_[index]->data_.push_back(value, key);
        }
    }
    else {

        records_[index] = new Record(key, value);
    }

    return true;
}

template <class TYPE>
bool ChainingTable<TYPE>::remove(const std::string& key) {
    std::hash<std::string> hashFunction;
    size_t hash = hashFunction(key);
    size_t index = hash % capacity_;
    if (records_[index]) {
        auto KV = records_[index]->data_.searchKey(key);
        if (KV != records_[index]->data_.end()) {
            records_[index]->data_.erase(KV);
            if (records_[index]->data_.size() == 0) {
                records_[index] = nullptr;

            }
            return true;


        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

template <class TYPE>
bool ChainingTable<TYPE>::find(const std::string& key, TYPE& value) {
    std::hash<std::string> hashFunction;
    size_t hash = hashFunction(key);
    size_t index = hash % capacity_;

    if (records_[index]) {

        auto KV = records_[index]->data_.searchKey(key);
        if (KV != records_[index]->data_.end()) {

            value = *KV;
            return true;
        }
        else {
            return false;
        }

    }
    else {

        return false;
    }




    return true;
}

template <class TYPE>
const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(const ChainingTable<TYPE>& other) {

    if (&other != this) {
        int i = 0;
        delete records_;
        records_ = new Record * [other.capacity_ + 1];

        capacity_ = other.capacity_;


        i = 0;
        while (i < capacity_ + 1) {

            if (other.records_[i]) {
                records_[i] = new Record();
                records_[i]->data_ = other.records_[i]->data_;


            }
            else {

                records_[i] = nullptr;
            }
            i++;

        }



    }



    return *this;

}

template <class TYPE>
const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(ChainingTable<TYPE>&& other) {

    if (&other != this) {
        int i = 0;
        delete records_;
        records_ = new Record * [other.capacity_ + 1];

        capacity_ = other.capacity_;


        i = 0;
        while (i < capacity_ + 1) {

            if (other.records_[i]) {
                records_[i] = new Record();
                records_[i]->data_ = other.records_[i]->data_;


            }
            else {

                records_[i] = nullptr;
            }
            i++;

        }



    }
    delete other.records_;

    return *this;

}

template <class TYPE>
ChainingTable<TYPE>::~ChainingTable() {

}

template <class TYPE>
bool ChainingTable<TYPE>::isEmpty() const {
    size_t i = 0;
    while (i != capacity_ + 1) {
        if (records_[i]) {

            return false;

        }
        i++;

    }
    return true;

}

template <class TYPE>
int ChainingTable<TYPE>::numRecords() const {
    size_t i = 0;
    size_t counter = 0;
    while (i != capacity_ + 1) {
        if (records_[i] != nullptr) {

            counter += records_[i]->data_.size();


        }
        i++;


    }
    return counter;

}
template <class TYPE>
int ChainingTable<TYPE>::capacity() const {
    return capacity_;
}

template <class TYPE>
double ChainingTable<TYPE>::loadFactor() const {
    return 0;
}



template <class TYPE>
class LPTable :public Table<TYPE> {
    struct Record {
        TYPE data_;
        std::string key_;
        bool taken;
        bool deleted;
        Record(const std::string& key, const TYPE& data) {
            key_ = key;
            data_ = data;
            deleted = false;

        }

    };

    Record** records_;   //the table
    bool sorted;// If the table is sorted 
    size_t capacity_;       //capacity of the array
    std::hash<std::string> hashFunction;
    size_t size;

public:
    LPTable(int maxExpected);
    LPTable(const LPTable& other);
    LPTable(LPTable&& other);
    double loadFactor() const;
    void mergeSort(Record** arr, Record** tmp, int start, int end);
    void merge(Record** arr, Record** tmp, int start, int start2, int end);
    virtual bool update(const std::string& key, const TYPE& value);
    virtual bool remove(const std::string& key);
    virtual bool find(const std::string& key, TYPE& value);
    virtual const LPTable& operator=(const LPTable& other);
    virtual const LPTable& operator=(LPTable&& other);
    virtual ~LPTable();
    virtual bool isEmpty() const;
    virtual int numRecords() const;
    virtual int capacity() const;

};
template <class TYPE>
LPTable<TYPE>::LPTable(int capacity) : Table<TYPE>() {
    records_ = new Record * [capacity + 1];
    capacity_ = capacity;
    for (int i = 0; i < capacity_ + 1; i++) {
        records_[i] = nullptr;

    }
    size = 0;

}

template <class TYPE>
LPTable<TYPE>::LPTable(const LPTable<TYPE>& other) {
    records_ = new Record * [other.capacity_ + 1];
    capacity_ = other.capacity_;
    size = other.size;

    for (int i = 0; i < capacity_ + 1; i++) {

        if (other.records_[i]) {
            records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);


        }
        else {
            records_[i] = nullptr;

        }

    }

}
template <class TYPE>
LPTable<TYPE>::LPTable(LPTable<TYPE>&& other) {
    records_ = new Record * [other.capacity_ + 1];
    capacity_ = other.capacity_;
    size = other.size;

    for (int i = 0; i < capacity_ + 1; i++) {

        if (other.records_[i]) {
            records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);


        }
        else {
            records_[i] = nullptr;

        }
        delete other.records_[i];

    }
    delete other.records_;
    other.records_ = nullptr;


}


template <class TYPE>
bool LPTable<TYPE>::update(const std::string& key, const TYPE& value) {


    size_t index = this->hashFunction(key) % this->capacity_;
    size_t recnum = numRecords();
    bool found = false;
    while (recnum < capacity_ - 1 && found == false) {


        if (records_[index] == nullptr) {

            records_[index] = new Record(key, value);
            records_[index]->taken = true;
            records_[index]->deleted = false;
            found = true;
            size++;
            return found;

        }
        else if (key == records_[index]->key_) {
            records_[index]->data_ = value;
            // std::cout << " key == records_[index]->key_ = " << records_[index]->key_ << std::endl;
            found = true;
            return found;
        } 

        index = (index + 1) % capacity_;
       
    }

    return false;
}
template <class TYPE>
bool LPTable<TYPE>::find(const std::string& key, TYPE& value) {
    std::hash<std::string> hashFunction;
    size_t hash = hashFunction(key);
    size_t index = hash % capacity_;


    int counter = 0;
    while (counter < capacity_ - 1) {
        if (!(records_[index])) {

            return false;
        }
        else if (records_[index]->key_ == key && records_[index]->deleted == false) {
            value = records_[index]->data_;
            return true;
        }


        index = (index + 1) % capacity_;
        counter++;
    }


    return false;
}


template <class TYPE>
void LPTable<TYPE>::mergeSort(Record** arr, Record** tmp, int start, int end) {
    if (start < end) {
        int mid = (start + end) / 2;
        mergeSort(arr, tmp, start, mid);
        mergeSort(arr, tmp, mid + 1, end);
        merge(arr, tmp, start, mid + 1, end);
    }
}

template <class TYPE>
void LPTable<TYPE>::merge(Record** arr, Record** tmp, int start, int start2, int end) {
    int aptr = start;
    int bptr = start2;
    int i = start;
    std::hash<std::string> hashFunction;
    size_t hash;
    size_t index;
    size_t hash2;
    size_t index2;
    while (aptr < start2 && bptr <= end) {
        hash = hashFunction(arr[aptr]->key_);
        index = hash % capacity_;
        hash2 = hashFunction(arr[bptr]->key_);
        index2 = hash % capacity_;

        //aptr index //bptr index2
        if (index < index2)
            tmp[i++] = arr[aptr++];
        else
            tmp[i++] = arr[bptr++];
    }
    while (aptr < start2) {
        tmp[i++] = arr[aptr++];
    }
    while (bptr <= end) {
        tmp[i++] = arr[bptr++];
    }
    for (i = start; i <= end; i++) {
        arr[i] = tmp[i];
    }
}




template <class TYPE>
bool LPTable<TYPE>::remove(const std::string& key) {
    std::hash<std::string> hashFunction;
    size_t index = hashFunction(key) % capacity_;
    int counter = 0;
    bool found = false;
    while (counter < capacity_ - 1 || found == false) {

        if (!(records_[index])) {
            return false;
        }
        else if (records_[index]->key_ == key && records_[index]->deleted == false) {

            records_[index]->taken = false;
            records_[index]->deleted = true;
            found = true;
            size--;
            break;

        }


        index = (index + 1) % capacity_;
        counter++;
    }




    return found;



}



template <class TYPE>

const LPTable<TYPE>& LPTable<TYPE>::operator=(const LPTable<TYPE>& other) {
    if (&other != this) {
        int i = 0;
        delete records_;
        records_ = new Record * [other.capacity_ + 1];

        capacity_ = other.capacity_;
        size = other.size;


        i = 0;
        while (i < capacity_ + 1) {

            if (other.records_[i]) {
                records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);


            }
            else {

                records_[i] = nullptr;
            }
            i++;

        }



    }


    return *this;

}

template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(LPTable<TYPE>&& other) {
    if (&other != this) {
        int i = 0;


        delete records_;
        records_ = new Record * [other.capacity_ + 1];

        capacity_ = other.capacity_;
        size = other.size;


        i = 0;
        while (i < capacity_ + 1) {

            if (other.records_[i]) {
                records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);


            }
            else {
                records_[i] = nullptr;

            }
            delete other.records_[i];
            i++;

        }



    }
    delete other.records_;
    other.records_ = nullptr;
    other.capacity_ = 0;

    return *this;

}

template <class TYPE>
LPTable<TYPE>::~LPTable() {


}

template <class TYPE>
bool LPTable<TYPE>::isEmpty() const {
    if (size == 0) {
        return true;
    }
    else {
        return false; 
    }

}

template <class TYPE>
int LPTable<TYPE>::numRecords() const {
    return size;



}
template <class TYPE>
int LPTable<TYPE>::capacity() const {
    return capacity_;
}
template <class TYPE>
double LPTable<TYPE>::loadFactor() const {
    return 0;
}








