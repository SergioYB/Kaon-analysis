#include "/home/sergioyb/root/kaon_analysis/macros/includes/tree_utils.cpp"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/Includes.h"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_event_tree.C"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_subrun_tree.C"

int findPos(const std::vector<int>* v, int a) {
    // Usar std::find para buscar el elemento en el puntero al vector
    auto it = std::find(v->begin(), v->end(), a);

    // Verificar si el elemento fue encontrado
    if (it != v->end()) {
        // Calcular y devolver la posición del elemento
        return std::distance(v->begin(), it);
    } else {
        // Si no se encontró, devolver -1
        return -1;
    }
}


//Recursive function that prints all info about a particle siblings
void print_siblings_gen(int ID, std::string indent, TTree* event_tree, int i_e){
    //Obtain current event from event_tree
    event_tree->GetEntry(i_e);

    int size_gen = gen_part_trackID->size();

    std::string new_tab = "\t";

    //Add tabulation
    std::string new_indent = indent + new_tab;

    for (int i_p = 0; i_p < size_gen; i_p++){
        if (gen_part_mother->at(i_p) == ID){
            //cout << new_indent << "----------------------------------------------------------------" << endl;
            //cout << new_indent << "ID: " << gen_part_trackID->at(i_p) << " Status code: "<< gen_part_statusCode->at(i_p) <<  " Mother: " << gen_part_mother->at(i_p) << endl;
            cout << new_indent << "   PDGCode: " << gen_part_PDGcode->at(i_p) << " Mass: " << gen_part_mass->at(i_p) << " Energy: " << gen_part_E0->at(i_p)<< endl;
            //cout << new_indent << "   Start Pos (x,y,z) : (" << gen_part_start_pos_X->at(i_p) << ", " << gen_part_start_pos_Y->at(i_p) << ", " << gen_part_start_pos_Z->at(i_p)  << ")" << endl;
            //cout << new_indent << "   Start Momentum (x,y,z) : (" << gen_part_P0_X->at(i_p) << ", " << gen_part_P0_Y->at(i_p) << ", " << gen_part_P0_Z->at(i_p)  << ")" << endl;

            //Call function again
            //print_siblings_gen(gen_part_trackID->at(i_p), new_indent, event_tree, i_e);
        } 
    }
}


void print_siblings_g4(int ID, std::string indent, TTree* event_tree, int i_e){
    //Recursive function that prints all info about a particle siblings
    //Obtain current event from event_tree
    event_tree->GetEntry(i_e);

    int size_g4 = g4_part_trackID->size();

    std::string new_tab = "\t";

    //Add tabulation
    std::string new_indent = indent + new_tab;

    for (int i_p = 0; i_p < size_g4; i_p++){
        if (g4_part_mother->at(i_p) == ID){
            cout << new_indent << "----------------------------------------------------------------" << endl;
            cout << new_indent << "ID: " << g4_part_trackID->at(i_p) << " Mother: " << g4_part_mother->at(i_p) << endl;
            cout << new_indent << "   PDGCode: " << g4_part_PDGcode->at(i_p) << " Mass: " << g4_part_mass->at(i_p) << " Initial Energy: " << g4_part_E0->at(i_p) << " Final Energy: " << g4_part_Ef->at(i_p) << endl;
            cout << new_indent << "   Start Pos (x,y,z) : (" << g4_part_start_pos_X->at(i_p) << ", " << g4_part_start_pos_Y->at(i_p) << ", " << g4_part_start_pos_Z->at(i_p)  << ")" << endl;
            cout << new_indent << "   End Pos (x,y,z) : (" << g4_part_end_pos_X->at(i_p) << ", " << g4_part_end_pos_Y->at(i_p) << ", " << g4_part_end_pos_Z->at(i_p)  << ")" << endl;
            //cout << new_indent << "   Start Momentum (x,y,z) : (" << g4_part_P0_X->at(i_p) << ", " << g4_part_P0_Y->at(i_p) << ", " << g4_part_P0_Z->at(i_p)  << ")" << endl;
            //cout << new_indent << "   End Momentum (x,y,z) : (" << g4_part_Pf_X->at(i_p) << ", " << g4_part_Pf_Y->at(i_p) << ", " << g4_part_Pf_Z->at(i_p)  << ")" << endl;
            cout << new_indent << "   Process: " << g4_part_process->at(i_p) << endl;
            cout << new_indent << "   End_process: " << g4_part_end_process->at(i_p) << endl;
            //Call function again
            print_siblings_g4(g4_part_trackID->at(i_p), new_indent, event_tree, i_e);
        } 
    }
}


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

