#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/Utils.h"

#include "UHH2/common/include/CommonModules.h"
#include <UHH2/common/include/JetIds.h>
#include <UHH2/common/include/MuonIds.h>
#include <UHH2/common/include/ElectronIds.h>

using namespace std;
using namespace uhh2;

/** \brief Example for creating additional branches in the event output tree
*/
class NTupleCreator: public AnalysisModule {
public:

  explicit NTupleCreator(Context & ctx);
  virtual bool process(Event & event);

private:
  Event::Handle<vector<double>> h_jetPt;
  Event::Handle<vector<double>> h_jetEta;
  Event::Handle<vector<double>> h_jetPhi;
  Event::Handle<vector<double>> h_jetE;
  Event::Handle<vector<double>> h_jetM;
  Event::Handle<vector<double>> h_jetPx;
  Event::Handle<vector<double>> h_jetPy;
  Event::Handle<vector<double>> h_jetPz;
  Event::Handle<vector<double>> h_jetCSV;
  Event::Handle<double>         h_jetN;

  Event::Handle<vector<double>> h_muonPt;
  Event::Handle<vector<double>> h_muonEta;
  Event::Handle<vector<double>> h_muonPhi;
  Event::Handle<vector<double>> h_muonE;
  Event::Handle<vector<double>> h_muonM;
  Event::Handle<vector<double>> h_muonPx;
  Event::Handle<vector<double>> h_muonPy;
  Event::Handle<vector<double>> h_muonPz;
  Event::Handle<double>         h_muonN;

  Event::Handle<vector<double>> h_elePt;
  Event::Handle<vector<double>> h_eleEta;
  Event::Handle<vector<double>> h_elePhi;
  Event::Handle<vector<double>> h_eleE;
  Event::Handle<vector<double>> h_eleM;
  Event::Handle<vector<double>> h_elePx;
  Event::Handle<vector<double>> h_elePy;
  Event::Handle<vector<double>> h_elePz;
  Event::Handle<double>         h_eleN;

  Event::Handle<double>         h_weight;

  // CommonModules
  unique_ptr<CommonModules> common;

  string SysType_PU;

};

NTupleCreator::NTupleCreator(Context & ctx){

  // clear all output branches:
  ctx.undeclare_all_event_output();

  // declare the outputs.
  h_jetPt = ctx.declare_event_output<vector<double>>("jetPt");
  h_jetEta = ctx.declare_event_output<vector<double>>("jetEta");
  h_jetPhi = ctx.declare_event_output<vector<double>>("jetPhi");
  h_jetE = ctx.declare_event_output<vector<double>>("jetE");
  h_jetM = ctx.declare_event_output<vector<double>>("jetM");
  h_jetPx = ctx.declare_event_output<vector<double>>("jetPx");
  h_jetPy = ctx.declare_event_output<vector<double>>("jetPy");
  h_jetPz = ctx.declare_event_output<vector<double>>("jetPz");
  h_jetCSV = ctx.declare_event_output<vector<double>>("jetCSV");
  h_jetN = ctx.declare_event_output<double>("jetN");


  h_muonPt = ctx.declare_event_output<vector<double>>("muonPt");
  h_muonEta = ctx.declare_event_output<vector<double>>("muonEta");
  h_muonPhi = ctx.declare_event_output<vector<double>>("muonPhi");
  h_muonE = ctx.declare_event_output<vector<double>>("muonE");
  h_muonM = ctx.declare_event_output<vector<double>>("muonM");
  h_muonPx = ctx.declare_event_output<vector<double>>("muonPx");
  h_muonPy = ctx.declare_event_output<vector<double>>("muonPy");
  h_muonPz = ctx.declare_event_output<vector<double>>("muonPz");
  h_muonN = ctx.declare_event_output<double>("muonN");

  h_elePt = ctx.declare_event_output<vector<double>>("elePt");
  h_eleEta = ctx.declare_event_output<vector<double>>("eleEta");
  h_elePhi = ctx.declare_event_output<vector<double>>("elePhi");
  h_eleE = ctx.declare_event_output<vector<double>>("eleE");
  h_eleM = ctx.declare_event_output<vector<double>>("eleM");
  h_elePx = ctx.declare_event_output<vector<double>>("elePx");
  h_elePy = ctx.declare_event_output<vector<double>>("elePy");
  h_elePz = ctx.declare_event_output<vector<double>>("elePz");
  h_eleN = ctx.declare_event_output<double>("eleN");

  h_weight = ctx.declare_event_output<double>("weight");


  //Lepton cleaner
  const     JetId  jetId(AndId<Jet>     (JetPFID(JetPFID::WP_TIGHT),      PtEtaCut  (30, 2.4)));
  const     MuonId muonId(AndId<Muon>   (MuonID(Muon::CutBasedIdTight),   PtEtaCut  (30, 2.4)));
  const ElectronId eleId(AndId<Electron>(ElectronID_MVA_Fall17_loose_iso, PtEtaSCCut(30, 2.4)));

  SysType_PU = ctx.get("SysType_PU");

  common.reset(new CommonModules());
  common->disable_jetpfidfilter();
  common->disable_mclumiweight();
  common->disable_metfilters();
  common->set_jet_id(jetId);
  common->set_muon_id(muonId);
  common->set_electron_id(eleId);
  common->switch_jetlepcleaner();
  common->switch_jetPtSorter();

  common->init(ctx,SysType_PU);

}


