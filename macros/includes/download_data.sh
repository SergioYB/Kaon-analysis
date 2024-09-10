#This code downloads the data form the sbndgpvm nodes into this folder. If you have access to the nodes and want to run it make sure to change the user "lpelegri" for your actual sbnd username. Make sure to have a kerberos token active to be able to download the files.
#The difference between scp and rsync is that rsync save progress of the file download, so if it is interrumpted you will restart it from where it failed, inestead of having to download it from 0. It might not work first try, so scp is also added as an alternative

#line to download 100 events using rsync
rsync -vt --append-verify --info=progress2 -e 'ssh' lpelegri@sbndgpvm02.fnal.gov:/exp/sbnd/data/users/lpelegri/Truth_Example/analysis_output_truth_BNB_single_100_events.root .

#line to download 100k events using rsync
#rsync -vt --append-verify --info=progress2 -e 'ssh' lpelegri@sbndgpvm02.fnal.gov:/exp/sbnd/data/users/lpelegri/Truth_Example/analysis_output_truth_BNB_single_100k_events.root .

#line to download 100 events using scp
#scp lpelegri@sbndgpvm02.fnal.gov:/exp/sbnd/data/users/lpelegri/Truth_Example/analysis_output_truth_BNB_single_100_events.root .

#line to download 100k events using scp
#scp lpelegri@sbndgpvm02.fnal.gov:/exp/sbnd/data/users/lpelegri/Truth_Example/analysis_output_truth_BNB_single_100k_events.root .

