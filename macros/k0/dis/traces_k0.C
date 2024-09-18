#include "/home/sergioyb/root/kaon_analysis/macros/includes/tree_utils.cpp"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/Includes.h"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_event_tree.C"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_subrun_tree.C"
#include <iostream>
#include <set>
#include <vector>
#include <string>



std::tuple<std::vector<int>, std::vector<int>> find_events_k0(TTree* event_tree, int nu_inter_mode, int nu_inter_type){
    //Returns two vector
    //  one with events with k0 and no ak0
    //  another with events with both k0 and ak0
    //Both with the given interaction mode
    int n_events = event_tree->GetEntries();
    float size_g4;

    bool k0_found;
    bool ak0_found;

    std::vector<int> events_k0;
    std::vector<int> events_k0ak0;

    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();

        //Initialice finding booleans
        k0_found = false;
        ak0_found = false;

        for(int i_p = 0; i_p < size_g4; i_p++){
            if((nu_interaction_mode == nu_inter_mode) & (nu_interaction_type == nu_inter_type)){
                if ((g4_part_PDGcode->at(i_p) == 311) & (g4_part_process->at(i_p) == "primary")){
                    k0_found = true;
                } else if ((g4_part_PDGcode->at(i_p) == -311) & (g4_part_process->at(i_p) == "primary")){
                    ak0_found = true;
                }
            }
        }

        if (k0_found & ak0_found){
            events_k0ak0.push_back(i_e);
        } else if (k0_found){
            events_k0.push_back(i_e);
        }
    }

    return std::make_tuple(events_k0, events_k0ak0);

}



