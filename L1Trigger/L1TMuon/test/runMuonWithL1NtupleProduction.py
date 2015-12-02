import FWCore.ParameterSet.Config as cms
process = cms.Process("L1TMuonEmulation")
import os
import sys
import commands

SAMPLE = "zmumu"  # "relval"##"minbias"
NEVENTS = 50

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(50)
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))
#process.Tracer = cms.Service("Tracer")

fnames = cms.untracked.vstring('file:/afs/cern.ch/work/g/gflouris/public/SingleMuPt6180_noanti_10k_eta1.root')
if SAMPLE == "zmumu":#    fnames = ['root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/B61E1FCD-A077-E311-8B65-001E673974EA.root',
#    fnames = ['root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/B61E1FCD-A077-E311-8B65-001E673974EA.root',
#              'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/10000/0023D81B-2980-E311-85A1-001E67398C0F.root',
#              'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/10000/248FB042-3080-E311-A346-001E67397D00.root']
#    fnames = ['/store/mc/Phys14DR/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/PU20bx25_tsg_castor_PHYS14_25_V1-v1/10000/044B58B4-9D75-E411-AB6C-002590A83218.root',
#              '/store/mc/Phys14DR/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/PU20bx25_tsg_castor_PHYS14_25_V1-v1/10000/045570BC-9175-E411-A06A-002590A887F0.root',
#              '/store/mc/Phys14DR/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/PU20bx25_tsg_castor_PHYS14_25_V1-v1/10000/0C8C76BC-9775-E411-882E-002481E0DCD8.root',]
    fnames = ['file:/afs/cern.ch/work/t/treis/public/testsamples/DYToMuMu_M-50_Tune4C_13TeV-pythia8_PU20bx25_tsg_castor_PHYS14_25_V1-v1_GEN-SIM-RAW_10000_044B58B4-9D75-E411-AB6C-002590A83218.root']
elif SAMPLE == "minbias":
    fnames = ['root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/00276D94-AA88-E311-9C90-0025905A6060.root',
              'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/004F8058-6F88-E311-B971-0025905A6094.root',
              'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/005C8F98-C288-E311-ADF1-0026189438BD.root',
              'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/006A1FB8-7D88-E311-B61B-0025905A60A0.root']

process.source = cms.Source('PoolSource',
 fileNames = cms.untracked.vstring(fnames)
	                    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(NEVENTS))

# PostLS1 geometry used
process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2015_cff')
############################
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

####Event Setup Producers
process.load('L1Trigger.L1TMuonBarrel.fakeMuonBarrelParams_cfi')
process.load('L1Trigger.L1TMuonOverlap.fakeMuonOverlapParams_cfi')
process.load('L1Trigger.L1TMuonEndCap.fakeMuonEndCapParams_cfi')
process.load('L1Trigger.L1TMuon.fakeMuonGlobalParams_cfi')


#### Emulators
process.load('L1Trigger.L1TMuonBarrel.simMuonBarrelDigis_cfi')
process.load('L1Trigger.L1TMuonOverlap.simMuonOverlapDigis_cfi')
process.load('L1Trigger.L1TMuonEndCap.simMuonEndCapDigis_cfi')
process.load('L1Trigger.L1TMuon.simMuonDigis_cfi')
process.load('L1Trigger.L1TCalorimeter.caloStage2Layer1Digis_cfi')

process.dumpED = cms.EDAnalyzer("EventContentAnalyzer")
process.dumpES = cms.EDAnalyzer("PrintEventSetupContent")

#process.l1tSummary = cms.EDAnalyzer("L1TSummary")
#process.l1tSummary.egToken   = cms.InputTag("simCaloStage2Digis");
#process.l1tSummary.tauToken  = cms.InputTag("simCaloStage2Digis");
#process.l1tSummary.jetToken  = cms.InputTag("simCaloStage2Digis");
#process.l1tSummary.sumToken  = cms.InputTag("simCaloStage2Digis");
#process.l1tSummary.muonToken = cms.InputTag("simGmtDigis","");
##process.l1tSummary.muonToken = cms.InputTag("simGmtDigis","imdMuonsBMTF");

