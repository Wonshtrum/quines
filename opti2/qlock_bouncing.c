_width = 80;
_pad = 53;
_data = "B4B'%B<8588B*'B:8,B8@$@&B8B:8B#-#";
_post = "//== Wonshtrum ==";

char *ctime(),c,*t,*z,*s,*S="#";

int time(),usleep(),x,y,O,X,Y,m,n[2],printf();

int p(int c) {
	printf(
		x>=0 & y>=0 & x<40 & y<5 & x%5<3 & S[3*t[x/5]-144+x%5]-35>>y
		? "\e[44m%c\e[0m"
		: "%c", c
	);
	++x-80-O || p(10) | y++ | (x = O);
}

int main() {
	for(X=Y=1;time(n);printf("\e[12A")+usleep(200000)) {
		t = ctime(n);
		for(c=3;c-=*t++==32;z=S);
		Y = y>=0 | y<-6  ? -Y : Y;
		X = x>=0 | x<-41 ? -X : X;
		for(s=z+33;c=*s++;)
			if (c-35) c-36 && c-94 ? p(c) : p(c-2);
			else for(;c=*z++; x-79-O || p(34)+p(34)) p(c);
		x = O+=X;
		y += Y-12;
	}
}

