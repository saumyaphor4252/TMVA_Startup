void GenerateTree(){

  // Make TFile object with recreate mode      
  TFile *f = TFile::Open("merged.root","recreate");

  // Make TTree object
  TTree *tree = new TTree("t_merged","tree for unknown particle's energy and momentum");

  // Make Branches inside TTree, which are linked to root variables (Energy & Momentum
  Float_t ene, mom;
  Int_t PID;

  tree->Branch("energy", &ene, "energy/F");
  tree->Branch("momentum", &mom, "momentum/F");
  tree->Branch("PID", &PID, "PID/I");
  
  // Define simple gaussian statistics for e- particle
  Float_t ene_mean_em  = 20.;
  Float_t ene_sigma_em = 3.4;
  Float_t mom_mean_em  = 27.5;
  Float_t mom_sigma_em = 3.3;

  // Define simple gaussian statistics for mu- particle
  Float_t ene_mean_mu  = 27.3;
  Float_t ene_sigma_mu = 5.2;
  Float_t mom_mean_mu  = 36.5;
  Float_t mom_sigma_mu = 5.7;

  // declare random number generator
  TRandom3 rand;
  
  // Generate randum numbers and add them (entries) into tree
  for (int i=0; i<2e4; i++){

    // if index is even, get e- entry
    if (i%2 == 0){
      ene = rand.Gaus(ene_mean_em, ene_sigma_em);
      mom = rand.Gaus(mom_mean_em, mom_sigma_em);
      PID = 11; // particle identification number of e- is 11
    }

    // if index is odd,  get mu- entry
    else if (i%2 == 1){
      ene = rand.Gaus(ene_mean_mu, ene_sigma_mu);
      mom = rand.Gaus(mom_mean_mu, mom_sigma_mu);
      PID = 13; // particle identification number of mu- is 13
    }
    // Fill tree with entry one-by-one
    tree->Fill();
  }

  // We will write TTree object in f (TFile) object
  f->cd();

  // Write Tree to TFile
  tree->Write();
  
  // Close TFile
  f->Close();
}
