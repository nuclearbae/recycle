// Implements the DeployInst class
#include "deploy_inst.h"

namespace recycle {

DeployInst::DeployInst(cyclus::Context* ctx) : cyclus::Institution(ctx) {}

DeployInst::~DeployInst() {}

void DeployInst::Build(cyclus::Agent* parent) {
  cyclus::Institution::Build(parent);
  BuildSched::iterator it;
  std::set<std::string> protos;
  for (int i = 0; i < prototypes.size(); i++) {
    std::string proto = prototypes[i];

    std::stringstream ss;
    ss << proto;

    if (lifetimes.size() == prototypes.size()) {
      cyclus::Agent* a = context()->CreateAgent<Agent>(proto);
      if (a->lifetime() != lifetimes[i]) {
        a->lifetime(lifetimes[i]);

        if (lifetimes[i] == -1) {
          ss << "_life_forever";
        } else {
          ss << "_life_" << lifetimes[i];
        }
        proto = ss.str();
        if (protos.count(proto) == 0) {
          protos.insert(proto);
          context()->AddPrototype(proto, a);
        }
      }
    }

    int t = build_times[i];
    for (int j = 0; j < n_build[i]; j++) {
      context()->SchedBuild(this, proto, t);
    }
  }
}

void DeployInst::EnterNotify() {
  cyclus::Institution::EnterNotify();

  int n = prototypes.size();
  if (build_times.size() != n) {
    std::stringstream ss;
    ss << "prototype '" << prototype() << "' has " << build_times.size()
       << " build_times vals, expected " << n;
    throw cyclus::ValueError(ss.str());
  } else if (n_build.size() != n) {
    std::stringstream ss;
    ss << "prototype '" << prototype() << "' has " << n_build.size()
       << " n_build vals, expected " << n;
    throw cyclus::ValueError(ss.str());
  } else if (lifetimes.size() > 0 && lifetimes.size() != n) {
    std::stringstream ss;
    ss << "prototype '" << prototype() << "' has " << lifetimes.size()
       << " lifetimes vals, expected " << n;
    throw cyclus::ValueError(ss.str());
  }
}



void DeployInst::Register_(Agent* a) {
  using cyclus::toolkit::CommodityProducer;
  using cyclus::toolkit::CommodityProducerManager;

  CommodityProducer* cp_cast = dynamic_cast<CommodityProducer*>(a);
  if (cp_cast != NULL) {
    LOG(cyclus::LEV_INFO3, "mani") << "Registering agent "
                                   << a->prototype() << a->id()
                                   << " as a commodity producer.";
    CommodityProducerManager::Register(cp_cast);
      }
  else{std::cout << "cp cast returned null  yall" << std::endl;}

}


extern "C" cyclus::Agent* ConstructDeployInst(cyclus::Context* ctx) {
  return new DeployInst(ctx);
}

}  // namespace recycle
