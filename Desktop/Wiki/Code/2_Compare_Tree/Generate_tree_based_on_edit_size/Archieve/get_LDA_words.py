# https://radimrehurek.com/gensim/tut1.html#corpus-formats
import scipy

import logging
logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s',
level=logging.INFO)
from gensim import corpora, models, similarities

documents = ["Human machine interface for lab abc computer applications",
			  "A survey of user opinion of computer system response time",
			  "The EPS user interface management system",
			 "System and human system engineering testing of EPS",
			 "Relation of user perceived response time to error measurement",
			 "The generation of random binary unordered trees",
			  "The intersection graph of paths in trees",
			 "Graph minors IV Widths of trees and well quasi ordering",
			  "Graph minors A survey"]

# remove common words and tokenize
stoplist = set('for a of the and to in'.split())
texts = [[word for word in document.lower().split() if word not in stoplist] for document in documents]

# remove words that appear only once
from collections import defaultdict
frequency = defaultdict(int)
for text in texts:
	for token in text:
		 frequency[token] += 1

texts = [[token for token in text if frequency[token] > 1] for text in texts]

from pprint import pprint
# pprint(texts)

# map each word onto an integer id
dictionary = corpora.Dictionary(texts)

# check the word id
print(dictionary.token2id)



# convert tokenized documents to vectors
number_of_terms = 29
number_of_doc = 9

# M = number_of_terms
# N = number_of_doc
# corpus_vec = lil_matrix((M, N))
revserse_map = dictionary.token2id
corpus_vec = []
doc_vec=[]
split = []
new_doc = "Human computer interaction Human machine interface for computer applications"
new_vec = dictionary.doc2bow(new_doc.lower().split()) # word_id, ctr
split = new_doc.lower().split(" ")
for i in range(1, len(split), 2):
	doc_vec.append((revserse_map[split[i]], 2.44))
print (doc_vec)

corpus_vec.append(new_vec)

print("\n\n++ new vec 1++")

print (new_vec)

new_doc = "Graph minors A survey"
new_vec = dictionary.doc2bow(new_doc.lower().split()) # word_id, ctr
corpus_vec.append(new_vec)
print("\n++ new vec 2++")
print (new_vec)
print("===============\n\n")


doc_vec=[]
split = []
class MyCorpus(object):
	def __iter__(self):
		for vec in corpus_vec:
			yield vec
		# # yield new_vec
		# new_doc = "Human computer interaction Human machine interface for computer applications"
		# split = new_doc.lower().split(" ")
		# for i in range(1, len(split), 2):
		# 	doc_vec.append((revserse_map[split[i]], 2.44))
		# yield doc_vec # word_id, ctr
		# # print (dictionary.doc2bow(new_doc.lower().split()))

		# new_doc = "minors  survey"
		# yield dictionary.doc2bow(new_doc.lower().split()) # word_id, ctr
		# # print (dictionary.doc2bow(new_doc.lower().split()))

corpus = MyCorpus()
# print(next(iter(corpus)))

# print("\n\n<------------------------------->")

# from gensim.corpora.dictionary import Dictionary
# from gensim.test.utils import common_texts
# common_dictionary = Dictionary(common_texts)
# print(common_dictionary.token2id)
# other_texts = [['computer', 'time', 'graph'],['survey', 'response', 'eps'],['human', 'system', 'computer']]
# other_corpus = [common_dictionary.doc2bow(text) for text in other_texts]
# print(next(iter(other_corpus)))

# print("\n\n<------------------------------->")

# print("\n\n//////////")

print (dictionary.token2id)
print(next(iter(corpus)))

lda = models.ldamodel.LdaModel(corpus, id2word=dictionary, num_topics=50, passes=3)


# w_o_stop_words_addr = '/Users/eve/Desktop/DLA/1_w_o_stop_words.txt'
# art_topic_addr = '/Users/eve/Desktop/DLA/art_topic.txt'

# art_topic = open(art_topic_addr, 'w')


# class MyCorpus(object):
# 	def __iter__(self):
		
# 		for line in open(w_o_stop_words_addr):
# 			line = line.lower().strip().replace(',', ' ')

# 			if len(line)!=0 and line!="************************** new article *************************":
# 				tmp = line.lower().strip().split()
# 				yield dictionary.doc2bow(tmp)



# corpus = MyCorpus()

# print("Finished storing corpus...")

# num_topics = 100

# lda = models.ldamodel.LdaModel(corpus, id2word=dictionary, num_topics=num_topics, passes=2)


doc_lda = lda[corpus]

print("Printing out topics...")
# print out topics overed for each article
article_id = 0
for article_topics in doc_lda:
	print(str(article_id))
	for topic in article_topics:
		print(", "+str(topic[0]))
	article_id += 1


