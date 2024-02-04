#ifndef VECTOR_H
#define VECTOR_H

#include<iostream>
#include<stdexcept>
#include<initializer_list>

template <typename T>

class Vector {
public:
  class ConstIterator;
  class Iterator;
  using size_type = std::size_t;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = Vector::Iterator;
  using const_iterator = Vector::ConstIterator;
  
private:
  static constexpr size_type base_size {5};
  static constexpr double growth_factor {1.61};
  size_type sz;
  size_type max_sz;
  pointer values;

public:
//constructors.
  Vector() : sz{0}, max_sz{0}, values{nullptr} {}
  
  explicit Vector(size_type n) : sz{0}, max_sz{0}, values{nullptr} {
    reserve(n);
  }
  
  Vector(std::initializer_list<value_type> l) : Vector(l.size()) {
    for (const auto& v : l)
      values[sz++] = v;
  }
  
//deconstructor.  
  ~Vector() {
    delete[] values;
  }
  
//copy constructor.
  Vector(const Vector& v) : Vector(v.size()) {
	  //reserve(v.max_sz);
	  for(; sz<v.sz; ++sz)
		  values[sz] = v.values[sz];
  }

//copy assignment operator.
  Vector& operator=(Vector v) {
    std::swap(sz, v.sz);
    std::swap(max_sz, v.max_sz);
    std::swap(values, v.values);
    return *this;
  }
  
//methods.
  iterator begin() {
    if(sz == 0)
      iterator temp (values+sz);
    iterator temp (values);
    return temp;
  }
  
  iterator end() {
    iterator temp (values+sz);
    return temp;
  }
  
  const_iterator begin() const {
    if(sz == 0)
      const_iterator temp (values+sz);
    const_iterator temp (values);
    return temp;
  }
  
  const_iterator end() const {
    const_iterator temp (values+sz);
    return temp;
  }
  
  size_type size() const {
    return sz;
  }
  
  size_type capacity() const {
    return max_sz;
  }
  
  bool empty() const {
    if (sz <= 0)
      return true;
    return false;
  }

  void clear() {
    sz = 0;
  }
  
  void reserve(size_type n) {
    if (max_sz >= n)
      return;
    pointer next {new value_type[n]};
    for (size_type i {0}; i<sz; ++i)
      next[i] = values[i];
    delete[] values;
    values = next;
    max_sz = n;
  }
  
  void push_back(const_reference val) {
    if (sz == max_sz){
      reserve(max_sz * growth_factor + base_size); 
    }
    values[sz++] = val;
  }
  
  void pop_back() {
    if (empty())
      throw std::exception();
    --sz;
  }
  
  void shrink_to_fit() {
    max_sz = sz;
  }
  
  reference operator[](size_type index) {
    if (index >= sz)
      throw std::exception();
    return values[index];
  }
  
  const_reference operator[](size_type index) const {
    if (index >= sz)
      throw std::exception();
    return values[index];
  }
  
  std::ostream& print(std::ostream& o) const {
    o << '[';
    bool first {true};
    for (size_type i {0}; i<sz; ++i) {
      if (first) {
        first = false;
      }
      else{
        o << ", ";
      }
      o << values[i];
    }
    return o << ']';
  }
  
  class Iterator {
    public:
      using value_type = Vector::value_type;
      using reference = Vector::reference;
      using pointer = Vector::pointer;
      using difference_type = Vector::difference_type;
      using iterator_category = std::forward_iterator_tag;
    private:
      pointer ptr;
    public:
    //constructors.
      Iterator() : ptr{nullptr} {}
      
      Iterator(pointer ptr) : ptr{ptr} {}
      
    //deconstructor.  
    //  ~Iterator() {
    //    delete[] ptr;
    //  }
    //methods.
      reference operator*() const {
        return *ptr;
      }
      
      pointer operator->() const {
        return ptr;
      }
      
      bool operator==(const const_iterator& ci) const {
        return static_cast<const_iterator>(*this) == ci;
      }
      
      bool operator!=(const const_iterator& ci) const {
        return static_cast<const_iterator>(*this) != ci;
      }
      
      iterator& operator++() {
        ++ptr;
        return *this;
      }
      
      iterator operator++(int) {
        Vector::iterator temp {ptr};
        ++*this;
        return temp;
      }
      
      operator const_iterator() const {
        Vector::const_iterator convert {ptr};
        return convert;
      }
  };
  
  class ConstIterator {
    public:
      using value_type = Vector::value_type;
      using reference = Vector::const_reference;
      using pointer = Vector::const_pointer;
      using difference_type = Vector::difference_type;
      using iterator_category = std::forward_iterator_tag;
    private:
    //Instanzvariablen
      pointer ptr;
      friend Vector::difference_type operator-(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop) {
        return lop.ptr-rop.ptr;
      }
    public:
      ConstIterator() : ptr{nullptr} {}
      
      ConstIterator(pointer ptr) : ptr{ptr} {}
    //methods.
      reference operator*() const {
        return *ptr;
      }
      
      pointer operator->() const {
        return ptr;
      }
      
      bool operator==(const const_iterator& ci) const {
        return this->ptr == ci.ptr;
      }
      
      bool operator!=(const const_iterator& ci) const {
        return this->ptr != ci.ptr;
      }
      
      const_iterator& operator++() {
        ++ptr;
        return *this;
      }
      
      const_iterator operator++(int) {
        Vector::const_iterator temp {ptr};
        ++*this;
        return temp;
      }
  };
  
  iterator insert(const_iterator pos, const_reference val) {
    auto diff = pos-begin();
    if (diff<0 || static_cast<size_type>(diff)>sz)
      throw std::runtime_error("Iterator out of bounds");
    size_type current{static_cast<size_type>(diff)};
    if (sz>=max_sz)
      reserve(max_sz*2+1);
    for (auto i {sz}; i-->current;)
      values[i+1]=values[i];
    values[current]=val;
    ++sz;
    return iterator{values+current};
  }
  
  iterator erase(const_iterator pos) {
    auto diff = pos-begin();
    if (diff<0 || static_cast<size_type>(diff)>=sz)
      throw std::runtime_error("Iterator out of bounds");
    size_type current{static_cast<size_type>(diff)};
    for (auto i{current}; i<sz-1; ++i)
      values[i]=values[i+1];
    --sz;
    return iterator{values+current};
  }  
};

template<typename T>
std::ostream& operator<<(std::ostream& o, const Vector<T>& v) {
  return v.print(o);
};
#endif
