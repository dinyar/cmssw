// -*- C++ -*-
//
// Package:    L1Trigger/L1TP2GTDoubleObjSketch
// Class:      L1TP2GTDoubleObjSketch
//
/**\class L1TP2GTDoubleObjSketch L1TP2GTDoubleObjSketch.cc L1Trigger/L1TP2GTDoubleObjSketch/plugins/L1TP2GTDoubleObjSketch.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Dinyar Rabady
//         Created:  Fri, 23 Apr 2021 16:01:22 GMT
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include <iostream>
#include <fstream>
#include <string>

//
// class declaration
//

class L1TP2GTDoubleObjSketch : public edm::stream::EDProducer<> {
public:
  explicit L1TP2GTDoubleObjSketch(const edm::ParameterSet&);
  ~L1TP2GTDoubleObjSketch();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginStream(edm::StreamID) override;
  void produce(edm::Event&, const edm::EventSetup&) override;
  void endStream() override;

  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  bool replace(std::string& str, const std::string& from, const std::string& to);

  // ----------member data ---------------------------
  bool m_dump;
  std::string m_templatefolder;
  std::string m_menuname;
  std::string m_idx;
  std::string m_algoname;
  int m_pT1;
  int m_pT2;
  int m_etaMin1;
  int m_etaMax1;
  int m_etaMin2;
  int m_etaMax2;
  std::string m_object1;
  std::string m_object2;
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
L1TP2GTDoubleObjSketch::L1TP2GTDoubleObjSketch(const edm::ParameterSet& iConfig) {
  m_templatefolder = iConfig.getParameter<std::string>("template_folder");
  m_menuname = iConfig.getParameter<std::string>("menu_name");
  m_idx = iConfig.getParameter<std::string>("algo_index");  // Eventually we might want to automatically generate this?
  m_algoname = iConfig.getParameter<std::string>("algo_name");
  // TODO: These should be optional eventually.
  m_pT1 = iConfig.getParameter<int>("pt1");
  m_pT2 = iConfig.getParameter<int>("pt2");
  m_etaMin1 = iConfig.getParameter<int>("etaMin1");
  m_etaMax1 = iConfig.getParameter<int>("etaMax1");
  m_etaMin2 = iConfig.getParameter<int>("etaMin2");
  m_etaMax2 = iConfig.getParameter<int>("etaMax2");
  // TODO: These could come from some inputTag -> string map? (A bit problematic because input tags can sometimes change.)
  m_object1 = "JET_SLOT";
  m_object2 = "MU_SLOT";
  m_dump = iConfig.getParameter<bool>("dump");
  //register your products
  /* Examples
  produces<ExampleData2>();

  //if do put with a label
  produces<ExampleData2>("label");

  //if you want to put into the Run
  produces<ExampleData2,InRun>();
*/
  //now do what ever other initialization is needed
}

L1TP2GTDoubleObjSketch::~L1TP2GTDoubleObjSketch() {
  // do anything here that needs to be done at destruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//

// ------------ method called to produce the data  ------------
void L1TP2GTDoubleObjSketch::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  /* This is an event example
  //Read 'ExampleData' from the Event
  ExampleData const& in = iEvent.get(inToken_);

  //Use the ExampleData to create an ExampleData2 which
  // is put into the Event
  iEvent.put(std::make_unique<ExampleData2>(in));
*/

  /* this is an EventSetup example
  //Read SetupData from the SetupRecord in the EventSetup
  SetupData& setup = iSetup.getData(setupToken_);
*/
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void L1TP2GTDoubleObjSketch::beginStream(edm::StreamID) {
  // We dump the firmware configuration here if it's requested.
  if (m_dump) {
    std::ifstream t(m_templatefolder + "doubleObjectCond.template");
    std::stringstream buf;
    buf << t.rdbuf();
    std::string doubleObjectTemplate{buf.str()};

    replace(doubleObjectTemplate, "{name}", m_algoname);
    replace(doubleObjectTemplate, "{pt1}", std::to_string(m_pT1));
    replace(doubleObjectTemplate, "{pt2}", std::to_string(m_pT2));
    replace(doubleObjectTemplate, "{etaMin1}", std::to_string(m_etaMin1));
    replace(doubleObjectTemplate, "{etaMax1}", std::to_string(m_etaMax1));
    replace(doubleObjectTemplate, "{etaMin2}", std::to_string(m_etaMin2));
    replace(doubleObjectTemplate, "{etaMax2}", std::to_string(m_etaMax2));
    replace(doubleObjectTemplate, "{object1}", m_object1);
    replace(doubleObjectTemplate, "{object2}", m_object2);
    replace(doubleObjectTemplate, "{algo_index}", m_idx);

    std::ofstream f(m_menuname, std::ios::app);
    f << doubleObjectTemplate;
    f.close();
  }
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void L1TP2GTDoubleObjSketch::endStream() {
  // please remove this method if not needed
}

// ------------ method called when starting to processes a run  ------------
/*
void
L1TP2GTDoubleObjSketch::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
L1TP2GTDoubleObjSketch::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
L1TP2GTDoubleObjSketch::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
L1TP2GTDoubleObjSketch::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void L1TP2GTDoubleObjSketch::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

bool L1TP2GTDoubleObjSketch::replace(std::string& str, const std::string& from, const std::string& to) {
  size_t start_pos = str.find(from);
  if (start_pos == std::string::npos)
    return false;
  str.replace(start_pos, from.length(), to);
  return true;
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TP2GTDoubleObjSketch);
