_escaped = 1;
_width = 80;
_post = " MIT Copyright (c) Wonshtrum // Inspired by Tsoding";

char *ctime(),c,*t,*z,*s,*S="#";

int f[] = {393,-12402,-1859,1,-12281,-16663,489,-21823,521,9,-30166},x,y,O,X,Y,n[2],printf(),time(),usleep();

int p(int c) {
	printf(
		x>=0 & y>=0 & x<40 & y<5 & x%5<3 & 31206+f[t[x/5]-48]>>(3*y+x%5)
		? "\e[44m%c\e[0m"
		: "%c", c
	);
	++x==80+O && p(10) | y++ | (x = O);
}

int main() {
	for(X=Y=1;time(n);printf("\n\e[15A")+usleep(200000)) {
		t = ctime(n);
		for(c=3;c-=*t++==32;z=s=S);
		Y = y <-9  | y>=0 ? -Y : Y;
		X = x <-41 | x>=0 ? -X : X;
		for(t[2]=t[5]=58; c=*s++;)
			if (c-35) p(c);
			else for(;c=*z++; x-79-O || p(34)+p(34))
				c-34 && c-92 ? p(c) : p(92)+p(c);
		x = O+=X;
		y += Y-14;
	}
}
