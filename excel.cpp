#include <conio.h>
#include <stdarg.h>
#include <windef.h>
#include <winbase.h>
#include <wincon.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TAD_ListaGen.h"

void gotoxy(int x, int y)
{
  COORD c ;
  c.X = x - 1;
  c.Y = y - 1;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void textattr(int _attr)
{
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _attr);
}

void textbackground(int _color)
{
  CONSOLE_SCREEN_BUFFER_INFO Info ;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
  textattr((Info.wAttributes & 0x0F) | (_color<<4));
}

void textcolor(int _color) {
  CONSOLE_SCREEN_BUFFER_INFO Info ;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
  textattr((Info.wAttributes & 0xF0) | _color);
}

int wherex()
{
  CONSOLE_SCREEN_BUFFER_INFO Info;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
  return Info.dwCursorPosition.X + 1;
}

int wherey()
{
  CONSOLE_SCREEN_BUFFER_INFO Info;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
  return Info.dwCursorPosition.Y + 1;
}

void clrscr(void)
{
  COORD coord = { 0, 0 };
  DWORD dwWritten, dwSize;
  HANDLE hScreen=GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO Info;

  if(GetConsoleScreenBufferInfo(hScreen, &Info))
  {
      dwSize = Info.dwSize.X * Info.dwSize.Y;
      FillConsoleOutputCharacter (hScreen, ' ', dwSize, coord, &dwWritten);
      FillConsoleOutputAttribute (hScreen, Info.wAttributes, dwSize, coord, &dwWritten);
      gotoxy (1, 1);
  }
}
//-------------------------------------------------------------------

void infoEsq(int n, int info, char s_info[])
{
    int i, tam;
    itoa(info,s_info,10);
    tam = strlen(s_info);
    for (i=0; i<n-tam; i++)
        strcat(s_info," ");
}

void infoCenter(int info, char s_info[])
{
    char s[9];
    strcpy(s,"    ");
    sprintf(s_info,"%c",info);
    strcat(s,s_info);
    strcat(s,"    ");
    strcpy(s_info,s);
}

char intParaChar(int c) {
	return (char)(c + 'A'-1);
}

void ExibirCelulas(int lin, int col, TpDesc *desc) {
	int l=lin-20;
	int c, telaL=4, telaC=4;
	MatEsp *aux;
	textbackground(0);
    textcolor(15);
	if(desc!=NULL) {
		while(l<lin) {
			c=col-8;
			telaC=4;
			while(c<col) {	
				VerificaOcupado(desc->plinha,l,(char)c,&aux);
				if(aux!=NULL) {
					gotoxy(telaC,telaL);
					if(aux->valor[0]=='=') 
						printf("%.2f",ConstroiListaGen(aux->valor,desc));			
					else
						printf("%.9s",aux->valor);
				}
				c++;
				telaC=telaC+9;
			}
			l++;
			telaL++;	
		}
	}
}

void desenhaTela(int lin, int col, TpDesc *desc)
{
    int i,j;
    char slin[4],scol[9];
    textbackground(0);
    clrscr();
    textbackground(3);
    textcolor(0);
    //desenha a coluna esq 1..20
    for (i=4; i<=23; i++)
    {
        gotoxy(1,i);
        infoEsq(3,lin,slin);
        printf("%s",slin);
        lin=lin+1;
    }
    //desenha a linha sup A..H
    j=4;
    for (i=1; i<=8; i++)
    {
        gotoxy(j,3);
        infoCenter(col,scol);
        printf("%s",scol);
        j=j+9;
        col=col+1;
    }
    ExibirCelulas(lin,col,desc);
}

