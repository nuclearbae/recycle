#ifndef RECYCLE_SRC_GROWTH_REGION_TESTS_H_
#define RECYCLE_SRC_GROWTH_REGION_TESTS_H_

#include <gtest/gtest.h>

#include "context.h"
#include "growth_region.h"
#include "recorder.h"
#include "timer.h"

#include "test_context.h"
#include "region_tests.h"
#include "agent_tests.h"


namespace recycle {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class GrowthRegionTests : public ::testing::Test {
 protected:
  cyclus::Context* ctx;
  cyclus::Timer ti;
  cyclus::Recorder rec;
  recycle::GrowthRegion* region;
  std::string commodity_name, demand_type, demand_params, demand_start;

  virtual void SetUp();
  virtual void TearDown();
  bool ManagesCommodity(cyclus::toolkit::Commodity& commodity);
};

}  // namespace recycle

#endif  // RECYCLE_SRC_GROWTH_REGION_TESTS_H_
