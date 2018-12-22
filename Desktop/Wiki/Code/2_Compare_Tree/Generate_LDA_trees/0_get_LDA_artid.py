w_o_stop_words_addr = '/Users/eve/Desktop/Major_Editor_5/Create_BoW_Tree/1_w_o_stop_words.txt'
art_topic_addr = '/Users/eve/Desktop/Major_Editor_5/LDA/art_id.txt'

art_id = open(art_topic_addr, 'w')

bow = []
tmp = []
curr_id = 0
artid = []
for line in open(w_o_stop_words_addr):
	line = line.lower().strip().replace(',', ' ')
 	if len(line)!=0 and line!="************************** new article *************************":
	    tmp = line.lower().strip().split()
	    bow.append(tmp)
	    artid.append(curr_id)
	if line == "************************** new article *************************":
		curr_id += 1


for curr_id in artid:
	art_id.write(str(curr_id))
	art_id.write("\n")