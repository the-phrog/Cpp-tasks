#include "circular_buffer.hpp"

typedef char value_type;

extern value_type *buf;
extern int fst;
extern int lst;
extern int len;

CircularBuffer::CircularBuffer(): buf(0), fst(0), lst(-1), len(0) {};
CircularBuffer::~CircularBuffer() {};
CircularBuffer::CircularBuffer(const CircularBuffer &cb): fst(cb.fst), lst(cb.lst), len(cb.len) {
    buf = new value_type[cb.len];
    // buf = (value_type*)realloc(buf, cb.len*sizeof(value_type));
    memcpy(buf, (cb.buf), cb.len*sizeof(value_type));
};
CircularBuffer::CircularBuffer(int capacity): fst(0), lst(capacity-1),  len(capacity){
    buf = new value_type[capacity];
    // buf = (value_type*)realloc(buf, capacity*sizeof(value_type));
    for(int i=0; i<len; i++)
        buf[i] = 0;
};

CircularBuffer::CircularBuffer(int capacity, const value_type &elem): buf(new value_type[capacity]), fst(0), lst(capacity-1), len(capacity) {
// CircularBuffer::CircularBuffer(int capacity, const value_type &elem): buf((value_type*)realloc(buf, capacity*sizeof(value_type))), fst(0), lst(capacity-1), len(capacity) {
    for(int i=0; i<capacity; i++)
        buf[i] = elem;
};

value_type &CircularBuffer::operator[](int i) { return buf[i]; };
const value_type &CircularBuffer::operator[](int i) const { return buf[i]; };

value_type &CircularBuffer::at(int i) {
    if(i >= 0 && i < len)
        return buf[i];
    throw "Index is out of range.";
};
const value_type &CircularBuffer::at(int i) const {
    if(i >= 0 && i < len)
        return buf[i];
    throw "Index is out of range.";
};

value_type &CircularBuffer::front() {
    if(this->empty() == false)
        return buf[fst];
    throw "The buffer has no elements.";
};
value_type &CircularBuffer::back() {
    if(this->empty() == false)
        return buf[lst];
    throw "The buffer has no elements.";
};
const value_type &CircularBuffer::front() const {
    if(this->empty() == false)
        return buf[fst];
    throw "The buffer has no elements.";
};
const value_type &CircularBuffer::back() const {
    if(this->empty() == false)
        return buf[lst];
    throw "The buffer has no elements.";
};

value_type *CircularBuffer::linearize() {
    CircularBuffer cb(len);
    for(int i = fst; i != (fst <= lst ? lst + 1 : len); i++){
        cb[i-fst] = buf[i];
        cb.lst++;
    }
    if(fst > lst)
        for(int i=0; i != lst+1; i++){
            cb[i-fst+len] = buf[i];
            cb.lst++;
        }
    return cb.buf;
};

bool CircularBuffer::is_linearized() const { return fst == 0; };

void CircularBuffer::rotate(int new_begin) {
    CircularBuffer cb(len);
    int nb = new_begin;
    this->at(nb);
    for(int i=fst; i != (fst <= lst ? lst + 1 : len); i++)
        cb[i-nb+(i-nb >= 0 ? 0 : len)] = buf[i];    
    if(fst > lst)
        for(int i=0; i<=lst; i++)
            cb[i-nb+(i-nb >= 0 ? 0 : len)] = buf[i];
    fst = fst-nb + (fst-nb >= 0 ? 0 : len);
    lst = lst-nb + (lst-nb >= 0 ? 0 : len);
    // for(int i=0; i<cb.len; i++)
    //     std::cout << "--- " << (int)cb[i] << std::endl;
    memcpy(buf, cb.buf, len);
};

int CircularBuffer::size() const {
    return (buf[fst] ? (lst-fst+(fst<=lst ? 0 : len)+1) : 0);
};

bool CircularBuffer::empty() const { return this->size() == 0; };

bool CircularBuffer::full() const { return this->size() == len; };

int CircularBuffer::reserve() const { return len - this->size(); };

int CircularBuffer::capacity() const { return len; };

void CircularBuffer::set_capacity(int new_capacity) {
    int ncap = new_capacity;
    int cap  = this->capacity();
    int size = this->size();
    memcpy(buf, this->linearize(), len*sizeof(value_type));
    fst = 0, lst = size-1;
    if(ncap < cap){
        this->at(ncap);
        lst = ncap-1;
    }

    value_type* sup = new value_type[ncap];
    for(int i=0; i<(ncap>cap ? cap : ncap); i++)
        sup[i] = buf[i];
    delete[] buf;
    buf = sup;
    // buf = (value_type*)realloc(buf, ncap*sizeof(value_type));

    if(ncap > cap)
        for(int i=cap; i<ncap; i++)
            buf[i] = 0;
    len = ncap;
};

