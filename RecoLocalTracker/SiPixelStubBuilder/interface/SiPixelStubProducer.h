#ifndef RecoLocalTracker_SiPixelStubBuilder_SiPixelStubProducer_h
#define RecoLocalTracker_SiPixelStubBuilder_SiPixelStubProducer_h

//---------------------------------------------------------------------------
//! \class SiPixelStubProducer
//!
//! \brief EDProducer to create SiPixelStubs from SiPixelClusters.
//!//!
//! \author Natalie Heracleous adapted from SiPixelClusterizer
//! \version February 2nd, 2015
//!
//---------------------------------------------------------------------------

#include "RecoLocalTracker/SiPixelStubBuilder/interface/SiPixelStubBuilderBase.h"

//#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/SiPixelStub/interface/SiPixelStub.h"


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"



namespace cms
{

  class SiPixelStubProducer : public edm::EDProducer {
  public:
    //--- Constructor, virtual destructor (just in case)
    explicit SiPixelStubProducer(const edm::ParameterSet& conf);
    virtual ~SiPixelStubProducer();

    void setupStubBuilder();

    // Begin Job
    //virtual void beginJob( const edm::EventSetup& );
    virtual void beginJob( );

    //--- The top-level event method.
    virtual void produce(edm::Event& e, const edm::EventSetup& c);

    //--- Execute the algorithm(s).
    void run(const edm::DetSetVector<SiPixelCluster>   & input,
	     edm::ESHandle<TrackerGeometry>       & geom,
             edmNew::DetSetVector<SiPixelStub> & output);

  private:
    edm::ParameterSet _conf;
    std::string clusterMode_;               // user's choice of the clusterizer
    SiPixelStubBuilderBase * _stubBuilder;    // what we got (for now, one ptr to base class)
    bool readyToCluster_;                   // needed clusterizers valid => good to go!
    edm::InputTag src_;

    //! Optional limit on the total number of clusters
    int32_t maxTotalClusters_;
  };
}


#endif
