import urllib2
import re

def GetTitles(cate):
    url = "https://en.wikipedia.org/w/api.php?action=query&format=xml&list=categorymembers&cmnamespace=0&cmprop=ids|title&cmlimit=500&cmpageid=" + cate
    art = []                                        #list of all accumulated articles
    next = ''    
                                             #information for the next request
    while True:
        req = urllib2.Request(url+next)
        response = urllib2.urlopen(req)
        response = response.read()     #web request

        art += re.findall('<cm [^>]*>', response)  #adds all article info from the current request to the list

        cont = re.search('<continue cmcontinue="([^"]+)"', response)
        if not cont:                                      #break the loop if 'continue' element missing
            break

        next = "&cmcontinue=" + cont.group(1)             #gets the revision Id from which to start the next request

    return art



if __name__ == "__main__":

    id_file = open("/Users/eve/Desktop/article_ids.txt", "w")
    art_file = open("/Users/eve/Desktop/article_name.txt", "w")
    
    start = 104624 #haven't run this category yet
    # start = 0
    counter = start

    cate_ids = []


    with open("/Users/eve/Desktop/cate_ids_0_44949.txt",'r') as f:
        c = 0
        for line in f:
            line = line.strip('\n')
            if c >= start:
                cate_ids.append(line)
            c += 1
    f.close()



    article = []
    artname = ""
    artid = ""
    
    while cate_ids:
        article = GetTitles(cate_ids[0])
        for i in article[0:len(article)]:
            # extract article title for current category (cate_name[0])
            artname = re.search('title="(.+?)"', i).group(1)
            artname = artname.replace(" ","_")
            art_file.write(artname)
            art_file.write('\n')
            
            artid = re.search('pageid="(.+?)"', i).group(1)
            # print artid
            id_file.write(artid)
            id_file.write('\n')

        art_file.write('\n\n') #seperation for each (sub)category
        id_file.write('\n\n')

        cate_ids.pop(0)
        print counter
        counter += 1
        


    art_file.close()
    id_file.close()
    print "Finished!!!!!Finallyyyyyyyyy"



    






