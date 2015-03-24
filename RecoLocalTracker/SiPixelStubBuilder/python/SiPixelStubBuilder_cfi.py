import FWCore.ParameterSet.Config as cms
 
siPixelStubs = cms.EDProducer("SiPixelStubBuilder",
#    Clusters = cms.InputTag('siPixelClusters'),
     Clusters = cms.InputTag('siPhase2Clusters'), 

#     TTClusters = cms.InputTag("TTClustersFromPixelDigis", "ClusterInclusive"),
)

#import FWCore.ParameterSet.Config as cms
#
##
#from CondTools.SiPixel.SiPixelGainCalibrationService_cfi import *
#siPixelStubs = cms.EDProducer("SiPixelStubBuilder",
#    SiPixelGainCalibrationServiceParameters,
#    src = cms.InputTag("siPixelDigis"),
#    ChannelThreshold = cms.int32(1000),
#    MissCalibrate = cms.untracked.bool(True),
#    SplitClusters = cms.bool(False),
#    VCaltoElectronGain = cms.int32(65),
#    VCaltoElectronOffset = cms.int32(-414),                          
#    # **************************************
#    # ****  payLoadType Options         ****
#    # ****  HLT - column granularity    ****
#    # ****  Offline - gain:col/ped:pix  ****
#    # **************************************
#    payloadType = cms.string('Offline'),
#    SeedThreshold = cms.int32(1000),
#    ClusterThreshold = cms.double(4000.0),
#    # **************************************
#    maxNumberOfClusters = cms.int32(-1), # -1 means no limit.
#)
#
#
