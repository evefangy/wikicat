OUTPUT_FOLDER=$1
MIN_WEIGHT_TO_CONSIDER=$2
TOTAL_NUMBER_OF_ART=$3
MIN_NUM_ART_TO_MERGE=$4
ARTNAME_ADDR=$5
USEDART_ADDR=$6
NUMBER_OF_USED_ART=$7

WORD_W_ART_ADDR=$OUTPUT_FOLDER'/3_word_w_art.txt'
ART_WEIGHT_ADDR=$OUTPUT_FOLDER'/4_art_weight_pair.txt'
SORTED_WEIGHT_ADDR=$OUTPUT_FOLDER'/5_sorted_weight.txt'
Group_Iteration_File=$OUTPUT_FOLDER'/group_ite.txt'
Encoded_Tree_Addr=$OUTPUT_FOLDER'/tree_enco.txt'
Decoded_Tree_Addr=$OUTPUT_FOLDER'/7_decoded_tree.txt'


./create_edge_list.out $WORD_W_ART_ADDR $OUTPUT_FOLDER 1000 $NUMBER_OF_USED_ART
./sort_edgelist.out $ART_WEIGHT_ADDR $OUTPUT_FOLDER
./get_enco.out $SORTED_WEIGHT_ADDR $OUTPUT_FOLDER $MIN_WEIGHT_TO_CONSIDER $TOTAL_NUMBER_OF_ART $MIN_NUM_ART_TO_MERGE
./create_tree.out $Group_Iteration_File $OUTPUT_FOLDER
./decode.out $ARTNAME_ADDR $Encoded_Tree_Addr $OUTPUT_FOLDER
./add_remain.out $USEDART_ADDR $Decoded_Tree_Addr $OUTPUT_FOLDER