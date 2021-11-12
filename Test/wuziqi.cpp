#include<bits/stdc++.h>
#include<conio.h>
#define MAXIMUS 31
	int p[MAXIMUS][MAXIMUS];
	int b[MAXIMUS][MAXIMUS]; 
	int Cx,Cy;
	int Now;
	char* showText;
	int count;
void Initialize(){
	int i,j;
	showText=(char*)""; 
	count=0; 
	for(i=1;i<MAXIMUS;i++)
		for(j=1;j<MAXIMUS;j++) p[i][j]=0; 
	Cx=Cy=MAXIMUS/2;
	Now=1;
}
void Display(){
	int i,j,l=strlen(showText);
    int offset=2*(7-(l/10));
    system("cls");
	printf("\n\n");
	for(i=1;i<=29;i++){
		if (i%2==1)printf("%2d",(i+1)/2);
        else printf("  ");
		for(j=1;j<=29;j++){
            if (i==15&&l>0&&j==offset)printf("%s",showText);
            if (i==15&&l>0&&j>=offset&&j<offset+l/2)continue;
			switch(b[i][j]){
				case 1:printf("┏"); break;
				case 2:printf("┓"); break;
				case 3:printf("┗"); break;
				case 4:printf("┛"); break;
				case 5:printf("┯"); break;
				case 6:printf("┷"); break;
				case 7:printf("┠"); break;
				case 8:printf("┨"); break;
				case 9:printf("┼"); break;
				case 10:printf("━━━");break;
				case 11:printf("───");break;
				case 12:printf("┃");break;
				case 13:printf("│");break;
                case 14:printf("   ");break;
                case 15:printf(" ┏ ");break;
                case 16:printf(" ┓ ");break;
                case 17:printf(" ┗ ");break;
                case 18:printf(" ┛ ");break;
				case 19:printf("●");break;
				case 20:printf("○");break;
                case 99:printf("──");break;
				default: break;
			}
		}
		printf("\n");
	}
	printf("  A   B   C   D   E   F   G   H   I   J   K   L   M   N   O \n");
}
void Print(){
	int i,j;
	for(i=1;i<=29;i++){
		for (j=1;j<=29;j++){
            if (p[i][j]==1) b[i][j]=19;
            else if (p[i][j]==2) b[i][j]=20;
            else if(b[i][j-1]==19||b[i][j-1]==20)b[i][j]=99;
			else if (i%2==0){
				if (j==1||j==29)b[i][j]=12;
				else if (j%2==1)b[i][j]=13;
                else b[i][j]=14;
			}
			else if (j%2==0){
				if (i==1||i==29) b[i][j]=10;
				else if (i%2==1) b[i][j]=11;
                else b[i][j]=14;
			}
			else if (i==1&&j==1) b[i][j]=1;
			else if (i==1&&j==29) b[i][j]=2;
			else if (i==29&&j==1) b[i][j]=3;
			else if (i==29&&j==29) b[i][j]=4;
			else if (i==1) b[i][j]=5;
			else if (i==29) b[i][j]=6;
			else if (j==1) b[i][j]=7;
			else if (j==29) b[i][j]=8;
			else b[i][j]=9;
		}
	}
    b[Cx-1][Cy-1]=15;
    b[Cx-1][Cy+1]=16;
    b[Cx+1][Cy-1]=17;
    b[Cx+1][Cy+1]=18;
	Display();
}
int Put(){
	if(p[Cx][Cy]==0) { 
		p[Cx][Cy]=Now; 
		return 1;
	} else return 0; 
} 
int Check(){
	int w=1,x=1,y=1,z=1,i; 
	for(i=1;i<5;i++)if(Cy+2*i<MAXIMUS&&p[Cx][Cy+2*i]==Now)w++;else break;
	for(i=1;i<5;i++)if(Cy-2*i>0&&p[Cx][Cy-2*i]==Now)w++;else break;
	if(w>=5)return Now;
	for(i=1;i<5;i++)if(Cx+2*i<MAXIMUS&&p[Cx+2*i][Cy]==Now)x++;else break;
	for(i=1;i<5;i++)if(Cx-2*i>0&&p[Cx-2*i][Cy]==Now)x++;else break; 
	if(x>=5)return Now;
	for(i=1;i<5;i++)if(Cx+2*i<MAXIMUS&&Cy+2*i<MAXIMUS&&p[Cx+2*i][Cy+2*i]==Now)y++;else break;
	for(i=1;i<5;i++)if(Cx-2*i>0&&Cy-2*i>0&&p[Cx-2*i][Cy-2*i]==Now)y++;else break; 
	if(y>=5)return Now;
	for(i=1;i<5;i++)if(Cx+2*i<MAXIMUS&&Cy-2*i>0&&p[Cx+2*i][Cy-2*i]==Now)z++;else break;
	for(i=1;i<5;i++)if(Cx-2*i>0&&Cy+2*i<MAXIMUS&&p[Cx-2*i][Cy+2*i]==Now)z++;else break;
	if(z>=5)return Now; 
	return 0;
} 
int RunGame(){
	int input; 
	int victor;
	Initialize(); 
	while(1){
		Print();
		input=getch();
		if(input==27)exit(0); 
		else if(input==0x20){
			if(Put()){
				victor=Check(); 
				Now=3-Now;
				count++; 
				if(victor==1){
					showText=(char*)"黑方获得了胜利!"; 
					Print(); 
					if(getch()==0xE0) getch();  
					return Now; 
				} 
				else if(victor==2){ 
					showText=(char*)"白方获得了胜利!"; 
					Display(); 
					if(getch()==0xE0) getch(); 
					return Now; 
				}
				else if(count==225){ 
					showText=(char*)"平局!"; 
					Display(); 
					if(getch()==0xE0) getch();
					return 0; 
				} 
			} 
		} 
		else if(input==0xE0){
			input=getch();
			switch(input){
				case 0x4B:Cy-=2; break; 
				case 0x48:Cx-=2; break; 
				case 0x4D:Cy+=2; break; 
				case 0x50:Cx+=2; break; 
			} 
			if(Cx<0)Cx=29;
			if(Cy<0)Cy=29; 
			if(Cx>MAXIMUS-1)Cx=1; 
			if(Cy>MAXIMUS-1)Cy=1; 
		} 
	} 
}
int main(){
	system("title 五子棋 By:机长大大");
	system("color A1");
    system("mode con cols=62 lines=32");
    while(1) RunGame();
    system("pause");
    return 0;
}