void traces_k0(){

    int nu_inter_mode = 2;
    int nu_inter_type = 1091;

    TTree *event_tree;
    TTree *subrun_tree;

    string data_dir = "/home/sergioyb/root/kaon_analysis/data/analysis_output_truth_BNB_single_100k_events.root";

    event_tree = (TTree*)read_event_tree(data_dir);
    //subrun_tree = (TTree*)read_subrun_tree(data_dir);

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;
    int i_e; //Event number

    bool par_found;

    //Get events with k0 and ak0
    std::vector<int> events_k0 = std::get<0>(find_events_k0(event_tree, nu_inter_mode, nu_inter_type));
    std::vector<int> events_k0ak0 = std::get<1>(find_events_k0(event_tree, nu_inter_mode, nu_inter_type));
    

    //Quark counting for barion charge
    int q1;
    int q2;
    int q3;
    int B_charge;

    //Create canvas and histogram 
    TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
    TCanvas *c2 = new TCanvas("c2", "canvas2", 800, 600);

    TH2F *h_k0 = new TH2F("h_k0", "", 3, 0.5 , 3.5, 3, 0.5, 3.5); //x-> 0,1,2+ traces ; y-> 0,1,2+ pi0
    TH2F *h_k0ak0 = new TH2F("h_k0ak0", "", 3, 0.5 , 3.5, 3, 0.5, 3.5); 


    //Create partial histograms to be added as a single one
    TH2F *h_B0 = new TH2F("h_B0", "", 3, 0.5 , 3.5, 3, 0.5, 3.5); 
    TH2F *h_Bpm = new TH2F("h_Bpm", "", 3, 0.5 , 3.5, 3, 0.5, 3.5);  
    TH2F *h_noB = new TH2F("h_noB0", "", 3, 0.5 , 3.5, 3, 0.5, 3.5);  

    //Magnitudes of the histograms
    int n_trace;
    int n_pi0;
    int n_B0;
    int n_Bpm;
    

    //Loop over events with k0 and no ak0
    for (size_t i_v = 0; i_v < events_k0.size(); ++i_v){
        i_e = events_k0[i_v];

        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();

        //Initialize counters
        n_trace = 0; 
        n_pi0 = 0;
        n_B0 = 0;
        n_Bpm = 0;

        par_found = false;

        for (int j_p = 0; j_p < size_g4; j_p++){
            par_found = false;
            if (g4_part_process->at(j_p) == "primary"){ //Both conditions equivalent?

                if ((g4_part_PDGcode->at(j_p) == 2212) || (std::abs(g4_part_PDGcode->at(j_p)) == 211)){
                    n_trace++;
                    par_found = true;
                }  else if ((std::abs(g4_part_PDGcode->at(j_p)) == 111)){
                    n_pi0++;
                    par_found = true;
                }

                // Find other farticles 
                if (!par_found & (g4_part_PDGcode->at(j_p) >= 2200 & g4_part_PDGcode->at(j_p) <= 6200)){
                    q1 = g4_part_PDGcode->at(j_p) / 1000;        // División entre 1000 para obtener la primera cifra
                    q2 = (g4_part_PDGcode->at(j_p) / 100) % 10;  // División entre 100 y módulo 10 para obtener la segunda cifra
                    q3 = (g4_part_PDGcode->at(j_p) / 10) % 10;
                    B_charge = (((q1+1) % 2 *3 - 1) + ((q2+1) % 2 *3 - 1) + ((q3+1) % 2 *3 - 1)) / 3;

                    //Distinguish charged baryon from neutral one
                    if (B_charge == 0){
                        //cout << g4_part_PDGcode->at(j_p) << "    " << B_charge << endl;
                        n_B0++;
                    } else {
                        //cout << g4_part_PDGcode->at(j_p) << "    " << B_charge << endl;
                        n_Bpm++;
                    }
                    
                    //n_par["B"]++;
                    par_found = true;

                } 
            }
        }

        //Fill histograms
        h_k0->Fill(fmin(n_trace + 1,3),fmin(n_pi0 + 1,3));

        if (n_B0){
            h_B0->Fill(fmin(n_trace + 1,3),fmin(n_pi0 + 1,3));
        } else if (n_Bpm){
            h_Bpm->Fill(fmin(n_trace + 1,3),fmin(n_pi0 + 1,3));
        } else {
            h_noB->Fill(fmin(n_trace + 1,3),fmin(n_pi0 + 1,3));
        }
    }

    //Draw k0 histogram

    c1->cd();

    h_k0->SetStats(0);

    h_k0->GetXaxis()->SetBinLabel(1, "0");
    h_k0->GetXaxis()->SetBinLabel(2, "1");
    h_k0->GetXaxis()->SetBinLabel(3, "2+");

    h_k0->GetYaxis()->SetBinLabel(1, "0");
    h_k0->GetYaxis()->SetBinLabel(2, "1");
    h_k0->GetYaxis()->SetBinLabel(3, "2+");

    h_k0->GetXaxis()->SetTitleSize(0.05);
    h_k0->GetYaxis()->SetTitleSize(0.05);
    h_k0->GetXaxis()->SetLabelSize(0.05);
    h_k0->GetYaxis()->SetLabelSize(0.05);

    h_k0->GetXaxis()->SetTitle("Primary traces");
    h_k0->GetYaxis()->SetTitle("#pi^{0} count");

    h_k0->SetTitle("DIS CC wo #bar{K}^{0}");

    gStyle->SetPalette(kViridis);


    h_k0->Draw("COLZ");

    //Add text to k0 histogram
    TLatex latex;
    TLatex latex_white;

    latex.SetTextSize(0.04);
    latex_white.SetTextSize(0.04);
    latex_white.SetTextColor(kWhite);
    latex.SetTextAlign(22);
    latex_white.SetTextAlign(22);

    double max_h_k0 = h_k0->GetMaximum();

    for (int x = 1; x <= h_k0->GetNbinsX(); x++){
        for (int y = 1; y <= h_k0->GetNbinsY(); y++){
            //Get bin content
            int nbin_k0 = h_k0->GetBinContent(x, y);
            int nbin_B0 = h_B0->GetBinContent(x, y);
            int nbin_Bpm = h_Bpm->GetBinContent(x, y);
            int nbin_noB = h_noB->GetBinContent(x, y);

            if (nbin_k0 > max_h_k0/2){
                latex.DrawLatex(h_k0->GetXaxis()->GetBinCenter(x), h_k0->GetYaxis()->GetBinCenter(y)+0.3, Form("Total = %d", nbin_k0));
                latex.DrawLatex(h_k0->GetXaxis()->GetBinCenter(x), h_k0->GetYaxis()->GetBinCenter(y)+0.15, Form("B^{0}: %d (%.2f%%)", nbin_B0, 100.0*nbin_B0/nbin_k0));
                latex.DrawLatex(h_k0->GetXaxis()->GetBinCenter(x), h_k0->GetYaxis()->GetBinCenter(y), Form("B^{#pm}: %d (%.2f%%)", nbin_Bpm, 100.0*nbin_Bpm/nbin_k0));
                latex.DrawLatex(h_k0->GetXaxis()->GetBinCenter(x), h_k0->GetYaxis()->GetBinCenter(y)-0.15, Form("B-less: %d (%.2f%%)", nbin_noB, 100.0*nbin_noB/nbin_k0));
            } else if (nbin_k0){
                latex_white.DrawLatex(h_k0->GetXaxis()->GetBinCenter(x), h_k0->GetYaxis()->GetBinCenter(y)+0.3, Form("Total = %d", nbin_k0));
                latex_white.DrawLatex(h_k0->GetXaxis()->GetBinCenter(x), h_k0->GetYaxis()->GetBinCenter(y)+0.15, Form("B^{0}: %d (%.2f%%)", nbin_B0, 100.0*nbin_B0/nbin_k0));
                latex_white.DrawLatex(h_k0->GetXaxis()->GetBinCenter(x), h_k0->GetYaxis()->GetBinCenter(y), Form("B^{#pm}: %d (%.2f%%)", nbin_Bpm, 100.0*nbin_Bpm/nbin_k0));
                latex_white.DrawLatex(h_k0->GetXaxis()->GetBinCenter(x), h_k0->GetYaxis()->GetBinCenter(y)-0.15, Form("B-less: %d (%.2f%%)", nbin_noB, 100.0*nbin_noB/nbin_k0));
            }
        }
    }

    c1->Update();

    //c1->SaveAs("/home/sergioyb/root/kaon_analysis/histograms/k0/dis/traces/traces_pi0_discc_noak0.pdf");

    
    TCanvas *cB0 = new TCanvas("cB0", "", 800, 600);
    TCanvas *cBpm = new TCanvas("cBpm", "", 800, 600);
    TCanvas *cnoB = new TCanvas("cnoB", "", 800, 600); 

    // *********************B0 ********************************* 
    
    cB0->cd();

    h_B0->SetStats(0);

    h_B0->GetXaxis()->SetBinLabel(1, "0");
    h_B0->GetXaxis()->SetBinLabel(2, "1");
    h_B0->GetXaxis()->SetBinLabel(3, "2+");

    h_B0->GetYaxis()->SetBinLabel(1, "0");
    h_B0->GetYaxis()->SetBinLabel(2, "1");
    h_B0->GetYaxis()->SetBinLabel(3, "2+");

    h_B0->GetXaxis()->SetTitleSize(0.05);
    h_B0->GetYaxis()->SetTitleSize(0.05);
    h_B0->GetXaxis()->SetLabelSize(0.05);
    h_B0->GetYaxis()->SetLabelSize(0.05);

    h_B0->GetXaxis()->SetTitle("Primary traces");
    h_B0->GetYaxis()->SetTitle("#pi^{0} count");

    h_B0->SetTitle("DIS CC w B^{0} wo #bar{K}^{0}");

    gStyle->SetPalette(kViridis);


    h_B0->Draw("COLZ ");

    double max_h_B0 = h_B0->GetMaximum();

    for (int x = 1; x <= h_B0->GetNbinsX(); x++){
        for (int y = 1; y <= h_B0->GetNbinsY(); y++){
            //Get bin content
            int nbin_B0 = h_B0->GetBinContent(x, y);

            if (nbin_B0 > max_h_B0/2){
                latex.DrawLatex(h_B0->GetXaxis()->GetBinCenter(x), h_B0->GetYaxis()->GetBinCenter(y)+0.15, Form("%d", nbin_B0));
            } else if (nbin_B0){
                latex_white.DrawLatex(h_B0->GetXaxis()->GetBinCenter(x), h_B0->GetYaxis()->GetBinCenter(y)+0.15, Form("%d", nbin_B0));
            }
        }
    }
    
    cB0->Update();

    //cB0->SaveAs("/home/sergioyb/root/kaon_analysis/histograms/k0/dis/traces/traces_pi0_discc_noak0_B0.pdf");

    // ***************** Bpm ****************************

    cBpm->cd();


    h_Bpm->SetStats(0);

    h_Bpm->GetXaxis()->SetBinLabel(1, "0");
    h_Bpm->GetXaxis()->SetBinLabel(2, "1");
    h_Bpm->GetXaxis()->SetBinLabel(3, "2+");

    h_Bpm->GetYaxis()->SetBinLabel(1, "0");
    h_Bpm->GetYaxis()->SetBinLabel(2, "1");
    h_Bpm->GetYaxis()->SetBinLabel(3, "2+");

    h_Bpm->GetXaxis()->SetTitleSize(0.05);
    h_Bpm->GetYaxis()->SetTitleSize(0.05);
    h_Bpm->GetXaxis()->SetLabelSize(0.05);
    h_Bpm->GetYaxis()->SetLabelSize(0.05);

    h_Bpm->GetXaxis()->SetTitle("Primary traces");
    h_Bpm->GetYaxis()->SetTitle("#pi^{0} count");

    h_Bpm->SetTitle("DIS CC w B^{#pm} wo #bar{K}^{0}");

    gStyle->SetPalette(kViridis);


    h_Bpm->Draw("COLZ");

    double max_h_Bpm = h_Bpm->GetMaximum();

    for (int x = 1; x <= h_Bpm->GetNbinsX(); x++){
        for (int y = 1; y <= h_Bpm->GetNbinsY(); y++){
            //Get bin content
            int nbin_Bpm = h_Bpm->GetBinContent(x, y);

            if (nbin_Bpm > max_h_Bpm/2){
                latex.DrawLatex(h_Bpm->GetXaxis()->GetBinCenter(x), h_Bpm->GetYaxis()->GetBinCenter(y)+0.15, Form("%d", nbin_Bpm));
            } else if (nbin_Bpm){
                latex_white.DrawLatex(h_Bpm->GetXaxis()->GetBinCenter(x), h_Bpm->GetYaxis()->GetBinCenter(y)+0.15, Form("%d", nbin_Bpm));
            }
        }
    }

    cBpm->Update();
    //cBpm->SaveAs("/home/sergioyb/root/kaon_analysis/histograms/k0/dis/traces/traces_pi0_discc_noak0_Bpm.pdf");


    // ******************************** no B ********************************
    cnoB->cd();

    h_noB->SetStats(0);

    h_noB->GetXaxis()->SetBinLabel(1, "0");
    h_noB->GetXaxis()->SetBinLabel(2, "1");
    h_noB->GetXaxis()->SetBinLabel(3, "2+");

    h_noB->GetYaxis()->SetBinLabel(1, "0");
    h_noB->GetYaxis()->SetBinLabel(2, "1");
    h_noB->GetYaxis()->SetBinLabel(3, "2+");

    h_noB->GetXaxis()->SetTitleSize(0.05);
    h_noB->GetYaxis()->SetTitleSize(0.05);
    h_noB->GetXaxis()->SetLabelSize(0.05);
    h_noB->GetYaxis()->SetLabelSize(0.05);

    h_noB->GetXaxis()->SetTitle("Primary traces");
    h_noB->GetYaxis()->SetTitle("#pi^{0} count");

    h_noB->SetTitle("DIS CC wo B wo #bar{K}^{0}");

    gStyle->SetPalette(kViridis);


    h_noB->Draw("COLZ");

    double max_h_noB = h_noB->GetMaximum();

    for (int x = 1; x <= h_noB->GetNbinsX(); x++){
        for (int y = 1; y <= h_noB->GetNbinsY(); y++){
            //Get bin content
            int nbin_noB = h_noB->GetBinContent(x, y);

            if (nbin_noB > max_h_noB/2){
                latex.DrawLatex(h_noB->GetXaxis()->GetBinCenter(x), h_noB->GetYaxis()->GetBinCenter(y)+0.15, Form("%d", nbin_noB));
            } else if (nbin_noB){
                latex_white.DrawLatex(h_noB->GetXaxis()->GetBinCenter(x), h_noB->GetYaxis()->GetBinCenter(y)+0.15, Form("%d", nbin_noB));
            }
        }
    } 

    cnoB->Update();
    //cnoB->SaveAs("/home/sergioyb/root/kaon_analysis/histograms/k0/dis/traces/traces_pi0_discc_noak0_noB.pdf");
    

    
    //Reset hitograms
    h_B0->Reset();
    h_Bpm->Reset();
    h_noB->Reset();



    //Loop over events with k0 and ak0
    for (size_t i_v = 0; i_v < events_k0ak0.size(); ++i_v){
        i_e = events_k0ak0[i_v];

        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();

        //Initialize counters
        n_trace = 0; 
        n_pi0 = 0;
        n_B0 = 0;
        n_Bpm = 0;

        par_found = false;

        for (int j_p = 0; j_p < size_g4; j_p++){
            par_found = false;
            if (g4_part_process->at(j_p) == "primary"){ //Both conditions equivalent?

                if ((g4_part_PDGcode->at(j_p) == 2212) || (std::abs(g4_part_PDGcode->at(j_p)) == 211)){
                    n_trace++;
                    par_found = true;
                }  else if ((std::abs(g4_part_PDGcode->at(j_p)) == 111)){
                    n_pi0++;
                    par_found = true;
                }

                // Find other farticles 
                if (!par_found & (g4_part_PDGcode->at(j_p) >= 2200 & g4_part_PDGcode->at(j_p) <= 6200)){
                    q1 = g4_part_PDGcode->at(j_p) / 1000;        // División entre 1000 para obtener la primera cifra
                    q2 = (g4_part_PDGcode->at(j_p) / 100) % 10;  // División entre 100 y módulo 10 para obtener la segunda cifra
                    q3 = (g4_part_PDGcode->at(j_p) / 10) % 10;
                    B_charge = (((q1+1) % 2 *3 - 1) + ((q2+1) % 2 *3 - 1) + ((q3+1) % 2 *3 - 1)) / 3;

                    //Distinguish charged baryon from neutral one
                    if (B_charge == 0){
                        //cout << g4_part_PDGcode->at(j_p) << "    " << B_charge << endl;
                        n_B0++;
                    } else {
                        //cout << g4_part_PDGcode->at(j_p) << "    " << B_charge << endl;
                        n_Bpm++;
                    }
                    
                    //n_par["B"]++;
                    par_found = true;

                } 
            }
        }

        //Fill histograms
        h_k0ak0->Fill(fmin(n_trace + 1,3),fmin(n_pi0 + 1,3));

        if (n_B0){
            h_B0->Fill(fmin(n_trace+1,3),fmin(n_pi0 + 1,3));
        } else if (n_Bpm){
            h_Bpm->Fill(fmin(n_trace+1,3),fmin(n_pi0 + 1,3));
        } else {
            h_noB->Fill(fmin(n_trace+1,3),fmin(n_pi0 + 1,3));
        }
    }

    //Draw k0 histogram

    c2->cd();

    h_k0ak0->SetStats(0);

    h_k0ak0->GetXaxis()->SetBinLabel(1, "0");
    h_k0ak0->GetXaxis()->SetBinLabel(2, "1");
    h_k0ak0->GetXaxis()->SetBinLabel(3, "2+");

    h_k0ak0->GetYaxis()->SetBinLabel(1, "0");
    h_k0ak0->GetYaxis()->SetBinLabel(2, "1");
    h_k0ak0->GetYaxis()->SetBinLabel(3, "2+");

    h_k0ak0->GetXaxis()->SetTitleSize(0.05);
    h_k0ak0->GetYaxis()->SetTitleSize(0.05);
    h_k0ak0->GetXaxis()->SetLabelSize(0.05);
    h_k0ak0->GetYaxis()->SetLabelSize(0.05);

    h_k0ak0->GetXaxis()->SetTitle("Primary traces");
    h_k0ak0->GetYaxis()->SetTitle("#pi^{0} count");

    h_k0ak0->SetTitle("DIS CC w #bar{K}^{0}");

    gStyle->SetPalette(kViridis);


    h_k0ak0->Draw("COLZ");

    //Add text to k0 histogram
    double max_h_k0ak0 = h_k0ak0->GetMaximum();

    for (int x = 1; x <= h_k0ak0->GetNbinsX(); x++){
        for (int y = 1; y <= h_k0ak0->GetNbinsY(); y++){
            //Get bin content
            int nbin_k0ak0 = h_k0ak0->GetBinContent(x, y);
            int nbin_B0 = h_B0->GetBinContent(x, y);
            int nbin_Bpm = h_Bpm->GetBinContent(x, y);
            int nbin_noB = h_noB->GetBinContent(x, y);

            if (nbin_k0ak0 > max_h_k0ak0/2){
                latex.DrawLatex(h_k0ak0->GetXaxis()->GetBinCenter(x), h_k0ak0->GetYaxis()->GetBinCenter(y)+0.3, Form("Total = %d", nbin_k0ak0));
                latex.DrawLatex(h_k0ak0->GetXaxis()->GetBinCenter(x), h_k0ak0->GetYaxis()->GetBinCenter(y)+0.15, Form("B^{0}: %d (%.2f%%)", nbin_B0, 100.0*nbin_B0/nbin_k0ak0));
                latex.DrawLatex(h_k0ak0->GetXaxis()->GetBinCenter(x), h_k0ak0->GetYaxis()->GetBinCenter(y), Form("B^{#pm}: %d (%.2f%%)", nbin_Bpm, 100.0*nbin_Bpm/nbin_k0ak0));
                latex.DrawLatex(h_k0ak0->GetXaxis()->GetBinCenter(x), h_k0ak0->GetYaxis()->GetBinCenter(y)-0.15, Form("B-less: %d (%.2f%%)", nbin_noB, 100.0*nbin_noB/nbin_k0ak0));
            } else if (nbin_k0ak0){
                latex_white.DrawLatex(h_k0ak0->GetXaxis()->GetBinCenter(x), h_k0ak0->GetYaxis()->GetBinCenter(y)+0.3, Form("Total = %d", nbin_k0ak0));
                latex_white.DrawLatex(h_k0ak0->GetXaxis()->GetBinCenter(x), h_k0ak0->GetYaxis()->GetBinCenter(y)+0.15, Form("B^{0}: %d (%.2f%%)", nbin_B0, 100.0*nbin_B0/nbin_k0ak0));
                latex_white.DrawLatex(h_k0ak0->GetXaxis()->GetBinCenter(x), h_k0ak0->GetYaxis()->GetBinCenter(y), Form("B^{#pm}: %d (%.2f%%)", nbin_Bpm, 100.0*nbin_Bpm/nbin_k0ak0));
                latex_white.DrawLatex(h_k0ak0->GetXaxis()->GetBinCenter(x), h_k0ak0->GetYaxis()->GetBinCenter(y)-0.15, Form("B-less: %d (%.2f%%)", nbin_noB, 100.0*nbin_noB/nbin_k0ak0));
            }
        }
    }

    c2->Update();
    //c2->SaveAs("/home/sergioyb/root/kaon_analysis/histograms/k0/dis/traces/traces_pi0_discc_ak0.pdf");

    /*
    TCanvas *cB0 = new TCanvas("cB0", "", 800, 600);
    TCanvas *cBpm = new TCanvas("cBpm", "", 800, 600);
    TCanvas *cnoB = new TCanvas("cnoB", "", 800, 600); 
    */

    // *********************B0 ********************************* 
    
    cB0->cd();

    h_B0->SetStats(0);

    h_B0->GetXaxis()->SetBinLabel(1, "0");
    h_B0->GetXaxis()->SetBinLabel(2, "1");
    h_B0->GetXaxis()->SetBinLabel(3, "2+");

    h_B0->GetYaxis()->SetBinLabel(1, "0");
    h_B0->GetYaxis()->SetBinLabel(2, "1");
    h_B0->GetYaxis()->SetBinLabel(3, "2+");

    h_B0->GetXaxis()->SetTitleSize(0.05);
    h_B0->GetYaxis()->SetTitleSize(0.05);
    h_B0->GetXaxis()->SetLabelSize(0.05);
    h_B0->GetYaxis()->SetLabelSize(0.05);

    h_B0->GetXaxis()->SetTitle("Primary traces");
    h_B0->GetYaxis()->SetTitle("#pi^{0} count");

    h_B0->SetTitle("DIS CC w B^{0} w #bar{K}^{0}");

    gStyle->SetPalette(kViridis);


    h_B0->Draw("COLZ ");

    max_h_B0 = h_B0->GetMaximum();

    for (int x = 1; x <= h_B0->GetNbinsX(); x++){
        for (int y = 1; y <= h_B0->GetNbinsY(); y++){
            //Get bin content
            int nbin_B0 = h_B0->GetBinContent(x, y);

            if (nbin_B0 > max_h_B0/2){
                latex.DrawLatex(h_B0->GetXaxis()->GetBinCenter(x), h_B0->GetYaxis()->GetBinCenter(y)+0.15, Form("%d", nbin_B0));
            } else if (nbin_B0){
                latex_white.DrawLatex(h_B0->GetXaxis()->GetBinCenter(x), h_B0->GetYaxis()->GetBinCenter(y)+0.15, Form("%d", nbin_B0));
            }
        }
    }

    cB0->Update();
    //cB0->SaveAs("/home/sergioyb/root/kaon_analysis/histograms/k0/dis/traces/traces_pi0_discc_ak0_B0.pdf");

    // ***************** Bpm ****************************

    cBpm->cd();


    h_Bpm->SetStats(0);

    h_Bpm->GetXaxis()->SetBinLabel(1, "0");
    h_Bpm->GetXaxis()->SetBinLabel(2, "1");
    h_Bpm->GetXaxis()->SetBinLabel(3, "2+");

    h_Bpm->GetYaxis()->SetBinLabel(1, "0");
    h_Bpm->GetYaxis()->SetBinLabel(2, "1");
    h_Bpm->GetYaxis()->SetBinLabel(3, "2+");

    h_Bpm->GetXaxis()->SetTitleSize(0.05);
    h_Bpm->GetYaxis()->SetTitleSize(0.05);
    h_Bpm->GetXaxis()->SetLabelSize(0.05);
    h_Bpm->GetYaxis()->SetLabelSize(0.05);

    h_Bpm->GetXaxis()->SetTitle("Primary traces");
    h_Bpm->GetYaxis()->SetTitle("#pi^{0} count");

    h_Bpm->SetTitle("DIS CC w B^{#pm} w #bar{K}^{0}");

    gStyle->SetPalette(kViridis);


    h_Bpm->Draw("COLZ");

    max_h_Bpm = h_Bpm->GetMaximum();

    for (int x = 1; x <= h_Bpm->GetNbinsX(); x++){
        for (int y = 1; y <= h_Bpm->GetNbinsY(); y++){
            //Get bin content
            int nbin_Bpm = h_Bpm->GetBinContent(x, y);

            if (nbin_Bpm > max_h_Bpm/2){
                latex.DrawLatex(h_Bpm->GetXaxis()->GetBinCenter(x), h_Bpm->GetYaxis()->GetBinCenter(y)+0.15, Form("%d", nbin_Bpm));
            } else if (nbin_Bpm){
                latex_white.DrawLatex(h_Bpm->GetXaxis()->GetBinCenter(x), h_Bpm->GetYaxis()->GetBinCenter(y)+0.15, Form("%d", nbin_Bpm));
            }
        }
    }

    cBpm->Update();
    //cBpm->SaveAs("/home/sergioyb/root/kaon_analysis/histograms/k0/dis/traces/traces_pi0_discc_ak0_Bpm.pdf");


    // ******************************** no B ********************************
    cnoB->cd();

    h_noB->SetStats(0);

    h_noB->GetXaxis()->SetBinLabel(1, "0");
    h_noB->GetXaxis()->SetBinLabel(2, "1");
    h_noB->GetXaxis()->SetBinLabel(3, "2+");

    h_noB->GetYaxis()->SetBinLabel(1, "0");
    h_noB->GetYaxis()->SetBinLabel(2, "1");
    h_noB->GetYaxis()->SetBinLabel(3, "2+");

    h_noB->GetXaxis()->SetTitleSize(0.05);
    h_noB->GetYaxis()->SetTitleSize(0.05);
    h_noB->GetXaxis()->SetLabelSize(0.05);
    h_noB->GetYaxis()->SetLabelSize(0.05);

    h_noB->GetXaxis()->SetTitle("Primary traces");
    h_noB->GetYaxis()->SetTitle("#pi^{0} count");

    h_noB->SetTitle("DIS CC wo B w #bar{K}^{0}");

    gStyle->SetPalette(kViridis);


    h_noB->Draw("COLZ");

    max_h_noB = h_noB->GetMaximum();

    for (int x = 1; x <= h_noB->GetNbinsX(); x++){
        for (int y = 1; y <= h_noB->GetNbinsY(); y++){
            //Get bin content
            int nbin_noB = h_noB->GetBinContent(x, y);

            if (nbin_noB > max_h_noB/2){
                latex.DrawLatex(h_noB->GetXaxis()->GetBinCenter(x), h_noB->GetYaxis()->GetBinCenter(y)+0.15, Form("%d", nbin_noB));
            } else if (nbin_noB){
                latex_white.DrawLatex(h_noB->GetXaxis()->GetBinCenter(x), h_noB->GetYaxis()->GetBinCenter(y)+0.15, Form("%d", nbin_noB));
            }
        }
    }

    cnoB->Update();
    //cnoB->SaveAs("/home/sergioyb/root/kaon_analysis/histograms/k0/dis/traces/traces_pi0_discc_ak0_noB.pdf");

}