/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_CACHE_PARTICLEFILTERCACHE_HPP
#define BI_CACHE_PARTICLEFILTERCACHE_HPP

#include "SimulatorCache.hpp"
#include "Cache1D.hpp"
#include "AncestryCache.hpp"
#include "../buffer/ParticleFilterNetCDFBuffer.hpp"

namespace bi {
/**
 * Cache for ParticleFilterNetCDFBuffer reads and writes, and efficiently
 * holding ancestry tree for drawing trajectories from the filter-smoother
 * distribution.
 *
 * @ingroup io_cache
 *
 * @tparam IO1 Buffer type.
 * @tparam CL Location.
 */
template<class IO1 = ParticleFilterNetCDFBuffer, Location CL = ON_HOST>
class ParticleFilterCache: public SimulatorCache<IO1,CL> {
public:
  /**
   * Vector type.
   */
  typedef typename temp_host_vector<real>::type vector_type;

  /**
   * Vector reference type.
   */
  typedef vector_type::vector_reference_type vector_reference_type;

  /**
   * Constructor.
   *
   * @param m Model.
   * @param out output buffer.
   */
  ParticleFilterCache(const Model& m, IO1* out = NULL);

  /**
   * Destructor.
   */
  ~ParticleFilterCache();

  /**
   * Get the most recent log-weights vector.
   *
   * @return The most recent log-weights vector to be written to the cache.
   */
  const vector_reference_type getLogWeights() const;

  /**
   * @copydoc ParticleFilterNetCDFBuffer::readLogWeights()
   */
  template<class V1>
  void readLogWeights(const int t, V1 lws) const;

  /**
   * @copydoc ParticleFilterNetCDFBuffer::writeLogWeights()
   */
  template<class V1>
  void writeLogWeights(const int t, const V1 lws);

  /**
   * @copydoc ParticleFilterNetCDFBuffer::readAncestors()
   */
  template<class V1>
  void readAncestors(const int t, V1 a) const;

  /**
   * @copydoc ParticleFilterNetCDFBuffer::writeAncestors()
   */
  template<class V1>
  void writeAncestors(const int t, const V1 a);

  /**
   * Read resample flag.
   *
   * @param t Time index.
   *
   * @return Was resampling performed at this time?
   */
  int readResample(const int t) const;

  /**
   * Write resample flag.
   *
   * @param t Time index.
   * @param r Was resampling performed at this time?
   */
  void writeResample(const int t, const int& r);

  /**
   * @copydoc ParticleFilterNetCDFBuffer::readResamples()
   */
  template<class V1>
  void readResamples(const int t, V1 r) const;

  /**
   * @copydoc ParticleFilterNetCDFBuffer::writeResamples()
   */
  template<class V1>
  void writeResamples(const int t, const V1 r);

  void writeLL(const double ll);

  /**
   * @copydoc AncestryCache::readTrajectory()
   */
  template<class M1>
  void readTrajectory(const int p, M1 X) const;

  /**
   * Write-through to the underlying buffer, as well as efficient caching
   * of the ancestry using AncestryCache.
   *
   * @tparam B Model type.
   * @tparam L Location.
   * @tparam V1 Vector type.
   *
   * @param t Time index.
   * @param s State.
   * @param as Ancestors.
   */
  template<class B, Location L, class V1>
  void writeState(const int t, const State<B,L>& s, const V1 as);

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
  /**
   * Ancestry cache.
   */
  AncestryCache ancestryCache;

  /**
   * Resampling cache.
   */
  Cache1D<real,CL> resampleCache;

  /**
   * Most recent log-weights.
   */
  vector_type logWeightsCache;

  /**
   * Output buffer.
   */
  IO1* out;
};

/**
 * Factory for creating ParticleFilterCache objects.
 *
 * @ingroup io_cache
 *
 * @see Forcer
 */
template<Location CL = ON_HOST>
struct ParticleFilterCacheFactory {
  /**
   * Create ParticleFilterCache.
   *
   * @return ParticleFilterCache object. Caller has ownership.
   *
   * @see ParticleFilterCache::ParticleFilterCache()
   */
  template<class IO1>
  static ParticleFilterCache<IO1,CL>* create(const Model& m,
      IO1* out = NULL) {
    return new ParticleFilterCache<IO1,CL>(m, out);
  }

