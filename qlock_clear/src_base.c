_escaped = 1;
_width = 80;

char*z,*s,c,*S="#";

// forward declare functions
int printf();
// global variables
int x,y;

void p(char c) {
	printf("%c", c);
	// automatically wraps when reaching the desired length
	if (++x == 80) {
		p('\n');
		x = 0;
		y++;
	}
}

int main() {
	// set coordinates and string pointers
	z = s = S;
	x = y = 0;
	for (;c=*s++;) {
		// if character is '#'
		if (c == 35) {
			// write the source code as a string literal
			for (;c=*z++;) {
				// split the string at each end of line
				if (x == 79) {
					p('"'); // this call automatically wraps to a new line
					p('"');
				}
				// print backslash to escape these character
				if (c == '"' || c == '\\') {
					p('\\');
				}
				p(c);
			}
		} else {
			// write the source code as code
			p(c);
		}
	}
	printf("\n");
}
