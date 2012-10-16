/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_METHOD_MISC_HPP
#define BI_METHOD_MISC_HPP

#include "../math/scalar.hpp"
#include "../cache/Cache2D.hpp"

#include <set>

namespace bi {
/**
 * Particle filtering modes.
 */
enum FilterMode {
  /**
   * Unconditioned filter.
   */
  UNCONDITIONED,

  /**
   * Filter conditioned on current state trajectory.
   */
  CONDITIONED
};

/**
 * Optimisation modes.
 */
enum OptimiserMode {
  /**
   * Maximum likelihood estimation.
   */
  MAXIMUM_LIKELIHOOD,

  /**
   * Maximum a posteriori.
   */
  MAXIMUM_A_POSTERIORI
};

/**
 * Compute next time for given delta that is greater than the current time.
 *
 * @ingroup method
 *
 * @param t Current time.
 * @param delta Time step.
 *
 * @return If @p delta is positive, next time that is a multiple of @p delta.
 * If @p delta is negative, previous time that is a multiple of
 * <tt>abs(delta)</tt>.
 */
real gt_step(const real t, const real delta);

/**
 * Compute next time for given delta that is greater than or equal to the
 * current time.
 *
 * @ingroup method
 *
 * @param t Current time.
 * @param delta Time step.
 *
 * @return If @p t a multiple of @p delta, then @p t. If @p delta is positive,
 * next time that is a multiple of @p delta. If @p delta is negative,
 * previous time that is a multiple of <tt>abs(delta)</tt>.
 */
real ge_step(const real t, const real delta);

/**
 * Number of time steps in closed interval.
 *
 * @ingroup method
 *
 * @param t Time.
 * @param delta Time step (positive).
 *
 * @return Number of multiples of @p delta on the interval <tt>[0,t]</tt>.
 */
int le_steps(const real t, const real delta);

/**
 * Number of time steps in open interval.
 *
 * @ingroup method
 *
 * @param t Time.
 * @param delta Time step (positive).
 *
 * @return Number of multiples of @p delta on the interval <tt>[0,t)</tt>.
 */
int lt_steps(const real t, const real delta);

/**
 * Number of time steps in closed interval
 *
 * @ingroup method
 *
 * @param ti Start of interval.
 * @param tj End of interval.
 * @param delta Time step.
 *
 * @return If @p delta is positive, number of multiples of @p delta on the
 * interval <tt>[ti,tj]</tt>. If @p delta is negative, number of multiples of
 * <tt>abs(delta)</tt> on the interval <tt>[tj,ti]</tt>;
 */
int le_steps(const real ti, const real tj, const real delta);

/**
 * Number of time steps in open interval
 *
 * @ingroup method
 *
 * @param ti Start of interval.
 * @param tj End of interval.
 * @param delta Time step.
 *
 * @return If @p delta is positive, number of multiples of @p delta on the
 * interval <tt>[ti,tj)</tt>. If @p delta is negative, number of multiples of
 * <tt>abs(delta)</tt> on the interval <tt>[tj,ti)</tt>;
 */
int lt_steps(const real ti, const real tj, const real delta);

}

#include "../math/function.hpp"
#include "../math/temp_vector.hpp"
#include "../primitive/vector_primitive.hpp"
#include "../primitive/matrix_primitive.hpp"

inline real bi::gt_step(const real t, const real delta) {
  return delta*bi::ceil((t + 1.0e-3*delta)/delta);
}

inline real bi::ge_step(const real t, const real delta) {
  return delta*bi::ceil((t - 1.0e-3*delta)/delta);
}

inline int bi::le_steps(const real t, const real delta) {
  return static_cast<int>(bi::ceil((t + 1.0e-3*delta)/delta));
}

inline int bi::lt_steps(const real t, const real delta) {
  return static_cast<int>(bi::ceil((t - 1.0e-3*delta)/delta));
}

inline int bi::le_steps(const real ti, const real tj, const real delta) {
  int steps;
  if (tj >= ti) {
    steps = le_steps(tj, delta) - lt_steps(ti, delta);
  } else {
    steps = le_steps(ti, delta) - lt_steps(tj, delta);
  }
  return steps;
}

inline int bi::lt_steps(const real ti, const real tj, const real delta) {
  int steps;
  if (tj >= ti) {
    steps = lt_steps(tj, delta) - lt_steps(ti, delta);
  } else {
    steps = lt_steps(ti, delta) - lt_steps(tj, delta);
  }
  return steps;
}

#endif
