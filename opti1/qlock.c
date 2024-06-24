_escaped = 1;
_width = 80;
_post = " MIT Copyright (c) Wonshtrum // Inspired by Tsoding";

char *ctime(),c,*t,*z,*s,*S="#";

// we can merge declaration of integers and functions returning an integer... yes
// the font bitmap is stored bits reversed (this avoids the "[12-...]") and minus 31206 to save 5 characters:
//        {31599,19812,14479,31207,23524,29411,29679,30866,31727,31719,1040}-12
//                                                                  +31406
int f[] = {393,-12402,-1859,1,-12281,-16663,489,-21823,521,9,-30166},x,y,n[2],time(),sleep(),printf();

// use int as it is the shortest type
// saves 2 characters compared to "void p(char c)"
int p(int c) {
	// declaring and initializing i and j to hold "x/5" and "x%5" is longer than
	// repeating them directly
	printf(
		// substract '0' to character and add back 31206 to the bitmap
		// use bitwise and instead of logical and to save one character each time
		x<40 & y<5 & x%5<3 & f[t[x/5]-48]+31206>>(3*y+x%5)
		// use ternary to switch the formatting string
		? "\e[44m%c\e[0m"
		: "%c", c
	);
	// the right side of a logical and is evaluated only if its right side is true
	// so this works exactly as an if
	++x==80 && p(10) | y++ | (x = 0);
}

int main() {
	// use "int[2]" instead of "unsigned long"
	for(;time(n);printf("\n\e[13A")+sleep(1)) {
		t = ctime(n);
		// skip 3 spaces and reset string pointers
		for(c=3;c-=*t++==32;z=s=S);
		// set ':' delimters
		t[2] = t[5] = 58;
		// reset coordinates
		for(x=y=0; c=*s++;)
			if (c-35) p(c);
			else for(;c=*z++; x-79 || p(34)+p(34))
				// if not escape print c, if escape print '\' then c
				c-34 && c-92 ? p(c) : p(92)+p(c);
	}
}