void k0_res_hist_mother(){

    TTree *event_tree;
    TTree *subrun_tree;

    event_tree = (TTree*)read_event_tree();
    subrun_tree = (TTree*)read_subrun_tree();

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;


    //Create canvas and histogram 
        //TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
        //TH1F *h1 = new TH1F("h1", "Ks vs Kl", 2, 0.5 , 2.5); //x-> type of K0, y-> counts

    int n_212212;
    int n_202114;
    int n_212114;
    int n_222114;
    int n_202214;
    int n_212112;
    int n_3222;
    int n_3122;
    int n_3212;

    bool first_res;

    //Create canvas and histogram 
    TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
    TH2F *h1 = new TH2F("h1", "", 6, 0.5 , 6.5, 3, 0.5, 3.5); //x-> mother baryon, y-> brother baryon


    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        first_res = true;

        //Set all count to 0
        n_212212 = 0;
        n_202114 = 0;
        n_212114 = 0;
        n_222114 = 0;
        n_202214 = 0;
        n_212112 = 0;
        n_3222 = 0;
        n_3122 = 0;
        n_3212 = 0;

        //Loop over ALL gen particles in i_e-th event and show info if there are stable final state k0 created via resonance decay
        for(int j = 0; j < size_gen; j++){
            if (gen_part_statusCode->at(j) == 1){
                if ((gen_part_PDGcode->at(j) == 311) & (nu_interaction_mode == 1)){

                    //Loop over all particles to find first resonance
                    for(int k = 0; k < size_gen; k++){
                        //If resonance is found fill histogram
                        if (((gen_part_PDGcode->at(k) > 1e5) & (gen_part_PDGcode->at(k) < 1e7)) & (first_res)){
                            first_res = false;
                            if (gen_part_PDGcode->at(k) == 212212){
                                n_212212++;
                            } else if (gen_part_PDGcode->at(k) == 202214){
                                n_202214++;
                            } else if (gen_part_PDGcode->at(k) == 212112){
                                n_212112++;
                            } else if (gen_part_PDGcode->at(k) == 202114){
                                n_202114++;
                            } else if (gen_part_PDGcode->at(k) == 212114){
                                n_212114++;
                            } else if (gen_part_PDGcode->at(k) == 222114){
                                n_222114++;
                            }
                            //Loop over all particles to find kaon brother
                            for (int i_bro = 0; i_bro < size_gen; i_bro++){
                                if ((gen_part_mother->at(i_bro) == gen_part_trackID->at(k)) & (gen_part_PDGcode->at(k) > 1e3)){
                                    if (gen_part_PDGcode->at(i_bro) == 3222){
                                        n_3222++;
                                    } else if (gen_part_PDGcode->at(i_bro) == 3122){
                                        n_3122++;
                                    } else if (gen_part_PDGcode->at(i_bro) == 3212){
                                        n_3212++;
                                    }
                                }
                            }

                            //Fill histogram

                            if (n_212212 > 0){
                                if (n_3222 > 0){
                                    h1->Fill(1,1);
                                } else if (n_3212 > 0){
                                    h1->Fill(1,2);
                                } else if (n_3122 >0){
                                    h1->Fill(1,3);
                                }
                            } else if (n_202214 > 0){
                                if (n_3222 > 0){
                                    h1->Fill(2,1);
                                } else if (n_3212 > 0){
                                    h1->Fill(2,2);
                                } else if (n_3122 >0){
                                    h1->Fill(2,3);
                                }
                            } else if (n_212112 > 0){
                                if (n_3222 > 0){
                                    h1->Fill(3,1);
                                } else if (n_3212 > 0){
                                    h1->Fill(3,2);
                                } else if (n_3122 >0){
                                    h1->Fill(3,3);
                                }
                            } else if (n_202114 > 0){
                                if (n_3222 > 0){
                                    h1->Fill(4,1);
                                } else if (n_3212 > 0){
                                    h1->Fill(4,2);
                                } else if (n_3122 >0){
                                    h1->Fill(4,3);
                                }
                            } else if (n_212114 > 0){
                                if (n_3222 > 0){
                                    h1->Fill(5,1);
                                } else if (n_3212 > 0){
                                    h1->Fill(5,2);
                                } else if (n_3122 >0){
                                    h1->Fill(5,3);
                                }
                            } else if (n_222114 > 0){
                                if (n_3222 > 0){
                                    h1->Fill(6,1);
                                } else if (n_3212 > 0){
                                    h1->Fill(6,2);
                                } else if (n_3122 >0){
                                    h1->Fill(6,3);
                                }
                            } else{
                                cout << gen_part_PDGcode->at(k) << "    " << gen_part_mass->at(k) << endl;
                            }

                            //cout << n_212212 << "   " << n_202114 << "  " << n_212114 << "  " << n_222114 << endl;

                        }
                    }

                }
            }
        }
    }
    
    //Draw histogram


    //Scaling histogram for one year production (3y = 10e21 POT)
    double n_POT = get_total_POT(subrun_tree);

    //DRAW h1 ===========================================================================

    h1->SetStats(0);

    h1->GetXaxis()->SetBinLabel(1, "N^{+}(1710)");
    h1->GetXaxis()->SetBinLabel(2, "N^{+}(1720)");
    h1->GetXaxis()->SetBinLabel(3, "N^{0}(1710)");
    h1->GetXaxis()->SetBinLabel(4, "N^{0}(1720)");
    h1->GetXaxis()->SetBinLabel(5, "#Delta^{0}(1600)");
    h1->GetXaxis()->SetBinLabel(6, "#Delta^{0}(1920)");

    h1->GetYaxis()->SetBinLabel(1, "#Sigma^{+}");
    h1->GetYaxis()->SetBinLabel(2, "#Sigma^{0}");
    h1->GetYaxis()->SetBinLabel(3, "#Lambda");

    h1->GetXaxis()->SetTitleSize(0.05);
    h1->GetYaxis()->SetTitleSize(0.05);
    h1->GetXaxis()->SetLabelSize(0.05);
    h1->GetYaxis()->SetLabelSize(0.05);

    h1->GetXaxis()->SetTitle("Mother particle");
    h1->GetYaxis()->SetTitle("Brother particle");

    //h1->Scale(1e21/(3 * n_POT));

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
                sprintf(text1, "%.0f", binContent);
                //sprintf(text2, "%.3f%% of total K events", binContent/h1->Integral()*100);
                sprintf(text2, "%.3f%% of ttl evnts", binContent/(n_events * 1e21/(3 * n_POT))*100);
                xlabel = h1->GetXaxis()->GetBinLabel(x);
                sprintf(text3, "%.3f%% of %s", binContent/h1->Integral(x,x,2,3)*100, xlabel.Data());

                //Creating TText objects
                TText *t1 = new TText(h1->GetXaxis()->GetBinCenter(x), h1->GetYaxis()->GetBinCenter(y), text1);
                TText *t2 = new TText(h1->GetXaxis()->GetBinCenter(x), h1->GetYaxis()->GetBinCenter(y), text2);
                //Adjust size and location
                t1->SetTextSize(0.03);
                t1->SetTextAlign(22);  //Centering text
                t2->SetTextSize(0.03);
                t2->SetTextAlign(22);

                //Draw text on canvas
                t1->Draw();
                //t2->Draw();

                if (y >1){
                    TText *t3 = new TText(h1->GetXaxis()->GetBinCenter(x), h1->GetYaxis()->GetBinCenter(y) - 0.2, text3);
                    
                    t3->SetTextSize(0.03);
                    t3->SetTextAlign(22);
                    //t3->Draw();
                }                
                
            }
        }
    }
}

/*RESONANCIES OBSERVED 

CODE        MASS        NAME            QUARK_COMP      AMOUNT (16 kaon from resonance)
212212      1.71        N+(1710)        uud   
202214      1.72        N+(1720)        uud  
212112      1.71        N0(1710)        udd         
202114      1.72        N0(1720)        udd            
212114      1.57        Delta0(1600)    udd             
222114      1.92        Delta0(1920)    udd             
*/

/*BROTHERS OBSERVED
CODE        MASS        NAME            QUARK_COMP      AMOUNT (16 kaon from resonance)
3222        1.18937     Sigma+          uus             7
3212        1.19255     Sigma0          uds             4
3122        1.11568     Lambda          uds             5
*/

//NEXT: HISTOGRAMA 2D X->BARION MADRE, Y->BARION HERMANO