void GerarGrafico(TpDesc *desc, int l1, int l2, char c1, char c2) {
	clrscr();
	MatEsp *colunas;
	MatEsp *linhas;
	MatEsp *aux;
	int x, tela;
	float quantValores = (c2-c1)*1.00;
	float num, maior=-9999.99, menor=9999.99;
	VerificaOcupado(desc->plinha,l1,c1+1,&colunas);
	VerificaOcupado(desc->plinha,l1+1,c1,&linhas);
	if(quantValores>6) 
		tela = 23+(3*(quantValores-7));	
	else
		tela = 22;
	gotoxy(15,tela);
	for(int i=c1+1; i<=c2; i++) {
		VerificaOcupado(desc->plinha,l1,i,&aux);
		if(aux!=NULL)
			printf("%.8s   ",aux->valor);
		if(strlen(aux->valor)<8) {
			for(int j=strlen(aux->valor); j<8; j++)
				printf(" ");
		}	
	}
	for(int i=l1+1; i<=l2; i++) {
		for(int j=c1+1; j<=c2; j++) {
			VerificaOcupado(desc->plinha,i,j,&aux);
			if(aux!=NULL) {
				if(aux->valor[0]=='=') 
					num = ConstroiListaGen(aux->valor,desc);				
				else 
					num = atof(aux->valor);
				if(num>maior)
					maior = num;
				if(num<menor)
					menor = num;
			}
		}
	}
	gotoxy(10,tela);
	num = maior/(quantValores);
	for(int i=0; i<quantValores+1; i++) {
		gotoxy(6,(tela-1)-3*i);
		printf("%.2f -",menor+num*i);	
	}
	for(int i=l1+1; i<=l2; i++) { 
		textcolor((i-l1)*2);		
		for(int j=c1+1; j<=c2; j++) {
			VerificaOcupado(desc->plinha,i,j,&aux);			
			if(aux!=NULL) {
				if(aux->valor[0]=='=') 
					num = ConstroiListaGen(aux->valor,desc);				
				else 
					num = atof(aux->valor);
					
				x=round((3*quantValores*num)/maior);
				for(int l=tela-x;l<tela;l++) {
					gotoxy((3+11*(j-c1))+(i-l1),l);
					printf("%c",219);
				}
			}
		}	
	}	
	for(int i=l1+1; i<=l2; i++) {
		VerificaOcupado(desc->plinha,i,c1,&aux);
		gotoxy(5,(tela+1)+(i-l1));
		if(aux!=NULL) {
			textcolor((i-l1)*2);
			printf("%c ",219);
			textcolor(15);
			printf("%s",aux->valor);
		}	
	}	
}

void ChamarGerarGrafico(char valor[], TpDesc *desc) {
	int l1=0, l2=0;
	char c1=0, c2=0;
	BuscarCelulas(valor,&l1,&l2,&c1,&c2);
	GerarGrafico(desc,l1,l2,c1,c2);
}

void ModoAbrirPlanilha(TpDesc **desc) {
	char nome[100];
	gotoxy(1,1);
	textbackground(0);
    textcolor(15);
	printf("Digite o nome do arquivo que deseja abrir: ");
	fflush(stdin);
	gets(nome);
	AbrirPlanilha(&(*desc),nome);
}

void ModoSalvarPlanilha(TpDesc *desc) {
	char nome[100];
	gotoxy(1,1);
	textbackground(0);
    textcolor(15);
	printf("Digite o nome que deseja salvar a planilha: ");
	fflush(stdin);
	gets(nome);
	SalvarPlanilha(desc,nome);
}

void ModoGerarGrafico(TpDesc *desc) {
	char coordenadas[100];
	gotoxy(1,1);
	textbackground(0);
    textcolor(15);
	printf("Digite as coordenadas do grafico: ");
	fflush(stdin);
	gets(coordenadas);
	ChamarGerarGrafico(coordenadas,desc);
}

void ModoEditarCelula (int l, char c, TpDesc **desc) {
	MatEsp *aux;
	char valor[100];
	gotoxy(1,1);
	textbackground(0);
    textcolor(15);
	printf("%c%d: ",c,l);
	if((*desc)!=NULL) {
		VerificaOcupado((*desc)->plinha,l,c,&aux);
		if(aux!=NULL)
			printf("%s",aux->valor);
	}
	printf("\n");
	fflush(stdin);
    gets(valor);	
	EditarCelula(l,c,&(*desc),valor);	
}

