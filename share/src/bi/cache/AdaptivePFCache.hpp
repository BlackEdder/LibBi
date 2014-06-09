/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_CACHE_ADAPTIVEPFCACHE_HPP
#define BI_CACHE_ADAPTIVEPFCACHE_HPP

#include "BootstrapPFCache.hpp"
#include "CacheObject.hpp"
#include "CacheCross.hpp"
#include "Cache1D.hpp"

namespace bi {
/**
 * Additional wrapper around BootstrapPFCache for use of
 * AdaptivePF. Buffers output in memory until stopping criterion
 * is met, and then passes only surviving particles to BootstrapPFCache
 * for output.
 *
 * @ingroup io_cache
 *
 * @tparam CL Location.
 * @tparam IO1 Buffer type.
 */
template<Location CL = ON_HOST, class IO1 = ParticleFilterNetCDFBuffer>
class AdaptivePFCache: public BootstrapPFCache<CL,IO1> {
public:
  /**
   * Pass-through constructor.
   */
  AdaptivePFCache();

  /**
   * Pass-through constructor.
   */
  template<class T1>
  AdaptivePFCache(T1& o1);

  /**
   * Pass-through constructor.
   */
  template<class T1, class T2>
  AdaptivePFCache(T1& o1, T2& o2);

  /**
   * Pass-through constructor.
   */
  template<class T1, class T2, class T3>
  AdaptivePFCache(T1& o1, T2& o2, T3& o3);

  /**
   * Pass-through constructor.
   */
  template<class T1, class T2, class T3, class T4>
  AdaptivePFCache(T1& o1, T2& o2, T3& o3, T4& o4);

  /**
   * Pass-through constructor.
   */
  template<class T1, class T2, class T3, class T4, class T5>
  AdaptivePFCache(T1& o1, T2& o2, T3& o3, T4& o4, T5& o5);

  /**
   * Shallow copy.
   */
  AdaptivePFCache(const AdaptivePFCache<CL,IO1>& o);

  /**
   * Destructor.
   */
  ~AdaptivePFCache();

  /**
   * Deep assignment.
   */
  AdaptivePFCache<CL,IO1>& operator=(const AdaptivePFCache<CL,IO1>& o);

  /**
   * @copydoc SimulatorNetCDFBuffer::writeTime()
   */
  void writeTime(const int k, const real& t);

  /**
   * @copydoc ParticleFilterNetCDFBuffer::writeLogWeights()
   */
  template<class V1>
  void writeLogWeights(const int k, const V1 lws);

  /**
   * Write-through to the underlying buffer, as well as efficient caching
   * of the ancestry using AncestryCache.
   *
   * @tparam M1 Matrix type.
   * @tparam V1 Vector type.
   *
   * @param k Time index.
   * @param X State.
   * @param as Ancestors.
   */
  template<class M1, class V1>
  void writeState(const int k, const M1 X, const V1 as);

  /**
   * Push down to BootstrapPFCache. This is a special method for
   * AdaptivePFCache that pushes temporary storage of particles
   * into BootstrapPFCache once the stopping criterion is met.
   *
   * @param P Number of particles to push down (allows last block to be
   * omitted, for example).
   */
  void push(const int P);

  /**
   * Swap the contents of the cache with that of another.
   */
  void swap(AdaptivePFCache<CL,IO1>& o);

  /**
   * Clear cache.
   */
  void clear();

  /**
   * Empty cache.
   */
  void empty();

  /**
   * Flush cache to output buffer.
   */
  void flush();

private:
  typedef typename loc_temp_matrix<CL,real>::type matrix_type;
  typedef typename loc_temp_vector<CL,real>::type vector_type;
  typedef typename loc_temp_vector<CL,int>::type int_vector_type;

  /**
   * Caches for times while adapting.
   */
  Cache1D<real,CL> timeCache;

  /**
   * Caches for particles while adapting, indexed by time.
   */
  CacheObject<matrix_type> particleCache;

  /**
   * Cache for log-weights while adapting.
   */
  CacheObject<vector_type> logWeightCache;

  /**
   * Cache for ancestry while adapting.
   */
  CacheObject<int_vector_type> ancestorCache;

