import random

s = ""
c=0
for i in range(int(1e3)):

    if random.random() <= 0.2:
        s += "("
        c+=1


    s+= str(random.randint(1,9))

    if random.random() <= 0.2 and c>0:
        s += ")"
        c-=1
    s+=random.choice(["*","+","-","/"])



s+=str(random.randint(1,9))
for i in range(c):
    s+=")"
open("input","w").write(s)
