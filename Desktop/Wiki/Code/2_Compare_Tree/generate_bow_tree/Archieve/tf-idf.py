from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
import numpy as np

# vectorizer = CountVectorizer(strip_accents='unicode', stop_words = 'english', analyzer == 'word')
vectorizer = CountVectorizer()
corpus = []
# input_addr = "/Users/eve/Desktop/w_o_stop_words_enhanced.txt"
input_addr = "/Users/eve/Desktop/test.txt"
output_addr = "/Users/eve/Desktop"
top_k = 10

bow_words_addr = output_addr+"/bow_words.txt"
count_mat_addr = output_addr+"/count_mat.txt"
tf_addr = output_addr+"/tf-idf_result.txt"
art_word_addr = output_addr+"/art_word.txt"
bow_words=open(bow_words_addr, 'w')
count_mat=open(count_mat_addr, 'w')
tf= open(tf_addr,'w')
art_word = open(art_word_addr, 'w')

line = ""
text = []
a = 0
c = 0
with open(input_addr,'r') as f:
	for line in f:
		line = line.strip()
		if(len(line)!=0):
			if line == "************************** New Article *************************":
				# c+=1
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
print("Loaded all articles")

counts_tmp = vectorizer.fit_transform(corpus) #transform all articles
print("Finish creating bag of words")

#get all bow words in order
words = vectorizer.get_feature_names()
for i in words:
	bow_words.write(i.decode('UTF-8'))
	bow_words.write('\n')
bow_words.close()
print "Total number of words in bow: "


#print out bow matrix
counts = counts_tmp.toarray() 

print(str(len(counts[0])))
# c=0
for i in counts:
	# print c
	count_mat.write(str(i))
	count_mat.write("\n")
count_mat.close()
print("Finished storing all bag of words\n")


####### the API already normalized each row in the transform function
# counts = counts.astype(float)

# c = 0
# for i in counts:
# 	minrow = min(i)
# 	maxrow = max(i)
# 	counts[c] = (i-minrow)/(maxrow-minrow)
# 	c+=1


transformer = TfidfTransformer()
tfidf = transformer.fit_transform(counts).toarray()
print("Finished calculating tf-idf score\n")
# print counts
for i in tfidf:
	tf.write(str(i))
	tf.write("\n")
tf.close()
print("Finished storing tf-idf score\n")

# get the index of top 100 biggest tf-idf value words of each row 
# x = np.array([3, 4, 2, 1, 5, 6, 7, 8, 100, 200, 300, 400, 500, 600, 20, -11])
# ind = np.argpartition(x, -7)[-7:]
# print ind
# print x[ind]
# print tfidf

c=0
for i in tfidf:
	#get index of top 100 words
	ind = np.argpartition(i, -top_k)[-top_k:]
	# ind.sort()
	art_word.write(str(c))
	for j in ind:
		art_word.write(", ")
		art_word.write(str(j))
	c+=1
	print(c)
	art_word.write("\n")

	# c=0
	# for j in range(0, len(i)):
	# 	if c<len(ind) and j==ind[c]:
	# 		c+=1
	# 	else:
	# 		i[j] = 0.0

# print tfidf

print "Processed ", str(c), " articles"

art_word.close()



