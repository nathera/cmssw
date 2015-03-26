#ifndef RecoLocalTracker_SiStubBuilder_DummyStubBuilder_H
#define RecoLocalTracker_SiStubBuilder_DummyStubBuilder_H

//-----------------------------------------------------------------------
//! \class DummyStubBuilder
//! \brief An dummy stub building algorithm
//!

//-----------------------------------------------------------------------

// Base class, defines SiPixelDigi and SiPixelCluster.  The latter includes
// Pixel, PixelPos and Shift as inner classes.
//
#include "DataFormats/Common/interface/DetSetVector.h"
#include "RecoLocalTracker/SiPixelStubBuilder/interface/SiPixelStubBuilderBase.h"

// The private pixel buffer
#include "RecoLocalTracker/SiPixelStubBuilder/interface/SiPixelArrayBuffer.h"

// Parameter Set:
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>


class DummyStubBuilder : public SiPixelStubBuilderBase {
 public:

  DummyStubBuilder(edm::ParameterSet const& conf);
  ~DummyStubBuilder();

  // Full I/O in DetSet
  void buildDetUnit( const edm::DetSet<Phase2TrackerCluster1D> & input, 
                     output_t& output);
//  void build( const edm::DetSet<Phase2TrackerCluster1D> & input, 
//                     output_t::FastFiller& output);

  
 private:


 template<class T> void buildDetUnit_(const T& input, output_t& output) {
   for(typename T::const_iterator it = input.begin(); it!=input.end(); it++) {
     output_t::FastFiller ff(output, it->detId()); 
//     build(*it, ff);   
     if(ff.empty()) ff.abort();    
   }   
 }

  edm::ParameterSet conf_;
/*
  //! Data storage
  SiPixelArrayBuffer               theBuffer;         // internal nrow * ncol matrix
  bool                             bufferAlreadySet;  // status of the buffer array
  std::vector<SiPixelCluster::PixelPos>  theSeeds;          // cached seed pixels
  std::vector<SiPixelCluster>            theClusters;       // resulting clusters  
  
  //! Clustering-related quantities:
  float thePixelThresholdInNoiseUnits;    // Pixel threshold in units of noise
  float theSeedThresholdInNoiseUnits;     // Pixel cluster seed in units of noise
  float theClusterThresholdInNoiseUnits;  // Cluster threshold in units of noise

  int   thePixelThreshold;  // Pixel threshold in electrons
  int   theSeedThreshold;   // Seed threshold in electrons 
  float theClusterThreshold;  // Cluster threshold in electrons
  int   theConversionFactor;  // adc to electron conversion factor
  int   theOffset;            // adc to electron conversion offset

  //! Geometry-related information
  int  theNumOfRows;
  int  theNumOfCols;
  uint32_t detid_;
  bool dead_flag;
  bool doMissCalibrate; // Use calibration or not
  bool doSplitClusters;
  //! Private helper methods:
  bool setup(const PixelGeomDetUnit * pixDet);

  // Calibrate the ADC charge to electrons 
  int   theStackADC_;          // The maximum ADC count for the stack layers
  int   theFirstStack_;        // The index of the first stack layer
*/

};

#endif