  /**
   * Create ParticleFilterCache.
   *
   * @return ParticleFilterCache object. Caller has ownership.
   *
   * @see ParticleFilterCache::ParticleFilterCache()
   */
  static ParticleFilterCache<ParticleFilterNetCDFBuffer,CL>* create(
      const Model& m) {
    return new ParticleFilterCache<ParticleFilterNetCDFBuffer,CL>(m);
  }
};
}

template<class IO1, bi::Location CL>
bi::ParticleFilterCache<IO1,CL>::ParticleFilterCache(const Model& m, IO1* out) :
    SimulatorCache<IO1,CL>(out), ancestryCache(m), out(out) {
  //
}

template<class IO1, bi::Location CL>
bi::ParticleFilterCache<IO1,CL>::~ParticleFilterCache() {
  flush();
}

template<class IO1, bi::Location CL>
const typename bi::ParticleFilterCache<IO1,CL>::vector_reference_type bi::ParticleFilterCache<
    IO1,CL>::getLogWeights() const {
  return logWeightsCache.ref();
}

template<class IO1, bi::Location CL>
template<class V1>
void bi::ParticleFilterCache<IO1,CL>::readLogWeights(const int t,
    V1 lws) const {
  BI_ASSERT(out != NULL);

  out->readLogWeights(t, lws);
}

template<class IO1, bi::Location CL>
template<class V1>
void bi::ParticleFilterCache<IO1,CL>::writeLogWeights(const int t,
    const V1 lws) {
  if (out != NULL) {
    out->writeLogWeights(t, lws);
  }
  logWeightsCache.resize(lws.size(), false);
  logWeightsCache = lws;
}

template<class IO1, bi::Location CL>
template<class V1>
void bi::ParticleFilterCache<IO1,CL>::readAncestors(const int t,
    V1 as) const {
  BI_ASSERT(out != NULL);

  out->readAncestors(t, as);
}

template<class IO1, bi::Location CL>
template<class V1>
void bi::ParticleFilterCache<IO1,CL>::writeAncestors(const int t,
    const V1 as) {
  if (out != NULL) {
    out->writeAncestors(t, as);
  }
}

template<class IO1, bi::Location CL>
int bi::ParticleFilterCache<IO1,CL>::readResample(const int t) const {
  return resampleCache.get(t);
}

template<class IO1, bi::Location CL>
void bi::ParticleFilterCache<IO1,CL>::writeResample(const int t,
    const int& x) {
  resampleCache.set(t, x);
}

template<class IO1, bi::Location CL>
template<class V1>
void bi::ParticleFilterCache<IO1,CL>::readResamples(const int t, V1 x) const {
  x = resampleCache.get(t, x.size());
}

template<class IO1, bi::Location CL>
template<class V1>
void bi::ParticleFilterCache<IO1,CL>::writeResamples(const int t,
    const V1 x) {
  resampleCache.set(t, x.size(), x);
}

template<class IO1, bi::Location CL>
inline void bi::ParticleFilterCache<IO1,CL>::writeLL(const double ll) {
  if (out != NULL) {
    out->writeLL(ll);
  }
}

template<class IO1, bi::Location CL>
template<class M1>
void bi::ParticleFilterCache<IO1,CL>::readTrajectory(const int p,
    M1 X) const {
  ancestryCache.readTrajectory(p, X);
}

template<class IO1, bi::Location CL>
template<class B, bi::Location L, class V1>
void bi::ParticleFilterCache<IO1,CL>::writeState(const int t,
    const State<B,L>& s, const V1 as) {
  SimulatorCache<IO1,CL>::writeState(t, s);
  ancestryCache.writeState(s, as);
}

template<class IO1, bi::Location CL>
void bi::ParticleFilterCache<IO1,CL>::clear() {
  ancestryCache.clear();
  resampleCache.clear();
  SimulatorCache<IO1,CL>::clear();
}

template<class IO1, bi::Location CL>
void bi::ParticleFilterCache<IO1,CL>::empty() {
  ancestryCache.empty();
  resampleCache.empty();
  SimulatorCache<IO1,CL>::empty();
}

template<class IO1, bi::Location CL>
void bi::ParticleFilterCache<IO1,CL>::flush() {
  if (out != NULL) {
    out->writeResamples(0, resampleCache.get(0, resampleCache.size()));
  }
  //ancestryCache.flush();
  resampleCache.flush();
  SimulatorCache<IO1,CL>::flush();
}

#endif