bool NTupleCreator::process(Event & event) {

  common->process(event);

  vector<double> jetPt, jetEta, jetPhi, jetE, jetM, jetPx, jetPy, jetPz, jetCSV;
  vector<double> muonPt, muonEta, muonPhi, muonE, muonM, muonPx, muonPy, muonPz;
  vector<double> elePt, eleEta, elePhi, eleE, eleM, elePx, elePy, elePz;

  double jetN, muonN, eleN, weight;
  weight = event.weight;

  for(const Jet & jet : *event.jets){
    LorentzVector v = jet.v4();
    jetPt.push_back(jet.pt());
    jetEta.push_back(jet.eta());
    jetPhi.push_back(jet.phi());
    jetE.push_back(jet.energy());
    jetM.push_back(v.M());
    jetPx.push_back(v.Px());
    jetPy.push_back(v.Py());
    jetPz.push_back(v.Pz());
    jetCSV.push_back(jet.btag_DeepCSV());
  }
  jetN = event.jets->size();

  for(const Muon & muon : *event.muons){
    LorentzVector v = muon.v4();
    muonPt.push_back(muon.pt());
    muonEta.push_back(muon.eta());
    muonPhi.push_back(muon.phi());
    muonE.push_back(muon.energy());
    muonM.push_back(v.M());
    muonPx.push_back(v.Px());
    muonPy.push_back(v.Py());
    muonPz.push_back(v.Pz());
  }
  muonN = event.muons->size();


  for(const Electron & ele : *event.electrons){
    LorentzVector v = ele.v4();
    elePt.push_back(ele.pt());
    eleEta.push_back(ele.eta());
    elePhi.push_back(ele.phi());
    eleE.push_back(ele.energy());
    eleM.push_back(v.M());
    elePx.push_back(v.Px());
    elePy.push_back(v.Py());
    elePz.push_back(v.Pz());
  }
  eleN = event.electrons->size();

  event.set(h_jetPt,    std::move(jetPt));
  event.set(h_jetEta,   std::move(jetEta));
  event.set(h_jetPhi,   std::move(jetPhi));
  event.set(h_jetE,     std::move(jetE));
  event.set(h_jetM,     std::move(jetM));
  event.set(h_jetPx,    std::move(jetPx));
  event.set(h_jetPy,    std::move(jetPy));
  event.set(h_jetPz,    std::move(jetPz));
  event.set(h_jetCSV,   std::move(jetCSV));
  event.set(h_jetN,     std::move(jetN));

  event.set(h_muonPt,   std::move(muonPt));
  event.set(h_muonEta,  std::move(muonEta));
  event.set(h_muonPhi,  std::move(muonPhi));
  event.set(h_muonE,    std::move(muonE));
  event.set(h_muonM,    std::move(muonM));
  event.set(h_muonPx,   std::move(muonPx));
  event.set(h_muonPy,   std::move(muonPy));
  event.set(h_muonPz,   std::move(muonPz));
  event.set(h_muonN,    std::move(muonN));

  event.set(h_elePt,    std::move(elePt));
  event.set(h_eleEta,   std::move(eleEta));
  event.set(h_elePhi,   std::move(elePhi));
  event.set(h_eleE,     std::move(eleE));
  event.set(h_eleM,     std::move(eleM));
  event.set(h_elePx,    std::move(elePx));
  event.set(h_elePy,    std::move(elePy));
  event.set(h_elePz,    std::move(elePz));
  event.set(h_eleN,     std::move(eleN));

  event.set(h_weight,   std::move(weight));

  return true;
}

UHH2_REGISTER_ANALYSIS_MODULE(NTupleCreator)
