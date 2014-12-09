/*! \class   SiPixelStubAlgorithm
 *  \brief   Base class for any algorithm to be used
 *           in SiPixelStubBuilder
 *  \details Some comments here
 *           Adapted from L1Trigger/TrackTriger/TTStubAlgorithm.h *
 *  \author Natalie Heracleas
 *  \date   2014, Dec 07
 *
 */

#ifndef SI_PIXEL_STUB_ALGO_BASE_H
#define SI_PIXEL_STUB_ALGO_BASE_H

#include "MagneticField/Engine/interface/MagneticField.h"
#include "Geometry/TrackerGeometryBuilder/interface/StackedTrackerGeometry.h"
#include "DataFormats/SiPixelStub/interface/SiPixelStub.h"

#include <sstream>
#include <string>
#include <map>



class SiPixelStubAlgorithm
{
  protected:
    /// Data members
    const StackedTrackerGeometry *theStackedTracker;
    std::string                  className_;

  public:
    /// Constructors
    SiPixelStubAlgorithm( const StackedTrackerGeometry *aStackedTracker,
                     std::string fName )
      : theStackedTracker( aStackedTracker )
    {
      className_ = fName;
    }

    /// Destructor
    virtual ~SiPixelStubAlgorithm(){}

    /// Matching operations
    virtual void PatternHitCorrelation( bool &aConfirmation,
                                        int &aDisplacement,
                                        int &anOffset,
                                        const SiPixelStub &aSiPixelStub ) const {}

    /// Algorithm name
    virtual std::string AlgorithmName() const { return className_; }

}; /// Close class

#endif

