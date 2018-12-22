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
    # print "article length: "
    # print len(art)
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
    # print "subcat length: "
    # print len(subcat)
    return subcat



if __name__ == "__main__":
  
    level = 0
    art_num = 0
    curr_cat_num = 0 #number of node we are looking at for current level
    curr_level_max = 1 #number of subcategories of current level 
    next_level_max = 0 #number of subcategories of next level

    id_file = open("/Users/eve/Desktop/Wiki/Life/New/article_id_104624.txt", "w")
    art_file = open("/Users/eve/Desktop/Wiki/Life/New/article_104624.txt", "w")
    cate_file = open("/Users/eve/Desktop/Wiki/Life/New/category_104624.txt", "w")
    # data_file = open("/Users/eve/Desktop/Wiki/Life/data.txt", "w")
    cate_name = []
    subcat = 0
    seen = set()

  
    start = 104624 #haven't run this category yet
    counter = start
    with open("/Users/eve/Desktop/Wiki/Life/Article_And_Category/category.txt",'r') as f:
        c = 0
        for line in f:
            line = line.strip('\n')
            seen.add(line)
            if c >= start:
                # print line
                cate_name.append(line)
                seen.add(line)
            c += 1





    while cate_name:
        # data_file.write(cate_name[0])

        article = GetTitles(cate_name[0])
    
        for i in article[0:len(article)]:
            # extract article title for current category (cate_name[0])
            try:
                artname = re.search('title="(.+?)"', i).group(1)
                artid = re.search('pageid="(.+?)"', i).group(1)
                # print artid
            except AttributeError:
                artname = ''
                artid = ''
            artname = artname.replace(" ","_")
            art_file.write(artname)
            art_file.write('\n')
            id_file.write(artid)
            id_file.write('\n')

        art_file.write('\n\n') #seperation for each (sub)category
        id_file.write('\n\n')
        art_num += len(article) #increment article number for current level

        # data_file.write("  ")
        # data_file.write(str(len(article)))
        # data_file.write("\n")


        subcat = 0
        print cate_name[0]
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
        # cate_file.write('\n\n') #seperation for children in each category

        next_level_max += subcat
        curr_cat_num += 1
        
        # if curr_cat_num==curr_level_max/2:
        #     print "-----------------------------50 percent completet-----------------------------"

        # we are currently at the last category of current level
        if curr_cat_num==curr_level_max:
            # print "level, number of subcategories, number of articles of the level"
            # print level
            # print next_level_max #category num of each level
            # print art_num #article num of each level
            art_num = 0
            level += 1
            curr_cat_num = 0
            curr_level_max = next_level_max
            next_level_max = 0

        cate_name.pop(0)
        counter += 1
        if counter%10 == 0:
            print counter


    # for i in cate_name[0:len(cate_name)]:   
    #     print i

    art_file.close()
    cate_file.close()
    id_file.close()



    