process.load('L1Trigger.L1TCalorimeter.caloStage2Layer1Digis_cfi')
process.caloStage2Layer1Digis.ecalToken = cms.InputTag("simEcalTriggerPrimitiveDigis")
process.caloStage2Layer1Digis.hcalToken = cms.InputTag("simHcalTriggerPrimitiveDigis")

#### L1Ntuple production
process.L1MuonFilter = cms.EDFilter("SelectL1Muons",)
process.GenMuonFilter = cms.EDFilter("SelectGenMuons",)

process.load("L1TriggerDPG.L1Ntuples.l1NtupleProducer_cfi")
#process.load("L1TriggerDPG.L1Ntuples.l1RecoTreeProducer_cfi")
#process.load("L1TriggerDPG.L1Ntuples.l1ExtraTreeProducer_cfi")
#process.load("L1TriggerDPG.L1Ntuples.l1MuonRecoTreeProducer_cfi")
process.load("L1TriggerDPG.L1Ntuples.l1MuonUpgradeTreeProducer_cfi")

process.l1MuonUpgradeTreeProducer.omtfTag = cms.InputTag("simOmtfDigis", "OMTF")
process.l1MuonUpgradeTreeProducer.emtfTag = cms.InputTag("simEmtfDigis", "EMTF")
process.l1MuonUpgradeTreeProducer.bmtfTag = cms.InputTag("simBmtfDigis", "BMTF")
process.l1MuonUpgradeTreeProducer.calo2x2Tag = cms.InputTag("simGmtCaloSumDigis", "TriggerTower2x2s")
process.l1MuonUpgradeTreeProducer.caloTag = cms.InputTag("caloStage2Layer1Digis")
process.l1MuonUpgradeTreeProducer.caloRecoTag = cms.InputTag("none")

# reset LUT paths to trigger CMSSW internal LUT generation
process.gmtParams.BrlSingleMatchQualLUTPath = cms.string('')
process.gmtParams.FwdPosSingleMatchQualLUTPath = cms.string('')
process.gmtParams.FwdNegSingleMatchQualLUTPath = cms.string('')
process.gmtParams.OvlPosSingleMatchQualLUTPath = cms.string('')
process.gmtParams.OvlNegSingleMatchQualLUTPath = cms.string('')
process.gmtParams.BOPosMatchQualLUTPath = cms.string('')
process.gmtParams.BONegMatchQualLUTPath = cms.string('')
process.gmtParams.FOPosMatchQualLUTPath = cms.string('')
process.gmtParams.FONegMatchQualLUTPath = cms.string('')

# analysis
process.l1NtupleProducer.hltSource = cms.InputTag("none")
process.l1NtupleProducer.gtSource = cms.InputTag("none")
process.l1NtupleProducer.gctCentralJetsSource = cms.InputTag("none")
process.l1NtupleProducer.gctNonIsoEmSource = cms.InputTag("none")
process.l1NtupleProducer.gctForwardJetsSource = cms.InputTag("none")
process.l1NtupleProducer.gctIsoEmSource = cms.InputTag("none")
process.l1NtupleProducer.gctEnergySumsSource = cms.InputTag("none")
process.l1NtupleProducer.gctTauJetsSource = cms.InputTag("none")
process.l1NtupleProducer.gctIsoTauJetsSource = cms.InputTag("none")
process.l1NtupleProducer.rctSource = cms.InputTag("none")
process.l1NtupleProducer.dttfSource = cms.InputTag("none")
process.l1NtupleProducer.ecalSource = cms.InputTag("none")
process.l1NtupleProducer.hcalSource = cms.InputTag("none")
process.l1NtupleProducer.csctfTrkSource = cms.InputTag("none")
process.l1NtupleProducer.csctfLCTSource = cms.InputTag("none")
process.l1NtupleProducer.csctfStatusSource = cms.InputTag("none")
process.l1NtupleProducer.generatorSource = cms.InputTag("genParticles")
process.l1NtupleProducer.csctfDTStubsSource = cms.InputTag("none")
process.l1NtupleProducer.csctfStatusSource = cms.InputTag("none")
process.l1NtupleProducer.csctfLCTSource = cms.InputTag("none")
process.l1NtupleProducer.csctfTrkSource = cms.InputTag("none")

