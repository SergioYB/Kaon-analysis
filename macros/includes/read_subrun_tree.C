//meto las variables que voy a usar y los branches que voy a necesitar
//#include "tree_utils.cpp"
//meto cosas necesarias para que esto funcione
//#include "Includes.h"


TTree *read_subrun_tree(string path_to_tree){

    //cout << "Cargando los datos del .root del tree 'tree' en event_tree." <<endl;
    //Defino el nombre del TFile (.root) donde voy a leer todas las cosas. Se va a leer el input_file y se va a almacenar su info en tree_dir
    TFile *input_file;
    TDirectoryFile *tree_dir;
    TTree *subrun_tree;

    //Aqui pongo el path y el nombre del .root que voy a leer
    //string path_to_tree = "/home/sergioyb/root/kaon_analysis/data/analysis_output_truth_BNB_single_100k_events.root";
    input_file = new TFile(path_to_tree.c_str());

    //aqui le digo que me coja las cosas que estan en "ana"
    tree_dir = (TDirectoryFile*)input_file->Get("ana");
    //y las subruns
    subrun_tree =(TTree*)tree_dir->Get("subrun_tree");

    //Meto el codigo de tree_utils.cpp donde se definen las variables que voy a usar y los Branches. MIRAR TREE_UTILS PARA MAS INFO
    set_branch_subtree(subrun_tree);

    //Ahora tengo acceso a todos los Branches de TTree (salvo que quedase alguno por poner) y los he guardado en las variables que estan definidas en tree_utils
    //Ya puedo usar las variables que quiera que ya lsa deberia tener en mi poder :)

    //Para acceder a las variables, tengo que recorrer todos los datos que haya en el Branch correspondiente:
    //  - Se recorre mediqante un indice i todas las entries que tiene el tree
    //  - Haciendo event_tree->GetEntry(i); se obtienen TODAS las variables de esa entrada del tree
    //  - Se hace lo que se quiera con esa entrada

    cout<<"Todo cargado :)"<<endl;

    return subrun_tree;
}