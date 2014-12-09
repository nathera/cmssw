/*! \class SiPixelStubBuilder
* \brief Plugin to load the Stub finding algorithm and produce the
* collection of Stubs that goes in the event content.
* \details After moving from SimDataFormats to DataFormats,
* the template structure of the class was maintained
* in order to accomodate any types other than PixelDigis
* in case there is such a need in the future.
*
* \author Andrew W. Rose
* \author Nicola Pozzobon
* \author Ivan Reid
* \date 2013, Jul 18
*
*/

#ifndef SI_PIXEL_STUB_BUILDER_H
#define SI_PIXEL_STUB_BUILDER_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"

#include "RecoLocalTracker/SiPixelStubBuilder/interface/SiPixelStubAlgorithm.h"
#include "RecoLocalTracker/SiPixelStubBuilder/interface/SiPixelStubAlgorithmRecord.h"

#include "DataFormats/Common/interface/DetSetVectorNew.h"

#include <memory>
#include <map>
#include <vector>


class SiPixelStubBuilder : public edm::EDProducer
{
  public:
    /// Constructor
    explicit SiPixelStubBuilder( const edm::ParameterSet& iConfig );

    /// Destructor;
    ~SiPixelStubBuilder();

  private:
    /// Data members
    const StackedTrackerGeometry *theStackedTracker;
    edm::ESHandle< SiPixelStubAlgorithm > theStubFindingAlgoHandle;
    edm::InputTag SiPixelClustersInputTag;

    /// Mandatory methods
    virtual void beginRun( const edm::Run& run, const edm::EventSetup& iSetup );
    virtual void endRun( const edm::Run& run, const edm::EventSetup& iSetup );
    virtual void produce( edm::Event& iEvent, const edm::EventSetup& iSetup );

    /// Sorting method for stubs
    /// NOTE: this must be static!
    static bool SortStubBendPairs( const std::pair< unsigned int, double >& left, const std::pair< unsigned int, double >& right );

}; /// Close class

/*! \brief Implementation of methods
* \details Here, in the header file, the methods which do not depend
* on the specific type <T> that can fit the template.
* Other methods, with type-specific features, are implemented
* in the source file.
*/

/// Constructors

SiPixelStubBuilder::SiPixelStubBuilder( const edm::ParameterSet& iConfig )
{
  SiPixelClustersInputTag = iConfig.getParameter< edm::InputTag >( "SiPixelClusters" );
  produces< edmNew::DetSetVector< SiPixelCluster > >( "ClusterAccepted" );
  produces< edmNew::DetSetVector< SiPixelStub > >( "StubAccepted" );
  produces< edmNew::DetSetVector< SiPixelStub > >( "StubRejected" );
}

/// Destructor

SiPixelStubBuilder::~SiPixelStubBuilder(){}

/// Begin run

void SiPixelStubBuilder::beginRun( const edm::Run& run, const edm::EventSetup& iSetup )
{
  /// Get the geometry references
  edm::ESHandle< StackedTrackerGeometry > StackedTrackerGeomHandle;
  iSetup.get< StackedTrackerGeometryRecord >().get( StackedTrackerGeomHandle );
  theStackedTracker = StackedTrackerGeomHandle.product();

  /// Get the stub finding algorithm
  iSetup.get< SiPixelStubAlgorithmRecord >().get( theStubFindingAlgoHandle );

  /// Print some information when loaded
  std::cout << std::endl;
  std::cout << "SiPixelStubBuilder: loaded modules:"
            << "\n\tSiPixelStubAlgorithm:\t" << theStubFindingAlgoHandle->AlgorithmName()
            << std::endl;
  std::cout << std::endl;
}

/// End run

void SiPixelStubBuilder::endRun( const edm::Run& run, const edm::EventSetup& iSetup ){}

/// Implement the producer

