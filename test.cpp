#include <iostream>
#include <algorithm>
#include <boost/foreach.hpp>
#include <mongo/client/dbclient.h>
#include <mongo/bson/bson.h>
#include <bson_iterators.h>

using std::cout;
using std::endl;
using namespace mongo;

struct true_f {
  bool operator()(const BSONElement&) const {
    return true;
  }
};


int main() {
  /* Simple stand-alone BSON tests without actual connection to mongo server */

  BSONObj bjarne = BSON( GENOID << "name" << "bjarne" << "lang" << "C++" );
  BSONObj larry  = BSON( GENOID << "name" << "larry"  << "lang" << "perl" );
  BSONObj matz   = BSON( GENOID << "name" << "matz"   << "lang" << "ruby" );

  /* Iterator */
  bsonobj_iterator first = mongo::begin(bjarne), last = mongo::end(bjarne);
  cout << "Standard Iterator Usage:" << endl;
  for (; first != last; ++first) {
    cout << "element: " << first->toString() << endl;
  }
  cout << endl;

  /* Iterator w/ EOO object */
  bsonobj_eoo_iterator eoo_first = mongo::eoo_begin(matz), eoo_last = mongo::eoo_end(matz);
  cout << "Standard Iterator Usage w/ EOO:" << endl;
  for (; eoo_first != eoo_last; ++eoo_first) {
    cout << "element: " << eoo_first->toString() << endl;
  }
  cout << endl;

  /* BOOST_FOREACH usage */
  cout << "BOOST_FOREACH iteration:" << endl;
  BOOST_FOREACH(BSONElement e, mongo::get_range(larry)) {
    cout << "element: " << e.toString() << endl;
  }
  cout << endl;

  // std distance
  cout << "std distance: " << std::endl;
  unsigned int num = std::distance(mongo::begin(matz), mongo::end(matz));
  cout << "document has " << num << " elements" << endl;
  cout << endl;

  // std algorithm usage
  cout << "std algorithm iteration;" << endl;
  num = std::count_if(mongo::begin(matz), mongo::end(matz), true_f());
  cout << "document has " << num << " elements" << endl;
  
  return 0;
}
