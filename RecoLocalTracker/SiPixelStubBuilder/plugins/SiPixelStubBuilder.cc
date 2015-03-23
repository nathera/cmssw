/** SiPixelStubBuilder.cc
 * ---------------------------------------------------------------
 * Natalie Heracleous modified from SiPixelClusterBuilder 
 * ---------------------------------------------------------------
 */

// Our own stuff
#include "RecoLocalTracker/SiPixelStubBuilder/plugins/SiPixelStubBuilder.h"
#include "RecoLocalTracker/SiPixelStubBuilder/interface/DummyStubBuilder.h"

// Geometry
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"

// Data Formats
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/DetId/interface/DetId.h"


// Framework
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

// STL
#include <vector>
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
  SiPixelStubBuilder::SiPixelStubBuilder(edm::ParameterSet const& conf) 
    : 
    _conf(conf)
//    clusterMode_("None"),     // bogus
//    _stubBuilder(0),          // the default, in case we fail to make one
//    readyToCluster_(false),   // since we obviously aren't
//    src_( conf.getParameter<edm::InputTag>( "src" ) ),
//    maxTotalClusters_( conf.getParameter<int32_t>( "maxNumberOfClusters" ) )
  {
    //--- Declare to the EDM what kind of collections we will be making.
    produces<SiPixelStubCollectionNew>(); 

//    std::string payloadType = conf.getParameter<std::string>( "payloadType" );



    //--- Make the algorithm(s) according to what the user specified
    //--- in the ParameterSet.
//    setupStubBuilder();

  }

  // Destructor
  SiPixelStubBuilder::~SiPixelStubBuilder() { 
//    delete _stubBuilder;
  }  

  //void SiPixelStubBuilder::beginJob( const edm::EventSetup& es ) 
  void SiPixelStubBuilder::beginJob( ) 
  {
    std::cout << "SiPixelStubBuilder::beginJob( )";
  }
  
  //---------------------------------------------------------------------------
  //! The "Event" entrypoint: gets called by framework for every event
  //---------------------------------------------------------------------------
  void SiPixelStubBuilder::produce(edm::Event& e, const edm::EventSetup& es)
  {
    std::cout << "SiPixelStubBuilder::produce( )";
/*

   // Step A.1: get input data
    //edm::Handle<PixelDigiCollection> pixDigis;
    edm::Handle< edm::DetSetVector<SiPixelCluster> >  input;
    e.getByLabel( src_, input);

    // Step A.2: get event setup
    edm::ESHandle<TrackerGeometry> geom;
    es.get<TrackerDigiGeometryRecord>().get( geom );

    // Step B: create the final output collection
    std::auto_ptr<SiPixelStubCollectionNew> output( new SiPixelStubCollectionNew() );
    //FIXME: put a reserve() here

    // Step C: Iterate over DetIds and invoke the pixel clusterizer algorithm
    // on each DetUnit
    run(*input, geom, *output );

    // Step D: write output to file
    e.put( output );

  }

  //---------------------------------------------------------------------------
  //!  Set up the specific algorithm we are going to use.  
  //!  TO DO: in the future, we should allow for a different algorithm for 
  //!  each detector subset (e.g. barrel vs forward, per layer, etc).
  //---------------------------------------------------------------------------
  void SiPixelStubBuilder::setupStubBuilder()  {
    clusterMode_ = 
    _conf.getUntrackedParameter<std::string>("ClusterMode","DummyStubBuilder");

    if ( clusterMode_ == "DummyStubBuilder" ) {
        _stubBuilder = new DummyStubBuilder(_conf);
      readyToCluster_ = true;
    } 
    else {
      edm::LogError("SiPixelStubBuilder") << "[SiPixelStubBuilder]:"
		<<" choice " << clusterMode_ << " is invalid.\n"
		<< "Possible choices:\n" 
        << "    DummyStubBuilder";
      readyToCluster_ = false;
    }
  }

  //---------------------------------------------------------------------------
  //!  Iterate over DetUnits, and invoke the PixelClusterizer on each.
  //---------------------------------------------------------------------------
  void SiPixelStubBuilder::run(const edm::DetSetVector<SiPixelCluster>   & input, 
				   edm::ESHandle<TrackerGeometry>       & geom,
                                   edmNew::DetSetVector<SiPixelStub> & output) {
    if ( ! readyToCluster_ ) {
      edm::LogError("SiPixelStubBuilder")
		<<" at least one clusterizer is not ready -- can't run!" ;
      // TO DO: throw an exception here?  The user may want to know...
      return;   // clusterizer is invalid, bail out
    }

//     int numberOfDetUnits = 0;
//     int numberOfClusters = 0;
*/ 
    // Iterate on detector units
/*    edm::DetSetVector<PixelDigi>::const_iterator DSViter = input.begin();
    for( ; DSViter != input.end(); DSViter++) {
      ++numberOfDetUnits;

      //  LogDebug takes very long time, get rid off.
      //LogDebug("SiStripClusterizer") << "[SiPixelStubBuilder::run] DetID" << DSViter->id;

      std::vector<short> badChannels; 
      DetId detIdObject(DSViter->detId());
      
      // Comment: At the moment the clusterizer depends on geometry
      // to access information as the pixel topology (number of columns
      // and rows in a detector module). 
      // In the future the geometry service will be replaced with
      // a ES service.
      const GeomDetUnit      * geoUnit = geom->idToDetUnit( detIdObject );
      const PixelGeomDetUnit * pixDet  = dynamic_cast<const PixelGeomDetUnit*>(geoUnit);
      if (! pixDet) {
	// Fatal error!  TO DO: throw an exception!
	assert(0);
      }
      // Produce clusters for this DetUnit and store them in 
      // a DetSet
      edmNew::DetSetVector<SiPixelCluster>::FastFiller spc(output, DSViter->detId());
      _stubBuilder->clusterizeDetUnit(*DSViter, pixDet, badChannels, spc);
      if ( spc.empty() ) {
        spc.abort();
      } else {
	numberOfClusters += spc.size();
      }

      if ((maxTotalClusters_ >= 0) && (numberOfClusters > maxTotalClusters_)) {
        edm::LogError("TooManyClusters") <<  "Limit on the number of clusters exceeded. An empty cluster collection will be produced instead.\n";
        edmNew::DetSetVector<SiPixelCluster> empty;
        empty.swap(output);
        break;
      }
    }*/ 
// end of DetUnit loop
    
    //LogDebug ("SiPixelStubBuilder") << " Executing " 
    //      << clusterMode_ << " resulted in " << numberOfClusters
    //				    << " SiPixelClusters in " << numberOfDetUnits << " DetUnits."; 
  }

}  // end of namespace cms
