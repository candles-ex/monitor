// monitor.cxx
// K. Mizukoshi

#include <iostream>

#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>

TTree* latest(){

	TString dir = "/home/candles/np1c/CANUG/PreAnalysis/Official_Run010/";
	const int Run = 10;
	const int maxSRun = 800;
	const int maxSsRun = 30;

	for (int sRun=maxSRun;sRun>0;--sRun){
		for (int ssRun=maxSsRun;ssRun>0;--ssRun){
			TString RunNum = Form("Run%03d-%03d-%03d",Run,sRun,ssRun);

			TString ifn = dir + RunNum + ".root";
			TString qstat = gSystem->GetFromPipe("qstat|grep CM");

			FileStat_t info;

			if (gSystem->GetPathInfo(ifn.Data(), info)!=0) {
				//File not exist
				std::cout <<RunNum<<std::endl;
				std::cout << "file does not exist." << std::endl;
			} else if( qstat.Index(RunNum)!=-1 ){
				//File is creating
				std::cout <<RunNum<<std::endl;
				std::cout << "file is creating now" << std::endl;
			} else {
				//File exists
				std::cout <<RunNum<<std::endl;
				TFile *fin = new TFile(ifn.Data(), "read");
				TTree* t = dynamic_cast<TTree*>(fin->Get("tree"));
				return t;
			}
		}
	}
}

int main(){
	TTree* t = latest();
	std::cout << t->GetEntries() << std::endl;
	return 0;
}
