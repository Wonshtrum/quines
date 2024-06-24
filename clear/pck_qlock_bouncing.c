char*ctime(),c,*t,*z,*s,*S="char*ctime(),c,*t,*z,*s,*S=\"#\";int printf(),uslee"
"p(),time();int x,y,X,Y,O;int f[]={31599,19812,14479,31207,////23524,29411,2967"
"9,30866,31727,31719,1040};void p(char c){int i=x/5;int j=x%5;if(x>=0&&y>=0&&i<"
"8&&j<3&&y<5&&f[t[i]]>>(12-3*y+j)&1){printf(\"\\e[44m%c\\e[0m\",c);}///else{pri"
"ntf(\"%c\",c);}if(++x==80+O){p('\\n');x=O;y++;}}int main(){x=0;y=0;O=0;X=-1;Y="
"-1;for(;;){unsigned long n;time(&n);t=ctime(&n);int spaces=0;while(spaces<3){i"
"f(*t==' ')spaces++;t++;}t[0]-='0';t[1]-='0';t[2]=10;t[3]-='0';t[4]-='0';t[5]=1"
"0;t[6]-='0';t[7]-='0';z=s=S;for(;c=*s++;){if(c==35){for(;c=*z++;){if(x==79+O){"
"p( '\"');p('\"');}if(c=='\"'||c=='\\\\'){p('\\\\');}p(c);}}else{p(c);}}O+=X;x="
"O;y-=20;y+=Y;if(x>=0||x<-41){X*=-1;}if(y>=0||y<-15){Y*=-1;}printf(\"\\n\\e[21A"
"\");usleep(200000);}}////////////////////////////////////////////////////////";
int printf(),usleep(),time();int x,y,X,Y,O;int f[]={31599,19812,14479,31207,////
23524,29411,29679,30866,31727,31719,1040};void p(char c){int i=x/5;int j=x%5;if(
x>=0&&y>=0&&i<8&&j<3&&y<5&&f[t[i]]>>(12-3*y+j)&1){printf("\e[44m%c\e[0m",c);}///
else{printf("%c",c);}if(++x==80+O){p('\n');x=O;y++;}}int main(){x=0;y=0;O=0;X=-1
;Y=-1;for(;;){unsigned long n;time(&n);t=ctime(&n);int spaces=0;while(spaces<3){
if(*t==' ')spaces++;t++;}t[0]-='0';t[1]-='0';t[2]=10;t[3]-='0';t[4]-='0';t[5]=10
;t[6]-='0';t[7]-='0';z=s=S;for(;c=*s++;){if(c==35){for(;c=*z++;){if(x==79+O){p( 
'"');p('"');}if(c=='"'||c=='\\'){p('\\');}p(c);}}else{p(c);}}O+=X;x=O;y-=20;y+=Y
;if(x>=0||x<-41){X*=-1;}if(y>=0||y<-15){Y*=-1;}printf("\n\e[21A");usleep(200000)
;}}////////////////////////////////////////////////////////
