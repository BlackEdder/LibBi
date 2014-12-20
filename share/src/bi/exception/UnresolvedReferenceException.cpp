/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#include "UnresolvedReferenceException.hpp"

#include <sstream>

biprog::UnresolvedReferenceException::UnresolvedReferenceException(
    Reference* ref) {
  std::stringstream buf;
  buf << "unresolved reference " << *dynamic_cast<Expression*>(ref) << std::endl;
  msg = buf.str();
}
