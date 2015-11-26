import FWCore.ParameterSet.Config as cms
process = cms.Process("L1TMuonEmulation")
import os
import sys
import commands

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(50)
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))

process.source = cms.Source('PoolSource',
 #fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/g/gflouris/public/SingleMuPt6180_noanti_10k_eta1.root')
 fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/t/treis/public/testsamples/DYToMuMu_M-50_Tune4C_13TeV-pythia8_PU20bx25_tsg_castor_PHYS14_25_V1-v1_GEN-SIM-RAW_10000_044B58B4-9D75-E411-AB6C-002590A83218.root')
	                    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50))

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

# changes to run in PHYS14 GEN-SIM-RAW
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
                                  )
process.L1TMuonSeq = cms.Sequence(   process.caloStage2Layer1Digis
                                   + process.simTwinMuxDigis
                                   + process.simBmtfDigis 
                                   + process.simEmtfDigis 
                                   + process.simOmtfDigis 
                                   + process.simGmtCaloSumDigis
                                   + process.simGmtDigis
#                                   + process.dumpED
#                                   + process.dumpES
#                                   + process.l1tSummary
)

process.L1TMuonPath = cms.Path(process.L1ReEmulSeq + process.L1TMuonSeq)

process.out = cms.OutputModule("PoolOutputModule", 
   fileName = cms.untracked.string("l1tbmtf_superprimitives1.root")
)

process.output_step = cms.EndPath(process.out)
process.schedule = cms.Schedule(process.L1TMuonPath)
process.schedule.extend([process.output_step])
