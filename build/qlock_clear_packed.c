char*ctime(),c,*t,*z,*s,*S="char*ctime(),c,*t,*z,*s,*S=\"#\";int printf(),sleep"
"(),time();int x,y;int f[]={31599,19812,14479,31207,23524,29411,29679,30866,317"
"27,31719,1040};void p(char c){int i=x/5;int j=x%5;if(x>=0&&y>=0 &&i<8&&j<3&&y<"
"5&&f[t[i]]>>(12-3*y+j)&1){printf(\"\\e[44m%c\\e[0m\",c);}else{printf( \"%c\",c"
");}if(++x==80){p('\\n');x=0;y++;}}int main(){for(;;){unsigned long n;time(&n);"
"t=ctime(&n);int spaces=0;while(spaces<3){if(*t==' ')spaces++;t++;}t[0]-='0';t["
"1]-='0';t[2]=10;t[3]-='0';t[4]-='0';t[5]=10;t[6]-='0';t[7]-='0';z=s=S;x=y=0;fo"
"r(;c=*s++;){if(c==35){for(;c=*z++;){if(x==79){p('\"');p('\"');}if(c=='\"'||c=="
"'\\\\'){p('\\\\');}p(c);}}else{p(c);}}printf(\"\\n\\e[17A\");sleep(1);}}/////";
int printf(),sleep(),time();int x,y;int f[]={31599,19812,14479,31207,23524,29411
,29679,30866,31727,31719,1040};void p(char c){int i=x/5;int j=x%5;if(x>=0&&y>=0 
&&i<8&&j<3&&y<5&&f[t[i]]>>(12-3*y+j)&1){printf("\e[44m%c\e[0m",c);}else{printf( 
"%c",c);}if(++x==80){p('\n');x=0;y++;}}int main(){for(;;){unsigned long n;time(&
n);t=ctime(&n);int spaces=0;while(spaces<3){if(*t==' ')spaces++;t++;}t[0]-='0';t
[1]-='0';t[2]=10;t[3]-='0';t[4]-='0';t[5]=10;t[6]-='0';t[7]-='0';z=s=S;x=y=0;for
(;c=*s++;){if(c==35){for(;c=*z++;){if(x==79){p('"');p('"');}if(c=='"'||c=='\\'){
p('\\');}p(c);}}else{p(c);}}printf("\n\e[17A");sleep(1);}}/////
