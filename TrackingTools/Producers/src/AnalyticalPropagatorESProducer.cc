#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include <FWCore/Utilities/interface/ESInputTag.h>

#include <string>
#include <memory>

using namespace edm;

class AnalyticalPropagatorESProducer : public edm::ESProducer {
public:
  AnalyticalPropagatorESProducer(const edm::ParameterSet& p);
  std::unique_ptr<Propagator> produce(const TrackingComponentsRecord&);

  static void fillDescriptions(edm::ConfigurationDescriptions& oDesc);

private:
  const std::string simpleMagField_;
  const double dphiCut_;
  const PropagationDirection dir_;
};

AnalyticalPropagatorESProducer::AnalyticalPropagatorESProducer(const edm::ParameterSet& p)
    : simpleMagField_{p.getParameter<std::string>("SimpleMagneticField")},
      dphiCut_{p.getParameter<double>("MaxDPhi")},
      dir_{[](std::string const& pdir) {
        if (pdir == "oppositeToMomentum")
          return oppositeToMomentum;
        if (pdir == "alongMomentum")
          return alongMomentum;
        if (pdir == "anyDirection")
          return anyDirection;
        return alongMomentum;
      }(p.getParameter<std::string>("PropagationDirection"))} {
  std::string myname = p.getParameter<std::string>("ComponentName");
  setWhatProduced(this, myname);
}

void AnalyticalPropagatorESProducer::fillDescriptions(edm::ConfigurationDescriptions& oDesc) {
  edm::ParameterSetDescription desc;
  desc.add<std::string>("ComponentName")->setComment("the data label assigned to the Propagator");
  desc.add<std::string>("SimpleMagneticField", "")->setComment("the data label used to retrieve the MagneticField");
  desc.add<std::string>("PropagationDirection");
  desc.add<double>("MaxDPhi");

  oDesc.addDefault(desc);
}

std::unique_ptr<Propagator> AnalyticalPropagatorESProducer::produce(const TrackingComponentsRecord& iRecord) {
  //   if (_propagator){
  //     delete _propagator;
  //     _propagator = 0;
  //   }
  ESHandle<MagneticField> magfield;
  iRecord.getRecord<IdealMagneticFieldRecord>().get(simpleMagField_, magfield);
  //  edm::ESInputTag mfESInputTag(mfName);
  //  iRecord.getRecord<IdealMagneticFieldRecord>().get(mfESInputTag,magfield);

  return std::make_unique<AnalyticalPropagator>(&(*magfield), dir_, dphiCut_);
}

DEFINE_FWK_EVENTSETUP_MODULE(AnalyticalPropagatorESProducer);
