#include "tree_utils.cpp"
#include "Includes.h"

//The aim of this code is to make an histogram that contains the number events that has X final particles

void particle_num(){
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
    TH1F *h1 = new TH1F("h1", "particle type", 20, 0, 30);

    //Get the number of events in the tree
    int num_events = event_tree->GetEntries();

    cout << num_events << endl;

    //The following loop will iterate over the number of events and it will count only the particles whose gen_part_statusCode is "Stable final state" (1)
    int sizevec; //Size of the vector containing ALL particles
    int count_parts;

    //Loop over the number of events
    for(int i_e = 0; i_e < num_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        sizevec=gen_part_trackID->size();
        count_parts = 0;

        //Loop over ALL particles in i-th event
        for(int j=0; j < sizevec; j++){
            if (gen_part_statusCode->at(j) == 1){
                count_parts++;
            }
        }
        //cout << count_parts << endl;

        //Fill histogram
        h1->Fill(count_parts);
    }

    //Pongo bonitos los histogramas
    h1->SetStats(0); //para que no muestre las stats en la leyenda
    h1->SetStats(0); //lo mismo

    h1->GetXaxis()->SetTitle("Number of particles");
    h1->GetYaxis()->SetTitle("Counts");
    h1->GetXaxis()->SetTitleSize(0.05);
    h1->GetYaxis()->SetTitleSize(0.05);
    h1->GetXaxis()->SetLabelSize(0.05);
    h1->GetYaxis()->SetLabelSize(0.05);
 
    h1->SetTitle("Final particles");

    h1->SetLineColor(kRed);
    h1->SetLineStyle(1);
    h1->SetLineWidth(2);
    //hist_part->SetFillColor(kRed-3); //el -3 baja el tono del color
    h1->SetFillStyle(1001);
    h1->SetFillColorAlpha(kRed-3, 0.75);
    
    h1->SetMarkerColor(kBlack);
    h1->SetMarkerStyle(21);
    h1->SetMarkerSize(2.);

    TLegend *leg = new TLegend(0.55, 0.75, 0.9, 0.9);
    leg->AddEntry(h1, "Final particles", "f"); //f muestra el filling
    leg->SetBorderSize(1); //asi el grosor del la legenda es 0
    leg->Draw();

    //Pongo un grid en el canvas
    c1->SetGridx();
    c1->SetGridy();

    //gPad->SetFillStyle(4050);

    //Draw histogram
    h1->Draw();
    leg->Draw("SAME");
}