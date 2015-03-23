# Auto generated configuration file
# using: 
# Revision: 1.20 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --conditions auto:upgradePLS3 -n 10 --eventcontent FEVTDEBUGHLT,DQM -s RAW2DIGI,L1Reco,RECO,VALIDATION,DQM --datatier GEN-SIM-RECO,DQMIO --customise SLHCUpgradeSimulations/Configuration/combinedCustoms.cust_2023Muondev --geometry Extended2023Muondev,Extended2023MuondevReco --magField 38T_PostLS1 --filein file:step2.root --fileout file:step3.root
import FWCore.ParameterSet.Config as cms

process = cms.Process('untilLOCALRECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023MuondevReco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
#ERICA
#process.load('RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitMatcher_cff')
#process.load('RecoLocalTracker.SiStripZeroSuppression.SiStripZeroSuppression_cff')
#process.load('RecoLocalTracker.SiStripClusterizer.SiStripClusterizer_cff')
process.load('RecoLocalTracker.Configuration.RecoLocalTracker_cff')

#NOT ERICA
#process.load('Configuration.StandardSequences.Reconstruction_cff')
#process.load('Configuration.StandardSequences.Validation_cff')
#process.load('DQMOffline.Configuration.DQMOfflineMC_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# import StubBuilder
process.load('RecoLocalTracker.SiPixelStubBuilder.SiPixelStubBuilder_cfi')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:ConfigTest/step2.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.20 $'),
    annotation = cms.untracked.string('step3 nevts:10'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    fileName = cms.untracked.string('file:ConfigTest/step3_ultilLOCALRECO.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    )
)

#process.DQMoutput = cms.OutputModule("DQMRootOutputModule",
#    splitLevel = cms.untracked.int32(0),
#    outputCommands = process.DQMEventContent.outputCommands,
#    fileName = cms.untracked.string('file:ConfigTest/step3_inDQM.root'),
#    dataset = cms.untracked.PSet(
#        filterName = cms.untracked.string(''),
#        dataTier = cms.untracked.string('DQMIO')
#    )
#)

# Additional output definition

# Other statements
process.mix.playback = True
process.mix.digitizers = cms.PSet()
for a in process.aliases: delattr(process, a)
process.RandomNumberGeneratorService.restoreStateLabel=cms.untracked.string("randomEngineStateProducer")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')



# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.striptrackerlocalreco = cms.Sequence(process.siStripZeroSuppression+process.siStripClusters+process.siPixelStubs)
process.striptrackerlocalreco_step = cms.Path(process.striptrackerlocalreco)
#process.trackerreconstruction_step         = cms.Path(process.trackerlocalreco)
#process.reconstruction_step = cms.Path(process.reconstruction)
#process.prevalidation_step = cms.Path(process.prevalidation)
#process.dqmoffline_step = cms.Path(process.DQMOffline)
#process.validation_step = cms.EndPath(process.validation)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)
#process.DQMoutput_step = cms.EndPath(process.DQMoutput)

# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.prevalidation_step,process.validation_step,process.dqmoffline_step,process.FEVTDEBUGHLToutput_step,process.DQMoutput_step)
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.striptrackerlocalreco_step,process.FEVTDEBUGHLToutput_step)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2023Muondev 

#call to customisation function cust_2023Muondev imported from SLHCUpgradeSimulations.Configuration.combinedCustoms
process = cust_2023Muondev(process)

# Automatic addition of the customisation function from SimGeneral.MixingModule.fullMixCustomize_cff
from SimGeneral.MixingModule.fullMixCustomize_cff import setCrossingFrameOn 

#call to customisation function setCrossingFrameOn imported from SimGeneral.MixingModule.fullMixCustomize_cff
process = setCrossingFrameOn(process)

# End of customisation functions
