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

  This iterator is based on the implementation of mongo::BSONObjIterator.
  It relies on the details of that implementation of mongo::BSONObj to iterate
  over the object natively as opposed to simply being a wrapper class around
  BSONObjIterator.  The EOO object will not be iterated over.

  bsonobj_iterator is designed to be an STL compliant iterator and is based
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

class bsonobj_iterator : public boost::iterator_facade<bsonobj_iterator,
						       BSONElement,
						       boost::forward_traversal_tag,
						       BSONElement> {
 public:
  explicit bsonobj_iterator(const char* pos=NULL) : _pos(pos) { }

 private:
  bool equal(const bsonobj_iterator& that) const { return _pos == that._pos; }

  void increment() {
    BSONElement e(_pos);
    _pos += e.size();
  }

  BSONElement dereference() const { return BSONElement(_pos); }

  const char* _pos;
  friend class boost::iterator_core_access;
};


bsonobj_iterator begin(const BSONObj& jso) {
  int sz = jso.objsize();
  return bsonobj_iterator(sz==0 ? NULL : jso.objdata() + 4);
}

bsonobj_iterator end(const BSONObj& jso) {
  int sz = jso.objsize();
  return bsonobj_iterator(sz==0 ? NULL : jso.objdata() + sz - 1);
}


class bsonobj_range : public boost::iterator_range<bsonobj_iterator> {
 public:
  bsonobj_range(const BSONObj& jso) 
    : boost::iterator_range<bsonobj_iterator>(mongo::begin(jso),mongo::end(jso)) {
  }
};


bsonobj_range get_range(const BSONObj& jso) {
  return bsonobj_range(jso);
}


} // end namespace mongo
