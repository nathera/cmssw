//----------------------------------------------------------------------------
//! \class DummyStubBuilder
//! \brief A specific threshold-based pixel clustering algorithm
//!

//----------------------------------------------------------------------------

// Our own includes
#include "RecoLocalTracker/SiPixelStubBuilder/interface/DummyStubBuilder.h"
#include "RecoLocalTracker/SiPixelStubBuilder/interface/SiPixelArrayBuffer.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelGainCalibrationOffline.h"
// Geometry
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
//#include "Geometry/CommonTopologies/RectangularPixelTopology.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"

// STL
#include <stack>
#include <vector>
#include <iostream>
using namespace std;

//----------------------------------------------------------------------------
//! Constructor: 
//!  Initilize the buffer to hold pixels from a detector module.
//!  This is a vector of 44k ints, stays valid all the time.  
//----------------------------------------------------------------------------
DummyStubBuilder::DummyStubBuilder
  (edm::ParameterSet const& conf) :
    conf_(conf)//, bufferAlreadySet(false), theNumOfRows(0), theNumOfCols(0), detid_(0) 
{
/*
  // Get thresholds in electrons
  thePixelThreshold   = 
    conf_.getParameter<int>("ChannelThreshold");
  theSeedThreshold    = 
    conf_.getParameter<int>("SeedThreshold");
  theClusterThreshold = 
    conf_.getParameter<double>("ClusterThreshold");
  theConversionFactor = 
    conf_.getParameter<int>("VCaltoElectronGain");
  theOffset = 
    conf_.getParameter<int>("VCaltoElectronOffset");
  if ( conf_.exists("AdcFullScaleStack") ) theStackADC_=conf_.getParameter<int>("AdcFullScaleStack");
  else 
    theStackADC_=255;
  if ( conf_.exists("FirstStackLayer") ) theFirstStack_=conf_.getParameter<int>("FirstStackLayer");
  else
    theFirstStack_=5;
  
  // Get the constants for the miss-calibration studies
  doMissCalibrate=conf_.getUntrackedParameter<bool>("MissCalibrate",true); 
  doSplitClusters = conf.getParameter<bool>("SplitClusters");
  theBuffer.setSize( theNumOfRows, theNumOfCols );
*/
}
/////////////////////////////////////////////////////////////////////////////
DummyStubBuilder::~DummyStubBuilder() {}
/*
//----------------------------------------------------------------------------
//!  Prepare the Clusterizer to work on a particular DetUnit.  Re-init the
//!  size of the panel/plaquette (so update nrows and ncols), 
//----------------------------------------------------------------------------
bool DummyStubBuilder::setup(const PixelGeomDetUnit * pixDet) 
{
  // Cache the topology.
  const PixelTopology & topol = pixDet->specificTopology();
  
  // Get the new sizes.
  int nrows = topol.nrows();      // rows in x
  int ncols = topol.ncolumns();   // cols in y
  
  theNumOfRows = nrows;  // Set new sizes
  theNumOfCols = ncols;
  
  if ( nrows > theBuffer.rows() || 
       ncols > theBuffer.columns() ) 
    { // change only when a larger is needed
      //if( nrows != theNumOfRows || ncols != theNumOfCols ) {
      //cout << " DummyStubBuilder: pixel buffer redefined to " 
      // << nrows << " * " << ncols << endl;      
      //theNumOfRows = nrows;  // Set new sizes
      //theNumOfCols = ncols;
      // Resize the buffer
      theBuffer.setSize(nrows,ncols);  // Modify
      bufferAlreadySet = true;
    }
  
  return true;   
}
*/
//----------------------------------------------------------------------------
// ERICA::FIXME why here, and just here, I have an error if I put StackClusters????
std::vector< std::pair< StackGeomDet, std::vector<Phase2TrackerCluster1D> > > DummyStubBuilder::groupinginStackModules(const edmNew::DetSetVector<Phase2TrackerCluster1D>& clusters, const TrackerGeometry& geom, const TrackerTopology& topo ){

  if(clusters.empty())
    return std::vector< StackClusters >(); 

  std::vector< StackClusters > result;
  result.reserve( clusters.size() );

  edmNew::DetSetVector<Phase2TrackerCluster1D>::const_iterator DSViter;
  for( DSViter = clusters.begin() ; DSViter != clusters.end(); DSViter++){

    vector<Phase2TrackerCluster1D> clustersInStack;

    // get the detector unit's id
    unsigned int rawid(DSViter->detId());
    DetId detId(rawid);
    unsigned int layer(getLayerNumber(detId, &topo));
    unsigned int InnerModule(getModuleNumber(detId, &topo));

    // if the module is odd (inner), I will search for the +1 (outer)
    // if the clusters are just in one detector a STACK is not created.
    if(InnerModule%2 != 0){

      std::cout << "This module is odd(" << InnerModule << "):: check if exists the +1" << std::endl;
      edmNew::DetSetVector<Phase2TrackerCluster1D>::const_iterator DSViter2;
      for( DSViter2 = DSViter+1 ; DSViter2 != clusters.end(); DSViter2++){

        unsigned int rawid2(DSViter2->detId());
        DetId detId2(rawid2);
        unsigned int layer2(getLayerNumber(detId2, &topo));
        unsigned int OuterModule(getModuleNumber(detId2, &topo));
    
        //exclude if are the same, or with different layer
        if(rawid == rawid2) continue;
        if(layer != layer2) continue;
        if(OuterModule == InnerModule+1) {
          std::cout << "\t Module exists!" << std::endl;
          std::cout << "\t Stack created with DetIds: " << rawid << "," << rawid2 << std::endl;

          //creation of the stack: the plane is the inner one
          const GeomDetUnit* geomDetUnit(geom.idToDetUnit(detId));
          const GeomDetUnit* geomDetUnit2(geom.idToDetUnit(detId2));
          if (!geomDetUnit || !geomDetUnit2) break;
          const PixelGeomDetUnit* theGeomDet = dynamic_cast< const PixelGeomDetUnit* >(geomDetUnit); 
          Plane* sf = new Plane(theGeomDet->surface());

	  StackGeomDet stack = StackGeomDet(sf,geomDetUnit,geomDetUnit2);

          //run on both DSV to find all the clusters associated to the Stack
          edmNew::DetSet< Phase2TrackerCluster1D >::const_iterator clustIt;
          for ( clustIt = DSViter->begin(); clustIt != DSViter->end(); ++clustIt) {
            clustersInStack.push_back(*clustIt);
          }
          for ( clustIt = DSViter2->begin(); clustIt != DSViter2->end(); ++clustIt) {
            clustersInStack.push_back(*clustIt);
          }
          std::cout << "\t with " << clustersInStack.size() << " clusters associated." << std::endl;

//	  buildVectorHits();

          result.push_back(make_pair(stack,clustersInStack));
          break;
        }
      }

    }


  }

  std::cout << " ... Number of Stack created: " << result.size() << std::endl;
  return result;
}

//----------------------------------------------------------------------------
void DummyStubBuilder::buildDetUnit( const edm::DetSetVector<Phase2TrackerCluster1D> & input, 
                                     output_t& output)  {
  
  buildDetUnit_(input, output);  
  
  
}



