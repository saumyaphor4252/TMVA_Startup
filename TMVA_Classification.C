void TMVA_Classification( TString myMethodList = "" )
{
   // This loads the library
   TMVA::Tools::Instance();

   // This loads the input file for machine learning
   TFile * input = TFile::Open("tmva_input.root", "read");
   
   // Register the training and test trees
   // signal     -> electron
   // background -> muon
   TTree *signal         = (TTree*)input->Get("t_em");
   TTree *background     = (TTree*)input->Get("t_mu");

   // Create a output file where TMVA results will be stored 
   TString outputFileName = "tmva_output.root";
   TFile* output = TFile::Open( outputFileName, "recreate" );

   // Create the factory object where methods for machine learning are contained
   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification",
					       output,
					       "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );

   // Create the dataloader where train/test objects is handled
   TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");

   // Add varaible for machine learning in Float ('F') type
   dataloader->AddVariable( "energy := energy", 'F' );
   dataloader->AddVariable( "momentum := momentum", 'F' );

   // Set event weights: Just make it 1 
   Float_t signalWeight     = 1.0;
   Float_t backgroundWeight = 1.0;

   // You can add an arbitrary number of signal or background trees
   dataloader->AddSignalTree    ( signal    ,     signalWeight );
   dataloader->AddBackgroundTree( background, backgroundWeight );

   dataloader->PrepareTrainingAndTestTree( "", "",
					   "nTrain_Signal=5000:nTrain_Background=5000:nTest_Signal=5000:nTest_Background=5000:SplitMode=Random:NormMode=NumEvents:!V" );

   // ### Book MVA methods
   // Every parameter for machine learning is roughly optimized already so you don't have to edit
   factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );
   factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT",
			"!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();

   // Save the output
   output->Close();

   delete factory;
   delete dataloader;

   // Pop-up GUI of TMVA result
   TMVA::TMVAGui( outputFileName );
}
