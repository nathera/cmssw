/*! \class   SiPixelStub
 *  \brief   C
 *  \details A
 *           the template structure of the class was maintained
 *           in order to accomodate any types other than PixelDigis
 *           in case there is such a need in the future.
 *           Adapred form L1Triggerdff;hljfd
 *
 *  \author Natalie Heracleous
 *  \date   2014, kfj
 *
 */

#ifndef SIPIXEL_STUB_FORMAT_H
#define SIPIXEL_STUB_FORMAT_H

#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
//Should perhaps move to include DetId, but SiPixelCluster uses directly uint32
#include "DataFormats/DetId/interface/DetId.h"

class SiPixelStub
{
  public:
    /// Constructors
    SiPixelStub();
    SiPixelStub( DetId aDetId );

    /// Destructor
    ~SiPixelStub();

    /// Data members:   getABC( ... )
    /// Helper methods: findABC( ... )

    /// Clusters composing the Stub
    std::vector< edm::Ref< edmNew::DetSetVector< SiPixelCluster >, SiPixelCluster > > getClusterRefs() const { return theClusterRefs; }
    const edm::Ref< edmNew::DetSetVector< SiPixelCluster >, SiPixelCluster >&         getClusterRef( unsigned int hitIdentifier ) const;
    void addClusterRef( edm::Ref< edmNew::DetSetVector< SiPixelCluster >, SiPixelCluster > aSiPixelCluster ) { theClusterRefs.push_back( aSiPixelCluster ); }

    /// Detector element
    DetId getDetId() const         { return theDetId; }
    void  setDetId( DetId aDetId ) { theDetId = aDetId; }

    /// Trigger information
    double getTriggerDisplacement() const;              /// In FULL-STRIP units! (hence, not implemented herein)
    void   setTriggerDisplacement( int aDisplacement ); /// In HALF-STRIP units!
    double getTriggerOffset() const;         /// In FULL-STRIP units! (hence, not implemented herein)
    void   setTriggerOffset( int anOffset ); /// In HALF-STRIP units!

    /// CBC3-style trigger information
    /// for sake of simplicity, these methods are
    /// slightly out of the getABC(...)/findABC(...) rule
    double getTriggerPosition() const; /// In FULL-STRIP units!
    double getTriggerBend() const;     /// In FULL-STRIP units!

    /// Information
    std::string print( unsigned int i = 0 ) const;

  private:
    /// Data members
    DetId theDetId;
    std::vector< edm::Ref< edmNew::DetSetVector< SiPixelCluster >, SiPixelCluster > >
        theClusterRefs;
    int theDisplacement;
    int theOffset;

}; /// Close class

/*! \brief   Implementation of methods
 *  \details Here, in the header file, the methods which do not depend
 *           on the specific type <T> that can fit the template.
 *           Other methods, with type-specific features, are implemented
 *           in the source file.
 */

/// Default Constructor

SiPixelStub::SiPixelStub()
{
  /// Set default data members
  theDetId = 0;
  theClusterRefs.clear();
  theDisplacement = 999999;
  theOffset = 0;
}

/// Another Constructor

SiPixelStub::SiPixelStub( DetId aDetId )
{
  /// Set data members
  this->setDetId( aDetId );

  /// Set default data members
  theClusterRefs.clear();
  theDisplacement = 999999;
  theOffset = 0;
}

/// Destructor

SiPixelStub::~SiPixelStub(){}

/// Get the Reference to a Cluster
const edm::Ref< edmNew::DetSetVector< SiPixelCluster >, SiPixelCluster >& SiPixelStub::getClusterRef( unsigned int hitIdentifier ) const
{
  /// Look for the SiPixelCluster with the stack member corresponding to the argument
  std::vector< edm::Ref< edmNew::DetSetVector< SiPixelCluster >, SiPixelCluster > >::const_iterator clusIter;
  for ( clusIter = theClusterRefs.begin();
        clusIter != theClusterRefs.end();
        ++clusIter )
  {
    
    //if ( (*clusIter)->getStackMember() == hitIdentifier )
    //FIXME test to see if it compiles. Replace here with proper function
    if(1)
    {
      return *clusIter;
    }
  }

  /// In case no SiPixelCluster is found, return a NULL edm::Ref
  /// (hopefully code doesn't reach this point)
  edm::Ref< edmNew::DetSetVector< SiPixelCluster >, SiPixelCluster >* tmpCluRef = new edm::Ref< edmNew::DetSetVector< SiPixelCluster >, SiPixelCluster >();
  return *tmpCluRef;
}

/// Trigger info
double SiPixelStub::getTriggerDisplacement() const { return 0.5*theDisplacement; }


void SiPixelStub::setTriggerDisplacement( int aDisplacement ) { theDisplacement = aDisplacement; }


double SiPixelStub::getTriggerOffset() const { return 0.5*theOffset; }


void SiPixelStub::setTriggerOffset( int anOffset ) { theOffset = anOffset; }

/// CBC3-style trigger info
double SiPixelStub::getTriggerPosition() const
{
  //FIXME SET TO 0 TO TEST
  // return this->getClusterRef(0)->findAverageLocalCoordinates().x();
  return 0.;
}


double SiPixelStub::getTriggerBend() const
{
  if ( theDisplacement == 999999 )
    return theDisplacement;

  return 0.5*( theDisplacement - theOffset );
}

/// Information

std::string SiPixelStub::print( unsigned int i ) const
{
  std::string padding("");
  for ( unsigned int j = 0; j != i; ++j )
  {
    padding+="\t";
  }

  std::stringstream output;
  output<<padding<<"SiPixelStub:\n";
  padding+='\t';
  output << padding << "DetId: " << theDetId.rawId() << ", position: " << this->getTriggerPosition();
  output << ", bend: " << this->getTriggerBend() << '\n';
  //  unsigned int iClu = 0; //FIXME
  std::vector< edm::Ref< edmNew::DetSetVector< SiPixelCluster >, SiPixelCluster > >::const_iterator clusIter;
  for ( clusIter = theClusterRefs.begin();
        clusIter!= theClusterRefs.end();
        ++clusIter )
  {
    //FIXME THIS DOES NOT WORK FOR SiPixelClusters since they don't have getStackMember or getHits functions
    // output << padding << "cluster: " << iClu++ << ", member: " << (*clusIter)->getStackMember() << ", address: " << (*clusIter).get();
    //output << ", cluster size: " << (*clusIter)->getHits().size() << '\n';
  }

  return output.str();
}


std::ostream& operator << ( std::ostream& os, const SiPixelStub& aSiPixelStub ) { return ( os << aSiPixelStub.print() ); }

#endif