void Limpar() {
	gotoxy(1,1);
	textbackground(0);
    textcolor(0);
    for(int i=0; i<2; i++) 
    	for(int j=0; j<50; j++)
    		printf(" ");
}

void iniciaExcel(TpDesc **desc)
{
    char key, caracterC;
    int c,l,col,lin;
	MatEsp *CaixaMatEsp;
	int colF, linF;
    textbackground(0);
    clrscr();
    desenhaTela(1,65,*desc);
    //coordenadas de tela
    c=l=1;
    colF=65;
    linF=1;
    //coordenadas da matriz
    col=65; //letra A
    lin=1;
    gotoxy(c*9-9+4,l+3);
    printf("         ");

    do
    {
    	gotoxy(1,1);
    	textbackground(0);
    	textcolor(15);
        printf("[F2]Editar Celula  [F3]Salvar Planilha  [F4]Abrir Planilha  [F5]Gerar Grafico");
    	
        key=getch();
        if (key==-32)
        {
        	
            key=getch();
            switch(key)
            {                       //linhas   linhas-1
                case 80:if (l==20 && lin<100 - 19) //seta p/ baixo
                            lin++;
                        if (l<20)
                            l++;
                        if (linF<100)
                        	linF++;
                        break;
                case 72:if (l==1 && lin>1) //seta p/ cima
                            lin--;
                        if (l>1)
                            l--;
                        if (linF>1)
                        	linF--;		
                        break;
                                     //'A' alfabeto  colunas-1
                case 77:if (c==8 && col<65 + 26 - 8) //seta p/ direita
                            col++;
                        if (c<8)
                            c++;
                        if (colF<90)
                        	colF++;
  
                        break;
                case 75:if (c==1 && col>65) //seta p/ esquerda
                            col--;
                        if (c>1)
                            c--;
                        if (colF>65)
                        	colF--;
            
                        break;
            }
            
            desenhaTela(lin,col,*desc);
            textbackground(0);
            textcolor(7);
            //<--------------------exibir todas as celulas na janela atual
            //                     de lin até lin+19, col até col+8
            gotoxy(c*9-9+4,l+3);
            textbackground(3);
            textcolor(0);
            if(*desc!=NULL) {
            	VerificaOcupado((*desc)->plinha,linF,(char)colF,&CaixaMatEsp);
	            if(CaixaMatEsp!=NULL) {
	            	textcolor(15);
	            	printf("%.9s",CaixaMatEsp->valor);
	            	if(strlen(CaixaMatEsp->valor)<9) {
	            		textcolor(0);
	            		for(int i=strlen(CaixaMatEsp->valor); i<9; i++)
	            			printf(" ");
	            	}
	            }
	            else
            		printf("         ");
	        }
            else
            	printf("         ");
             //desenhar o conteudo da celula atual a partir da matriz esparsa
        }
        
        if (key == 0 || key == 224) {
        	key = getch(); 
            switch (key) {
                case 60: // f2 editar celula
                	Limpar();
                	ModoEditarCelula(linF,colF,&(*desc));
                    break;
                case 61: // f3 salvar planilha
                	Limpar();
                	ModoSalvarPlanilha(*desc);
                    break;
                case 62: // f4 abrir planilha
                	Limpar();
                	ModoAbrirPlanilha(&(*desc));
                    break;
                case 63:
                	Limpar();
                	ModoGerarGrafico(*desc);
                	getch();
                	break;
                default:
                    break;
            }
			desenhaTela(lin,col,*desc);
            textbackground(0);
            textcolor(7);
            //<--------------------exibir todas as celulas na janela atual
            //                     de lin até lin+19, col até col+8
            gotoxy(c*9-9+4,l+3);
            textbackground(3);
            textcolor(0);            
        }
    }while (key!=27);
}

main(void)
{	
	TpDesc *desc = NULL;
    iniciaExcel(&desc);
    clrscr();
    DeletarMatriz(&desc);
}
