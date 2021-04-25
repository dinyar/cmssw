import FWCore.ParameterSet.Config as cms

process = cms.Process("L1MicroGMTEmulator")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1))

dumpMenu = cms.bool(True)

process.algo1 = cms.EDProducer("L1TP2GTDoubleObjSketch",
    template_folder = cms.string("."),
    menu_name = cms.string("testMenu.vhd"),
    algo_index = cms.string("1"),
    algo_name = cms.string("jetMu_2_3_eta4_5-6_7"),
    pt1 = cms.untracked.int32(2),
    pt2 = cms.untracked.int32(3),
    etaMin1 = cms.untracked.int32(4),
    etaMax1 = cms.untracked.int32(5),
    etaMin2 = cms.untracked.int32(6),
    etaMax2 = cms.untracked.int32(7),
    dump = dumpMenu
)

process.algo2 = cms.EDProducer("L1TP2GTDoubleObjSketch",
    template_folder = cms.string(""),
    menu_name = cms.string("testMenu.vhd"),
    algo_index = cms.string("2"),
    algo_name = cms.string("jetMu_7_6_eta5_4-3_2"),
    pt1 = cms.untracked.int32(7),
    pt2 = cms.untracked.int32(6),
    etaMin1 = cms.untracked.int32(5),
    etaMax1 = cms.untracked.int32(4),
    etaMin2 = cms.untracked.int32(3),
    etaMax2 = cms.untracked.int32(2),
    dump = dumpMenu
)

process.dumpPath = cms.Path( process.algo1 + process.algo2 )
process.schedule = cms.Schedule(process.dumpPath)
