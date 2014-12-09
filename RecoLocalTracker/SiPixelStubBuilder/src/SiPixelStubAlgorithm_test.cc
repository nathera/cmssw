/*! \brief   Implementation of methods of SiPixelStubAlgorithm_test
 *  \details Only test algorithm, does nothing
 *
 *  \author Natalie Heracleous
 *  \date   2014, Dec 07
 *
 */

#include "RecoLocalTracker/SiPixelStubBuilder/interface/SiPixelStubAlgorithm_test.h"

/// Matching operations

void SiPixelStubAlgorithm_test::PatternHitCorrelation( bool &aConfirmation,
                                                                       int &aDisplacement, 
                                                                       int &anOffset, 
                                                                       const SiPixelStub &aSiPixelStub ) const
{


  //Do nothing. match always and put dummy values

  aConfirmation = true;
  aDisplacement= 0;
  anOffset=0;


}

