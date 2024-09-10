#include "../includes/tree_utils.cpp"
#include "../includes/Includes.h"
#include "../includes/read_event_tree.C"
#include "../includes/read_subrun_tree.C"

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

/*
******* Classifying the amount of k0s and k0l in k0 production events *****
*/

void k0_neutrinoE(){
    cout << "k0_neutrinoE_all" << endl;
    cout << "k0_neutrinoE_split" << endl;
}

void k0_neutrinoE_all(){

    TTree *event_tree;
    TTree *subrun_tree;

    event_tree = (TTree*)read_event_tree();
    subrun_tree = (TTree*)read_subrun_tree();

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;


    //Create canvas and histogram 
    TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
    TH1F *h_res = new TH1F("h1", "", 15, 0, 7);
    TH1F *h_discc = new TH1F("h2", "", 15, 0 , 7);
    TH1F *h_disnc = new TH1F("h3", "", 15, 0 , 7);
    TH1F *h_other = new TH1F("h4", "", 15, 0 , 7);

    int n_res = 0;
    int n_discc = 0;
    int n_disnc = 0;

    bool k0_found;

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        k0_found = false;
        
        //Loop over ALL gen particles in i_e-th event and show info if there are stable final state k0
        for(int j = 0; j < size_gen; j++){
            if (gen_part_statusCode->at(j) == 1){
                if (gen_part_PDGcode->at(j) == 311){
                    k0_found = true;
                    if (nu_interaction_mode == 1){
                        n_res++;
                        h_res->Fill(nu_E0);
                    } else if (nu_interaction_mode == 2){
                        if (nu_interaction_type == 1091){
                            n_discc++;
                            h_discc->Fill(nu_E0);
                        } else if (nu_interaction_type == 1092){
                            h_disnc->Fill(nu_E0);
                        }
                    }
                }
            }
        }

        if (!k0_found){
            //cout << "Interaction mode: " << nu_interaction_mode << endl;
            //cout << "Interaction type: " << nu_interaction_type << endl<< endl << endl;
            h_other->Fill(nu_E0);
        }
    }

    //Color settings
    h_res->SetLineColor(kBlue);
    h_res->SetLineWidth(3);
    h_res->SetFillColor(kBlue-9);
    h_res->SetFillStyle(1001);

    h_discc->SetLineColor(kGreen);
    h_discc->SetLineWidth(3);
    h_discc->SetFillColor(kGreen-9);
    h_discc->SetFillStyle(1001);

    h_disnc->SetLineColor(kRed);
    h_disnc->SetLineWidth(3);
    h_disnc->SetFillColor(kRed-9);
    h_disnc->SetFillStyle(1001);

    h_other->SetLineColor(kBlack);
    h_other->SetLineWidth(3);
    h_other->SetFillColor(kGray);
    h_other->SetFillStyle(1001);

    //Scaling histogram for one year production (3y = 10e21 POT)
    double n_POT = get_total_POT(subrun_tree);

    //Create THStack histogram
    THStack *hs = new THStack("hs", ";Neutrino E0;Number of events");

    hs->Add(h_discc);
    hs->Add(h_disnc);
    hs->Add(h_res);
    hs->Add(h_other);

    hs->Draw("hist");
    
    hs->GetXaxis()->SetTitleSize(0.05);
    hs->GetYaxis()->SetTitleSize(0.05);
    hs->GetXaxis()->SetLabelSize(0.05);
    hs->GetYaxis()->SetLabelSize(0.05);

    

    //Add legend
    TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9); 
    legend->AddEntry(h_res, "RES", "f");
    legend->AddEntry(h_discc, "DIS CC", "f");
    legend->AddEntry(h_disnc, "DIS NC", "f");
    legend->AddEntry(h_other, "OTHER", "f");
    legend->Draw();

    c1->SetLogy();
    c1->Update();
    

}




