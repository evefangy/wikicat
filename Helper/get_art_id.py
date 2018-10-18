import urllib2
import re

def GetArtId(cate_name):
    url = "https://en.wikipedia.org/w/api.php?action=query&format=xml&titles=:" + cate_name
    subcat = [] 
    found = ''
    # while True:
    req = urllib2.Request(url)
    response = urllib2.urlopen(req)
    response = response.read()     #web request
    subcat += re.findall('<page [^>]*>', response)  
    # cont = re.search('<continue cmcontinue="([^"]+)"', response)
    # if not cont:                                      #break the loop if 'continue' element missing
    #     break   
    try:
        found = re.search('pageid="(.+?)"',subcat[0]).group(1)
    except AttributeError:
        found = ''
        print cate_name
        print ("No pageid is found for the article")
    return found  



if __name__ == "__main__":

    id_file = open("/Users/eve/Desktop/article_ids.txt", "w")
    error_file = open("/Users/eve/Desktop/article_with_error_ids.txt", "w")

    start = 0 #haven't run this category yet
    # start = 0
    counter = start

    art_title = []

    with open("/Users/eve/Desktop/combined.txt",'r') as f:
        c = 0
        for line in f:
            line = line.strip('\n')
            if len(line.strip()) != 0 :
                if c >= start:
                    art_title.append(line)
                c += 1
    f.close()



    article = []
    artid = ""
    
    while art_title:
        artid = GetArtId(art_title[0])
        id_file.write(artid)
        id_file.write('\n')

        if(artid == ''):
            error_file.write(art_title[0])
            error_file.write(', ')
            error_file.write(str(counter)) #record the line number of which the error article exisits 
            error_file.write('\n')


        art_title.pop(0)
        print counter
        counter += 1

    error_file.close()
    id_file.close()
    print "Finished!!!!!Finallyyyyyyyyy"



    






