/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev: 2589 $
 * $Date: 2012-05-23 13:15:11 +0800 (Wed, 23 May 2012) $
 */
#ifndef BI_CUDA_UPDATER_STATICSAMPLERKERNEL_CUH
#define BI_CUDA_UPDATER_STATICSAMPLERKERNEL_CUH

#include "../cuda.hpp"

namespace bi {
/**
 * Kernel function for static sampling.
 *
 * @tparam B Model type.
 * @tparam S Action type list.
 */
template<class B, class S>
CUDA_FUNC_GLOBAL void kernelStaticSampler(Random rng);

}

#include "StaticSamplerVisitorGPU.cuh"
#include "../constant.cuh"
#include "../global.cuh"
#include "../../state/Pa.hpp"

template<class B, class S>
void bi::kernelStaticSampler(Random rng) {
  typedef Pa<ON_DEVICE,B,real,global,global,global,global> PX;
  typedef Ox<ON_DEVICE,B,real,global> OX;
  typedef StaticSamplerVisitorGPU<B,S,PX,OX> Visitor;

  PX pax;
  OX x;

  Rng<ON_DEVICE> rng1(rng.getDevRng()); // local copy, faster
  Visitor::accept(rng1, pax, x);
  rng.setDevRng(rng1);
}

#endif
