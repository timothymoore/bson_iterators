bson_iterators
==============

C++ STL compliant iterators for mongoDB BSON objects.

bson_iterators are iterators based on the implementation of the
mongo::BSONObjIterator iterator object.  These iterators rely on
the details of that implementation of mongo::BSONObj to iterate
over the object natively as opposed to simply being a wrapper
class around BSONObjIterator. 

For bsonobj_iterator, EOO object will not be iterated over and
returned.  For bsonobj_eoo_iterator, the EOO object *WILL* be iterated over
and returned.

bsonobj_iterators are designed to be an STL compliant iterator and is based
on boost iterator facade.  It is designed to be compatiable with STL
algorithms and boost range functionality.

WARNING: Due to current mongo namespace pollution (in the goodies.h header,
it has "using namespace boost"), even if you have "using namespace mongo"
in your implementation, you will need to prefix the begin/end/get_range
functions with the mongo::namespace qualification to avoid get compilation
errors related to boost range.

This has been tested with Boost 1.42.0 and version 2.0 of the mongodb
c++ drivers.
