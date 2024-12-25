# Advent calander 2024 day 3 part 2

file = open("Day3_input.txt", "r")
content = file.read()
data=[]
inside_brackets=[]
number_pair=[]
number1=0
number2=0
sum_of_numbers=0
new_content=0

should_i_do_it=[]
good_data=[]


should_i_do_it=content.split("do()")
for n in range(len(should_i_do_it)):
        if (should_i_do_it[n].find("don't()"))!=-1:
            good_data=should_i_do_it[n].rsplit("don't()")
            good_data.pop()
        else:                                                                                   #if teh last bit is do it will add it at the end of good data
                good_data.append(should_i_do_it[n])
new_content= (''.join(map(str,good_data)))      
            

#same code as in part 1 as i am lazy

data=new_content.rsplit("mul(")                                                                  #check if it contains mul instruction
for a in range(len(data)):
        if (data[a].find(")")!=-1):                                                             # if it contains mulC and then C there is a chance taht between brackets is valid data
                inside_brackets=data[a].rsplit(")")
                inside_brackets.pop()                                                           # no need ofr rubbish outside brackets
                if (inside_brackets[0].find(","))!=-1:                                          # if the data are separated b , there is a chnce they aqre legit data
                        number_pair= inside_brackets[0].rsplit(",")
                        if number_pair[0].isnumeric() and number_pair[1].isnumeric():           #only legint data of they are numbers
                                
                                number1=int(number_pair[0])
                                number2=int(number_pair[1])
                                sum_of_numbers=sum_of_numbers+number1*number2
                                
print(sum_of_numbers)                    
        

        



file.close()