void SiPixelStubBuilder::produce( edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  /// Prepare output
  std::auto_ptr< edmNew::DetSetVector< SiPixelCluster > > SiPixelClusterDSVForOutput( new edmNew::DetSetVector< SiPixelCluster > );
  std::auto_ptr< edmNew::DetSetVector< SiPixelStub > > SiPixelStubDSVForOutputTemp( new edmNew::DetSetVector< SiPixelStub > );
  std::auto_ptr< edmNew::DetSetVector< SiPixelStub > > SiPixelStubDSVForOutputAccepted( new edmNew::DetSetVector< SiPixelStub > );
  std::auto_ptr< edmNew::DetSetVector< SiPixelStub > > SiPixelStubDSVForOutputRejected( new edmNew::DetSetVector< SiPixelStub > );

  /// Get the Clusters already stored away
  edm::Handle< edmNew::DetSetVector< SiPixelCluster > > SiPixelClusterHandle;
  iEvent.getByLabel( SiPixelClustersInputTag, SiPixelClusterHandle );

  /// Get the maximum number of stubs per ROC
  /// (CBC3-style)
  unsigned maxStubs = theStackedTracker->getCBC3MaxStubs();

  /// Loop over the detector elements
  StackedTrackerGeometry::StackContainerIterator StackedTrackerIterator;
  for ( StackedTrackerIterator = theStackedTracker->stacks().begin();
        StackedTrackerIterator != theStackedTracker->stacks().end();
        ++StackedTrackerIterator )
  {
    StackedTrackerDetUnit* Unit = *StackedTrackerIterator;
    StackedTrackerDetId Id = Unit->Id();
    assert(Unit == theStackedTracker->idToStack(Id));
    
    /// Get the DetIds of each sensor
    DetId id0 = Unit->stackMember(0);
    DetId id1 = Unit->stackMember(1);

    /// Check that everything is ok in the maps
    if ( theStackedTracker->findPairedDetector( id0 ) != id1 ||
         theStackedTracker->findPairedDetector( id1 ) != id0 )
    {
      std::cerr << "A L E R T! error in detector association within Pt module (detector-to-detector)" << std::endl;
      continue;
    }

    if ( theStackedTracker->findStackFromDetector( id0 ) != Id ||
         theStackedTracker->findStackFromDetector( id1 ) != Id )
    {
      std::cerr << "A L E R T! error in detector association within Pt module (detector-to-module)" << std::endl;
      continue;
    }

    /// Go on only if both detectors have Clusters
    if ( SiPixelClusterHandle->find( id0 ) == SiPixelClusterHandle->end() ||
         SiPixelClusterHandle->find( id1 ) == SiPixelClusterHandle->end() )
      continue;

    /// Get the DetSets of the Clusters
    edmNew::DetSet< SiPixelCluster > innerClusters = (*SiPixelClusterHandle)[ id0 ];
    edmNew::DetSet< SiPixelCluster > outerClusters = (*SiPixelClusterHandle)[ id1 ];

    typename edmNew::DetSet< SiPixelCluster >::iterator innerClusterIter, outerClusterIter;

    /// If there are Clusters in both sensors
    /// you can try and make a Stub
    /// This is ~redundant
    if ( innerClusters.size() == 0 || outerClusters.size() == 0 )
      continue;

/* IR 2014 04 20
 * from pointer to object to deallocate memory in the correct way
*/
    /// Create the vectors of objects to be passed to the FastFillers
    std::vector< SiPixelCluster > tempInner; // = new std::vector< SiPixelCluster >();
    std::vector< SiPixelCluster > tempOuter; // = new std::vector< SiPixelCluster >();
    std::vector< SiPixelStub >   tempOutput; // = new std::vector< SiPixelStub >();
    //std::vector< SiPixelStub > tempRejected; // = new std::vector< SiPixelStub >();
    tempInner.clear();
    tempOuter.clear();
    tempOutput.clear();
    //tempRejected.clear();

    /// Get chip size information
    const GeomDetUnit* det0 = theStackedTracker->idToDetUnit( Id, 0 );
    const PixelGeomDetUnit* pix0 = dynamic_cast< const PixelGeomDetUnit* >( det0 );
    const PixelTopology* top0 = dynamic_cast< const PixelTopology* >( &(pix0->specificTopology()) );
    const int chipSize = 2 * top0->rowsperroc(); /// Need to find ASIC size in half-strip units
    std::map< int, std::vector< SiPixelStub > > moduleStubs; /// Temporary storage for stubs before max check

    /// Loop over pairs of Clusters
    for ( innerClusterIter = innerClusters.begin();
          innerClusterIter != innerClusters.end();
          ++innerClusterIter )
    {
      for ( outerClusterIter = outerClusters.begin();
            outerClusterIter != outerClusters.end();
            ++outerClusterIter )
      {
        /// Build a temporary Stub
        SiPixelStub tempSiPixelStub( Id );
        tempSiPixelStub.addClusterRef( edmNew::makeRefTo( SiPixelClusterHandle, innerClusterIter ) );
        tempSiPixelStub.addClusterRef( edmNew::makeRefTo( SiPixelClusterHandle, outerClusterIter ) );

        /// Check for compatibility
        bool thisConfirmation = false;
        int thisDisplacement = 999999;
        int thisOffset = 0;

        theStubFindingAlgoHandle->PatternHitCorrelation( thisConfirmation, thisDisplacement, thisOffset, tempSiPixelStub );

        /// If the Stub is above threshold
        if ( thisConfirmation )
        {
          tempSiPixelStub.setTriggerDisplacement( thisDisplacement );
          tempSiPixelStub.setTriggerOffset( thisOffset );

          /// Put in the output
          if ( maxStubs == 0 )
          {
            /// This means that ALL stubs go into the output
            tempInner.push_back( *innerClusterIter );
            tempOuter.push_back( *outerClusterIter );
            tempOutput.push_back( tempSiPixelStub );
          }
          else
          {
            /// This means that only some of them do
            /// Put in the temporary output
            int chip = tempSiPixelStub.getTriggerPosition() / chipSize; /// Find out which ASIC
            if ( moduleStubs.find( chip ) == moduleStubs.end() ) /// Already a stub for this ASIC?
            {
              /// No, so new entry
              std::vector< SiPixelStub > tempStubs;
              tempStubs.clear();
              tempStubs.push_back( tempSiPixelStub );
              moduleStubs.insert( std::pair< int, std::vector< SiPixelStub > >( chip, tempStubs ) );
            }
            else
            {
              /// Already existing entry
              moduleStubs[chip].push_back( tempSiPixelStub );
            }
          }
        } /// Stub accepted
/* NP 2014 02 25
 * this is commented to avoid memory exhaustion in hi PU events
        else
        {
          tempRejected->push_back( tempSiPixelStub );
        } /// Stub rejected
*/
      } /// End of nested loop
    } /// End of loop over pairs of Clusters

    /// If we are working with max no. stub/ROC, then clean the temporary output
    /// and store only the selected stubs
    if ( moduleStubs.empty() == false )
    {
      /// Loop over ROC's
      /// the ROC ID is not important
      for ( auto const & is : moduleStubs )
      {
        /// Put the stubs into the output
        if ( is.second.size() <= maxStubs )
        {
          for ( auto const & ts: is.second )
          {
            tempInner.push_back( *(ts.getClusterRef(0)) );
            tempOuter.push_back( *(ts.getClusterRef(1)) );
            tempOutput.push_back( ts );
          }
        }
        else
        {
          /// Sort them and pick up only the first N.
          std::vector< std::pair< unsigned int, double > > bendMap;
          for ( unsigned int i = 0; i < is.second.size(); ++i )
          {
            bendMap.push_back( std::pair< unsigned int, double >( i, is.second[i].getTriggerBend() ) );
          }
          std::sort( bendMap.begin(), bendMap.end(), SiPixelStubBuilder::SortStubBendPairs );

          for ( unsigned int i = 0; i < maxStubs; ++i )
          {
            /// Put the highest momenta (lowest bend) stubs into the event
            tempInner.push_back( *(is.second[bendMap[i].first].getClusterRef(0)) );
            tempOuter.push_back( *(is.second[bendMap[i].first].getClusterRef(1)) );
            tempOutput.push_back( is.second[bendMap[i].first] );
          }
/* NP 2014 02 25
 * this is commented to avoid memory exhaustion in hi PU events
          for ( unsigned int i = maxStubs; i < is.second.size(); ++i )
          {
            /// Reject the rest
            tempRejected->push_back( is.second[bendMap[i].first] );
          }
*/
        }
      } /// End of loop over temp output
    } /// End store only the selected stubs if max no. stub/ROC is set

    /// Create the FastFillers
    if ( tempInner.size() > 0 )
    {
      typename edmNew::DetSetVector< SiPixelCluster >::FastFiller innerOutputFiller( *SiPixelClusterDSVForOutput, id0 );
      for ( unsigned int m = 0; m < tempInner.size(); m++ )
      {
        innerOutputFiller.push_back( tempInner.at(m) );
      }
      if ( innerOutputFiller.empty() )
        innerOutputFiller.abort();
    }

    if ( tempOuter.size() > 0 )
    {
      typename edmNew::DetSetVector< SiPixelCluster >::FastFiller outerOutputFiller( *SiPixelClusterDSVForOutput, id1 );
      for ( unsigned int m = 0; m < tempOuter.size(); m++ )
      {
        outerOutputFiller.push_back( tempOuter.at(m) );
      }
      if ( outerOutputFiller.empty() )
        outerOutputFiller.abort();
    }

    if ( tempOutput.size() > 0 )
    {
      typename edmNew::DetSetVector< SiPixelStub >::FastFiller tempOutputFiller( *SiPixelStubDSVForOutputTemp, DetId(Id.rawId()) );
      for ( unsigned int m = 0; m < tempOutput.size(); m++ )
      {
        tempOutputFiller.push_back( tempOutput.at(m) );
      }
      if ( tempOutputFiller.empty() )
        tempOutputFiller.abort();
    }

/* NP 2014 02 25
 * this is commented to avoid memory exhaustion in hi PU events
    if ( tempRejected->size() > 0 )
    {
      typename edmNew::DetSetVector< SiPixelStub >::FastFiller rejectedOutputFiller( *SiPixelStubDSVForOutputRejected, DetId(Id.rawId()) );
      for ( unsigned int m = 0; m < tempRejected->size(); m++ )
      {
        rejectedOutputFiller.push_back( tempRejected->at(m) );
      }
      if ( rejectedOutputFiller.empty() )
      rejectedOutputFiller.abort();
    }
*/

  } /// End of loop over detector elements

  /// Put output in the event (1)
  /// Get also the OrphanHandle of the accepted clusters
  edm::OrphanHandle< edmNew::DetSetVector< SiPixelCluster > > SiPixelClusterAcceptedHandle = iEvent.put( SiPixelClusterDSVForOutput, "ClusterAccepted" );

  /// Now, correctly reset the output
  typename edmNew::DetSetVector< SiPixelStub >::const_iterator stubDetIter;

  for ( stubDetIter = SiPixelStubDSVForOutputTemp->begin();
        stubDetIter != SiPixelStubDSVForOutputTemp->end();
        ++stubDetIter )
  {
    /// Get the DetId and prepare the FastFiller
    DetId thisStackedDetId = stubDetIter->id();
    typename edmNew::DetSetVector< SiPixelStub >::FastFiller acceptedOutputFiller( *SiPixelStubDSVForOutputAccepted, thisStackedDetId );

    /// Get its DetUnit
    const StackedTrackerDetUnit* thisUnit = theStackedTracker->idToStack( thisStackedDetId );
    DetId id0 = thisUnit->stackMember(0);
    DetId id1 = thisUnit->stackMember(1);

    /// Check that everything is ok in the maps
    /// Redundant up to (*)
    if ( theStackedTracker->findPairedDetector( id0 ) != id1 ||
         theStackedTracker->findPairedDetector( id1 ) != id0 )
    {
      std::cerr << "A L E R T! error in detector association within Pt module (detector-to-detector)" << std::endl;
      continue;
    }

    if ( theStackedTracker->findStackFromDetector( id0 ) != thisStackedDetId ||
         theStackedTracker->findStackFromDetector( id1 ) != thisStackedDetId )
    {
      std::cerr << "A L E R T! error in detector association within Pt module (detector-to-module)" << std::endl;
      continue;
    }

    /// Go on only if both detectors have clusters
    if ( SiPixelClusterAcceptedHandle->find( id0 ) == SiPixelClusterAcceptedHandle->end() ||
         SiPixelClusterAcceptedHandle->find( id1 ) == SiPixelClusterAcceptedHandle->end() )
      continue;

    /// (*)

    /// Get the DetSets of the clusters
    edmNew::DetSet< SiPixelCluster > innerClusters = (*SiPixelClusterAcceptedHandle)[ id0 ];
    edmNew::DetSet< SiPixelCluster > outerClusters = (*SiPixelClusterAcceptedHandle)[ id1 ];

    /// Get the DetSet of the stubs
    edmNew::DetSet< SiPixelStub > theseStubs = (*SiPixelStubDSVForOutputTemp)[ thisStackedDetId ];

    /// Prepare the new DetSet to replace the current one
    /// Loop over the stubs
    typename edmNew::DetSet< SiPixelCluster >::iterator clusterIter;
    typename edmNew::DetSet< SiPixelStub >::iterator stubIter;
    for ( stubIter = theseStubs.begin();
          stubIter != theseStubs.end();
          ++stubIter )
    {
      /// Create a temporary stub
      SiPixelStub tempSiPixelStub( stubIter->getDetId() );

      /// Compare the clusters stored in the stub with the ones of this module
      edm::Ref< edmNew::DetSetVector< SiPixelCluster >, SiPixelCluster > innerClusterToBeReplaced = stubIter->getClusterRef(0);
      edm::Ref< edmNew::DetSetVector< SiPixelCluster >, SiPixelCluster > outerClusterToBeReplaced = stubIter->getClusterRef(1);

      bool innerOK = false;
      bool outerOK = false;

      for ( clusterIter = innerClusters.begin();
            clusterIter != innerClusters.end() && !innerOK;
            ++clusterIter )
      {
        if ( clusterIter->getHits() == innerClusterToBeReplaced->getHits() )
        {
          tempSiPixelStub.addClusterRef( edmNew::makeRefTo( SiPixelClusterAcceptedHandle, clusterIter ) );
          innerOK = true;
        }
      }

      for ( clusterIter = outerClusters.begin();
            clusterIter != outerClusters.end() && !outerOK;
            ++clusterIter )
      {
        if ( clusterIter->getHits() == outerClusterToBeReplaced->getHits() )
        {
          tempSiPixelStub.addClusterRef( edmNew::makeRefTo( SiPixelClusterAcceptedHandle, clusterIter ) );
          outerOK = true;
        }
      }

      /// If no compatible clusters were found, skip to the next one
      if ( !innerOK || !outerOK )
        continue;

      tempSiPixelStub.setTriggerDisplacement( 2.*stubIter->getTriggerDisplacement() ); /// getter is in FULL-strip units, setter is in HALF-strip units
      tempSiPixelStub.setTriggerOffset( 2.*stubIter->getTriggerOffset() );             /// getter is in FULL-strip units, setter is in HALF-strip units

      acceptedOutputFiller.push_back( tempSiPixelStub );

    } /// End of loop over stubs of this module

    if ( acceptedOutputFiller.empty() )
      acceptedOutputFiller.abort();

  } /// End of loop over stub DetSetVector

  /// Put output in the event (2)
  iEvent.put( SiPixelStubDSVForOutputAccepted, "StubAccepted" );
  iEvent.put( SiPixelStubDSVForOutputRejected, "StubRejected" );
}

/// Sort routine for stub ordering

bool SiPixelStubBuilder::SortStubBendPairs( const std::pair< unsigned int, double >& left, const std::pair< unsigned int, double >& right )
{
  return left.second < right.second;
}

#endif
