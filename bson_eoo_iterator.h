#pragma once

/*=============================================================================

  Permission is hereby granted, free of charge, to any person or organization
  obtaining a copy of this software to use, reproduce, display, distribute,
  execute, and transmit this software, and to prepare derivative works of this
  software, and to permit third-parties to whom this software is furnished to
  do so, as long as this entire notice is kept intact with the software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT.

  Timothy Moore
  tim [at] timothy-moore [dot] com

  This iterator is based on the implementation of mongo::BSONObjIterator with
  the EOO value returned.

  It relies on the details of that implementation of mongo::BSONObj to iterate
  over the object natively as opposed to simply being a wrapper class around
  BSONObjIterator.

  bsonobj_eoo_iterator is designed to be an STL compliant iterator and is based
  on boost iterator facade.  It is designed to be compatiable with STL
  algorithms and boost range functionality.

  WARNING: Due to current mongo namespace pollution (in the goodies.h header,
  it has "using namespace boost"), even if you have "using namespace mongo"
  in your implementation, you will need to prefix the begin/end/get_range
  functions with the mongo:: namespadce qualification to avoid get compilation
  errors related to boost range.

  Comments, questions, etc. to the email address above.

  =============================================================================*/

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>

namespace mongo {

class bsonobj_eoo_iterator : public boost::iterator_facade<bsonobj_eoo_iterator,
							   BSONElement,
							   boost::forward_traversal_tag,
							   BSONElement> {
 public:
  bsonobj_eoo_iterator(const char* pos=NULL, const char* end=NULL) 
    : _pos(pos), _end(end) {
  }

 private:
  bool equal(const bsonobj_eoo_iterator& that) const { return _pos == that._pos; }

  void increment() {
    assert(_pos <= _end);
    int sz = (int)(_end - _pos);
    BSONElement e(_pos, sz);
    _pos += e.size(sz);
  }

  BSONElement dereference() const { return BSONElement(_pos); }

  const char* _pos;
  const char* _end;
  friend class boost::iterator_core_access;
};

bsonobj_eoo_iterator eoo_begin(const BSONObj& jso) {
  int sz = jso.objsize();
  if (MONGO_unlikely(sz == 0)) {
    return bsonobj_eoo_iterator(NULL, NULL);
  }
  return bsonobj_eoo_iterator(jso.objdata() + 4, jso.objdata() + sz);
}

bsonobj_eoo_iterator eoo_end(const BSONObj& jso) {
  int sz = jso.objsize();
  if (MONGO_unlikely(sz == 0)) {
    return bsonobj_eoo_iterator(NULL, NULL);
  }
  const char* pos = jso.objdata() + sz;
  return bsonobj_eoo_iterator(pos,pos);
}


class bsonobj_eoo_range : public boost::iterator_range<bsonobj_eoo_iterator> {
 public:
  bsonobj_eoo_range(const BSONObj& jso) 
    : boost::iterator_range<bsonobj_eoo_iterator>(mongo::eoo_begin(jso),mongo::eoo_end(jso)) {
  }
};

} // end namespace mongo
