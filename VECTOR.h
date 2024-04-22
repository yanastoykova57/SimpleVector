#ifndef vector_h
#define vector_h
#include<iostream>
#include<stdexcept>
#include<memory>
#include<initializer_list>
#include<algorithm>
using namespace std;

template<typename T>
class Vector {
public:
    class ConstIterator;
    class Iterator;
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = Vector :: Iterator;
    using const_iterator = Vector :: ConstIterator;
private:
    size_t sz;
    size_t max_sz;
    T* values;

public:
    
    Vector (): sz{0}, max_sz{0}, values {nullptr} {}
    
    Vector (const Vector& v):sz(v.sz), max_sz(v.max_sz), values (new T[max_sz]) {
         for(size_t i = 0; i<v.size(); ++i)
             values[i] = v.values[i];
     }
     
    Vector(size_t n) : sz{0}, max_sz{0}, values {nullptr} {
        max_sz = n;
    }
    
    
    Vector(initializer_list<T> list) : Vector (list.size()){
    sz = list.size();
    values = new T[sz];
    int i{0};
    for (auto &elem : list){
        values[i] = elem;
        i++;}
    }
    
    ~ Vector () {
        delete[] values;
    }
    
    Vector& operator= (const Vector& v) {
    delete[] values;
    sz = v.size();
    max_sz = v.capacity();
    values = new double*[max_sz];
    for(size_t i = 0; i<max_sz; ++i){
     values[i] = v[i];}
     return *this;
     
}

    size_t size() const {return sz;}
    
    bool empty () const {
        return sz == 0;
    }
    
    void clear() {
        sz = 0;
    }
    
    void reserve (size_t n) {
        
        if (max_sz >= n) return;
        
        
        T* next {new T [n]};
        for(size_t i (0); i < max_sz; i++){
            next[i] = values [i];}

        delete [] values;
        max_sz = n;
        values = next;
    }
    
    void shrink_to_fit(){
        if(max_sz == sz) return;
        if(sz == 0) {
            delete[] values;
            values = nullptr;
            max_sz = 0;
        }
      T* next {new T [sz]};
        for(size_t i (0); i < max_sz; i++)
            next[i] = values [i];

        delete [] values;
        max_sz = sz;
        values = next;
       
    }
    
    void push_back(const T &x) {
        if (sz >= max_sz) {
            reserve(2*max_sz +1);
        }
       
        values [sz++] = x;
    }
    
    void pop_back() {
        if (sz==0) throw runtime_error ("Vector schon leer!");
        sz--;
    }
    
    T& operator[] (size_t index) {
        if (index>=sz) throw runtime_error ("Index bigger than sz");
        return this->values[index];
    }
    
    const T& operator [] (size_t index) const {
        if (index>=sz) throw runtime_error ("Index bigger than sz");
        return this->values[index];
    }
    
    size_t capacity() const {
        return max_sz;
    }

    iterator begin() {return values;}
    iterator end () {return values+sz;}
    const_iterator begin() const{return values;}
    const_iterator end () const{return values+sz;}
    
    friend ostream& operator<<(ostream& o, const Vector& v) {
    o << "[";
    for (size_t i = 0; i <= v.sz -1 ; ++i) {
        if (i == v.sz -1)
            o << v.values[i]<< "]";
        else o << v.values[i] << ", ";
        }
        return o;
    }
    
    class Iterator {
    public:
        using value_type = Vector :: value_type;
        using difference_type = Vector::difference_type;
        using reference = Vector::reference;
        using pointer = Vector::pointer;
        using iterator_category = forward_iterator_tag;
    private:
        pointer ptr;
    public:
        Iterator (): ptr{nullptr} {}
        Iterator (pointer pntr) : ptr{nullptr} {
            ptr = pntr;
        }
        
        reference operator*() const {return *ptr;}
        
        pointer operator->() const {return ptr;}
        
        friend bool operator ==  (const iterator& a, const iterator& b) {
            return a.ptr==b.ptr;
        }
        
        friend bool operator !=  (const iterator& a, const iterator& b) {
            return !(a.ptr==b.ptr);
        }
        
        iterator& operator++() {
            ++ptr;
            return (*this);
        }
        
        iterator operator++(int) {
            Iterator it = *this;
            ++(*this);
            return it;
        }
        
       operator const_iterator() const{
           ConstIterator it{ptr};
           return it;
        }
    };
    
    class ConstIterator {
    public:
        using value_type = Vector :: value_type;
        using difference_type = Vector::difference_type;
        using reference = Vector::const_reference;
        using pointer = Vector::pointer;
        using iterator_category = forward_iterator_tag;
    private:
        pointer ptr;
    public:
        ConstIterator ()  {}

        ConstIterator(pointer pntr){
            ptr = pntr;
        }
       
        reference operator*() const {return *ptr;}
        
        pointer operator->() const {return ptr;}


        friend bool operator == (const const_iterator& a, const const_iterator& b) {
            return a.ptr==b.ptr;
        }
        
        friend bool operator != (const const_iterator& a, const const_iterator& b) {
            return !(a.ptr==b.ptr);
        }


        const_iterator& operator++(){
            ++ptr;
            return (*this);
        }


        const_iterator operator++(int){
            ConstIterator iter (ptr);
            ++(*this);
            return iter;
        }

        friend Vector :: difference_type operator-(const Vector :: ConstIterator& lop ,const Vector :: ConstIterator& rop) {
           return lop.ptr-rop.ptr ;
       }
    };
  
    iterator insert(const_iterator pos, const_reference val) {
        auto diff = pos-begin();
        if (diff<0 || static_cast<size_type>(diff)>sz)
         throw std::runtime_error("Iterator out of bounds");
        size_type current{static_cast<size_type>(diff)};
           if (sz>=max_sz)
            reserve (max_sz*2+1); //Achtung Sonderfall , wenn keine Mindestgroesse definiert ist
        for (auto i{sz}; i-->current;) {
          values[i+1]= values[i];}
        values[current] = val ;
        ++sz ;
           return Vector::iterator{values+current};
       }
       
       iterator erase(const_iterator pos) {
        auto diff = pos-begin();
        if (diff<0 || static_cast<size_type>(diff)>=sz)
         throw std::runtime_error("Iterator out of bounds");
        size_type current{static_cast<size_type>(diff)};
           for (auto i{current}; i<sz-1; ++i)
            values [i]=values [i +1];
           --sz;
         return iterator{values+current};
       }
};

#endif
