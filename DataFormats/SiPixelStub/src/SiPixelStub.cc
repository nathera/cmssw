#include "DataFormats/SiPixelStub/interface/SiPixelStub.h"


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

