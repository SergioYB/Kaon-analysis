#include "tree_utils.cpp"
#include "Includes.h"


//Computing total POT in all events
double get_total_POT(TTree* subrun_tree){
    double n_POT = 0;
    set_branch_subtree(subrun_tree);
    int n_subruns = subrun_tree->GetEntries();

    for (int i = 0; i<= n_subruns; i++){
        subrun_tree->GetEntry(i);
        n_POT = n_POT + POT;
    }

    return n_POT;
}


void kaon_ratio(){
    //Definition of the file from which the tree is readed and the folder inside the ".root" file where the tree is stored
    TFile *input_file;
    TDirectoryFile *tree_dir;

    //Definition of the tree that stores information about each LArSoft event
    TTree *event_tree;
    TTree *subrun_tree;

    //Definition of the path where the file containing the truth tree is stored, along with the code that opens the file and both the subrun and the event by event tree
    string path_to_tree = "analysis_output_truth_BNB_single_100k_events.root";
    input_file = new TFile(path_to_tree.c_str());
    tree_dir = (TDirectoryFile*)input_file->Get("ana");
    event_tree =(TTree*)tree_dir->Get("tree");
    subrun_tree =(TTree*)tree_dir->Get("subrun_tree");


    //Code that sets the Branches fot the subrun and event trees in order to use the information they store, please visit "tree_utils.cpp" to see how the functions are defined
    set_branch(event_tree);
    set_branch_subtree(subrun_tree);

    int n_events = event_tree->GetEntries();
    float sizevec; // size of the vector containing all particles of each event

    //Number of K0+- in each event
    int n_k0;
    int n_ak0;
    int n_kp;
    int n_km;

    //Create canvas and histogram 
    TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
    TH2F *h1 = new TH2F("h1", "", 4, 0.5 , 4.5, 3, 0.5, 3.5); //x-> charge, y-> number of kaons

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        sizevec = gen_part_trackID->size();

        //Set number of kaons to 0
        n_k0 = 0;
        n_ak0 = 0;
        n_kp = 0;
        n_km = 0;

        //Choose interaction mode
        //if (nu_interaction_mode == 2 & nu_interaction_type == 1092){
        if (true){
            //Loop over ALL particles in i-th event
            for(int j = 0; j < sizevec; j++){
                if (gen_part_statusCode->at(j) == 1){
                    if (gen_part_PDGcode->at(j) == 311){
                        n_k0++;
                    } else if (gen_part_PDGcode->at(j) == 321){
                        n_kp++;
                    } else if (gen_part_PDGcode->at(j) == -321){
                        n_km++;
                    } else if (gen_part_PDGcode->at(j) == -311){
                        n_ak0++;
                    }
                }
            }
        }

        //FILL h1 

        h1->Fill(1,n_k0+1);
        h1->Fill(2,n_ak0+1);
        h1->Fill(3,n_kp+1);
        h1->Fill(4,n_km+1);
    }

    //Scaling histogram for one year production (3y = 10e21 POT)
    double n_POT = get_total_POT(subrun_tree);

    //DRAW h1 ===========================================================================

    h1->SetStats(0);

    h1->GetXaxis()->SetBinLabel(1, "K0");
    h1->GetXaxis()->SetBinLabel(2, "#bar{K}0");
    h1->GetXaxis()->SetBinLabel(3, "K+");
    h1->GetXaxis()->SetBinLabel(4, "K-");

    h1->GetYaxis()->SetBinLabel(1, "0");
    h1->GetYaxis()->SetBinLabel(2, "1");
    h1->GetYaxis()->SetBinLabel(3, "2+");

    h1->GetXaxis()->SetTitleSize(0.05);
    h1->GetYaxis()->SetTitleSize(0.05);
    h1->GetXaxis()->SetLabelSize(0.05);
    h1->GetYaxis()->SetLabelSize(0.05);

    h1->GetXaxis()->SetTitle("K charge");
    h1->GetYaxis()->SetTitle("Number of K");

    //Scaling
    h1->Scale(1e21/(3 * n_POT));

    c1->cd(); //Activate canvas c1
    h1->Draw("COLZ");

    //Add text
    TString xlabel; //To get x label in each iteration
    int binsX = h1->GetNbinsX();
    int binsY = h1->GetNbinsY();
    for (int x = 1; x <= binsX; x++) {
        for (int y = 1; y <= binsY; y++) {
            float binContent = h1->GetBinContent(x, y);
            if (binContent > 0) {  //Only adding text if bin is not empty
                char text1[50];
                char text2[50];
                char text3[50];
                sprintf(text1, "%.2f", binContent);
                //sprintf(text2, "%.3f%% of total K events", binContent/h1->Integral()*100);
                sprintf(text2, "%.3f%% of ttl evnts", binContent/(n_events * 1e21/(3 * n_POT))*100);
                xlabel = h1->GetXaxis()->GetBinLabel(x);
                sprintf(text3, "%.3f%% of %s", binContent/h1->Integral(x,x,2,3)*100, xlabel.Data());

                //Creating TText objects
                TText *t1 = new TText(h1->GetXaxis()->GetBinCenter(x), h1->GetYaxis()->GetBinCenter(y) + 0.2, text1);
                TText *t2 = new TText(h1->GetXaxis()->GetBinCenter(x), h1->GetYaxis()->GetBinCenter(y), text2);
                //Adjust size and location
                t1->SetTextSize(0.03);
                t1->SetTextAlign(22);  //Centering text
                t2->SetTextSize(0.03);
                t2->SetTextAlign(22);

                //Draw text on canvas
                t1->Draw();
                t2->Draw();

                if (y >1){
                    TText *t3 = new TText(h1->GetXaxis()->GetBinCenter(x), h1->GetYaxis()->GetBinCenter(y) - 0.2, text3);
                    
                    t3->SetTextSize(0.03);
                    t3->SetTextAlign(22);
                    t3->Draw();
                }                
                
            }
        }
    }


    //SAVE PLOTS
    //c1->SaveAs("histograms/kaon_counting.pdf");

}
   