void GenerateTMVAinput()
{

  // Make TFile object with recreate mode      
  TFile *f = TFile::Open("tmva_input.root","recreate");

  // Make TTree object
  TTree *t_em = new TTree("t_em","tree for measured e-  energy and momentum");
  TTree *t_mu = new TTree("t_mu","tree for measured mu- energy and momentum");

  // Make Branches inside TTree, which are linked to root variables (Energy & Momentum
  Float_t ene_em, mom_em;
  Float_t ene_mu, mom_mu;

  t_em->Branch("energy", &ene_em, "energy/F");
  t_em->Branch("momentum", &mom_em, "momentum/F");
  t_mu->Branch("energy", &ene_mu, "energy/F");
  t_mu->Branch("momentum", &mom_mu, "momentum/F");
  
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
  for (int i=0; i<1e4; i++){
    ene_em = rand.Gaus(ene_mean_em, ene_sigma_em);
    mom_em = rand.Gaus(mom_mean_em, mom_sigma_em);
    
    ene_mu = rand.Gaus(ene_mean_mu, ene_sigma_mu);
    mom_mu = rand.Gaus(mom_mean_mu, mom_sigma_mu);

    // Fill tree with entry one-by-one
    t_em->Fill();
    t_mu->Fill();
  }

  // We will write TTree object in f (TFile) object
  f->cd();

  // Write Tree to TFile
  t_em->Write();
  t_mu->Write();
  
  // Close TFile
  f->Close();
}
