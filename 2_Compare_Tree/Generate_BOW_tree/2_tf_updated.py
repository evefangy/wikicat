from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer
import numpy as np

# vectorizer = CountVectorizer(strip_accents='unicode', stop_words = 'english', analyzer == 'word')
vectorizer = TfidfVectorizer()
corpus = []
input_addr = "/Users/eve/Desktop/w_o_stop_words.txt"
# input_addr = "/Users/eve/Desktop/test.txt"
output_addr = "/Users/eve/Desktop"
top_k = 100 #get top 100 words with highest idf score each row

bow_words_addr = output_addr+"/bow_words.txt"
# count_mat_addr = output_addr+"/count_mat_2.txt"
tf_addr = output_addr+"/tf-idf_result.txt"
art_word_addr = output_addr+"/art_word.txt"
bow_words=open(bow_words_addr, 'w')
# count_mat=open(count_mat_addr, 'w')
tf= open(tf_addr,'w')
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
print("Finish calculating tf-ifd")


# get the top 100 words with biggest idf score 
# store the article, w1, w2, ...  in the art_word
num_art = c
c = 0
for r in range (0, num_art):
	tf_idf = tf_tmp[r,:].toarray()
	tf.write(str(tf_idf[0]))
	tf.write("\n")
	ind = np.argpartition(tf_idf[0], -top_k)[-top_k:]
	art_word.write(str(r))
	for j in ind:
		art_word.write(", ")
		art_word.write(str(j))
	c+=1
	print(c)
	art_word.write("\n")

print "Processed ", str(c), " articles"

# tf_idf = tf_tmp[0,:].toarray()
# print "Total number of words in bow: ", str(len(tf_idf[0]))

art_word.close()
tf.close()


#get all bow words in order
words = vectorizer.get_feature_names()
for i in words:
	bow_words.write(i.decode('UTF-8'))
	bow_words.write('\n')
bow_words.close()
print "Total number of words in bow: ", str(len(words))
