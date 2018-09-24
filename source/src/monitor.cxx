// monitor.cxx
// K. Mizukoshi

#include <iostream>

#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TCanvas.h>

TTree* latest(){
	int verboseLevel = 0;
	TString dir = "/home/candles/np1c/CANUG/PreAnalysis/Official_Run010/";
	const int Run = 10;
	const int maxSRun = 999;
	const int maxSsRun = 50;

	TString lsFile = gSystem->GetFromPipe("ls "+ dir);
	if(verboseLevel>3) std::cout << lsFile << std::endl;

	for (int sRun=maxSRun;sRun>0;--sRun){
		for (int ssRun=maxSsRun;ssRun>0;--ssRun){
			TString RunNum = Form("Run%03d-%03d-%03d",Run,sRun,ssRun);

			TString ifn = dir + RunNum + ".root";

			if ( lsFile.Index(RunNum)==-1) {
				//File not exist
				if(verboseLevel>2) std::cout << RunNum << std::endl;
				if(verboseLevel>2) std::cout << "file does not exist." << std::endl;
				continue;
			} 

			TString qstat = gSystem->GetFromPipe("qstat|grep CM");
		  	if( qstat.Index(RunNum)!=-1 ){
				//File is creating
				if(verboseLevel>2) std::cout << RunNum << std::endl;
				if(verboseLevel>2) std::cout << "file is creating now" << std::endl;
				continue;
			}

			if(verboseLevel>1) std::cout << RunNum << std::endl;
			TFile *fin = new TFile(ifn.Data(), "read");
			TTree* t = dynamic_cast<TTree*>(fin->Get("tree"));
			return t;
		}
	}
}

int main(){

	TTree* t = latest();

	TCanvas* C = new TCanvas("c1","c1",600,400);
	C->Divide(3,2);
	C->cd(1);
	t->Draw("ADCTotal4us>>h1(100,0,200000)");

	C->cd(2);
	t->Draw("Pedestal[63]>>h2(90,225,255)");

	C->cd(3);
	t->Draw("Pedestal[64]>>h3(90,225,255)");

	C->cd(4);
	t->Draw("Pedestal[0]:Header[1]");

	C->cd(5);


	C->cd(6);

	C->SaveAs("latest.pdf");
	return 0;
}
