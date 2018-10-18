import urllib2
import re

def GetTitles(cate):
    url = "https://en.wikipedia.org/w/api.php?action=query&format=xml&list=categorymembers&cmnamespace=0&cmprop=ids|title&cmlimit=500&cmtitle=Category:" + cate
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

def GetSubcate(cate):
    url = "https://en.wikipedia.org/w/api.php?action=query&format=xml&list=categorymembers&cmtype=subcat&cmlimit=500&cmtitle=Category:" + cate
    subcat = []                                        
    next = ''    
                                           
    while True:
        req = urllib2.Request(url+next)
        response = urllib2.urlopen(req)
        response = response.read()     #web request

        subcat += re.findall('<cm [^>]*>', response)  

        cont = re.search('<continue cmcontinue="([^"]+)"', response)
        if not cont:                                      #break the loop if 'continue' element missing
            break

        next = "&cmcontinue=" + cont.group(1)             
    return subcat



if __name__ == "__main__":

    id_file = open("/Users/eve/Desktop/Wiki/Life/Article_And_Category/article_id_104624.txt", "w")
    art_file = open("/Users/eve/Desktop/Wiki/Life/Article_And_Category/article_104624.txt", "w")
    cate_file = open("/Users/eve/Desktop/Wiki/Life/Article_And_Category/category_104624.txt", "w")
    cate_name = []
    # subcat = 0
    seen = set()

  
    start = 104624 #haven't run this category yet
    counter = start
    with open("/Users/eve/Desktop/Wiki/Life/Article_And_Category/category.txt",'r') as f:
        c = 0
        for line in f:
            line = line.strip('\n')
            seen.add(line)
            if c >= start:
                cate_name.append(line)
                seen.add(line)
            print c
            c += 1





    while cate_name:
        article = GetTitles(cate_name[0])
    
        for i in article[0:len(article)]:
            # extract article title for current category (cate_name[0])
            try:
                artname = re.search('title="(.+?)"', i).group(1)
                artid = re.search('pageid="(.+?)"', i).group(1)
            except AttributeError:
                artname = ''
                artid = ''
            artname = artname.replace(" ","_")
            art_file.write(artname)
            id_file.write(artid)
            art_file.write('\n')
            id_file.write('\n')

        art_file.write('\n\n') #seperation for each (sub)category
        id_file.write('\n\n')

        tmp = GetSubcate(cate_name[0])
        for i in tmp[0:len(tmp)]:
            try:
                found = re.search('title="Category:(.+?)"', i).group(1)
            except AttributeError:
                found = ''
            found = found.replace(" ","_")
            found = found.lower()
            # subcat += 1
            if found not in seen:
                seen.add(found)
                cate_name.append(found)
                cate_file.write(found)
                cate_file.write('\n')


        cate_name.pop(0)
        counter += 1
        print counter


    art_file.close()
    cate_file.close()
    id_file.close()
    print "Finished!!!!!Finallyyyyyyyyy"



    






