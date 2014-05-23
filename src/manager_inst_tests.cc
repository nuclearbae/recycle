#include "manager_inst_tests.h"


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TestProducer::TestProducer(cyclus::Context* ctx)
    : cyclus::Facility(ctx) {};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TestProducer::~TestProducer() {};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ManagerInstTests::SetUp() {
  ctx_ = new cyclus::Context(&ti_, &rec_);
  src_inst = new cycamore::ManagerInst(ctx_);
  producer = new TestProducer(ctx_);
  commodity = cyclus::toolkit::Commodity("commod");
  capacity = 5;
  producer->AddCommodity(commodity);
  producer->SetCapacity(commodity, capacity);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ManagerInstTests::TearDown() {
  delete producer;
  delete src_inst;
  delete ctx_;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
cyclus::Agent* ManagerInstitutionConstructor(cyclus::Context* ctx) {
  return new cycamore::ManagerInst(ctx);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(ManagerInstTests, producerexists) {
  using std::set;
  ctx_->AddPrototype("foop", producer);
  src_inst->RegisterAvailablePrototype("foop");
  set<cyclus::toolkit::CommodityProducer*>::iterator it;
  for (it = src_inst->BeginningProducer(); it != src_inst->EndingProducer();
       it++) {
    EXPECT_EQ(dynamic_cast<TestProducer*>(*it)->prototype(), producer->prototype());
  }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(ManagerInstTests, productioncapacity) {
  EXPECT_EQ(src_inst->TotalProductionCapacity(commodity), 0);
  src_inst->BuildNotify(producer);
  EXPECT_EQ(src_inst->TotalProductionCapacity(commodity), capacity);
  src_inst->DecomNotify(producer);
  EXPECT_EQ(src_inst->TotalProductionCapacity(commodity), 0);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
INSTANTIATE_TEST_CASE_P(ManagerInst, InstitutionTests,
                        Values(&ManagerInstitutionConstructor));
INSTANTIATE_TEST_CASE_P(ManagerInst, AgentTests,
                        Values(&ManagerInstitutionConstructor));
