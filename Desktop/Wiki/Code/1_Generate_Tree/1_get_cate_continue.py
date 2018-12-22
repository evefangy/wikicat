import urllib2
import re

def GetSubcate(cate):
    url = "https://en.wikipedia.org/w/api.php?action=query&format=xml&list=categorymembers&cmtype=subcat&cmlimit=500&cmpageid=" + str(cate)
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

def GetCateId(cate_name):
    url = "https://en.wikipedia.org/w/api.php?action=query&format=xml&titles=Category:" + cate_name
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
    if len(subcat)>1:
        printf("\n\rGet multiple ids for the category")
        return
    try:
        found = re.search('pageid="(.+?)"',subcat[0]).group(1)
    except AttributeError:
        found = ''
        printf("No pageid is found for the category")
    return found                                     
    


if __name__ == "__main__":

    start = 44949
 
    add = "/Users/eve/Desktop/Wiki/Life/All_Cate/cate_ids_"
    add += str(start)
    add += ".txt"

    add2 = "/Users/eve/Desktop/Wiki/Life/All_Cate/cate_name_"
    add2 += str(start)
    add2 += ".txt"


    id_file = open(add, "w")
    cate_file = open(add2, "w")

    tmp = []
    seen = set()
    cate_name = []
    found = ''
    c_id = 0
    c = 0
    counter = 0


    counter = start
    with open("/Users/eve/Desktop/Wiki/Life/All_Cate/cate_ids.txt",'r') as f:
        c = 0
        for line in f:
            line = line.strip('\n')
            seen.add(int(line))
            if c >= start:
                cate_name.append(int(line))
                seen.add(int(line))
            # print c
            c += 1

    # main_cate = "Life"
    # #category id for main category
    # found  = GetCateId(main_cate)
    # c_id = int(found)
    # seen.add(c_id)
    # cate_name.append(c_id)
    # id_file.write(found)
    # cate_file.write(main_cate)



    while cate_name:
        tmp = GetSubcate(cate_name[0])
        for i in tmp[0:len(tmp)]:
            found = re.search('pageid="(.+?)"', i).group(1)
            c_id=int(found)
            if c_id not in seen:
                seen.add(c_id)
                cate_name.append(c_id)
                id_file.write('\n')
                id_file.write(found)
                # id_file.write('why')
                found = re.search('title="Category:(.+?)"', i).group(1)
                found = found.replace(" ","_")
#                found = found.lower()
                cate_file.write('\n')
                cate_file.write(found)                            
        tmp = []
        cate_name.pop(0)
        counter += 1
        print counter

    cate_file.close()
    id_file.close()
    print "Finished!!!!!Finallyyyyyyyyy"



    






