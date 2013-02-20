//                                               -*- C++ -*-
/**
 *  @file  Memory.hxx
 *  @brief This file declares memory debugging functions
 *
 *  Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author dutka
 *  @date   2008-05-21 17:44:02 +0200 (Wed, 21 May 2008)
 *  Id      Memory.hxx 818 2008-05-21 15:44:02Z dutka
 */
#ifndef OPENTURNS_MEMORY_HXX
#define OPENTURNS_MEMORY_HXX

#ifdef DEBUG_MEMORY

#include <new>
#include <map>
#include <cstdlib>

#include "OTthread.hxx"
#include "OTmemory.hxx"

#ifndef SWIG

template <typename T>
struct CStyleAllocator
{
  typedef T         value_type;
  typedef T*        pointer;
  typedef const T*  const_pointer;
  typedef T&        reference;
  typedef const T&  const_reference;
  typedef ptrdiff_t difference_type;
  typedef size_t    size_type;

  template <typename U>
  struct rebind
  {
    typedef CStyleAllocator<U> other;
  };

  CStyleAllocator() {}

  CStyleAllocator(const CStyleAllocator & other) {}

  template <typename U> CStyleAllocator(const CStyleAllocator<U> & other) {}

  ~CStyleAllocator() {}

  pointer allocate(size_type n, const void * = 0)
  {
    if (n > this->max_size()) std::__throw_bad_alloc();
    return static_cast<T*>( std::calloc( n, sizeof(T) ) );
  }

  void deallocate(pointer p, size_type)
  {
    std::free( p );
  }

  size_type max_size() const
  {
    return size_t(-1) / sizeof(T);
  }

  void construct(void * p, const T & x)
  {
    ::new((void *)p) T(x);
  }

  void destroy(pointer p)
  {
    p->~T();
  }

  pointer address(reference x) const
  {
    return &x;
  }

  const_pointer address(const_reference x) const
  {
    return &x;
  }
};

template <typename T>
bool operator == (const CStyleAllocator<T> & lhs, const CStyleAllocator<T> & rhs)
{
  return true;
}

template <typename T>
bool operator != (const CStyleAllocator<T> & lhs, const CStyleAllocator<T> & rhs)
{
  return false;
}

struct _OpenTURNS_memory
{
  void * ptr_;
  size_t size_;
};

typedef std::map < void *,
                   _OpenTURNS_memory *,
                   std::less<void*>,
                   CStyleAllocator<std::pair<void *, _OpenTURNS_memory *> > > MemContainer;

typedef std::map<size_t, size_t, std::less<size_t>,  CStyleAllocator<std::pair<size_t, size_t> > > MemSpectrum;

class MemoryImplementation
{
public:
  MemContainer memList_;
  MemSpectrum memSpectrum_;
  size_t memoryUsage_;
  size_t fullMemoryUsage_;
  size_t maxMemoryUsage_;
  size_t maxFullMemoryUsage_;
  bool flag_;

  MemoryImplementation();
  ~MemoryImplementation();
};

class Memory
{
  static MemoryImplementation * p_impl_;
  Memory() {}
  static void Release();
public:
  ~Memory() {}
  static MemoryImplementation & GetInstance();
};

class LockNewDelete
{
  static pthread_mutex_t NewMutex_;
  static void Lock();
  static void Unlock();
public:
  LockNewDelete();
  ~LockNewDelete();
};

#endif /* SWIG */

#endif /* DEBUG_MEMORY */

extern size_t GetMemoryUsage();
extern size_t GetFullMemoryUsage();
extern size_t GetMaxMemoryUsage();
extern size_t GetMaxFullMemoryUsage();

extern void printMemoryUsage();
extern void printMemorySpectrum();

extern void SetMemoryFlag(bool);

#endif /* OPENTURNS_MEMORY_HXX */
