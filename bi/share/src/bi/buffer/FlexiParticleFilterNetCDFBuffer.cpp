/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#include "FlexiParticleFilterNetCDFBuffer.hpp"

bi::FlexiParticleFilterNetCDFBuffer::FlexiParticleFilterNetCDFBuffer(const Model& m,
    const std::string& file, const FileMode mode) :
    FlexiSimulatorNetCDFBuffer(m, file, mode) {
  map();
}

bi::FlexiParticleFilterNetCDFBuffer::FlexiParticleFilterNetCDFBuffer(
    const Model& m, const int T, const std::string& file,
    const FileMode mode) : FlexiSimulatorNetCDFBuffer(m, T, file, mode) {
  if (mode == NEW || mode == REPLACE) {
    create();
  } else {
    map();
  }
}

void bi::FlexiParticleFilterNetCDFBuffer::create() {
  ncFile->add_att("data_format", "ANPF");

  aVar = ncFile->add_var("ancestor", ncInt, nrpDim);
  BI_ERROR_MSG(aVar != NULL && aVar->is_valid(),
      "Could not create variable ancestor");

  lwVar = ncFile->add_var("logweight", netcdf_real, nrpDim);
  BI_ERROR_MSG(lwVar != NULL && lwVar->is_valid(),
      "Could not create variable logweight");

  rVar = ncFile->add_var("resample", ncInt, nrDim);
  BI_ERROR_MSG(rVar != NULL && rVar->is_valid(),
      "Could not create variable resample");
}

void bi::FlexiParticleFilterNetCDFBuffer::map() {
  aVar = ncFile->get_var("ancestor");
  BI_ERROR_MSG(aVar != NULL && aVar->is_valid(),
      "File does not contain variable ancestor");
  BI_ERROR_MSG(aVar->num_dims() == 1, "Variable ancestor has " <<
      aVar->num_dims() << " dimensions, should have 1");
  BI_ERROR_MSG(aVar->get_dim(0) == nrpDim,
      "Dimension 0 of variable ancestor should be nrp");

  lwVar = ncFile->get_var("logweight");
  BI_ERROR_MSG(lwVar != NULL && lwVar->is_valid(),
      "File does not contain variable logweight");
  BI_ERROR_MSG(lwVar->num_dims() == 1, "Variable logweight has " <<
      lwVar->num_dims() << " dimensions, should have 1");
  BI_ERROR_MSG(lwVar->get_dim(0) == nrpDim,
      "Dimension 0 of variable logweight should be nrp");

  rVar = ncFile->get_var("resample");
  BI_ERROR_MSG(rVar != NULL && rVar->is_valid(),
      "File does not contain variable resample");
  BI_ERROR_MSG(rVar->num_dims() == 1, "Variable resample has " <<
      rVar->num_dims() << " dimensions, should have 1");
  BI_ERROR_MSG(rVar->get_dim(0) == nrDim,
      "Dimension 0 of variable resample should be nr");
}
