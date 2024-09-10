#include "tree_utils.cpp"
#include "Includes.h"

// Creat histogram of the initial neutrinos energy distribution

void initial_nu_energy(){
    //Definition of the file from which the tree is readed and the folder inside the ".root" file where the tree is stored
    TFile *input_file;
    TDirectoryFile *tree_dir;

    //Definition of the tree that stores information about each LArSoft event
    TTree *event_tree;

    //Definition of the path where the file containing the truth tree is stored, along with the code that opens the file and both the subrun and the event by event tree
    string path_to_tree = "analysis_output_truth_BNB_single_100k_events.root";
    input_file = new TFile(path_to_tree.c_str());
    tree_dir = (TDirectoryFile*)input_file->Get("ana");
    event_tree =(TTree*)tree_dir->Get("tree");

    //Code that sets the Branches fot the subrun and event trees in order to use the information they store, please visit "tree_utils.cpp" to see how the functions are defined
    set_branch(event_tree);

    //Create canvas and histogram where to store particle type
    TCanvas *c1 = new TCanvas("c1", "canvas", 800, 600);
    TH1F *h1 = new TH1F("h1", "particle type", 100, 0, 7);

    int n_entries = event_tree->GetEntries();

    // OBTAIN MAXIMUM INITIAL ENERGY
    /*
    float max_e = 0;

    for (int i=0; i<n_entries; i++){
        event_tree->GetEntry(i);

        if (nu_E0 > max_e){
            max_e = nu_E0;
        }
    }

    cout << "Maximum E0 = " << max_e << endl;
    //*/

   for (int i=0; i<n_entries; i++){
        event_tree->GetEntry(i);
        h1->Fill(nu_E0);
    }
  

    h1->SetStats(0); //not showing stats 

    h1->GetXaxis()->SetTitle("Energy");
    h1->GetYaxis()->SetTitle("Counts");
    h1->GetXaxis()->SetTitleSize(0.05);
    h1->GetYaxis()->SetTitleSize(0.05);
    h1->GetXaxis()->SetLabelSize(0.05);
    h1->GetYaxis()->SetLabelSize(0.05);
 
    h1->SetTitle("Initial neutrinos energy distribution");

    h1->SetLineColor(kRed);
    h1->SetLineStyle(1);
    h1->SetLineWidth(2);
    h1->SetFillStyle(1001);
    h1->SetFillColorAlpha(kRed-3, 0.75);
    
    h1->SetMarkerColor(kBlack);
    h1->SetMarkerStyle(21);
    h1->SetMarkerSize(2.);

    TLegend *leg = new TLegend(0.6, 0.8, 0.9, 0.9);
    leg->AddEntry(h1, "nu_E0", "f"); //f for showing filling
    leg->SetBorderSize(1); //leg width 0
    leg->Draw();

    //Canvas grid
    c1->SetGridx();
    c1->SetGridy();

    //gPad->SetFillStyle(4050);

    //Draw histogram
    h1->Draw();
    leg->Draw("SAME");

    c1->Print("histograms/nu_E0.pdf");

}