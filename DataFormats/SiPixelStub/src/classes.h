#ifndef DataFormats_SiPixelStub_classes_h
#define DataFormats_SiPixelStub_classes_h
#include "DataFormats/SiPixelStub/interface/SiPixelStub.h"
#include "DataFormats/Common/interface/RefProd.h" 
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/ContainerMask.h"

namespace {
  struct dictionary {
    std::vector<SiPixelStub> v1;
    edm::DetSet<SiPixelStub> ds1;
    std::vector<edm::DetSet<SiPixelStub> > vds1;
    SiPixelStubCollection c1;
    SiPixelStubCollectionNew c1_new;
    edm::Wrapper<SiPixelStubCollection> w1;
    edm::Wrapper<SiPixelStubCollectionNew> w1_new;
    SiPixelStubRef r1;
    SiPixelStubRefNew r1_new;
    SiPixelStubRefVector rv1;
    SiPixelStubRefProd rp1;
    edm::Ref<edm::DetSetVector<SiPixelStub>,edm::DetSet<SiPixelStub>,edm::refhelper::FindDetSetForDetSetVector<SiPixelStub,edm::DetSetVector<SiPixelStub> > > boguscrap;

    std::vector<edm::Ref<edmNew::DetSetVector<SiPixelStub>,SiPixelStub,edmNew::DetSetVector<SiPixelStub>::FindForDetSetVector> > dsvr_v;
    edmNew::DetSetVector<edm::Ref<edmNew::DetSetVector<SiPixelStub>,SiPixelStub,edmNew::DetSetVector<SiPixelStub>::FindForDetSetVector> > dsvr;
    edm::Wrapper<edmNew::DetSetVector<edm::Ref<edmNew::DetSetVector<SiPixelStub>,SiPixelStub,edmNew::DetSetVector<SiPixelStub>::FindForDetSetVector> > > dsvr_w;

    edm::ContainerMask<SiPixelStubCollectionNew> cm1;
    edm::Wrapper<edm::ContainerMask<SiPixelStubCollectionNew> > w_cm1;
  };
}

#endif // SIPIXELSTUB_CLASSES_H
