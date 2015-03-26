#ifndef RecoLocalTracker_SiPixelClusterizer_SiPixelStubBuilderBase_H
#define RecoLocalTracker_SiPixelClusterizer_SiPixelStubBuilderBase_H

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/Phase2TrackerCluster/interface/Phase2TrackerCluster1D.h"
#include "DataFormats/SiPixelStub/interface/SiPixelStub.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include <map>
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

  // grouping clusters
  virtual std::vector< std::pair< int, std::vector<Phase2TrackerCluster1D> > > groupinginStackModules(const edmNew::DetSetVector<Phase2TrackerCluster1D>& clusters, const TrackerTopology& topo) = 0;

  // Build stubs in a DetUnit
  virtual void buildDetUnit( const edm::DetSetVector<Phase2TrackerCluster1D> & input,	
			     output_t& output) = 0;

  unsigned int getLayerNumber(const DetId& detid, const TrackerTopology* topo) {
    if (detid.det() == DetId::Tracker) {
        if (detid.subdetId() == PixelSubdetector::PixelBarrel) {
          return (topo->pxbLayer(detid));
        }
        // ERICA::chech: E+ 100; E- 200 : needed?
        else if (detid.subdetId() == PixelSubdetector::PixelEndcap) {
          return (100 * topo->pxfSide(detid) + topo->pxfDisk(detid));
        }
        else return 999;
    }
    return 999;
  }


  unsigned int getModuleNumber(const DetId& detid, const TrackerTopology* topo) {
    if (detid.det() == DetId::Tracker) {
        if (detid.subdetId() == PixelSubdetector::PixelBarrel) {
          return ( topo->pxbModule(detid) );
        }
        else if (detid.subdetId() == PixelSubdetector::PixelEndcap) {
          return ( topo->pxfModule(detid) );
        }
        else return 999;
    }
    return 999;
  }
//  protected:


};

#endif
