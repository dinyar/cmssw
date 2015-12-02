// -*- C++ -*-
//
// Package:    L1Analysis/SelectGenMuons
// Class:      SelectGenMuons
//
/**\class SelectGenMuons SelectGenMuons.cc L1Analysis/SelectGenMuons/plugins/SelectGenMuons.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Joschka Philip Lingemann
//         Created:  Mon, 13 Jul 2015 18:45:16 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


//
// class declaration
//

class SelectGenMuons : public edm::EDFilter {
   public:
      explicit SelectGenMuons(const edm::ParameterSet&);
      ~SelectGenMuons();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
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
SelectGenMuons::SelectGenMuons(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  edm::InputTag genp = edm::InputTag("genParticles");
  consumes<reco::GenParticleCollection>(genp);
}


SelectGenMuons::~SelectGenMuons()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
SelectGenMuons::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByLabel("genParticles", genParticles);

  bool haveValidMuons = false;
  for(size_t i = 0; i < genParticles->size(); ++ i) {
    const reco::GenParticle & p = (*genParticles)[i];
    int id = p.pdgId();
    if (abs(id) == 13) {
      if (fabs(p.eta()) < 2.4) {
        haveValidMuons = true;
        break;
      }
    }
  }


   return (haveValidMuons);
}

// ------------ method called once each job just before starting event loop  ------------
void
SelectGenMuons::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
SelectGenMuons::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
SelectGenMuons::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
SelectGenMuons::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
SelectGenMuons::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
SelectGenMuons::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SelectGenMuons::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(SelectGenMuons);
