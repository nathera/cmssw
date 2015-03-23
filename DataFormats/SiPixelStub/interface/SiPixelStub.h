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
//Should perhaps move to include DetId, but SiPixelCluster uses directly uint32
#include "DataFormats/DetId/interface/DetId.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/DetSetRefVector.h"

class SiPixelStub
{
  public:
    /// Constructors
    SiPixelStub();
    SiPixelStub( DetId aDetId );

    /// Destructor
    virtual ~SiPixelStub();

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

///FIXME IMPLEMENT COMPARISON OPERATORS PROPERLY
inline bool operator<( const SiPixelStub& one, const SiPixelStub& other) {
    if ( one.getTriggerOffset() < other.getTriggerOffset() ) {
        return true;
    } else if ( one.getTriggerOffset() > other.getTriggerOffset() ) {
        return false;
    } else if ( one.getTriggerOffset() < other.getTriggerOffset() ) {
        return true;
    } else {
        return false;
    }
}

typedef edm::DetSetVector<SiPixelStub> SiPixelStubCollection;
typedef edm::Ref<SiPixelStubCollection, SiPixelStub> SiPixelStubRef;
typedef edm::DetSetRefVector<SiPixelStub> SiPixelStubRefVector;
typedef edm::RefProd<SiPixelStubCollection> SiPixelStubRefProd;

typedef edmNew::DetSetVector<SiPixelStub> SiPixelStubCollectionNew;
typedef edm::Ref<SiPixelStubCollectionNew, SiPixelStub> SiPixelStubRefNew;
#endif
