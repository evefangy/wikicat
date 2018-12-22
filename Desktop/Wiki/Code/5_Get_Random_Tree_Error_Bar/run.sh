ALL_TREE_FOLDER="/Users/eve/Desktop/Wiki/Code/5_Get_Random_Tree_Error_Bar/Test/All_Trees"
SAMPLED_FOLDER="/Users/eve/Desktop/Wiki/Code/5_Get_Random_Tree_Error_Bar/Test/Sampled" 
NUMBER_OF_SAMPLES="5"
NUMBER_OF_ARTICLES_TO_SAMPLE="10"
END_FOLDER_NUMBER="4"
g++ 1_create_random_tree.cpp -o a.out
./a.out $ALL_TREE_FOLDER $SAMPLED_FOLDER $NUMBER_OF_SAMPLES
g++ 2_pick_n_locate_art.cpp -o b.out
./b.out $NUMBER_OF_ARTICLES_TO_SAMPLE $SAMPLED_FOLDER $ALL_TREE_FOLDER
g++ 3_find_similarity.cpp -o c.out
./c.out 0 $END_FOLDER_NUMBER $SAMPLED_FOLDER $ALL_TREE_FOLDER
g++ 4_get_tree_similarity.cpp -o d.out
./d.out $NUMBER_OF_SAMPLES $SAMPLED_FOLDER $ALL_TREE_FOLDER
