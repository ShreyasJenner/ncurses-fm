f = open("output","r")
l = []
for x in f:
    l.append(x.strip())
f.close()

print(l)
