art_topic_addr = '/Users/eve/Desktop/DLA/art_topic_pass_2.txt'
art_id_addr = '/Users/eve/Desktop/DLA/art_id.txt'
decoded_art_topic_addr = '/Users/eve/Desktop/DLA/art_topic_new.txt'

decoded = open(decoded_art_topic_addr, 'w')

art_id = []

for line in open(art_id_addr):
	line = line.lower().strip().replace(',', ' ')
	art_id.append(line)

tmp = []
for line in open(art_topic_addr):
	line = line.lower().strip().replace(',', ' ')
	tmp = line.split()
	for i in range(0, len(tmp)):
		if i==0:
			decoded.write(art_id[int(tmp[0])])
		else:
			decoded.write(", ")
			decoded.write(tmp[i])
	decoded.write("\n")


