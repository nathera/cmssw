/** SiPixelStubBuilder.cc
 * ---------------------------------------------------------------
 * Natalie Heracleous, Erica Brondolin
 * ---------------------------------------------------------------
 */

// Our own stuff
#include "RecoLocalTracker/SiPixelStubBuilder/plugins/SiPixelStubBuilder.h"
#include "RecoLocalTracker/SiPixelStubBuilder/interface/DummyStubBuilder.h"

// Geometry
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"

// Data Formats
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/DetId/interface/DetId.h"

// Framework
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

// STL
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <iostream>

// MessageLogger
#include "FWCore/MessageLogger/interface/MessageLogger.h"

namespace cms
{

  //---------------------------------------------------------------------------
  //!  Constructor: set the ParameterSet and defer all thinking to setupStubBuilder().
  //---------------------------------------------------------------------------
  SiPixelStubBuilder::SiPixelStubBuilder(edm::ParameterSet const& conf) : 
    Conf_(conf),
    ClustersInputTag_( conf.getParameter<edm::InputTag>( "Clusters" ) ),
    StubBuilderAlgo_("None"),
    StubBuilder_(0),
    ReadyToBuild_(false)
//    maxTotalClusters_( conf.getParameter<int32_t>( "maxNumberOfClusters" ) )
  {
    //--- Declare to the EDM what kind of collections we will be making.
    produces< edmNew::DetSetVector< Phase2TrackerCluster1D > >( "ClusterAccepted" );
    produces< SiPixelStubCollectionNew >( "StubAccepted" );
    produces< SiPixelStubCollectionNew >( "StubRejected" );

    //--- Make the algorithm(s) according to what the user specified
    //--- in the ParameterSet.
    setupStubFindingAlgorithm();

  }

  // Destructor
  SiPixelStubBuilder::~SiPixelStubBuilder() { 
    delete StubBuilder_;
  }  

  //void SiPixelStubBuilder::beginJob( const edm::EventSetup& es ) 
  void SiPixelStubBuilder::beginJob( ) 
  {
    std::cout << "SiPixelStubBuilder::beginJob()" << std::endl;
  }
  
  //---------------------------------------------------------------------------
  //! The "Event" entrypoint: gets called by framework for every event
  //---------------------------------------------------------------------------
  void SiPixelStubBuilder::produce(edm::Event& event, const edm::EventSetup& eventSetup)
  {
    std::cout << "SiPixelStubBuilder::produce() begin" << std::endl;

    // get input clusters data
    edm::Handle< edmNew::DetSetVector<Phase2TrackerCluster1D> >  ClustersHandle;
    event.getByLabel( ClustersInputTag_, ClustersHandle);

    // create the final output collection
    std::auto_ptr< edmNew::DetSetVector< Phase2TrackerCluster1D > > outputClusterAccept( new edmNew::DetSetVector< Phase2TrackerCluster1D > );
    std::auto_ptr< SiPixelStubCollectionNew > outputStubsAccepted( new SiPixelStubCollectionNew() );
    std::auto_ptr< SiPixelStubCollectionNew > outputStubsRejected( new SiPixelStubCollectionNew() );

    // get the geometry
    edm::ESHandle< TrackerGeometry > geomHandle;
    eventSetup.get< TrackerDigiGeometryRecord >().get( geomHandle );
    const TrackerGeometry* tkGeom = &(*geomHandle);    

    edm::ESHandle< TrackerTopology > tTopoHandle;
    eventSetup.get< IdealGeometryRecord >().get(tTopoHandle);
    const TrackerTopology* tkTopo = tTopoHandle.product();

    // check on the input clusters
    check( *ClustersHandle, *tkGeom, *tkTopo);

    // running the stub building algorithm
    run( *ClustersHandle, *tkGeom, *tkTopo, *outputClusterAccept);//, *outputStubsAccepted, *outputStubsRejected );

    // ERICA::check::Are the output clusters empty?
   // *outputClusterAccept = *ClustersHandle;
   // int numberOfDetUnits_produced = 0;
   // edmNew::DetSetVector<Phase2TrackerCluster1D>::const_iterator ClusterIter_new;
   // for( ClusterIter_new = (*outputClusterAccept).begin() ; ClusterIter_new != (*outputClusterAccept).end(); ClusterIter_new++) {
   //   ++numberOfDetUnits_produced;
   // }

   // std::cout << " ... Number of clusters produced: " << numberOfDetUnits_produced << std::endl;

    // write output to file
    event.put( outputClusterAccept, "ClusterAccepted" );
    event.put( outputStubsAccepted, "StubAccepted" );
    event.put( outputStubsRejected, "StubRejected" );

    std::cout << "SiPixelStubBuilder::produce() end" << std::endl;

  }

