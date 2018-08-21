branch_names_jet = ["jetPt", "jetEta", "jetPhi", "jetE", "jetM", "jetPx", "jetPy", "jetPz", "jetCSV", "jetN", "weight"]
branch_names_muon = ["muonPt", "muonEta", "muonPhi", "muonE", "muonM", "muonPx", "muonPy", "muonPz", "muonN", "weight"]
branch_names_ele = ["elePt", "eleEta", "elePhi", "eleE", "eleM", "elePx", "elePy", "elePz", "eleN", "weight"]


branch_outputnames = {"jet": "JetVariables", "muon": "MuonVariables", "ele": "EleVariables" }
branch_name_list = {"jet": branch_names_jet, "muon": branch_names_muon, "ele": branch_names_ele }
nobjects = {"jet":10, "muon":4, "ele":4 }

selection_cuts = "(abs(jetEta)<=2.4)"

path_QCD= "/pnfs/desy.de/cms/tier2/store/user/akaravdi/RunII_94X_v1/QCD_Ptbinned/QCD_Pt_80to120_TuneCP5_13TeV_pythia8/crab_QCD_Pt_80to120_TuneCP5_13TeV_pythia8/180509_172322"
path_DY= "/pnfs/desy.de/cms/tier2/store/user/afroehli/RunII_94X_v1/DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DY2JetsToLL_M-50/180206_124333"
