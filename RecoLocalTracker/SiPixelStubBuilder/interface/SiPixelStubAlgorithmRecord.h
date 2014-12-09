/*! \class   SiPixelStubAlgorithmRecord
 *  \brief   Class to store the SiPixelStubAlgorithm used
 *           in SiliconStripStubBuilder (copied from
 *           TTStubBuilderAlgorithmRecord.h
 *
 *  \author Natalie Heracleous
 *  \date   2014, Dec 04
 *
 */

#ifndef SI_PIXEL_STUB_ALGO_RECORD_H
#define SI_PIXEL_STUB_ALGO_RECORD_H

#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "Geometry/Records/interface/StackedTrackerGeometryRecord.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "boost/mpl/vector.hpp"

class SiPixelStubAlgorithmRecord
  : public edm::eventsetup::DependentRecordImplementation< SiPixelStubAlgorithmRecord, boost::mpl::vector< StackedTrackerGeometryRecord, IdealMagneticFieldRecord > >{};

#endif

