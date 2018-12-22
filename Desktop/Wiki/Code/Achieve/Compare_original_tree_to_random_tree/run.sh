ALL_TREE_FOLDER="/Users/eve/Desktop/Wiki/Code/Compare_original_tree_to_random_tree/Error_Bar/All_Trees"
SAMPLED_FOLDER="/Users/eve/Desktop/Wiki/Code/Compare_original_tree_to_random_tree/Error_Bar/Random_Version" 
NUMBER_OF_SAMPLES="1"
NUMBER_OF_ARTICLES_TO_SAMPLE="1000"
END_FOLDER_NUMBER="0"
g++ 2_pick_n_locate_art.cpp -o b.out
./b.out $NUMBER_OF_ARTICLES_TO_SAMPLE $SAMPLED_FOLDER $ALL_TREE_FOLDER
g++ 3_find_similarity.cpp -o c.out
./c.out 0 $END_FOLDER_NUMBER $SAMPLED_FOLDER $ALL_TREE_FOLDER
g++ 4_get_tree_similarity.cpp -o d.out
./d.out $NUMBER_OF_SAMPLES $SAMPLED_FOLDER $ALL_TREE_FOLDER
