#ifndef SIPIXELSTUB_CLASSES_H
#define SIPIXELSTUB_CLASSES_H

//#include "DataFormats/SiPixelCluster/interface/SiPixelClusterCollection.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include <vector>
#include <map>
#include "DataFormats/Common/interface/LazyGetter.h"


#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/Common/interface/ContainerMask.h"
#include "DataFormats/SiPixelStub/interface/SiPixelStub.h"


namespace {
  struct dictionary2 {


    //Dictionaries for SiPixelStub and collections
    SiPixelStub                                               SSTUB;
    std::vector< SiPixelStub >                              V_SSTUB;
    edm::Wrapper< std::vector< SiPixelStub > >            W_V_SSTUB;
    edmNew::DetSetVector< SiPixelStub >                   SDV_SSTUB;
    edm::Wrapper< edmNew::DetSetVector< SiPixelStub > > W_SDV_SSTUB;


    edm::Ref< edmNew::DetSetVector< SiPixelStub >, SiPixelStub >                                    R_SSTUB;
    edm::Wrapper< edm::Ref< edmNew::DetSetVector< SiPixelStub >, SiPixelStub > >                  W_R_SSTUB;
    std::vector< edm::Ref< edmNew::DetSetVector< SiPixelStub >, SiPixelStub > >                   V_R_SSTUB;
    edm::Wrapper< std::vector< edm::Ref< edmNew::DetSetVector< SiPixelStub >, SiPixelStub > > > W_V_R_SSTUB;




  };
}

#endif // SIPIXELSTUB_CLASSES_H
