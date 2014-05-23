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
 * @tparam IO1 Buffer type.
 * @tparam CL Location.
 */
template<class IO1 = ParticleFilterNetCDFBuffer, Location CL = ON_HOST>
class AdaptivePFCache: public BootstrapPFCache<IO1,CL> {
public:
  /**
   * Constructor.
   *
   * @param out output buffer.
   */
  AdaptivePFCache(IO1* out = NULL);

  /**
   * Shallow copy.
   */
  AdaptivePFCache(const AdaptivePFCache<IO1,CL>& o);

  /**
   * Destructor.
   */
  ~AdaptivePFCache();

  /**
   * Deep assignment.
   */
  AdaptivePFCache<IO1,CL>& operator=(
      const AdaptivePFCache<IO1,CL>& o);

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
  void swap(AdaptivePFCache<IO1,CL>& o);

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

/**
 * Factory for creating BootstrapPFCache objects.
 *
 * @ingroup io_cache
 *
 * @see Forcer
 */
template<Location CL = ON_HOST>
struct AdaptivePFCacheFactory {
  /**
   * Create AdaptivePFCache.
   *
   * @return AdaptivePFCache object. Caller has ownership.
   *
   * @see AdaptivePFCache::AdaptivePFCache()
   */
  template<class IO1>
  static AdaptivePFCache<IO1,CL>* create(IO1* out = NULL) {
    return new AdaptivePFCache<IO1,CL>(out);
  }

  /**
   * Create AdaptivePFCache.
   *
   * @return AdaptivePFCache object. Caller has ownership.
   *
   * @see AdaptivePFCache::BootstrapPFCache()
   */
  static AdaptivePFCache<ParticleFilterNetCDFBuffer,CL>* create() {
    return new AdaptivePFCache<ParticleFilterNetCDFBuffer,CL>();
  }
};
}

template<class IO1, bi::Location CL>
bi::AdaptivePFCache<IO1,CL>::AdaptivePFCache(IO1* out) :
    BootstrapPFCache<IO1,CL>(out), base(-1), P(0) {
  //
}

template<class IO1, bi::Location CL>
bi::AdaptivePFCache<IO1,CL>::AdaptivePFCache(
    const AdaptivePFCache<IO1,CL>& o) :
    BootstrapPFCache<IO1,CL>(o), timeCache(o.timeCache), particleCache(
        o.particleCache), logWeightCache(o.logWeightCache), ancestorCache(
        o.ancestorCache), base(o.base), P(o.P) {
  //
}

template<class IO1, bi::Location CL>
bi::AdaptivePFCache<IO1,CL>& bi::AdaptivePFCache<IO1,
    CL>::operator=(const AdaptivePFCache<IO1,CL>& o) {
  BootstrapPFCache<IO1,CL>::operator=(o);
  timeCache = o.timeCache;
  particleCache = o.particleCache;
  logWeightCache = o.logWeightCache;
  ancestorCache = o.ancestorCache;
  base = o.base;
  P = o.P;
  return *this;
}

template<class IO1, bi::Location CL>
bi::AdaptivePFCache<IO1,CL>::~AdaptivePFCache() {
  flush();
}

template<class IO1, bi::Location CL>
void bi::AdaptivePFCache<IO1,CL>::writeTime(const int k,
    const real& t) {
  if (base < 0) {
    base = k;
  }
  timeCache.set(k - base, t);
}

template<class IO1, bi::Location CL>
template<class V1>
void bi::AdaptivePFCache<IO1,CL>::writeLogWeights(const int k,
    const V1 lws) {
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

template<class IO1, bi::Location CL>
template<class M1, class V1>
void bi::AdaptivePFCache<IO1,CL>::writeState(const int k,
    const M1 X, const V1 as) {
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

template<class IO1, bi::Location CL>
void bi::AdaptivePFCache<IO1,CL>::push(const int P) {
  int k = 0;
  while (timeCache.isValid(k)) {
    BootstrapPFCache<IO1,CL>::writeTime(base + k, timeCache.get(k));
    BootstrapPFCache<IO1,CL>::writeState(base + k, rows(particleCache.get(k), 0, P),
        subrange(ancestorCache.get(k), 0, P), true);
    BootstrapPFCache<IO1,CL>::writeLogWeights(base + k,
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

template<class IO1, bi::Location CL>
void bi::AdaptivePFCache<IO1,CL>::swap(
    AdaptivePFCache<IO1,CL>& o) {
  BootstrapPFCache<IO1,CL>::swap(o);
  timeCache.swap(o.timeCache);
  particleCache.swap(o.particleCache);
  logWeightCache.swap(o.logWeightCache);
  ancestorCache.swap(o.ancestorCache);
  std::swap(base, o.base);
  std::swap(P, o.P);
}

template<class IO1, bi::Location CL>
void bi::AdaptivePFCache<IO1,CL>::clear() {
  BootstrapPFCache<IO1,CL>::clear();
  timeCache.clear();
  particleCache.clear();
  logWeightCache.clear();
  ancestorCache.clear();
  base = -1;
  P = 0;
}

template<class IO1, bi::Location CL>
void bi::AdaptivePFCache<IO1,CL>::empty() {
  BootstrapPFCache<IO1,CL>::empty();
  timeCache.empty();
  particleCache.empty();
  logWeightCache.empty();
  ancestorCache.empty();
  base = -1;
  P = 0;
}

template<class IO1, bi::Location CL>
void bi::AdaptivePFCache<IO1,CL>::flush() {
  push(P);
  BootstrapPFCache<IO1,CL>::flush();
  timeCache.flush();
  particleCache.flush();
  logWeightCache.flush();
  ancestorCache.flush();
}

template<class IO1, bi::Location CL>
template<class Archive>
void bi::AdaptivePFCache<IO1,CL>::save(Archive& ar,
    const unsigned version) const {
  ar
      & boost::serialization::base_object < BootstrapPFCache<IO1,CL>
          > (*this);
  ar & particleCache;
  ar & logWeightCache;
  ar & ancestorCache;
  ar & base;
  ar & P;
}

template<class IO1, bi::Location CL>
template<class Archive>
void bi::AdaptivePFCache<IO1,CL>::load(Archive& ar,
    const unsigned version) {
  ar
      & boost::serialization::base_object < BootstrapPFCache<IO1,CL>
          > (*this);
  ar & particleCache;
  ar & logWeightCache;
  ar & ancestorCache;
  ar & base;
  ar & P;
}

#endif