import sys, re

if sys.argv < 2:
    print "usage %s <file.dot>"%(sys.argv[0])

file = open(sys.argv[1], "r+")
content=""
out=""
while(True):
    line = file.readline()
    if line=="":
        break
    content=content+line

label = re.compile('\[label="([^\\"]+)"\]')
while content != "":
    res = label.search(content)
    if res:
        group = res.group(1)
        out=out+content[0:res.start(1)]+group.replace("\n", "\\l")+content[res.end(1):res.end(0)]
        content=content[res.end(0):]
    else:
        out=out+content
        content=""

file.seek(0)
file.write(out);
