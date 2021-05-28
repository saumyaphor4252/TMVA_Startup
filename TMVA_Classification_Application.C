void TMVA_Classification_Application()
{

   // This loads the library
   TMVA::Tools::Instance();

   // Create the Reader object
   // : This will evaluate our data according to TMVA machine learning result
   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );
   
   // Create a set of variables and declare them to the reader
   // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
   Float_t energy, momentum;
   reader->AddVariable( "energy   := energy",   &energy );
   reader->AddVariable( "momentum := momentum", &momentum);

   // We are going to evaluate only with MLP method
   reader->BookMVA( "MLP method", "dataset/weights/TMVAClassification_MLP.weights.xml" );
   
   // Prepare input tree for application
   TFile *input = TFile::Open( "merged.root","read" ); 
   TTree* tree = (TTree*)input->Get("t_merged");

   Int_t PID;
   tree->SetBranchAddress( "energy",   &energy );
   tree->SetBranchAddress( "momentum", &momentum );
   tree->SetBranchAddress( "PID",      &PID );

   Float_t MLP_optCut = 0.4637; // From machine learning log

   Bool_t isSignal; // ==  is it electron??

   // Number of right classification
   Int_t sig_to_sig = 0;
   Int_t bg_to_bg = 0;

   // Output histograms for evaluated value
   TH1F *histMLP = new TH1F("MVA_MLP","MVA_MLP",100,-1.25,1.5 );;

   for (int i=0; i<tree->GetEntries();i++) {
     tree->GetEntry(i);
     
     Float_t mlp_eval = reader->EvaluateMVA("MLP method");
     histMLP->Fill(mlp_eval);

     if (mlp_eval > MLP_optCut) isSignal = true;
     else isSignal = false;

     // check right classification
     // case 1. e-  is classified as e-
     if (isSignal == true && PID == 11){
       sig_to_sig++;
     }
     // case 2. mu- is classified as mu-
     else if (isSignal == false && PID == 13){
       bg_to_bg++;
     }              
   }

   Float_t sig_eff = Float_t(sig_to_sig)/(tree->GetEntries()/2);
   Float_t bg_rej  = Float_t(bg_to_bg)  /(tree->GetEntries()/2);

   std::cout << "Signal Efficiency:     " << sig_eff << std::endl;
   std::cout << "Background Efficiency: " << bg_rej  << std::endl;
   
   // Write histograms
   TFile *output  = new TFile( "TMVApp_output.root","recreate" );
   histMLP->Write();
   output->Close();
   
   delete reader;
}
