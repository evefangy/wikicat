Tree_Addr=$1
Output_Random_Tree_Addr=$2
Wiki_Addr=$3
Num_Articles_to_Sample=50
./a.out $Tree_Addr $Output_Random_Tree_Addr
./b.out $Output_Random_Tree_Addr/random_tree_structure.txt $Wiki_Addr $Output_Random_Tree_Addr/Random_vs_Wiki $Num_Articles_to_Sample
./c.out $Output_Random_Tree_Addr/random_tree_structure.txt $Wiki_Addr $Output_Random_Tree_Addr/Random_vs_Wiki/sampled_arts.txt $Output_Random_Tree_Addr/Random_vs_Wiki 0
./d.out $Output_Random_Tree_Addr/Random_vs_Wiki/reported_similartity.txt $Output_Random_Tree_Addr/Random_vs_Wiki


