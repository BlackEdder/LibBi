/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_RANDOM_RANDOMGPU_CUH
#define BI_RANDOM_RANDOMGPU_CUH

#include "RandomKernel.cuh"
#include "../../random/Random.hpp"
#include "../device.hpp"

template<class V1>
void bi::RandomGPU::uniforms(Random& rng, V1 x,
    const typename V1::value_type lower,
    const typename V1::value_type upper) {
  dim3 Db, Dg;
  Db.x = deviceIdealThreadsPerBlock();
  Dg.x = (std::min(x.size(), deviceIdealThreads()) + Db.x - 1)/Db.x;

  kernelUniforms<<<Dg,Db>>>(Random(rng), x, lower, upper);
  CUDA_CHECK;
}

template<class V1>
void bi::RandomGPU::gaussians(Random& rng, V1 x,
    const typename V1::value_type mu, const typename V1::value_type sigma) {
  dim3 Db, Dg;
  Db.x = deviceIdealThreadsPerBlock();
  Dg.x = (std::min(x.size(), deviceIdealThreads()) + Db.x - 1)/Db.x;

  kernelGaussians<<<Dg,Db>>>(Random(rng), x, mu, sigma);
  CUDA_CHECK;
}

template<class V1>
void bi::RandomGPU::gammas(Random& rng, V1 x,
    const typename V1::value_type alpha, const typename V1::value_type beta) {
  BI_ERROR(false, "Not implemented on device");
}

template<class V1, class V2>
void bi::RandomGPU::multinomials(Random& rng, const V1 ps, V2 xs) {
  BI_ERROR(false, "Not implemented on device");
}

#endif