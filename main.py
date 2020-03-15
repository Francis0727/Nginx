print ("print a number : ")
print ("if the number is int , please jisuan sum :")

theNumStr = raw_input(" number : " )
theSum = 0

while theNumStr != "." :
    if not theNumStr.isdigit() :
        print ("Error")
        print ("print a number :" )
        theNumStr = raw_input("number :")
        continue     
    theNum = int (theNumStr)
    theSum += theNum
    theNumStr = raw_input("number :")

print ("the Sum : " ,theSum)



      
    