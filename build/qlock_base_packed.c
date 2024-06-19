char*ctime(),c,*t,*z,*s,*S="char*ctime(),c,*t,*z,*s,*S=\"#\";int printf();int x"
",y;void p(char c){printf(\"%c\",c);if(++x==80){p('\\n');x=0;y++;}}int main(){z"
"=s=S;x=y=0;for(;c=*s++;){if(c==35){for(;c=*z++;){if(x==79){p('\"');p('\"');}if"
"(c=='\"'||c=='\\\\'){p('\\\\');}p(c);}}else{p(c);}}printf(\"\\n\");}/////////";
int printf();int x,y;void p(char c){printf("%c",c);if(++x==80){p('\n');x=0;y++;}
}int main(){z=s=S;x=y=0;for(;c=*s++;){if(c==35){for(;c=*z++;){if(x==79){p('"');p
('"');}if(c=='"'||c=='\\'){p('\\');}p(c);}}else{p(c);}}printf("\n");}/////////
