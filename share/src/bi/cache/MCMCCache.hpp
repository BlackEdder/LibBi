/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_CACHE_MCMCCACHE_HPP
#define BI_CACHE_MCMCCACHE_HPP

#include "SimulatorCache.hpp"
#include "Cache1D.hpp"
#include "CacheCross.hpp"
#include "../netcdf/MCMCNetCDFBuffer.hpp"

namespace bi {
/**
 * Cache for MCMCNetCDFBuffer reads and writes.
 *
 * @ingroup io_cache
 *
 * @tparam IO1 Output type.
 * @tparam CL Location.
 */
template<Location CL = ON_HOST, class IO1 = MCMCNetCDFBuffer>
class MCMCCache: public SimulatorCache<CL,IO1> {
public:
  /**
   * Pass-through constructor.
   */
  template<class T1>
  MCMCCache(T1& o1);

  /**
   * Pass-through constructor.
   */
  template<class T1, class T2>
  MCMCCache(T1& o1, T2& o2);

  /**
   * Pass-through constructor.
   */
  template<class T1, class T2, class T3>
  MCMCCache(T1& o1, T2& o2, T3& o3);

  /**
   * Pass-through constructor.
   */
  template<class T1, class T2, class T3, class T4>
  MCMCCache(T1& o1, T2& o2, T3& o3, T4& o4);

  /**
   * Pass-through constructor.
   */
  template<class T1, class T2, class T3, class T4, class T5>
  MCMCCache(T1& o1, T2& o2, T3& o3, T4& o4, T5& o5);

  /**
   * Shallow copy constructor.
   */
  MCMCCache(const MCMCCache<CL,IO1>& o);

  /**
   * Destructor.
   */
  ~MCMCCache();

  /**
   * Deep assignment operator.
   */
  MCMCCache<CL,IO1>& operator=(const MCMCCache<CL,IO1>& o);

  /**
   * Read log-likelihood.
   *
   * @param p Sample index.
   *
   * @return Log-likelihood.
   */
  real readLogLikelihood(const int p);

  /**
   * Write log-likelihood.
   *
   * @param p Sample index.
   * @param ll Log-likelihood.
   */
  void writeLogLikelihood(const int p, const real ll);

  /**
   * Read log-prior density.
   *
   * @param p Sample index.
   *
   * @return Log-prior density.
   */
  real readLogPrior(const int p);

  /**
   * Write log-prior density.
   *
   * @param p Sample index.
   * @param lp Log-prior density.
   */
  void writeLogPrior(const int p, const real lp);

  /**
   * Read parameter sample.
   *
   * @tparam V1 Vector type.
   *
   * @param p Sample index.
   * @param[out] theta Sample.
   */
  template<class V1>
  void readParameter(const int p, V1 theta);

  /**
   * Write parameter sample.
   *
   * @tparam V1 Vector type.
   *
   * @param p Sample index.
   * @param theta Sample.
   */
  template<class V1>
  void writeParameter(const int p, const V1 theta);

  /**
   * Read state path sample.
   *
   * @tparam M1 Matrix type.
   *
   * @param p Sample index.
   * @param[out] X Path. Rows index variables, columns index times.
   */
  template<class M1>
  void readPath(const int p, M1 X);

  /**
   * Write state path sample.
   *
   * @tparam M1 Matrix type.
   *
   * @param p Sample index.
   * @param X Trajectories. Rows index variables, columns index times.
   */
  template<class M1>
  void writePath(const int p, const M1 X);

  /**
   * Is cache full?
   */
  bool isFull() const;

  /**
   * Swap the contents of the cache with that of another.
   */
  void swap(MCMCCache<CL,IO1>& o);

  /**
   * Clear cache.
   */
  void clear();

  /**
   * Empty cache.
   */
  void empty();

  /**
   * Flush to output buffer.
   */
  void flush();

private:
  /**
   * Flush state trajectories to disk.
   *
   * @param type Variable type.
   */
  void flushPaths(const VarType type);

  /**
   * Log-likelihoods cache.
   */
  Cache1D<real,CL> llCache;

  /**
   * Log-prior densities cache.
   */
  Cache1D<real,CL> lpCache;

  /**
   * Parameters cache.
   */
  CacheCross<real,CL> parameterCache;

  /**
   * Trajectories cache.
   */
  std::vector<CacheCross<real,CL>*> pathCache;

  /**
   * Id of first sample in cache.
   */
  int first;

  /**
   * Number of samples in cache.
   */
  int len;

