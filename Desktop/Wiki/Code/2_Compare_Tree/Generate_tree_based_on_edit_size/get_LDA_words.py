# https://radimrehurek.com/gensim/tut1.html#corpus-formats

import logging
logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s',
level=logging.INFO)
from gensim import corpora, models, similarities


ed_ed_size_addr = '/Users/eve/Desktop/Edit_Edit_Size_Tree/1_art_ed_ed_size.txt'
art_topic_addr = '/Users/eve/Desktop/Edit_Edit_Size_Tree/4_art_topic.txt'

art_topic = open(art_topic_addr, 'w')

try:
	editor_ed_size = open(ed_ed_size_addr, 'r+')
except IOError:
	print("Failed to open "+ed_ed_size_addr)

split_info = []
text = []
texts = []
for line in editor_ed_size:
	if len(line)!=0:
		split_info = line.split(", ")
		for i in range(1, len(split_info), 2):
			text.append(split_info[i])
		texts.append(text)

dictionary = corpora.Dictionary(texts)
editor_ed_size.close()

revserse_map = dictionary.token2id

# print(revserse_map["227287"])
# print(dictionary[108])

# print(dictionary.token2id)


editor_ed_size = open(ed_ed_size_addr, 'r+')
corpus_vec = []
doc_vec = []
tup = ()
for line in editor_ed_size:
	if len(line)!=0:
		split_info = line.split(", ")
		for i in range(1, len(split_info), 2):
			tup = (revserse_map[split_info[i]], float(split_info[i+1]))
			doc_vec.append(tup)
		corpus_vec.append(doc_vec)
editor_ed_size.close()

# print(corpus_vec)

class MyCorpus(object):
	def __iter__(self):
		for vec in corpus_vec:
			yield vec

corpus = MyCorpus()

print("Finished storing corpus...\n\n")

# print(next(iter(corpus)))

# print("\n\n")

num_topics = 1000

lda = models.ldamodel.LdaModel(corpus, id2word=dictionary, num_topics=num_topics, passes=1)


doc_lda = lda[corpus]

print("Printing out topics...")
# print out topics overed for each article
article_id = 0
for article_topics in doc_lda:
	art_topic.write(str(dictionary[article_id]))
	for topic in article_topics:
		art_topic.write(", "+str(topic[0]))
	article_id += 1
	art_topic.write('\n')

print("Processed "+str(article_id)+" articles")
