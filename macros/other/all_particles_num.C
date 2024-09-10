#include "tree_utils.cpp"
#include "Includes.h"

// Create a histogram that shows the amount of each type of particle

void all_particles_num(){
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
    TH1F *h1 = new TH1F("h1", "particle counting", 1000, -500, 500);

    int n_events = event_tree->GetEntries();
    int sizevec;

    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        sizevec=gen_part_PDGcode->size();

        //Loop over ALL particles in i-th event
        for(int j=0; j < sizevec; j++){
            h1->Fill(gen_part_PDGcode->at(j));
        }   
    }

    h1->SetStats(0); //not showing stats 

    h1->GetXaxis()->SetTitle("ID");
    h1->GetYaxis()->SetTitle("Counts");
    h1->GetXaxis()->SetTitleSize(0.05);
    h1->GetYaxis()->SetTitleSize(0.05);
    h1->GetXaxis()->SetLabelSize(0.05);
    h1->GetYaxis()->SetLabelSize(0.05);
 
    h1->SetTitle("All particles counting");

    h1->SetLineColor(kRed);
    h1->SetLineStyle(1);
    h1->SetLineWidth(2);
    h1->SetFillStyle(1001);
    h1->SetFillColorAlpha(kRed-3, 0.75);
    
    h1->SetMarkerColor(kBlack);
    h1->SetMarkerStyle(21);
    h1->SetMarkerSize(2.);

    TLegend *leg = new TLegend(0.6, 0.8, 0.9, 0.9);
    leg->AddEntry(h1, "all_particles", "f"); //f for showing filling
    leg->SetBorderSize(1); //leg width 0
    leg->Draw();

    //Canvas grid
    c1->SetGridx();
    c1->SetGridy();

    //gPad->SetFillStyle(4050);

    //Draw histogram
    h1->Draw();
    leg->Draw("SAME");
    int target_pdg = 311;
    cout << h1->GetBinContent(target_pdg + 1) << endl;

    //c1->Print("histograms/all_particles_num.pdf");
}