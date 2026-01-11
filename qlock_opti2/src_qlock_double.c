_width = 80;
_pad = 52;
_data = "B4B'%B<8588B*'B:8,B8@$@&B8B:8B#-#";
_post = "////";

char *ctime(),c,*t,*z,*s,*S="#";

int printf(),time(),sleep(),x,y,n[2];

int p(int c) {
	printf(
		x<80 & y<10 & x/2%5<3 & S[3*t[x/10]+x/2%5-144]-35>>y/2
		? "\e[4""4m%c\e[0m"
		: "%c", c
	);
	++x-80 || p(10) | y++ | (x = 0);
}

int main() {
	for(;time(n);x=y=0) {
		t = ctime(n);
		for(c=3;c-=*t++==32;z=S);
		for(s=z+33;c=*s++;)
			if (c-35) c-36 && c-94 ? p(c) : p(c-2);
			else for(;c=*z++; x-79 || p(34)+p(34)) p(c);
		printf("\e[10A");
		sleep(1);
	}
}

