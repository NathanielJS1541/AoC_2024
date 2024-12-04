file = open("input.txt", "r")
content = file.readlines()

column1=[]
column2=[]

for l in content:
    as_list=l.split("   ")
    column1.append(as_list[0])
    column2.append(as_list[1].replace("\n",""))


column1.sort()
column2.sort()

difference =[]

for i in range (len(column1)):
    difference.append(abs(int(column1[i])-int(column2[i])))

print(sum(difference))
file.close()