  /**
   * Maximum number of samples to store in cache.
   */
  static const int NUM_SAMPLES = 16384 / sizeof(real);

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
template<class T1>
bi::MCMCCache<CL,IO1>::MCMCCache(T1& o1) :
    SimulatorCache<CL,IO1>(o1), llCache(NUM_SAMPLES), lpCache(NUM_SAMPLES), parameterCache(
        NUM_SAMPLES, this->m.getNetSize(P_VAR)), first(0), len(0) {
  //
}

template<bi::Location CL, class IO1>
template<class T1, class T2>
bi::MCMCCache<CL,IO1>::MCMCCache(T1& o1, T2& o2) :
    SimulatorCache<CL,IO1>(o1, o2), llCache(NUM_SAMPLES), lpCache(
        NUM_SAMPLES), parameterCache(NUM_SAMPLES, this->m.getNetSize(P_VAR)), first(
        0), len(0) {
  //
}

template<bi::Location CL, class IO1>
template<class T1, class T2, class T3>
bi::MCMCCache<CL,IO1>::MCMCCache(T1& o1, T2& o2, T3& o3) :
    SimulatorCache<CL,IO1>(o1, o2, o3), llCache(NUM_SAMPLES), lpCache(
        NUM_SAMPLES), parameterCache(NUM_SAMPLES, this->m.getNetSize(P_VAR)), first(
        0), len(0) {
  //
}

template<bi::Location CL, class IO1>
template<class T1, class T2, class T3, class T4>
bi::MCMCCache<CL,IO1>::MCMCCache(T1& o1, T2& o2, T3& o3, T4& o4) :
    SimulatorCache<CL,IO1>(o1, o2, o3, o4), llCache(NUM_SAMPLES), lpCache(
        NUM_SAMPLES), parameterCache(NUM_SAMPLES, this->m.getNetSize(P_VAR)), first(
        0), len(0) {
  //
}

template<bi::Location CL, class IO1>
template<class T1, class T2, class T3, class T4, class T5>
bi::MCMCCache<CL,IO1>::MCMCCache(T1& o1, T2& o2, T3& o3, T4& o4, T5& o5) :
    SimulatorCache<CL,IO1>(o1, o2, o3, o4, o5), llCache(NUM_SAMPLES), lpCache(
        NUM_SAMPLES), parameterCache(NUM_SAMPLES, this->m.getNetSize(P_VAR)), first(
        0), len(0) {
  //
}

template<bi::Location CL, class IO1>
bi::MCMCCache<CL,IO1>::MCMCCache(const MCMCCache<CL,IO1>& o) :
    SimulatorCache<CL,IO1>(o), llCache(o.llCache), lpCache(o.lpCache), parameterCache(
        o.parameterCache), first(o.first), len(o.len) {
  pathCache.resize(o.pathCache.size());
  for (int i = 0; i < pathCache.size(); ++i) {
    pathCache[i] = new CacheCross<real,CL>(*o.pathCache[i]);
  }
}

template<bi::Location CL, class IO1>
bi::MCMCCache<CL,IO1>::~MCMCCache() {
  flush();
  for (int i = 0; i < int(pathCache.size()); ++i) {
    delete pathCache[i];
  }
}

template<bi::Location CL, class IO1>
bi::MCMCCache<CL,IO1>& bi::MCMCCache<CL,IO1>::operator=(
    const MCMCCache<CL,IO1>& o) {
  SimulatorCache<CL,IO1>::operator=(o);

  empty();
  llCache = o.llCache;
  lpCache = o.lpCache;
  parameterCache = o.parameterCache;
  first = o.first;
  len = o.len;

  pathCache.resize(o.pathCache.size());
  for (int i = 0; i < pathCache.size(); ++i) {
    pathCache[i] = new CacheCross<real,CL>();
    pathCache[i] = *o.pathCache[i];
  }

  return *this;
}

template<bi::Location CL, class IO1>
real bi::MCMCCache<CL,IO1>::readLogLikelihood(const int p) {
  /* pre-condition */
  BI_ASSERT(p >= first && p < first + len);

  return llCache.get(p - first);
}

template<bi::Location CL, class IO1>
void bi::MCMCCache<CL,IO1>::writeLogLikelihood(const int p, const real ll) {
  /* pre-condition */
  BI_ASSERT(len == 0 || (p >= first && p <= first + len));

  if (len == 0) {
    first = p;
  }
  if (p - first == len) {
    len = p - first + 1;
  }
  llCache.set(p - first, ll);
}

template<bi::Location CL, class IO1>
real bi::MCMCCache<CL,IO1>::readLogPrior(const int p) {
  /* pre-condition */
  BI_ASSERT(p >= first && p < first + len);

  return lpCache.get(p - first);
}

template<bi::Location CL, class IO1>
void bi::MCMCCache<CL,IO1>::writeLogPrior(const int p, const real lp) {
  /* pre-condition */
  BI_ASSERT(len == 0 || (p >= first && p <= first + len));

  if (len == 0) {
    first = p;
  }
  if (p - first == len) {
    len = p - first + 1;
  }
  lpCache.set(p - first, lp);
}

template<bi::Location CL, class IO1>
template<class V1>
void bi::MCMCCache<CL,IO1>::readParameter(const int p, V1 theta) {
  /* pre-condition */
  BI_ASSERT(p >= first && p < first + len);

  theta = parameterCache.get(p - first);
}

template<bi::Location CL, class IO1>
template<class V1>
void bi::MCMCCache<CL,IO1>::writeParameter(const int p, const V1 theta) {
  /* pre-condition */
  BI_ASSERT(len == 0 || (p >= first && p <= first + len));

  if (len == 0) {
    first = p;
  }
  if (p - first == len) {
    len = p - first + 1;
  }
  parameterCache.set(p - first, theta);
}

template<bi::Location CL, class IO1>
template<class M1>
void bi::MCMCCache<CL,IO1>::readPath(const int p, M1 X) {
  /* pre-condition */
  BI_ASSERT(len == 0 || (p >= first && p <= first + len));
  BI_ASSERT(X.size2() == pathCache.size());

  for (int t = 0; t < pathCache.size(); ++t) {
    column(X, t) = pathCache[t]->get(p - first);
  }
}

template<bi::Location CL, class IO1>
template<class M1>
void bi::MCMCCache<CL,IO1>::writePath(const int p, const M1 X) {
  /* pre-condition */
  BI_ASSERT(len == 0 || (p >= first && p <= first + len));

  if (len == 0) {
    first = p;
  }
  if (p - first == len) {
    len = p - first + 1;
  }

  if (int(pathCache.size()) < X.size2()) {
    pathCache.resize(X.size2(), NULL);
  }
  for (int t = 0; t < X.size2(); ++t) {
    if (pathCache[t] == NULL) {
      pathCache[t] = new CacheCross<real,CL>(NUM_SAMPLES, this->m.getDynSize());
    }
    pathCache[t]->set(p - first, column(X, t));
  }
}

template<bi::Location CL, class IO1>
bool bi::MCMCCache<CL,IO1>::isFull() const {
  return len == NUM_SAMPLES;
}

template<bi::Location CL, class IO1>
void bi::MCMCCache<CL,IO1>::swap(MCMCCache<CL,IO1>& o) {
  llCache.swap(o.llCache);
  lpCache.swap(o.lpCache);
  parameterCache.swap(o.parameterCache);
  pathCache.swap(o.pathCache);
  std::swap(first, o.first);
  std::swap(len, o.len);
}

template<bi::Location CL, class IO1>
void bi::MCMCCache<CL,IO1>::clear() {
  llCache.clear();
  lpCache.clear();
  parameterCache.clear();
  for (int t = 0; t < int(pathCache.size()); ++t) {
    pathCache[t]->clear();
  }
  first = 0;
  len = 0;
}

template<bi::Location CL, class IO1>
void bi::MCMCCache<CL,IO1>::empty() {
  llCache.empty();
  lpCache.empty();
  parameterCache.empty();
  for (int k = 0; k < pathCache.size(); ++k) {
    pathCache[k]->empty();
    delete pathCache[k];
  }
  pathCache.resize(0);
  first = 0;
  len = 0;
}

template<bi::Location CL, class IO1>
void bi::MCMCCache<CL,IO1>::flush() {
  IO1::writeLogLikelihoods(first, llCache.get(0, len));
  IO1::writeLogPriors(first, lpCache.get(0, len));
  IO1::writeParameters(first, parameterCache.get(0, len));

  llCache.flush();
  lpCache.flush();
  parameterCache.flush();

  flushPaths(R_VAR);
  flushPaths(D_VAR);
}

template<bi::Location CL, class IO1>
void bi::MCMCCache<CL,IO1>::flushPaths(const VarType type) {
  /* don't do it time-by-time, too much seeking in looping over variables
   * several times... */
  //for (int k = 0; k < int(pathCache.size()); ++k) {
  //  IO1::writeState(k, first, pathCache[k]->get(0, len));
  //  pathCache[k]->flush();
  //}
  /* ...do it variable-by-variable instead, and loop over times several
   * times */
  Var* var;
  int id, i, k, start, size;

  for (id = 0; id < this->m.getNumVars(type); ++id) {
    var = this->m.getVar(type, id);
    start = var->getStart() + ((type == D_VAR) ? this->m.getNetSize(R_VAR) : 0);
    size = var->getSize();

    for (k = 0; k < int(pathCache.size()); ++k) {
      IO1::writeStateVar(type, id, k, first,
          columns(pathCache[k]->get(0, len), start, size));
    }
  }
}

template<bi::Location CL, class IO1>
template<class Archive>
void bi::MCMCCache<CL,IO1>::save(Archive& ar, const unsigned version) const {
  ar & boost::serialization::base_object < SimulatorCache<CL,IO1> > (*this);
  ar & llCache;
  ar & lpCache;
  ar & parameterCache;
  ar & pathCache;
  ar & first;
  ar & len;
}

template<bi::Location CL, class IO1>
template<class Archive>
void bi::MCMCCache<CL,IO1>::load(Archive& ar, const unsigned version) {
  ar & boost::serialization::base_object < SimulatorCache<CL,IO1> > (*this);
  ar & llCache;
  ar & lpCache;
  ar & parameterCache;
  ar & pathCache;
  ar & first;
  ar & len;
}

#endif
