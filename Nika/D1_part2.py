file = open("input.txt", "r")
content = file.readlines()

column1=[]
column2=[]
similarity =[]

for l in content:
    as_list=l.split("   ")
    column1.append(as_list[0])
    column2.append(as_list[1].replace("\n",""))

for i in range(len(column1)):
    similarity.append(column2.count(column1[i])*int(column1[i]))
    

print(sum(similarity))


file.close()