void k0_neutrinoE_split(){

    TTree *event_tree;
    TTree *subrun_tree;

    event_tree = (TTree*)read_event_tree();
    subrun_tree = (TTree*)read_subrun_tree();

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;


    //Create canvas and histogram 
    TCanvas *c_res = new TCanvas("c1", "canvas1", 800, 600);
    TCanvas *c_discc = new TCanvas("c2", "canvas2", 800, 600);
    TCanvas *c_disnc = new TCanvas("c3", "canvas3", 800, 600);
    TH1F *h_res = new TH1F("h1", "", 15, 0 , 7);
    TH1F *h_discc = new TH1F("h2", "", 15, 0 , 7);
    TH1F *h_disnc = new TH1F("h3", "", 15, 0 , 7);
    TH1F *h_other_res = new TH1F("h4", "", 15, 0 , 7);
    TH1F *h_other_discc = new TH1F("h5", "", 15, 0 , 7);
    TH1F *h_other_disnc = new TH1F("h6", "", 15, 0 , 7);

    int n_res = 0;
    int n_discc = 0;
    int n_disnc = 0;
    int n_res_nok0 = 0;
    int n_discc_nok0 = 0;
    int n_disnc_nok0 = 0;

    bool k0_found;

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        k0_found = false;
        
        //Loop over ALL gen particles in i_e-th event and show info if there are stable final state k0
        for(int j = 0; j < size_gen; j++){
            if (gen_part_statusCode->at(j) == 1){
                if (gen_part_PDGcode->at(j) == 311){
                    k0_found = true;
                    if (nu_interaction_mode == 1){
                        n_res++;
                        h_res->Fill(nu_E0);
                    } else if (nu_interaction_mode == 2){
                        if (nu_interaction_type == 1091){
                            n_discc++;
                            h_discc->Fill(nu_E0);
                        } else if (nu_interaction_type == 1092){
                            h_disnc->Fill(nu_E0);
                        }
                    }
                }
            }
        }

        if (!k0_found){
            if (nu_interaction_mode == 1){
                n_res_nok0++;
                h_other_res->Fill(nu_E0);
            } else if ((nu_interaction_mode == 2) & (nu_interaction_type == 1091)){
                n_discc_nok0++;
                h_other_discc->Fill(nu_E0);
            } else if ((nu_interaction_mode == 2) & (nu_interaction_type == 1092)){
                n_disnc_nok0++;
                h_other_disnc->Fill(nu_E0);
            }
        }
    }

    cout << "Resonance without k0: " << n_res_nok0 << endl;
    cout << "DIS cc without k0: " << n_discc_nok0 << endl;
    cout << "Dis nc without k0: " << n_disnc_nok0 << endl;

    //Color settings
    h_res->SetLineColor(kBlue);
    h_res->SetLineWidth(3);
    h_res->SetFillColor(kBlue-9);
    h_res->SetFillStyle(1001);
    h_res->SetStats(0);

    h_discc->SetLineColor(kGreen);
    h_discc->SetLineWidth(3);
    h_discc->SetFillColor(kGreen-9);
    h_discc->SetFillStyle(1001);
    h_discc->SetStats(0);

    h_disnc->SetLineColor(kRed);
    h_disnc->SetLineWidth(3);
    h_disnc->SetFillColor(kRed-9);
    h_disnc->SetFillStyle(1001);
    h_disnc->SetStats(0);

    h_other_res->SetLineColor(kBlack);
    h_other_res->SetLineWidth(3);
    h_other_res->SetFillColor(kGray);
    h_other_res->SetFillStyle(1001);
    h_other_res->SetStats(0);

    h_other_discc->SetLineColor(kBlack);
    h_other_discc->SetLineWidth(3);
    h_other_discc->SetFillColor(kGray);
    h_other_discc->SetFillStyle(1001);
    h_other_discc->SetStats(0);

    h_other_disnc->SetLineColor(kBlack);
    h_other_disnc->SetLineWidth(3);
    h_other_disnc->SetFillColor(kGray);
    h_other_disnc->SetFillStyle(1001);
    h_other_disnc->SetStats(0);

    //Scaling histogram for one year production (3y = 10e21 POT)
    double n_POT = get_total_POT(subrun_tree);

    //Create THStack histogram
    THStack *hs_res = new THStack("hs1", "RES;Neutrino E0;Number of events");
    THStack *hs_discc = new THStack("hs2", "DIS CC;Neutrino E0;Number of events");
    THStack *hs_disnc = new THStack("hs3", "DIS NC;Neutrino E0;Number of events");

    //Stack histograms
    hs_discc->Add(h_discc);
    hs_discc->Add(h_other_discc);

    hs_disnc->Add(h_disnc);
    hs_disnc->Add(h_other_disnc);

    hs_res->Add(h_res);
    hs_res->Add(h_other_res);


    //Add legend
    TLegend *leg_res = new TLegend(0.7, 0.7, 0.9, 0.9); 
    TLegend *leg_discc = new TLegend(0.7, 0.7, 0.9, 0.9); 
    TLegend *leg_disnc = new TLegend(0.7, 0.7, 0.9, 0.9); 

    leg_res->AddEntry(h_res, "k0 RES", "f");
    leg_discc->AddEntry(h_discc, "k0 DIS CC", "f");
    leg_disnc->AddEntry(h_disnc, "k0 DIS NC", "f");
    leg_res->AddEntry(h_other_res, "OTHER RES", "f");
    leg_discc->AddEntry(h_other_discc, "OTHER DIS CC", "f");
    leg_disnc->AddEntry(h_other_disnc, "OTHER DIS NC", "f");

    //Add text
    TLatex latex;
    double frac_res;
    double frac_discc;
    double frac_disnc;

    //Obtain total of events in each histogram
    double k0_res = h_res->Integral();
    double k0_discc = h_discc->Integral();
    double k0_disnc = h_disnc->Integral();
    double nok0_res = h_other_res->Integral();
    double nok0_discc = h_other_discc->Integral();
    double nok0_disnc = h_other_disnc->Integral();

    frac_res = k0_res/(k0_res + nok0_res);
    frac_discc = k0_discc/(k0_discc + nok0_discc);
    frac_disnc = k0_disnc/(k0_disnc + nok0_disnc);


    latex.SetTextSize(0.04);  //Text size
   
    //Draw histograms
    c_res->cd();
    hs_res->Draw("hist");
    leg_res->Draw();
    
    hs_res->GetXaxis()->SetTitleSize(0.05);
    hs_res->GetYaxis()->SetTitleSize(0.05);
    hs_res->GetXaxis()->SetLabelSize(0.05);
    hs_res->GetYaxis()->SetLabelSize(0.05);

    latex.DrawLatex(2, h_res->GetMaximum() * 1.2, Form("Ratio of K0: %.2f%%", frac_res*100)); //Draw text
    
    //SetRangeUser does not work with THStack
    hs_res->SetMinimum(0.5);
    hs_res->SetMaximum(1.5e4);
    c_res->SetLogy();
    c_res->Update();


    c_discc->cd();
    hs_discc->Draw("hist");
    leg_discc->Draw();
    
    hs_discc->GetXaxis()->SetTitleSize(0.05);
    hs_discc->GetYaxis()->SetTitleSize(0.05);
    hs_discc->GetXaxis()->SetLabelSize(0.05);
    hs_discc->GetYaxis()->SetLabelSize(0.05);

    latex.DrawLatex(2, h_discc->GetMaximum() * 1.2, Form("Ratio of K0: %.2f%%", frac_discc*100)); //Draw text
    
    c_discc->SetLogy();
    c_discc->Update();



    c_disnc->cd();
    hs_disnc->Draw("hist");
    leg_disnc->Draw();
    
    hs_disnc->GetXaxis()->SetTitleSize(0.05);
    hs_disnc->GetYaxis()->SetTitleSize(0.05);
    hs_disnc->GetXaxis()->SetLabelSize(0.05);
    hs_disnc->GetYaxis()->SetLabelSize(0.05);

    latex.DrawLatex(2, h_disnc->GetMaximum() * 1.2, Form("Ratio of K0: %.2f%%", frac_disnc*100)); //Draw text
    
    c_disnc->SetLogy();
    c_disnc->Update();
}