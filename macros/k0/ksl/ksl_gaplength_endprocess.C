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

void ksl_gaplength_endprocess(){

    TTree *event_tree;
    TTree *subrun_tree;

    double fv = 10;

    string data_dir = "/home/sergioyb/root/kaon_analysis/data/analysis_output_truth_BNB_single_100k_events.root";

    event_tree = (TTree*)read_event_tree(data_dir);
    subrun_tree = (TTree*)read_subrun_tree(data_dir);

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;

    auto bins_tuple_s = binsGen(0,10,100, 5,18);
    int nbins_s = bins_tuple_s.first;
    std::vector<double> bins_s = bins_tuple_s.second;

    auto bins_tuple_l = binsGen(0,20,100, 10,16);
    int nbins_l = bins_tuple_l.first;
    std::vector<double> bins_l = bins_tuple_l.second;

    
    
    //Create canvas and histogram 
    TCanvas *c_ks = new TCanvas("c_ks", "cs", 800, 600);
    TCanvas *c_kl = new TCanvas("c_kl", "cl", 800, 600);
    TH1F *h_ks_dec = new TH1F("h_ks_dec", "", nbins_s, &bins_s[0]); 
    TH1F *h_ks_ine = new TH1F("h_ks_ine", "", nbins_s, &bins_s[0]);
    TH1F *h_ks_dec_small = new TH1F("h_ks_dec_small", "", 15, 0, 4); 
    TH1F *h_ks_ine_small = new TH1F("h_ks_ine_small", "", 15, 0, 4);
    TH1F *h_kl_dec = new TH1F("h_ks_dec", "", nbins_l, &bins_l[0]);  
    TH1F *h_kl_ine = new TH1F("h_ks_ine", "", nbins_l, &bins_l[0]); 

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();


        //Loop over g4 particles to finds primary k0 created inside detector
        for(int i_p = 0; i_p < size_g4; i_p++){
            if((g4_part_PDGcode->at(i_p) == 311) & (g4_part_process->at(i_p) == "primary") & (starts_inside_detector(event_tree, i_p, fv))){

                //Loop over g4 particles to find k0s or l and check if it is inside or outside
                for (int j_p = 0; j_p < size_g4; j_p++){

                    //Find ks
                    if ((g4_part_mother->at(j_p) == g4_part_trackID->at(i_p)) & (g4_part_PDGcode->at(j_p) == 310)){
                        if (ends_inside_detector(event_tree, j_p, fv)){
                            if (g4_part_end_process->at(j_p) == "Decay"){
                                h_ks_dec->Fill(trackLength(event_tree, j_p));
                                h_ks_dec_small->Fill(trackLength(event_tree, j_p));
                            } else {
                                h_ks_ine->Fill(trackLength(event_tree, j_p));
                                h_ks_ine_small->Fill(trackLength(event_tree, j_p));
                            }
                            
                        }

                    }

                    //Find kl
                    if ((g4_part_mother->at(j_p) == g4_part_trackID->at(i_p)) & (g4_part_PDGcode->at(j_p) == 130)){
                        if (ends_inside_detector(event_tree, j_p, fv)){
                            if (g4_part_end_process->at(j_p) == "Decay"){
                                h_kl_dec->Fill(trackLength(event_tree, j_p));
                            } else {
                                h_kl_ine->Fill(trackLength(event_tree, j_p));
                            }
                            
                        }

                    }
                }
            }
        }
        
    }

    //Draw histograms
    h_ks_dec->SetLineColor(kBlue);
    h_ks_dec->SetLineWidth(3);
    h_ks_dec->SetFillColor(kBlue-9);
    h_ks_ine->SetLineColor(kGreen);
    h_ks_ine->SetLineWidth(3);
    h_ks_ine->SetFillColor(kGreen-9);

    h_ks_dec_small->SetLineColor(kBlue);
    h_ks_dec_small->SetLineWidth(3);
    h_ks_dec_small->SetFillColor(kBlue-9);
    h_ks_ine_small->SetLineColor(kGreen);
    h_ks_ine_small->SetLineWidth(3);
    h_ks_ine_small->SetFillColor(kGreen-9);
    
    h_kl_dec->SetLineColor(kBlue);
    h_kl_dec->SetLineWidth(3);
    h_kl_dec->SetFillColor(kBlue-9);
    h_kl_ine->SetLineColor(kGreen);
    h_kl_ine->SetLineWidth(3);
    h_kl_ine->SetFillColor(kGreen-9);

    THStack* hs_ks = new THStack("hs_ks", "K^{0}_{S};Range [cm];Events");
    THStack* hs_kl = new THStack("hs_kl", "K^{0}_{L};Range [cm];Events");
    THStack* hs_ks_small = new THStack("hs_ks_small", ";;");

    // **************** Draw Ks ********************

    c_ks->cd();

    hs_ks->Add(h_ks_dec);
    hs_ks->Add(h_ks_ine);

    hs_ks_small->Add(h_ks_dec_small);
    hs_ks_small->Add(h_ks_ine_small);

    hs_ks->Draw("hist");

    
    hs_ks->GetXaxis()->SetTitleSize(0.05);
    hs_ks->GetXaxis()->SetLabelSize(0.07);
    hs_ks->GetYaxis()->SetTitleSize(0.05);
    hs_ks->GetYaxis()->SetLabelSize(0.07);
    hs_ks->GetXaxis()->SetTitleOffset(1.5);


    //Add legend
    TLegend *legend_s = new TLegend(0.7, 0.75, 0.9, 0.9); 
    legend_s->AddEntry(h_ks_dec, "Decay", "f");
    legend_s->AddEntry(h_ks_ine, "Inelastic", "f");
    legend_s->Draw();

    //Draw small histogram for k_s with more precision

    TPad *pad = new TPad("pad", "Inset", 0.2, 0.2, 0.9, 0.7); // (x1, y1, x2, y2) en coordenadas normalizadas
    pad->SetFillStyle(4000); // Hacer transparente el fondo del pad
    pad->Draw(); // Dibujar el pad en el canvas principal
    pad->cd();   // Cambiar el contexto al pad pequeño

    hs_ks_small->Draw("hist");

    hs_ks_small->GetXaxis()->SetTitleSize(0.07);
    hs_ks_small->GetXaxis()->SetLabelSize(0.1);
    hs_ks_small->GetYaxis()->SetTitleSize(0.07);
    hs_ks_small->GetYaxis()->SetLabelSize(0.1);

    //Modify margin
    c_ks->SetLeftMargin(0.15);  
    c_ks->SetRightMargin(0.1);  
    c_ks->SetTopMargin(0.10);   
    c_ks->SetBottomMargin(0.15);

    c_ks->Update();

    // **************** Draw Kl ********************

    c_kl->cd();

    hs_kl->Add(h_kl_dec);
    hs_kl->Add(h_kl_ine);

    hs_kl->Draw("hist");
    
    hs_kl->GetXaxis()->SetTitleSize(0.05);
    hs_kl->GetXaxis()->SetLabelSize(0.07);
    hs_kl->GetYaxis()->SetTitleSize(0.05);
    hs_kl->GetYaxis()->SetLabelSize(0.07);
    hs_kl->GetXaxis()->SetTitleOffset(1.5);


    //Add legend
    TLegend *legend_l = new TLegend(0.7, 0.75, 0.9, 0.9); 
    legend_l->AddEntry(h_kl_dec, "Decay", "f");
    legend_l->AddEntry(h_kl_ine, "Inelastic", "f");
    legend_l->Draw();

    //Modify margin
    c_kl->SetLeftMargin(0.15);  
    c_kl->SetRightMargin(0.1);  
    c_kl->SetTopMargin(0.10);   
    c_kl->SetBottomMargin(0.15);

    c_kl->Update();    
    
}