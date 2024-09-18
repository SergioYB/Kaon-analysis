#include "/home/sergioyb/root/kaon_analysis/macros/includes/tree_utils.cpp"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/Includes.h"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_event_tree.C"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_subrun_tree.C"


bool ends_inside_detector(TTree* event_tree, int i_p, double fv){
    // returns true if particle end pos is inside detector
    double x = g4_part_end_pos_X->at(i_p);
    double y = g4_part_end_pos_Y->at(i_p);
    double z = g4_part_end_pos_Z->at(i_p);

    //return ((x >= -200) & (x <= 200) & (y >= -200) & (y <= 200) & (z >= 0) & (z <= 500));
    return ((x >= -200 + fv) & (x <= 200 - fv) & (y >= -200 + fv) & (y <= 200 - fv) & (z >= 0 + fv) & (z <= 500 - fv)); //FV
}

bool starts_inside_detector(TTree* event_tree, int i_p, double fv){
    // returns true if particle start pos is inside detector
    double x = g4_part_start_pos_X->at(i_p);
    double y = g4_part_start_pos_Y->at(i_p);
    double z = g4_part_start_pos_Z->at(i_p);

    //return ((x >= -200) & (x <= 200) & (y >= -200) & (y <= 200) & (z >= 0) & (z <= 500));
    return ((x >= -200 + fv) & (x <= 200 - fv) & (y >= -200 + fv) & (y <= 200 - fv) & (z >= 0 + fv) & (z <= 500 - fv)); //FV
}

double vectorNorm(const std::vector<double>& vec) {
    double sum = 0.0;
    for (double val : vec) {
        sum += val * val;  
    }
    return std::sqrt(sum); 
}


double trackLength(TTree* event_tree, int i_p){
    std::vector<double> startPos = {g4_part_start_pos_X->at(i_p),g4_part_start_pos_Y->at(i_p),g4_part_start_pos_Y->at(i_p)};
    std::vector<double> endPos = {g4_part_end_pos_X->at(i_p),g4_part_end_pos_Y->at(i_p),g4_part_end_pos_Y->at(i_p)};

    std::vector<double> track(startPos.size());

    for (size_t i = 0; i < startPos.size(); ++i) {
        track[i] = endPos[i] - startPos[i];
    }

    return vectorNorm(track);
}

std::pair<int, std::vector<double>> binsGen(double inicio, double punto_medio, double final, int bins1, int bins2) {
    // El número total de bins es la suma de los bins de cada tramo
    int nbins = bins1 + bins2;

    // Vector para almacenar los bordes de los bins
    std::vector<double> binEdges(nbins + 1);  // +1 para incluir el último borde

    // Calcular el tamaño de los bins en cada tramo
    double paso1 = (punto_medio - inicio) / bins1;  // Tamaño de los bins en el primer tramo
    double paso2 = (final - punto_medio) / bins2;   // Tamaño de los bins en el segundo tramo

    // Llenar los bordes de los bins para el primer tramo
    for (int i = 0; i <= bins1; ++i) {
        binEdges[i] = inicio + i * paso1;
    }

    // Llenar los bordes de los bins para el segundo tramo
    for (int i = 1; i <= bins2; ++i) {
        binEdges[bins1 + i] = punto_medio + (i - 1) * paso2;
    }

    // Devolver el número de bins y el array de bordes
    return std::make_pair(nbins, binEdges);
}

void range_in_out(){

    double n_k;

    TTree *event_tree;

    double fv = 5;

    string data_dir = "/home/sergioyb/root/kaon_analysis/data/analysis_output_truth_k0_events.root";

    event_tree = (TTree*)read_event_tree(data_dir);

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;

    auto bins_tuple_s = binsGen(0,5,30,20,60);
    int nbins_s = bins_tuple_s.first;
    std::vector<double> bins_s = bins_tuple_s.second;

    
    
    //Create canvas and histogram 
    TCanvas *c_ks = new TCanvas("c_ks", "cs", 800, 600);
    TH1F *h_ks_in = new TH1F("h_ks_in", "", 100, 0, 30); 
    TH1F *h_ks_out = new TH1F("h_ks_out", "", 100, 0, 30); 

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        //Loop over g4 particles to finds primary k0 created inside detector
        for(int i_p = 0; i_p < size_g4; i_p++){
            if(g4_part_process->at(i_p) == "primary"){
                if (ends_inside_detector(event_tree, i_p, fv)){
                    h_ks_in->Fill(trackLength(event_tree, i_p));
                } else {
                    h_ks_out->Fill(trackLength(event_tree, i_p));
                }
            }
        }
    }

    cout << n_k << endl;

    //Draw histograms
    h_ks_in->SetLineColor(kBlue);
    h_ks_in->SetLineWidth(3);
    h_ks_in->SetFillColor(kBlue-9);
    h_ks_out->SetLineColor(kRed);
    h_ks_out->SetLineWidth(3);
    h_ks_out->SetFillColor(kRed-9);

    THStack* hs_ks = new THStack("hs_ks", ";Range [cm];Events");

    // **************** Draw Ks ********************

    c_ks->cd();

    hs_ks->Add(h_ks_in);
    hs_ks->Add(h_ks_out);

    hs_ks->Draw("hist");

    
    hs_ks->GetXaxis()->SetTitleSize(0.05);
    hs_ks->GetXaxis()->SetLabelSize(0.07);
    hs_ks->GetYaxis()->SetTitleSize(0.05);
    hs_ks->GetYaxis()->SetLabelSize(0.07);
    hs_ks->GetXaxis()->SetTitleOffset(1.5);


    //Add legend
    TLegend *legend_s = new TLegend(0.7, 0.75, 0.9, 0.9); 
    legend_s->AddEntry(h_ks_in, "In", "f");
    legend_s->AddEntry(h_ks_out, "Out", "f");
    legend_s->Draw();

    //Modify margin
    c_ks->SetLeftMargin(0.15);  
    c_ks->SetRightMargin(0.1);  
    c_ks->SetTopMargin(0.10);   
    c_ks->SetBottomMargin(0.15);

    c_ks->Update();    

    cout << endl << "K0 que acaban dentro del detector: " << h_ks_in->Integral() << endl;
    cout << "K0 que acaban fuera del detector: " << h_ks_out->Integral() << endl;
}