  /**
   * Base time index.
   */
  int base;

  /**
   * Number of particles written while adapting.
   */
  int P;

  /**
   * Serialize.
   */
  template<class Archive>
  void save(Archive& ar, const unsigned version) const;

  /**
   * Restore from serialization.
   */
  template<class Archive>
  void load(Archive& ar, const unsigned version);

  /*
   * Boost.Serialization requirements.
   */
  BOOST_SERIALIZATION_SPLIT_MEMBER()
  friend class boost::serialization::access;
};
}

template<bi::Location CL, class IO1>
bi::AdaptivePFCache<CL,IO1>::AdaptivePFCache(), base(-1), P(0) {
  //
}

template<bi::Location CL, class IO1>
template<class T1>
bi::AdaptivePFCache<CL,IO1>::AdaptivePFCache(T1& o1) :
    BootstrapPFCache<CL,IO1>(o1), base(-1), P(0) {
  //
}

template<bi::Location CL, class IO1>
template<class T1, class T2>
bi::AdaptivePFCache<CL,IO1>::AdaptivePFCache(T1& o1, T2& o2) :
    BootstrapPFCache<CL,IO1>(o1, o2), base(-1), P(0) {
  //
}

template<bi::Location CL, class IO1>
template<class T1, class T2, class T3>
bi::AdaptivePFCache<CL,IO1>::AdaptivePFCache(T1& o1, T2& o2, T3& o3) :
    BootstrapPFCache<CL,IO1>(o1, o2, o3), base(-1), P(0) {
  //
}

template<bi::Location CL, class IO1>
template<class T1, class T2, class T3, class T4>
bi::AdaptivePFCache<CL,IO1>::AdaptivePFCache(T1& o1, T2& o2, T3& o3, T4& o4) :
    BootstrapPFCache<CL,IO1>(o1, o2, o3, o4), base(-1), P(0) {
  //
}

template<bi::Location CL, class IO1>
template<class T1, class T2, class T3, class T4, class T5>
bi::AdaptivePFCache<CL,IO1>::AdaptivePFCache(T1& o1, T2& o2, T3& o3, T4& o4,
    T5& o5) :
    BootstrapPFCache<CL,IO1>(o1, o2, o3, o4, o5), base(-1), P(0) {
  //
}

template<bi::Location CL, class IO1>
bi::AdaptivePFCache<CL,IO1>::AdaptivePFCache(const AdaptivePFCache<CL,IO1>& o) :
    BootstrapPFCache<CL,IO1>(o), timeCache(o.timeCache), particleCache(
        o.particleCache), logWeightCache(o.logWeightCache), ancestorCache(
        o.ancestorCache), base(o.base), P(o.P) {
  //
}

template<bi::Location CL, class IO1>
bi::AdaptivePFCache<CL,IO1>& bi::AdaptivePFCache<CL,IO1>::operator=(
    const AdaptivePFCache<CL,IO1>& o) {
  BootstrapPFCache<CL,IO1>::operator=(o);
  timeCache = o.timeCache;
  particleCache = o.particleCache;
  logWeightCache = o.logWeightCache;
  ancestorCache = o.ancestorCache;
  base = o.base;
  P = o.P;
  return *this;
}

template<bi::Location CL, class IO1>
bi::AdaptivePFCache<CL,IO1>::~AdaptivePFCache() {
  flush();
}

template<bi::Location CL, class IO1>
void bi::AdaptivePFCache<CL,IO1>::writeTime(const int k, const real& t) {
  if (base < 0) {
    base = k;
  }
  timeCache.set(k - base, t);
}

template<bi::Location CL, class IO1>
template<class V1>
void bi::AdaptivePFCache<CL,IO1>::writeLogWeights(const int k, const V1 lws) {
  int j = k - base;

  if (j >= logWeightCache.size()) {
    logWeightCache.resize(j + 1);
  }
  if (!logWeightCache.isValid(j)) {
    logWeightCache.setValid(j);
  }
  if (logWeightCache.get(j).size() < P) {
    logWeightCache.get(j).resize(P, true);
  }

  subrange(logWeightCache.get(j), P - lws.size(), lws.size()) = lws;
}

