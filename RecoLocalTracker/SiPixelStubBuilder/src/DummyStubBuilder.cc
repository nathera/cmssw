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
//----------------------------------------------------------------------------
//!  \brief Cluster pixels.
//!  This method operates on a matrix of pixels
//!  and finds the largest contiguous cluster around
//!  each seed pixel.
//!  Input and output data stored in DetSet
*/
//----------------------------------------------------------------------------
std::vector< std::pair< int, std::vector<Phase2TrackerCluster1D> > > DummyStubBuilder::groupinginStackModules(const edmNew::DetSetVector<Phase2TrackerCluster1D>& clusters, const TrackerTopology& topo){

  if(clusters.empty())
    return std::vector< std::pair< int, vector<Phase2TrackerCluster1D> > >(); 

  std::vector< std::pair< int, vector<Phase2TrackerCluster1D> > > result;
  result.reserve( clusters.size() );

  int numberOfDSV = 0;
  edmNew::DetSetVector<Phase2TrackerCluster1D>::const_iterator DSViter;
  for( DSViter = clusters.begin() ; DSViter != clusters.end(); DSViter++){
    ++numberOfDSV;
    // get the detector unit's id
    unsigned int rawid(DSViter->detId());
    DetId detId(rawid);
    unsigned int layer(getLayerNumber(detId, &topo));
    unsigned int module(getModuleNumber(detId, &topo));
    std::cout << rawid << std::endl;
    std::cout << layer << std::endl;
    std::cout << module << std::endl;
//      if(module%2 == 0){

  }

  std::cout << " ... Number of DSV in run: " << numberOfDSV << std::endl;
  return result;
}

//----------------------------------------------------------------------------
void DummyStubBuilder::buildDetUnit( const edm::DetSetVector<Phase2TrackerCluster1D> & input, 
                                     output_t& output)  {
  
  buildDetUnit_(input, output);  
  
  
}



