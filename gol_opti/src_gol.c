_width=24;
_pad=8;
// _data[0]: glider phase ('1' + 0 to 11)
// _data[0:7]: GOL seed ('$' + 6x6 bitmap)
// _data[7:23]: gilder animation ('$' + 4x4x4 bitmap)
_data="1$+=7@$&(+$$)*&$()*$&0*";
// _post[4:]: format string
_post=";;;;%c[4%d;3%dm%c%c[0m";
_skip_post=1;

// naming is a mess:
// T: 2 grids of 48x48 cells
// U: front GOL grid
// V: back GOL grid
// W: tmporary grid to swap front and back
// G: index used to display the code
// X: index used to compute and display the GOL grid
// I, J: index used to count the neigbors
// N: number of neighbors (counting itself)
// M: text mode (code = 0, code_string = 1)
// Z: is in string boolean (for color highlighting)
// L: color of the last character
int *U, *V, *W, T[4608], G, E=2304, usleep(), printf(), (*p)()=printf, I, J, L, M, N, X, Z;
// Q: pointer used to write code_string (M = 1)
// R: pointer used to write code (M = 0)
// A: character to draw
// C: temporary storage for character
// S: code_string (array not a pointer for mutability)
char *Q, *R, A=32, C, S[] = "#";

// display character in flight (A), with foreground color K and cyan background if cell is alive
int a(int K) {
	// S+691 = _post+4 (format string)
	// 27 = '\e' (ansi escape code)
	p(S+691, 27, U[X]*6, K, A, 27);
}

// compute and display next glider character
// K: glider cell boundary to check
int b(int K) {
	// switch text mode
	(M ? *Q : *R-35) || (M || R++) | (M = !M);
	// compute in flight character
	// responsible for:
	// - inserting quotes in code_string mode on boundaries
	// - unescaping characters in code mode
	// - advancing source pointer if necessary
	A = M & G%12==K
		? 34
		: (C = M ? *Q++ : *R++) * M || C-36 && 94-C ? C : C-2;
	// toggle in string boolean on quote
	34==A && (Z = !Z);
	// is in flight character lowercase letter
	K = A>96 && 123>A;
	// primary color highlighting:
	// - if not lowercase: 0
	// - if lowercase and last character was too: keep the same color
	// - if lowercase and last character was not:
	//   - if letter is 'c' or 'i': color as type (works for "char", "int")
	//     this gives a false positive for "if", so we don't use them here
	//   - if letter is not 'c' or 'i': color as function name
	//     this is why every variable is uppercase, to distinguish them from functions
	K = K && L
		? L
		: 99-A && A-105
			? K*5
			: 2;
	// remember primary color highlighting for the next character
	L = K;
	// display in flight character with final color highlighting compositing:
	// - if is in string or character is a quote: use string color
	// - if character is a number: use number color
	// - if primary is 0: use default color
	// - else use primary color
	a(Z || 34==A ? 1 : A>47 && A<58 ? 3 : K ? K : 7);
	// set space as next character in flight
	A=32;
}

int main() {
	// initialize the front GOL grid with the _data seed bitmap
	for (U=T; I<36; I++)
		U[I%6+48*(I/6)+839] = S[1+I/6]-36 >> I%6 & 1;
	for(
		V=T+E; // the back grid starts at the end of the front grid
		Q=S;   // code_string pointer is reset before each iteration
		*S=*S%12+49 // update glider phase
	) {
		// code pointer is reset before each iteration, skips the _data section
		R=S+23;
		for (X=0; X<E; X++) {
			N=0;
			// glider index, simulate up-left sliding with glider phase
			G = (X+*S*49-200)%E;
			// count neigbors (and itself)
			for (I=-1; I<2; I++)
				for (J=-1; J<2; J++)
					N += U[(X+I+J*48+E)%E];
			// new line after every 48 cells
			X%48-47 || p("\n");
			// get _data glider bitmap:
			S[4*(*S%49/3)+7+G/576]-36 >> G/12%4 & 1
				// compute and print two character if in alive glider cell
				? b(0) + b(11)
				// print two spaces if in dead glider cell
				: a(1) + a(1); 
			// update back GOL grid
			// back cell is alive if:
			// - alive and 2 neigbors => N==3
			// - dead  and 3 neigbors => N==3
			// - alive and 3 neigbors => U[X] & N==4
			V[X] = U[X] & N == 4 || N==3;
		}
		// reset cursor
		p("\e[48A");
		// swap GOL grids
		W=U;
		U=V;
		V=W;
		usleep(200000);
	}
}
