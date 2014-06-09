/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_CACHE_SIMULATORCACHE_HPP
#define BI_CACHE_SIMULATORCACHE_HPP

#include "Cache1D.hpp"
#include "../netcdf/SimulatorNetCDFBuffer.hpp"

namespace bi {
/**
 * Cache for SimulatorNetCDFBuffer reads and writes. This caches reads and
 * writes of times. Reads and writes of variables are assumed to be large
 * and contiguous, such that NetCDF/HDF5's own buffering mechanisms, or even
 * direct reads/write on disk, are efficient enough.
 *
 * @ingroup io_cache
 *
 * @tparam CL Location.
 * @tparam IO1 Buffer type.
 */
template<Location CL = ON_HOST, class IO1 = SimulatorNetCDFBuffer>
class SimulatorCache: public IO1 {
public:
  /**
   * Pass-through constructor.
   */
  SimulatorCache();

  /**
   * Pass-through constructor.
   */
  template<class T1>
  SimulatorCache(T1& o1);

  /**
   * Pass-through constructor.
   */
  template<class T1, class T2>
  SimulatorCache(T1& o1, T2& o2);

  /**
   * Pass-through constructor.
   */
  template<class T1, class T2, class T3>
  SimulatorCache(T1& o1, T2& o2, T3& o3);

  /**
   * Pass-through constructor.
   */
  template<class T1, class T2, class T3, class T4>
  SimulatorCache(T1& o1, T2& o2, T3& o3, T4& o4);

  /**
   * Pass-through constructor.
   */
  template<class T1, class T2, class T3, class T4, class T5>
  SimulatorCache(T1& o1, T2& o2, T3& o3, T4& o4, T5& o5);

  /**
   * Shallow copy constructor.
   */
  SimulatorCache(const SimulatorCache<CL,IO1>& o);

  /**
   * Destructor.
   */
  ~SimulatorCache();

  /**
   * Deep assignment operator.
   */
  SimulatorCache<CL,IO1>& operator=(const SimulatorCache<CL,IO1>& o);

  /**
   * @copydoc SimulatorNetCDFBuffer::readTime()
   */
  void readTime(const int k, real& t) const;

  /**
   * @copydoc SimulatorNetCDFBuffer::writeTime()
   */
  void writeTime(const int k, const real& t);

  /**
   * @copydoc SimulatorNetCDFBuffer::readTimes()
   */
  template<class V1>
  void readTimes(const int k, V1 ts) const;

  /**
   * @copydoc SimulatorNetCDFBuffer::writeTimes()
   */
  template<class V1>
  void writeTimes(const int k, const V1 ts);

  /**
   * Swap the contents of the cache with that of another.
   */
  void swap(SimulatorCache<CL,IO1>& o);

  /**
   * Size of the cache.
   */
  int size() const;

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
   * Time cache.
   */
  Cache1D<real,ON_HOST> timeCache;

  /**
   * Number of times in cache.
   */
  int len;

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
bi::SimulatorCache<CL,IO1>::SimulatorCache() :
    len(0) {
  //
}

template<bi::Location CL, class IO1>
template<class T1>
bi::SimulatorCache<CL,IO1>::SimulatorCache(T1& o1) :
    IO1(o1), len(0) {
  //
}

template<bi::Location CL, class IO1>
template<class T1, class T2>
bi::SimulatorCache<CL,IO1>::SimulatorCache(T1& o1, T2& o2) :
    IO1(o1, o2), len(0) {
  //
}

template<bi::Location CL, class IO1>
template<class T1, class T2, class T3>
bi::SimulatorCache<CL,IO1>::SimulatorCache(T1& o1, T2& o2, T3& o3) :
    IO1(o1, o2, o3), len(0) {
  //
}

template<bi::Location CL, class IO1>
template<class T1, class T2, class T3, class T4>
bi::SimulatorCache<CL,IO1>::SimulatorCache(T1& o1, T2& o2, T3& o3, T4& o4) :
    IO1(o1, o2, o3, o4), len(0) {
  //
}

template<bi::Location CL, class IO1>
template<class T1, class T2, class T3, class T4, class T5>
bi::SimulatorCache<CL,IO1>::SimulatorCache(T1& o1, T2& o2, T3& o3, T4& o4,
    T5& o5) :
    IO1(o1, o2, o3, o4, o5), len(0) {
  //
}

template<bi::Location CL, class IO1>
bi::SimulatorCache<CL,IO1>::SimulatorCache(const SimulatorCache<CL,IO1>& o) :
    timeCache(o.timeCache), len(o.len) {
  //
}

template<bi::Location CL, class IO1>
bi::SimulatorCache<CL,IO1>::~SimulatorCache() {
  flush();
}

template<bi::Location CL, class IO1>
bi::SimulatorCache<CL,IO1>& bi::SimulatorCache<CL,IO1>::operator=(
    const SimulatorCache<CL,IO1>& o) {
  timeCache = o.timeCache;
  len = o.len;

  return *this;
}

template<bi::Location CL, class IO1>
inline void bi::SimulatorCache<CL,IO1>::readTime(const int k, real& t) const {
  /* pre-condition */
  BI_ASSERT(k >= 0 && k < len);

  t = timeCache.get(k);
}

template<bi::Location CL, class IO1>
inline void bi::SimulatorCache<CL,IO1>::writeTime(const int k,
    const real& t) {
  /* pre-condition */
  BI_ASSERT(k >= 0 && k <= len);

  if (k == len) {
    ++len;
  }
  timeCache.set(k, t);
}

template<bi::Location CL, class IO1>
template<class V1>
inline void bi::SimulatorCache<CL,IO1>::readTimes(const int k, V1 ts) const {
  /* pre-condition */
  BI_ASSERT(k >= 0 && k + ts.size() <= len);

  ts = timeCache.get(k, ts.size());
}

template<bi::Location CL, class IO1>
template<class V1>
inline void bi::SimulatorCache<CL,IO1>::writeTimes(const int k, const V1 ts) {
  /* pre-condition */
  BI_ASSERT(k >= 0 && k <= len);

  if (k + ts.size() > len) {
    len = k + ts.size();
  }
  timeCache.set(k, ts.size(), ts);
}

template<bi::Location CL, class IO1>
inline void bi::SimulatorCache<CL,IO1>::swap(SimulatorCache<CL,IO1>& o) {
  timeCache.swap(o.timeCache);
  std::swap(len, o.len);
}

template<bi::Location CL, class IO1>
inline int bi::SimulatorCache<CL,IO1>::size() const {
  return len;
}

template<bi::Location CL, class IO1>
inline void bi::SimulatorCache<CL,IO1>::clear() {
  timeCache.clear();
  len = 0;
}

template<bi::Location CL, class IO1>
inline void bi::SimulatorCache<CL,IO1>::empty() {
  timeCache.empty();
  len = 0;
}

template<bi::Location CL, class IO1>
inline void bi::SimulatorCache<CL,IO1>::flush() {
  IO1::writeTimes(0, timeCache.get(0, len));
  timeCache.flush();
}

template<bi::Location CL, class IO1>
template<class Archive>
void bi::SimulatorCache<CL,IO1>::save(Archive& ar,
    const unsigned version) const {
  ar & timeCache;
  ar & len;
}

template<bi::Location CL, class IO1>
template<class Archive>
void bi::SimulatorCache<CL,IO1>::load(Archive& ar, const unsigned version) {
  ar & timeCache;
  ar & len;
}

#endif
