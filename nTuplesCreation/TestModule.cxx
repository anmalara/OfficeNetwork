#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

using namespace std;
using namespace uhh2;

/** \brief Example for creating additional branches in the event output tree
*/
class TestModule: public AnalysisModule {
public:

  explicit TestModule(Context & ctx);
  virtual bool process(Event & event);

private:
  Event::Handle<vector<double>> h_jetpt;
};

TestModule::TestModule(Context & ctx){

  // clear all output branches:
  ctx.undeclare_all_event_output();

  // declare the outputs.
  h_jetpt = ctx.declare_event_output<vector<double>>("jetpt");

}


bool TestModule::process(Event & event) {

  vector<double> jetpt;
  for(const Jet & jet : *event.jets) jetpt.push_back(jet.pt());

  event.set(h_jetpt, std::move(jetpt));

  return true;
}

UHH2_REGISTER_ANALYSIS_MODULE(TestModule)
