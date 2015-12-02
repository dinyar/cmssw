// -*- C++ -*-
//
// Package:    TrackFinderQualityFilter
// Class:      TrackFinderQualityFilter
//
/**\class TrackFinderQualityFilter TrackFinderQualityFilter.cc L1Trigger/L1TGlobalMuon/plugins/TrackFinderQualityFilter.cc

 Description: Takes txt-file input and produces barrel- / overlap- / forward TF muons

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Joschka Philip Lingemann,40 3-B01,+41227671598,
//         Created:  Thu Oct  3 10:12:30 CEST 2013
// $Id$
//
//


// system include files
#include <memory>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"

#include <iostream>
//
// class declaration
//
namespace l1t {
class TrackFinderQualityFilter : public edm::EDProducer {
   public:
      explicit TrackFinderQualityFilter(const edm::ParameterSet&);
      ~TrackFinderQualityFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      // ----------member data ---------------------------
      // edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> m_barrelTfInputToken;
      edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> m_overlapTfInputToken;
      edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> m_endcapTfInputToken;
      // edm::InputTag m_barrelTfInputTag;
      edm::InputTag m_overlapTfInputTag;
      edm::InputTag m_endcapTfInputTag;
      std::map<int, int> ptMap_;
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
TrackFinderQualityFilter::TrackFinderQualityFilter(const edm::ParameterSet& iConfig) :
  // m_barrelTfInputTag("bmtfEmulator", "BM")
  m_overlapTfInputTag("simOmtfDigis", "OMTF"),
  m_endcapTfInputTag("simEmtfDigis", "EMTF")
{
  // m_barrelTfInputToken = consumes<l1t::RegionalMuonCandBxCollection>(m_barrelTfInputTag);
  m_overlapTfInputToken = consumes<l1t::RegionalMuonCandBxCollection>(m_overlapTfInputTag);
  m_endcapTfInputToken = consumes<l1t::RegionalMuonCandBxCollection>(m_endcapTfInputTag);
  //register your products
  produces<RegionalMuonCandBxCollection>("OMTF");
  produces<RegionalMuonCandBxCollection>("EMTF");
}


TrackFinderQualityFilter::~TrackFinderQualityFilter()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//


// ------------ method called to produce the data  ------------
void
TrackFinderQualityFilter::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  std::auto_ptr<l1t::RegionalMuonCandBxCollection> filteredOMTFMuons (new l1t::RegionalMuonCandBxCollection());
  std::auto_ptr<l1t::RegionalMuonCandBxCollection> filteredEMTFMuons (new l1t::RegionalMuonCandBxCollection());

  Handle<l1t::RegionalMuonCandBxCollection> omtfMuons;
  Handle<l1t::RegionalMuonCandBxCollection> emtfMuons;

  iEvent.getByToken(m_endcapTfInputToken, emtfMuons);
  iEvent.getByToken(m_overlapTfInputToken, omtfMuons);

  for (auto mu = emtfMuons->begin(0); mu != emtfMuons->end(0); ++mu) {
    if (mu->hwQual() == 11 || mu->hwQual() > 12) {
      l1t::RegionalMuonCand newMu((*mu));
      filteredEMTFMuons->push_back(0, newMu);
    }
  }

  for (auto mu = omtfMuons->begin(0); mu != omtfMuons->end(0); ++mu) {
    if (mu->trackAddress().at(0)!=99840 &&
        mu->trackAddress().at(0)!=34304 &&
        mu->trackAddress().at(0)!=3075 &&
        mu->trackAddress().at(0)!=36928 &&
        mu->trackAddress().at(0)!=12300 &&
        mu->trackAddress().at(0)!=98816 &&
        mu->trackAddress().at(0)!=98944 &&
        mu->trackAddress().at(0)!=33408 &&
        mu->trackAddress().at(0)!=66688 &&
        mu->trackAddress().at(0)!=66176) {
      l1t::RegionalMuonCand newMu((*mu));
      filteredOMTFMuons->push_back(0, newMu);
    }
  }

  iEvent.put(filteredOMTFMuons, "OMTF");
  iEvent.put(filteredEMTFMuons, "EMTF");
}

// ------------ method called once each job just before starting event loop  ------------
void
TrackFinderQualityFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
TrackFinderQualityFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void
TrackFinderQualityFilter::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void
TrackFinderQualityFilter::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void
TrackFinderQualityFilter::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void
TrackFinderQualityFilter::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TrackFinderQualityFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
}
//define this as a plug-in
DEFINE_FWK_MODULE(l1t::TrackFinderQualityFilter);
