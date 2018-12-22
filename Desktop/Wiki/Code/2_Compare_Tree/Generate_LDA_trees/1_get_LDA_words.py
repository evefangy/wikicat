# https://radimrehurek.com/gensim/tut1.html#corpus-formats

import logging
logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s',
level=logging.INFO)
from gensim import corpora, models, similarities

# documents = ["Human machine interface for lab abc computer applications",
# 			  "A survey of user opinion of computer system response time",
# 			  "The EPS user interface management system",
# 			 "System and human system engineering testing of EPS",
# 			 "Relation of user perceived response time to error measurement",
# 			 "The generation of random binary unordered trees",
# 			  "The intersection graph of paths in trees",
# 			 "Graph minors IV Widths of trees and well quasi ordering",
# 			  "Graph minors A survey"]

# # remove common words and tokenize
# stoplist = set('for a of the and to in'.split())
# texts = [[word for word in document.lower().split() if word not in stoplist] for document in documents]

# # remove words that appear only once
# from collections import defaultdict
# frequency = defaultdict(int)
# for text in texts:
# 	for token in text:
# 		 frequency[token] += 1

# texts = [[token for token in text if frequency[token] > 1] for text in texts]

# from pprint import pprint
# pprint(texts)

# # map each word onto an integer id
# dictionary = corpora.Dictionary(texts)
# dictionary.save('/tmp/deerwester.dict')  # store the dictionary, for future reference
# print(dictionary)

# # check the word id
# print(dictionary.token2id)


# # convert tokenized documents to vectors
# new_doc = "Human computer interaction"
# new_vec = dictionary.doc2bow(new_doc.lower().split()) # word_id, ctr

w_o_stop_words_addr = '/Users/eve/Desktop/Major_Editor_5/Create_BoW_Tree/1_w_o_stop_words.txt'
art_topic_addr = '/Users/eve/Desktop/Major_Editor_5/LDA/art_topic.txt'

art_topic = open(art_topic_addr, 'w')

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
	if line=="************************** new article *************************":
		curr_id += 1
dictionary = corpora.Dictionary(bow)




# print(dictionary.token2id)
class MyCorpus(object):
	def __iter__(self):
		
		for line in open(w_o_stop_words_addr):
			line = line.lower().strip().replace(',', ' ')

			if len(line)!=0 and line!="************************** new article *************************":
				tmp = line.lower().strip().split()
				yield dictionary.doc2bow(tmp)

corpus = MyCorpus()
# for vector in corpus:
# 	print(vector)
print("Finished storing corpus...")

num_topics = 1000

lda = models.ldamodel.LdaModel(corpus, id2word=dictionary, num_topics=num_topics, passes=1)


doc_lda = lda[corpus]

print("Printing out topics...")
# print out topics overed for each article
article_id = 0
for article_topics in doc_lda:
	art_topic.write(str(artid[article_id]))
	for topic in article_topics:
		art_topic.write(", "+str(topic[0]))
	article_id += 1
	art_topic.write('\n')