  //---------------------------------------------------------------------------
  //  Set up the specific algorithm we are going to use.  
  //---------------------------------------------------------------------------
  void SiPixelStubBuilder::setupStubFindingAlgorithm()  {
    StubBuilderAlgo_ = Conf_.getUntrackedParameter<std::string>("AlgorithmName","DummyStubBuilder");

    if ( StubBuilderAlgo_ == "DummyStubBuilder" ) {
      StubBuilder_ = new DummyStubBuilder(Conf_);
      ReadyToBuild_ = true;
    } 
    else {
      std::cout << " Choice " << StubBuilderAlgo_ << " is invalid.\n" ;
      ReadyToBuild_ = false;
    }

  }

  //---------------------------------------------------------------------------
  void SiPixelStubBuilder::run(const edmNew::DetSetVector<Phase2TrackerCluster1D>& clusters,
 	     const TrackerGeometry& geom,
             const TrackerTopology& topo,
             edmNew::DetSetVector<Phase2TrackerCluster1D> output ){

    if ( ! ReadyToBuild_ ) {
      std::cout << " No stub builder algorithm was found - cannot run!" ;
      return;
    }

    std::vector< std::pair< StackGeomDet, std::vector<Phase2TrackerCluster1D> > > groupClusterBySM;    
    groupClusterBySM = StubBuilder_->groupinginStackModules(clusters, geom, topo);

/*
      // Produce stubs for this DetUnit and store them in a DetSetVector
      edmNew::DetSetVector<SiPixelCluster>::FastFiller spc(output, DSViter->detId());
      _stubBuilder->clusterizeDetUnit(*DSViter, pixDet, badChannels, spc);
      if ( spc.empty() ) {
        spc.abort();
      } else {
	numberOfClusters += spc.size();
      }
*/

/*
 

      if ((maxTotalClusters_ >= 0) && (numberOfClusters > maxTotalClusters_)) {
        edm::LogError("TooManyClusters") <<  "Limit on the number of clusters exceeded. An empty cluster collection will be produced instead.\n";
        edmNew::DetSetVector<SiPixelCluster> empty;
        empty.swap(output);
        break;
      }
    } 
// end of DetUnit loop
    
    //LogDebug ("SiPixelStubBuilder") << " Executing " 
    //      << StubBuilderAlgo_ << " resulted in " << numberOfClusters
    //				    << " SiPixelClusters in " << numberOfDetUnits << " DetUnits."; 
*/
  }

  //---------------------------------------------------------------------------
  void SiPixelStubBuilder::check(const edmNew::DetSetVector<Phase2TrackerCluster1D>& clusters,
             const TrackerGeometry& geom,
             const TrackerTopology& topo){
  
    int nCluster = 0;
    int numberOfDSV = 0;
    edmNew::DetSetVector<Phase2TrackerCluster1D>::const_iterator DSViter;
    for( DSViter = clusters.begin() ; DSViter != clusters.end(); DSViter++){

      ++numberOfDSV;

      // get the detector unit's id
      unsigned int rawid(DSViter->detId());
      DetId detId(rawid);
      unsigned int layer(StubBuilder_->getLayerNumber(detId, &topo));
      unsigned int module(StubBuilder_->getModuleNumber(detId, &topo));
      
      // get the geom of the tracker
      const GeomDetUnit* geomDetUnit(geom.idToDetUnit(detId));
      const PixelGeomDetUnit* theGeomDet = dynamic_cast< const PixelGeomDetUnit* >(geomDetUnit);
      const PixelTopology& topol = theGeomDet->specificTopology();

      std::cout << "Layer:" << layer << std::endl;
      if(topol.ncolumns() == 32) std::cout << "Pixel cluster with detId:" << rawid << "(module:" << module
					   << ") in DetSet#" << numberOfDSV << std::endl;
      else if(topol.ncolumns() == 2 ) std::cout << "Strip cluster with detId " << rawid << "(module:" << module
					   << ") in DetSet#" << numberOfDSV << std::endl;
      else std::cout << "no module?!" << std::endl;

      if (!geomDetUnit) break;

      // Loop over the clusters in the detector unit
      for (edmNew::DetSet< Phase2TrackerCluster1D >::const_iterator clustIt = DSViter->begin(); clustIt != DSViter->end(); ++clustIt) {

        nCluster++;

        MeasurementPoint mpClu(clustIt->center(), clustIt->column() + 0.5);
        Local3DPoint localPosClu = geomDetUnit->topology().localPosition(mpClu);
        Global3DPoint globalPosClu = geomDetUnit->surface().toGlobal(localPosClu);

        std::cout << "\t local  pos " << localPosClu << std::endl;
        std::cout << "\t global pos " << globalPosClu << std::endl;

      }
    }
    std::cout << " Number of input clusters: " << nCluster << std::endl;

  }
/*  unsigned int SiPixelStubBuilder::getLayerNumber(const DetId& detid, const TrackerTopology* topo) {
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


  unsigned int SiPixelStubBuilder::getModuleNumber(const DetId& detid, const TrackerTopology* topo) {
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
*/
}  // end of namespace cms
