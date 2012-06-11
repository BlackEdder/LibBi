/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev: 2576 $
 * $Date: 2012-05-18 19:16:32 +0800 (Fri, 18 May 2012) $
 */
#ifndef BI_HOST_UPDATER_STATICMAXLOGDENSITYVISITORHOST_HPP
#define BI_HOST_UPDATER_STATICMAXLOGDENSITYVISITORHOST_HPP

namespace bi {
/**
 * Visitor for StaticMaxLogDensityHost.
 */
template<class B, class S, class PX, class OX>
class StaticMaxLogDensityVisitorHost {
public:
  template<class T1>
  static void accept(const int p, const PX& pax, OX& x, T1& lp);
};

/**
 * @internal
 *
 * Base case of StaticMaxLogDensityVisitorHost.
 */
template<class B, class PX, class OX>
class StaticMaxLogDensityVisitorHost<B,empty_typelist,PX,OX> {
public:
  template<class T1>
  static void accept(const int p, const PX& pax, OX& x, T1& lp) {
    //
  }
};
}

#include "../../typelist/front.hpp"
#include "../../typelist/pop_front.hpp"
#include "../../traits/target_traits.hpp"

template<class B, class S, class PX, class OX>
template<class T1>
void bi::StaticMaxLogDensityVisitorHost<B,S,PX,OX>::accept(
    const int p, const PX& pax, OX& x, T1& lp) {
  typedef typename front<S>::type front;
  typedef typename pop_front<S>::type pop_front;
  typedef typename front::target_type target_type;
  typedef typename target_type::coord_type coord_type;

  int ix = 0;
  coord_type cox;
  while (ix < target_size<target_type>::value) {
    front::maxLogDensities(p, ix, cox, pax, x, lp);
    ++cox;
    ++ix;
  }
  StaticMaxLogDensityVisitorHost<B,pop_front,PX,OX>::accept(p, pax, x, lp);
}

#endif
