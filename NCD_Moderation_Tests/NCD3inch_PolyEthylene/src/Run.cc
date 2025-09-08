#include "G4CsvAnalysisManager.hh"
#include "Run.hh"

MyRunAction::MyRunAction()
{
	man = G4AnalysisManager::Instance();
	man->CreateNtuple("energy", "energy");
	man->CreateNtupleDColumn("fEvents");
	man->CreateNtupleDColumn("Greater_than_10keV");
	man->CreateNtupleDColumn("Lower_than_10keV");
	man->FinishNtuple(0);


	//Ntuple Neutron_Spectras
	man->CreateNtuple("Primary Neutron spectra", "P_Neutron spectra");
	man->CreateNtupleDColumn("Primary_Neutron_Spectra");
	man->FinishNtuple(1);

	man->CreateNtuple("Secondary Neutron spectra", "S_Neutron spectra");
	man->CreateNtupleDColumn("Secondary_Neutron_Spectra");
	man->FinishNtuple(2);

	man->CreateNtuple("Overall Neutron Spectra", "Neutron Spectra");
	man->CreateNtupleDColumn("Neutron_Spectra");
	man->FinishNtuple(3);


	//Ntuple Gamma_Spectras
	man->CreateNtuple("Primary Gamma spectra", "P_Gamma spectra");
	man->CreateNtupleDColumn("Primary_Gamma_Spectra");
	man->FinishNtuple(4);

	man->CreateNtuple("Secondary Gamma spectra", "S_Gamma spectra");
	man->CreateNtupleDColumn("Secondary_Gamma_Spectra");
	man->FinishNtuple(5);


	//Ntuple Proton_Spectras
	man->CreateNtuple("Primary Proton spectra", "P_Proton_spectra");
	man->CreateNtupleDColumn("Primary_Proton_Spectra");
	man->FinishNtuple(6);

	man->CreateNtuple("Secondary Proton spectra", "S_Proton_spectra");
	man->CreateNtupleDColumn("Secondary_Proton_Spectra");
	man->FinishNtuple(7);


	//NTuple Alpha_spectra
	man->CreateNtuple("Primary Alpha spectra", "P_Alpha_spectra");
	man->CreateNtupleDColumn("Primary_Alpha_Spectra");
	man->FinishNtuple(8);

	man->CreateNtuple("Secondary Alpha spectra", "S_Alpha_spectra");
	man->CreateNtupleDColumn("Secondary_Alpha_Sepctra");
	man->FinishNtuple(9);

	//NTuple Alpha_spectra
	man->CreateNtuple("Primary Triton spectra", "P_Triton_spectra");
	man->CreateNtupleDColumn("Primary_Triton_Spectra");
	man->FinishNtuple(10);

	man->CreateNtuple("Secondary Triton spectra", "S_Triton_spectra");
	man->CreateNtupleDColumn("Secondary_Triton_Sepctra");
	man->FinishNtuple(11);


	//NTuple edep
	man->CreateNtuple("Energy deposited", "Edep");
	man->CreateNtupleDColumn("Total_Energy_deposited");
	man->FinishNtuple(12);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run*)
{
	
	//Ntuple energy
	man->SetFileName("NeutronFluence");  // Without .root
	man->OpenFile();
	

}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager* man = G4AnalysisManager::Instance();

	man->Write();
	man->CloseFile();
}