# output file
process.TFileService = cms.Service("TFileService",
   fileName=cms.string('/afs/cern.ch/work/t/treis/private/l1ntuples_upgrade/l1ntuple_{sample}_n.root'.format(sample=SAMPLE)))

process.L1NtupleSeq = cms.Sequence(process.l1NtupleProducer + process.l1MuonUpgradeTreeProducer)
    # +process.l1extraParticles
    # +process.l1ExtraTreeProducer
    # +process.l1GtTriggerMenuLite
    # +process.l1MenuTreeProducer
    # +process.l1RecoTreeProducer
    # +process.l1MuonRecoTreeProducer

process.tfQualFilter = cms.EDProducer("l1t::TrackFinderQualityFilter",)
process.simGmtDigis.overlapTFInput = cms.InputTag("tfQualFilter", "OMTF")
process.simGmtDigis.forwardTFInput = cms.InputTag("tfQualFilter", "EMTF")

process.MuonFilter = cms.Sequence()
if SAMPLE == "minbias":
    process.MuonFilter = cms.Sequence(process.L1MuonFilter)
else:
    process.MuonFilter = cms.Sequence(process.GenMuonFilter)

process.L1ReEmulSeq = cms.Sequence()
#### changes to run in PHYS14 GEN-SIM-RAW
if SAMPLE == 'zmumu' or SAMPLE == 'minbias':
    process.load("Configuration.StandardSequences.RawToDigi_cff")
    process.caloStage2Layer1Digis.ecalToken = cms.InputTag("ecalDigis", "EcalTriggerPrimitives")
    process.caloStage2Layer1Digis.hcalToken = cms.InputTag("hcalDigis")
    process.simEmtfDigis.CSCInput = cms.InputTag("csctfDigis")
    process.simOmtfDigis.srcDTPh = cms.InputTag('dttfDigis')
    process.simOmtfDigis.srcDTTh = cms.InputTag('dttfDigis')
    process.simOmtfDigis.srcCSC = cms.InputTag('csctfDigis')
    process.simOmtfDigis.srcRPC = cms.InputTag('muonRPCDigis')
    process.simTwinMuxDigis.DTDigi_Source = cms.InputTag("dttfDigis")
    process.simTwinMuxDigis.DTThetaDigi_Source = cms.InputTag("dttfDigis")
    process.simTwinMuxDigis.RPC_Source = cms.InputTag("muonRPCDigis")
    process.simBmtfDigis.DTDigi_Theta_Source = cms.InputTag("dttfDigis")

    process.L1ReEmulSeq = cms.Sequence(  process.ecalDigis
                                       + process.hcalDigis
                                       + process.csctfDigis
                                       + process.dttfDigis
                                       + process.muonRPCDigis
                                       + process.gtDigis
                                      )

process.L1TMuonSeq = cms.Sequence(   process.caloStage2Layer1Digis
                                   + process.simTwinMuxDigis
                                   + process.simBmtfDigis 
                                   + process.simEmtfDigis 
                                   + process.simOmtfDigis 
                                   + process.simGmtCaloSumDigis
                                   + process.tfQualFilter
                                   + process.simGmtDigis
#                                   + process.dumpED
#                                   + process.dumpES
#                                   + process.l1tSummary
)

process.L1TMuonPath = cms.Path(process.L1ReEmulSeq + process.L1TMuonSeq + process.MuonFilter + process.L1NtupleSeq)

process.out = cms.OutputModule("PoolOutputModule", 
   outputCommands=cms.untracked.vstring(
       'drop *',
       'keep *_*_*_L1TMuonEmulation'
   ),
   fileName = cms.untracked.string("l1tMuon.root")
)

process.output_step = cms.EndPath(process.out)
process.schedule = cms.Schedule(process.L1TMuonPath)
process.schedule.extend([process.output_step])
