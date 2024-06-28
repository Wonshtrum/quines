_escaped = 1;
_width = 80;

char *ctime(),c,*t,*z,*s,*S="#";

int printf(),sleep(),time();
int x,y;
// bitmap font for characters 0 to 9 and ':'
int f[] = {31599,19812,14479,31207,23524,29411,29679,30866,31727,31719,1040};

void p(char c) {
	int i = x/5;
	int j = x%5;
	if (x>=0 && y>=0 && i<8 && j<3 && y < 5 && f[t[i]]>>(12-3*y+j) & 1) {
		printf("\e[44m%c\e[0m", c);
	} else {
		printf("%c", c);
	}
	if (++x == 80) {
		p('\n');
		x = 0;
		y++;
	}
}

int main() { for(;;) {
	unsigned long n;
	time(&n);
	// get time formatted as a string "DayName Month Day HH:MM:SS Year"
	t = ctime(&n);
	int spaces = 0;
	// skip 3 spaces to get the start "HH:MM:SS"
	while (spaces < 3) {
		if (*t == ' ') spaces++;
		t++;
	}
	// convert ASCII codes to corresponding numbers, inserting 10 as ':'
	t[0] -= '0';
	t[1] -= '0';
	t[2] = 10;
	t[3] -= '0';
	t[4] -= '0';
	t[5] = 10;
	t[6] -= '0';
	t[7] -= '0';

	z = s = S;
	x = y = 0;
	for (;c=*s++;) {
		if (c == 35) {
			for (;c=*z++;) {
				if (x == 79) {
					p('"');
					p('"');
				}
				if (c == '"' || c == '\\') {
					p('\\');
				}
				p(c);
			}
		} else {
			p(c);
		}
	}

	// move cursor to start
	printf("\n\e[17A");
	sleep(1);
}}
