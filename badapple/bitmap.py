from PIL import Image, ImageSequence
from time import sleep

gif = Image.open("badapple.gif")

# Bitmap
class Compress1:
    def __init__(self):
        self.c = 0
        self.d = 0
        self.r = ""
    def put(self, v):
        self.d += v<<self.c
        self.c += 1
        if self.c >= 6:
            self.flush()
    def flush(self):
        if self.c > 0:
            self.r += chr(35+self.d)
            self.d = self.c = 0

# Run Length
class Compress2:
    def __init__(self):
        self.c = 0
        self.d = 0
        self.h = 63
        self.hc = 0
        self.r = ""
    def put(self, v):
        self.d += v<<self.c
        self.c += 1
        if self.c >= 5:
            self.flush(self.d)
    def flush(self, d=-1):
        if d == self.h:
            self.hc += 1
        else:
            if self.hc >= 2:
                n = 35+32+self.hc-2
                if n > 126:
                    self.r += "  "
                else:
                    self.r += chr(n)
            if self.hc > 0:
                self.r += chr(93+self.h)
            self.hc = 1
            self.h = d
        self.d = self.c = 0

# LZ77
class Compress3:
    def __init__(self, w):
        self.g = []
        self.r = ""
        self.w = w
    def put(self, v):
        self.g.append(v)
    def flush(self):
        l = len(self.g)
        i = 0
        c = Compress1()
        while i < l:
            J = 0
            M = 1
            for j in range(max(i-self.w, 0), i):
                n = 0
                while j+n<l and self.g[i]==self.g[j+n]:
                    n += 1
                if n > M:
                    M = n
                    J = j
            if M<6:
                n = 0
                while n < 6 and i+n < l:
                    n += 1
                    c.put(self.g[i+n])
                i += 6
                self.r += " "
            else:
                self.r += chr(35+64+i-J-1)+chr(35+M-6)
                if M>96:
                    self.r += " "
                    print(i-J, M)
                i += M

# delta frames
class Compress4:
    def __init__(self, s, v=False):
        self.o = [0]*s
        self.g = [0]*s
        self.c = 0
        self.r = ""
        self.v = v
        self.s = s
        self.f = [{}, {}, {}]
        self.saved = 0
    def put(self, v):
        self.g[self.c] = v
        self.c += 1
        if self.c == self.s:
            self.flush()
            self.c = 0
    def flush(self):
        if self.c == 0:
            return
        n = 0
        i = 0
        r = ""
        ox = self.g[0] != self.o[0]
        while i < self.s:
            x = self.g[i] != self.o[i]
            if x == ox:
                n += 1
            elif n <= 5:
                self.f[2][n] = self.f[2].get(n, 0) + 1
                d = (1<<n)-1 if ox else 0
                while i < self.s and n < 5:
                    d += (self.g[i] != self.o[i]) << n
                    n += 1
                    i += 1
                r += chr(93+d)
                if self.v: print("xor", bin(d))
                if i < self.s:
                    x = self.g[i] != self.o[i]
                n = 1
            else:
                self.f[ox][n] = self.f[ox].get(n, 0) + 1
                n -= 5
                if ox:
                    if self.v: print("jmp", n, 1)
                    r += "#}"*(n//90) + "#"
                    if n%90 != 0:
                        r += chr(35+n%90)
                        i += 1
                elif n < 56-8:
                    if self.v: print("jmp", n, 0)
                    r += chr(35+n)
                    i += 1
                else:
                    if self.v: print("ljmp", n, 0)
                    if (n>>3) > 89:
                        raise Exception(n)
                    r += chr(35+56-8+(n & 7)) + chr(35+(n>>3))
                    i += 1
                if i < self.s:
                    x = self.g[i] != self.o[i]
                n = 1
            i += 1
            ox = x
        if ox and n:
            if n <= 5:
                if self.v: print("xor", bin(31))
                r += chr(93+31)
            else:
                if self.v: print("jmp", n, 1)
                n -= 5
                r += "#}"*(n//90) + "#" + chr(35+n%90)
        for i in range(self.s):
            self.o[i] = self.g[i]
        self.r += r + "~"

def distribution(r):
    from collections import Counter
    a = Counter(r)
    for c in range(35, 126):
        n = a[chr(c)]
        print(f"{c:3}", str(chr(c)), "█"*(n//2)+"▌"*(n%2), n)

class FakeFrame:
    def __init__(self, data, w, W=1):
        self.data = [data[i:i+w] for i in range(0, len(data), w)]
        self.height = len(self.data)
        self.width = len(self.data[0])//W
        self.W = W
    def getpixel(self, c):
        return (0,0,0) if self.data[c[1]][c[0]*self.W] == " " else (255,255,255)

END = [
#========================================#
"                                        "
"                                        "
"                                        "
"                   ###                  "
"                   ####                 "
"                    ####                "
"                   ###########          "
"                    ###########         "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        ",
#========================================#
"                                        "
"                                        "
"                                        "
"                                        "
"                    ##                  "
"                     ##                 "
"                    ##########          "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        ",
#========================================#
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                     ##                 "
"                      ####              "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        ",
#========================================#
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                       ##               "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        ",
#========================================#
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                       #                "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        ",
#========================================#
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        "
"                                        ",
#========================================#
]

def screen(s, W, w, h, px, py, i=0):
    r = ""
    for c in s:
        if i%W == 0:
            r += "\n"
        r += c
        if i//W>=py and i//W<py+h and i%W==px-1:
            r += " "*w
            i += w
        i += 1
    return r

def display(frame, c, i=0):
    s = 60
    if i > 150:
        s = 80
    elif i > 75:
        s = 40
    for y in range(frame.height):
        for x in range(frame.width):
            if frame.getpixel((x, y))[0] < s:
                print(" "*W, end="")
                c.put(0)
            else:
                print("\x1b[41m"+" "*W+"\x1b[0m", end="")
                c.put(1)
        print()

# Verbose
V = 0
# Pixel Width
W = 1
# Skip frames
S = 1
# Speed
D = 2
c = Compress1()
c = Compress2()
c = Compress3(26)
c = Compress4((40//W)*15, V)
for i, frame in enumerate(ImageSequence.Iterator(gif)):
    if i > 183:
        break
    if i > 0:
        sleep(frame.info['duration']/(D*1000))
    if i % S > 0:
        continue
    if frame.mode not in ("RGB", "RGBA"):
        continue
    display(frame.resize((40//W, 15)), c, i)
    if V:
        input()
        pass
    else:
        print("\x1b[16A")

for frame in END[1:]:
    sleep(100/(D*1000))
    display(FakeFrame(frame, 40, W), c)
    if V:
        input()
        pass
    else:
        print("\x1b[16A")

print("\n"*15)
c.flush()
print(c.r)
distribution(c.r)
s = screen(c.r, 118, 40, 15, 39, 19, 20)
print(s)
print(len(c.r))
print(len(s.replace("\n","")))
