#ifndef RecoLocalTracker_SiPixelClusterizer_SiPixelStubBuilderBase_H
#define RecoLocalTracker_SiPixelClusterizer_SiPixelStubBuilderBase_H

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/Phase2TrackerCluster/interface/Phase2TrackerCluster1D.h"
#include "DataFormats/SiPixelStub/interface/SiPixelStub.h"
#include <vector>

class PixelGeomDetUnit;

/**
 * Abstract interface for Pixel Clusterizers
 */
class SiPixelStubBuilderBase {
public:
  typedef edmNew::DetSetVector<SiPixelStub> output_t;

  // Virtual destructor, this is a base class.
  virtual ~SiPixelStubBuilderBase() {}

  // Build stubs in a DetUnit
  virtual void buildDetUnit( const edm::DetSet<Phase2TrackerCluster1D> & input,	
			     output_t& output) = 0;

//  protected:


};

#endif