template<bi::Location CL, class IO1>
template<class M1, class V1>
void bi::AdaptivePFCache<CL,IO1>::writeState(const int k, const M1 X,
    const V1 as) {
  /* pre-condition */
  assert(X.size1() == as.size());

  int j = k - base;
  P += X.size1();

  if (j >= particleCache.size()) {
    particleCache.resize(j + 1);
  }
  if (!particleCache.isValid(j)) {
    particleCache.setValid(j);
  }
  if (particleCache.get(j).size1() < P) {
    particleCache.get(j).resize(P, X.size2(), true);
  }

  if (j >= ancestorCache.size()) {
    ancestorCache.resize(j + 1);
  }
  if (!ancestorCache.isValid(j)) {
    ancestorCache.setValid(j);
  }
  if (ancestorCache.get(j).size() < P) {
    ancestorCache.get(j).resize(P, true);
  }

  rows(particleCache.get(j), P - X.size1(), X.size1()) = X;
  subrange(ancestorCache.get(j), P - as.size(), as.size()) = as;
}

template<bi::Location CL, class IO1>
void bi::AdaptivePFCache<CL,IO1>::push(const int P) {
  int k = 0;
  while (timeCache.isValid(k)) {
    BootstrapPFCache<CL,IO1>::writeTime(base + k, timeCache.get(k));
    BootstrapPFCache<CL,IO1>::writeState(base + k,
        rows(particleCache.get(k), 0, P),
        subrange(ancestorCache.get(k), 0, P));
    BootstrapPFCache<CL,IO1>::writeLogWeights(base + k,
        subrange(logWeightCache.get(k), 0, P));
    ++k;
  }

  timeCache.clear();
  particleCache.clear();
  logWeightCache.clear();
  ancestorCache.clear();
  base = -1;
  this->P = 0;
}

template<bi::Location CL, class IO1>
void bi::AdaptivePFCache<CL,IO1>::swap(AdaptivePFCache<CL,IO1>& o) {
  BootstrapPFCache<CL,IO1>::swap(o);
  timeCache.swap(o.timeCache);
  particleCache.swap(o.particleCache);
  logWeightCache.swap(o.logWeightCache);
  ancestorCache.swap(o.ancestorCache);
  std::swap(base, o.base);
  std::swap(P, o.P);
}

template<bi::Location CL, class IO1>
void bi::AdaptivePFCache<CL,IO1>::clear() {
  BootstrapPFCache<CL,IO1>::clear();
  timeCache.clear();
  particleCache.clear();
  logWeightCache.clear();
  ancestorCache.clear();
  base = -1;
  P = 0;
}

template<bi::Location CL, class IO1>
void bi::AdaptivePFCache<CL,IO1>::empty() {
  BootstrapPFCache<CL,IO1>::empty();
  timeCache.empty();
  particleCache.empty();
  logWeightCache.empty();
  ancestorCache.empty();
  base = -1;
  P = 0;
}

template<bi::Location CL, class IO1>
void bi::AdaptivePFCache<CL,IO1>::flush() {
  push(P);
  BootstrapPFCache<CL,IO1>::flush();
  timeCache.flush();
  particleCache.flush();
  logWeightCache.flush();
  ancestorCache.flush();
}

template<bi::Location CL, class IO1>
template<class Archive>
void bi::AdaptivePFCache<CL,IO1>::save(Archive& ar,
    const unsigned version) const {
  ar & boost::serialization::base_object < BootstrapPFCache<CL,IO1> > (*this);
  ar & particleCache;
  ar & logWeightCache;
  ar & ancestorCache;
  ar & base;
  ar & P;
}

template<bi::Location CL, class IO1>
template<class Archive>
void bi::AdaptivePFCache<CL,IO1>::load(Archive& ar, const unsigned version) {
  ar & boost::serialization::base_object < BootstrapPFCache<CL,IO1> > (*this);
  ar & particleCache;
  ar & logWeightCache;
  ar & ancestorCache;
  ar & base;
  ar & P;
}

#endif
