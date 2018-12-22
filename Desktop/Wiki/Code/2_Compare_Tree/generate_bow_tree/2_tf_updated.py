from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer
import numpy as np
import os

output_folder = "/Users/eve/Desktop/Major_Editor_5/Create_BoW_Tree"

# vectorizer = CountVectorizer(strip_accents='unicode', stop_words = 'english', analyzer == 'word')
vectorizer = TfidfVectorizer()
corpus = []
input_addr = output_folder+"/1_w_o_stop_words.txt"
# input_addr = "/Users/eve/Desktop/test.txt"
path = output_folder+"/TF"
try:  
    os.mkdir(path)
except OSError:  
    print ("Creation of the directory %s failed" % path)
else:  
    print ("Successfully created the directory %s " % path)

output_addr = output_folder+"/TF"
top_k = 100 #get top 100 words with highest idf score each row

bow_words_addr = output_addr+"/2_bow_words_tf.txt"
art_word_addr = output_addr+"/2_art_word_tf.txt"
bow_words=open(bow_words_addr, 'w')

# count_mat=open(count_mat_addr, 'w')
tf= open(bow_words_addr,'w')
art_word = open(art_word_addr, 'w')

#store all articles in a corpus
line = ""
text = []
a = 0
c = 0
with open(input_addr,'r') as f:
	for line in f:
		line = line.strip()
		if(len(line)!=0):
			if line == "************************** New Article *************************":
				c+=1
				# print(c)

				line = ""
				a = 0
				for i in text:
					a+=1
					line+=i
					if len(text)!=a:
						line+=", "

				corpus.append(line)

				text = []

			else:
				text.append(line)
f.close()
print "Loaded all", str(c), " articles"

# caluclate the idf score for the corpus [note that a row-wise normalization is done in the transform]
tf_tmp = vectorizer.fit_transform(corpus) 
print("Finish calculating tf-idf")


# get the top 100 words with biggest idf score 
# store the article, w1, w2, ...  in the art_word
num_art = c
c = 0
for r in range (0, num_art):
	tf_idf = tf_tmp[r,:].toarray()
	ind = np.argpartition(tf_idf[0], -top_k)[-top_k:]
	art_word.write(str(r))
	for j in ind:
		if tf_idf[0][j]>0:
			art_word.write(", ")
			art_word.write(str(j))
	c+=1
	print(c)
	art_word.write("\n")

print "Processed ", str(c), " articles"

# tf_idf = tf_tmp[0,:].toarray()
# print "Total number of words in bow: ", str(len(tf_idf[0]))

art_word.close()


#get all bow words in order
words = vectorizer.get_feature_names()
for i in words:
	bow_words.write(i.decode('UTF-8'))
	bow_words.write('\n')
bow_words.close()
print "Total number of words in bow: ", str(len(words))