void CircularBuffer::resize(int new_size, const value_type &item/* = value_type()*/) {
    int nsize = new_size;
    int size  = this->size();
    int cap   = this->capacity();
    memcpy(buf, this->linearize(), len*sizeof(value_type));
    fst = 0, lst = size-1 + (size-1 > 0 ? 0 : len);
    if(nsize > cap){
        value_type* sup = new value_type[nsize];
        for(int i=0; i<size; i++)
            sup[i] = buf[i];
        delete[] buf;
        buf = sup;
        // buf = (value_type*)realloc(buf, nsize*sizeof(value_type));

        for(int i=size; i<nsize; i++)
            buf[i] = item;
        len = nsize;
    }
    else if(nsize > size)
        for(int i=size; i<nsize; i++)
            buf[i] = item;
    else
        for(int i=nsize; i<size; i++)
            buf[i] = 0;
    lst = nsize-1 + (nsize-1 > 0 ? 0 : len);
};

CircularBuffer &CircularBuffer::operator=(const CircularBuffer &cb) {
    len = cb.len, fst = cb.fst, lst = cb.lst;
    delete[] buf;
    buf = new value_type[cb.len];
    // buf = (value_type*)realloc(buf, cb.len*sizeof(value_type));
    memcpy(buf, cb.buf, cb.len*sizeof(value_type));
    return *this;
};

void CircularBuffer::swap(CircularBuffer &cb){
    CircularBuffer a(cb);
    cb = *this;
    *this = a;
};

void CircularBuffer::push_back(const value_type &item/* = value_type()*/) {
    if(this->capacity() == 0)
        throw "Buffer capacity is zero.";
    if(this->size() > 0){
        lst = lst+1 - (lst+1 != len ? 0 : len);
        this->at(lst) = item;
        if(fst == lst)
            fst = fst+1 - (fst+1 != len ? 0 : len);
    }
    else{
        this->at(fst) = item;
        lst = fst;
    }
};

void CircularBuffer::push_front(const value_type &item/* = value_type()*/) {
    if(this->capacity() == 0)
        throw "Buffer capacity is zero.";
    if(this->size() > 0){
        fst = fst-1 + (fst-1 != -1 ? 0 : len);
        (*this)[fst] = item;
        if(fst == lst)
        lst = lst-1 + (lst-1 != len ? 0 : len);
    }
    else{
        lst = fst;
        buf[fst] = item;
    }
};

void CircularBuffer::pop_back() {
    if(this->empty())
        throw "Buffer is empty.";
    this->back() = 0;
    lst = lst-1 + (lst-1 != -1 ? 0 : len);
};

void CircularBuffer::pop_front() {
    if(this->empty())
        throw "Buffer is empty.";
    this->front() = 0;
    fst = fst+1 - (fst+1 != len ? 0 : len);
};

void CircularBuffer::insert(int pos, const value_type &item/* = value_type()*/) {
    if(this->empty())
        throw "Buffer is empty.";
    this->at(pos) = item;
    // if((fst <= lst && fst >= 0 && lst < len) ||
    //    (fst > lst && fst < len && lst >= 0))
    //     buf[pos] = item;
    // else
    //     throw "You can't change element of buffer that goes out of its bounds.";
};

void CircularBuffer::erase(int first, int last) {
    this->at(first);
    this->at(last-1);
    for(int i= first; i<last; i++)
        (*this)[i] = 0;
};

void CircularBuffer::clear() {
    for(int i=0; i<len; i++)
        buf[i] = 0;
    fst = 0;
    lst = len-1;
};

bool operator==(const CircularBuffer &a, const CircularBuffer &b){
    if(a.size() == b.size()){
        CircularBuffer A = a;
        CircularBuffer B = b;
        value_type a_back, b_back;
        while(A.size() != 0 && B.size() != 0){
            a_back = A.back();
            b_back = B.back();
            if(a_back != b_back)
                return false;
            A.pop_back();
            B.pop_back();
        }
        return true;
    }
    else
        return false;
};

bool operator!=(const CircularBuffer &a, const CircularBuffer &b){
    if(a == b)
        return false;
    else
        return true;
};
