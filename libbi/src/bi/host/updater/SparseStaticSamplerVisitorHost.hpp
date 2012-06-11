/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev: 2576 $
 * $Date: 2012-05-18 19:16:32 +0800 (Fri, 18 May 2012) $
 */
#ifndef BI_HOST_UPDATER_SPARSESTATICSAMPLERVISITORHOST_HPP
#define BI_HOST_UPDATER_SPARSESTATICSAMPLERVISITORHOST_HPP

namespace bi {
/**
 * Visitor for SparseStaticSamplerHost.
 */
template<class B, class S, class PX, class OX>
class SparseStaticSamplerVisitorHost {
public:
  static void accept(Random& rng, const Mask<ON_HOST>& mask, const int p, const PX& pax, OX& x);
};

/**
 * @internal
 *
 * Base case of SparseStaticSamplerVisitorHost.
 */
template<class B, class PX, class OX>
class SparseStaticSamplerVisitorHost<B,empty_typelist,PX,OX> {
public:
  static void accept(Random& rng, const Mask<ON_HOST>& mask, const int p, const PX& pax, OX& x) {
    //
  }
};
}

#include "../../typelist/front.hpp"
#include "../../typelist/pop_front.hpp"
#include "../../traits/target_traits.hpp"

template<class B, class S, class PX, class OX>
void bi::SparseStaticSamplerVisitorHost<B,S,PX,OX>::accept(Random& rng, const Mask<ON_HOST>& mask, const int p, const PX& pax, OX& x) {
  typedef typename front<S>::type front;
  typedef typename pop_front<S>::type pop_front;
  typedef typename front::target_type target_type;
  typedef typename target_type::coord_type coord_type;

  const int id = var_id<target_type>::value;
  int ix = 0;
  coord_type cox;
  if (mask.isDense(id)) {
    while (ix < target_size<target_type>::value) {
      front::samples(rng, p, ix, cox, pax, x);
      ++cox;
      ++ix;
    }
  } else if (mask.isSparse(id)) {
    while (ix < mask.getSize(id)) {
      cox.setIndex(mask.getIndex(id, ix));
      front::samples(rng, p, ix, cox, pax, x);
      ++ix;
    }
  }
  SparseStaticSamplerVisitorHost<B,pop_front,PX,OX>::accept(rng, mask, p, pax, x);
}

#endif
