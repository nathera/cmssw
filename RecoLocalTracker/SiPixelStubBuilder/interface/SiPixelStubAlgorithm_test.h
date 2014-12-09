/*! \class   SiPixelStubAlgorithm_test
 *
 *  \author Natalie Heracleous
 *  \date   2014, Dec 04
 *
 */

#ifndef SI_PIXEL_STUB_ALGO_TEST_H
#define SI_PIXEL_STUB_ALGO_TEST_H


#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "RecoLocalTracker/SiPixelStubBuilder/interface/SiPixelStubAlgorithm.h"
#include "RecoLocalTracker/SiPixelStubBuilder/interface/SiPixelStubAlgorithmRecord.h"

#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementPoint.h"
#include "Geometry/CommonTopologies/interface/Topology.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"

#include <boost/shared_ptr.hpp>
#include <memory>
#include <string>
#include <map>
#include <typeinfo>

class SiPixelStubAlgorithm_test : public SiPixelStubAlgorithm
{
  private:
    /// Data members
    bool        mPerformZMatchingPS;
    bool        mPerformZMatching2S;
    std::string className_;

    std::vector< double >                barrelCut;
    std::vector< std::vector< double > > ringCut;

  public:
    /// Constructor
    SiPixelStubAlgorithm_test( const StackedTrackerGeometry *aStackedTracker,
                             std::vector< double > setBarrelCut,
                             std::vector< std::vector< double > > setRingCut,
                             bool aPerformZMatchingPS, bool aPerformZMatching2S )
      : SiPixelStubAlgorithm( aStackedTracker, __func__ )
    {
      barrelCut = setBarrelCut;
      ringCut = setRingCut;
      mPerformZMatchingPS = aPerformZMatchingPS;
      mPerformZMatching2S = aPerformZMatching2S;
    }

    /// Destructor
    ~SiPixelStubAlgorithm_test(){}

    /// Matching operations
    void PatternHitCorrelation( bool &aConfirmation,
                                int &aDisplacement,
                                int &anOffset,
                                const SiPixelStub &aSiPixelStub ) const;

}; /// Close class



/*! \class   ED_SiPixelStubAlgorithm_test
 *  \brief   Class to declare the algorithm to the framework
 *
 *  \author Nicola Pozzobon
 *  \date   2013, Jul 18
 *
 */


class ED_SiPixelStubAlgorithm_test : public edm::EDProducer
{
  private:
    /// Data members
    boost::shared_ptr< SiPixelStubAlgorithm > _theAlgo;

    /// Windows
    std::vector< double >                setBarrelCut;
    std::vector< std::vector< double > > setRingCut;

    /// Z-matching
    bool  mPerformZMatchingPS;
    bool  mPerformZMatching2S;

  public:
    /// Constructor
    ED_SiPixelStubAlgorithm_test( const edm::ParameterSet & p )
    {
      mPerformZMatchingPS =  p.getParameter< bool >("zMatchingPS");
      mPerformZMatching2S =  p.getParameter< bool >("zMatching2S");

      setBarrelCut = p.getParameter< std::vector< double > >("BarrelCut");

      std::vector< edm::ParameterSet > vPSet = p.getParameter< std::vector< edm::ParameterSet > >("EndcapCutSet");
      std::vector< edm::ParameterSet >::const_iterator iPSet;
      for ( iPSet = vPSet.begin(); iPSet != vPSet.end(); iPSet++ )
      {
        setRingCut.push_back( iPSet->getParameter< std::vector< double > >("EndcapCut") );
      }
      //setWhatProduced( this );
    }

    /// Destructor
    virtual ~ED_SiPixelStubAlgorithm_test(){}

    /// Implement the producer
    boost::shared_ptr< SiPixelStubAlgorithm > produce( const SiPixelStubAlgorithmRecord & record )
    { 
  
      SiPixelStubAlgorithm* SiPixelStubAlgo =0;

      _theAlgo = boost::shared_ptr< SiPixelStubAlgorithm >( SiPixelStubAlgo );
      return _theAlgo;
    } 

};

#endif

