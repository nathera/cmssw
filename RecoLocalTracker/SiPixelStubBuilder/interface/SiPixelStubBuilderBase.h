#ifndef RecoLocalTracker_SiPixelClusterizer_SiPixelStubBuilderBase_H
#define RecoLocalTracker_SiPixelClusterizer_SiPixelStubBuilderBase_H

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/SiPixelStub/interface/SiPixelStub.h"
#include <vector>

class PixelGeomDetUnit;

/**
 * Abstract interface for Pixel Clusterizers
 */
class SiPixelStubBuilderBase {
public:
  typedef edm::DetSet<SiPixelCluster>::const_iterator    SiClusterIterator;

  // Virtual destructor, this is a base class.
  virtual ~SiPixelStubBuilderBase() {}

  // Build clusters in a DetUnit. Both digi and cluster stored in a DetSet

  virtual void buildDetUnit( const edm::DetSet<SiPixelCluster> & input,	
				  const PixelGeomDetUnit * pixDet,
				  edmNew::DetSetVector<SiPixelStub>::FastFiller& output) = 0;


//  protected:


};

#endif
