import sys

if len(sys.argv) <3:
    print("Usage: python3 mapping.py Big5-ZhuYin.map ZhuYin-Big5.map")
    sys.exit()

f = open(sys.argv[1], 'r', encoding='big5-hkscs')
l = f.read().split('\n')[:-1]
w = [x.split(' ') for x in l]
w = [[x[0], x[1].split('/') + [x[0]]] for x in w]
di = {}
for i in w:
    for j in i[1]:
        di.setdefault(j[0], set()).add(i[0])
fw = open(sys.argv[2], 'w', encoding='big5-hkscs')
for i in di.keys():
    fw.write("%s\t%s\n" % (i, ' '.join(di[i])))
fw.close()
