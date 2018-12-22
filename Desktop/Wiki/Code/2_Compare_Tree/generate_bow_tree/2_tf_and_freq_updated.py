from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer
import numpy as np
import os

output_folder = "/Users/eve/Desktop/Major_Editor_5/Create_BoW_Tree"
# ------- input address -------
input_addr = output_folder+"/1_w_o_stop_words.txt"

path = output_folder+"/TF_IDF_Freq"
try:  
    os.mkdir(path)
except OSError:  
    print ("Creation of the directory %s failed" % path)
else:  
    print ("Successfully created the directory %s " % path)

output_addr = output_folder+"/TF_IDF_Freq"
top_k = 100 #get top 100 words with highest idf score each row

# ------- output address -------
bow_words_addr = output_addr+"/2_bow_words_tf_and_freq.txt"
count_mat_addr = output_addr+"/2_count_mat_tf_and_freq.txt"
tf_addr = output_addr+"/2_tf-idf_result_tf_and_freq.txt"
art_word_addr = output_addr+"/2_art_word_tf_and_freq.txt"

bow_words = open(bow_words_addr, 'w')
# count_file = open(count_mat_addr, 'w')
# tf_file = open(tf_addr,'w')
art_word = open(art_word_addr, 'w')

# --------- initiate corpus and verctors -------
# vectorizer_count = CountVectorizer(strip_accents='unicode', stop_words = 'english', analyzer = 'word')
vectorizer_count = CountVectorizer(strip_accents='unicode', analyzer = 'word')

vectorizer_tfidf = TfidfVectorizer()
corpus = []

# store contents for all articles in a corpus
# corpus: [contents for article 1, contents for article 2...]
line = ""
content = []
content_line_ctr = 0
num_art = 0 # article counter
with open(input_addr,'r') as f:
	for line in f:
		line = line.strip()
		if(len(line)!=0):
			# finish reading in content for one article
			if line == "************************** New Article *************************":
				num_art += 1
				line = ""
				content_line_ctr = 0
				# concat every section in an artcle to one line
				for text in content:
					content_line_ctr += 1
					line += text
					if content_line_ctr!=len(content):
						line += ", "

				corpus.append(line)

				content = []

			else:
				content.append(line)
f.close()
print "Loaded all", str(num_art), " articles"


count_matrix = []
tf_matrix = []

# caluclate the counts for the corpus 
count_matrix = (vectorizer_count.fit_transform(corpus))
# caluclate the tf-idf score for the corpus [note that a row-wise normalization is done in the transform]
tf_matrix = (vectorizer_tfidf.fit_transform(corpus))
	
print("Finish calculating word counts")
print("Finish calculating tf-idf")

# error check (if two matrices are different sized)
for r in range (0, num_art):
	count_score = count_matrix[r,:].toarray()
	tf_idf_score = tf_matrix[r,:].toarray()
	if (len(tf_idf_score[0])!=len(count_score[0])):
		print("count_matrix and tf-idf_marix are different in size")
		print("count_matrix ["+str(r)+"] length="+str(len(count_score[0])))
		print("tf_matrix ["+str(r)+"] length="+str(len(tf_idf_score[0])))
		exit()


# get the top 100 words with biggest idf*frequency score 
# store the article, w1, w2, ...  in the art_word
c = 0
for r in range (0, num_art):
	tf_idf_score = tf_matrix[r,:].toarray()
	count_score = count_matrix[r,:].toarray()
	# tf_file.write(str(tf_idf_score[0]))
	# tf_file.write("\n")
	# count_file.write(str(count_score[0]))
	# count_file.write("\n")

	multiplied_score = []

	for i in range(0, len(tf_idf_score[0])):
		multiplied_score.append(tf_idf_score[0][i]*count_score[0][i])
	
	multiplied_score = np.array(multiplied_score)

	# print("tfidf")
	# print(tf_idf_score[0])
	# print("count")
	# print(count_score[0])
	# print("multiplied_score")
	# print(multiplied_score)

	# if (top_k>len(tf_idf_score[0])):
	# 	top_k = len(tf_idf_score[0])
	# 	print("Top_k is bigger than the size of bag of words ("+str(len(tf_idf_score[0]))+")")
	# 	print("We are using k = "+str(len(tf_idf_score[0]))+'\n')

	ind = np.argpartition(multiplied_score, -top_k)[-top_k:]
	# print("ind")
	# print(ind)
	
	art_word.write(str(r))
	# if there are fewer than k words in the article, get maximum
	for j in ind:
		if multiplied_score[j]>0:
			art_word.write(", ")
			art_word.write(str(j))
	c += 1
	print(c)
	art_word.write("\n")

print "Processed ", str(c), " articles"

# tf_idf = tf_tmp[0,:].toarray()
# print "Total number of words in bow: ", str(len(tf_idf[0]))

# art_word.close()
# tf_file.close()
# count_file.close()


#get all bow words in order
words = vectorizer_tfidf.get_feature_names()
for i in words:
	bow_words.write(i.decode('UTF-8'))
	bow_words.write('\n')
bow_words.close()
print "Total number of words in bow: ", str(len(words))
