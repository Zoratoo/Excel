#include "TAD_Pilha_ListaGen.h"
#include "TAD_Pilha_Float.h"
#include "TAD_Pilha_Char.h"
#include "TAD_MatEsp.h"

struct lista {
	struct lista *prox;
	char info[20];
};
typedef struct lista Lista;

char Nula(ListaGen *L) {
	return L==NULL;
}

ListaGen *Head(ListaGen *L) {
	return L->cabeca;
}

ListaGen *Tail(ListaGen *L) {
	return L->cauda;
}

char Valor(ListaGen *L) {
	return !Nula(L) && (L->terminal=='v' || L->terminal=='V');
}

char Funcao(ListaGen *L) {
	return !Nula(L) && (L->terminal=='f' || L->terminal=='F');
}

char Operador(ListaGen *L) {
	return !Nula(L) && (L->terminal=='o' || L->terminal=='O');
}

ListaGen *Cons(ListaGen *H, ListaGen *T) {
	ListaGen *L = (ListaGen*)malloc(sizeof(ListaGen));
	if(!Valor(T) && !Operador(T) && !Funcao(T)) {
		L->terminal = '0';
		L->cabeca = H;
		L->cauda = T;
		return L;
	}
	return NULL;
}

ListaGen *ConsV(char valor[]) {
	ListaGen *L = (ListaGen*)malloc(sizeof(ListaGen));
	L->terminal = 'V';
	L->no.valor = atof(valor);
	L->cabeca = L->cauda = NULL;
	return L;
}

ListaGen *ConsO(char valor[]) {
	ListaGen *L = (ListaGen*)malloc(sizeof(ListaGen));
	L->terminal = 'O';
	L->no.operador = valor[0];
	L->cabeca = L->cauda = NULL;
	return L;
}

ListaGen *ConsF(char valor[]) {
	ListaGen *L = (ListaGen*)malloc(sizeof(ListaGen));
	L->terminal = 'F';
	strcpy(L->no.funcao,valor);
	L->cabeca = L->cauda = NULL;
	return L;	
}

Lista *CriaCaixaLista (char palavra[]) {
	Lista *caixa = (Lista*)malloc(sizeof(Lista));
	caixa->prox = NULL;
	strcpy(caixa->info,palavra);
	return caixa;
}

Lista *SeparaTermos(char equacao[]) {
	Lista *lista = NULL;
	Lista *aux = NULL;
	Lista *caixa = NULL;
	int i=0, j=0;
	char palavra[50];
	while(i<strlen(equacao)) {
		if(equacao[i]=='(' || equacao[i]==')' || equacao[i] == '+' || equacao[i]=='-' || equacao[i]=='*' || equacao[i]=='/') {//operador
			palavra[j] = equacao[i];
			j++;
			i++;
		}
		else {
			if((equacao[i]>='a' && equacao[i]<='z') || (equacao[i]>='A' && equacao[i]<='Z')) {
				palavra[j] = equacao[i];
				i++;
				j++;
				if(equacao[i]>='0' && equacao[i]<='9') { //celula
					while(equacao[i]>='0' && equacao[i]<='9') {
						palavra[j] = equacao[i];
						i++; 
						j++;
					}
				}
				else {
					while(equacao[i-1]!=')') { //funcao
						palavra[j] = equacao[i];
						i++; 
						j++;
					}
				}
			}
			else {
				if(equacao[i]>='0' && equacao[i]<='9'){ //numero
					while((equacao[i]>='0' && equacao[i]<='9') || equacao[i]=='.') { 
						palavra[j] = equacao[i];
						i++; 
						j++;
					}
				}
				else 
					i++;
			}
		}
		palavra[j] = '\0';
		if(palavra[0]!='\0')
			caixa = CriaCaixaLista(palavra);
		if(lista==NULL) {
			lista = caixa;
			aux = lista;
		}
		else {
			aux->prox = caixa;
			aux = aux->prox;
		}
		j=0;
	}
	return lista;
}

float CalculaSinal(float num1, float num2, char sinal) {
	float resultado;
	if(sinal == '*') 
		resultado = num1*num2;
	else if(sinal == '/')
		resultado = num1/num2;
	else if(sinal == '-')
		resultado = num1-num2;
	else
		resultado = num1+num2;
	return resultado;
}

float CalculaFuncao(char funcao[], TpDesc *desc) {
	float valor;
	int flag=0;
	for(int i=0;i<strlen(funcao) && flag==0;i++) {
		if((funcao[i]=='s' || funcao[i]=='S') && (funcao[i+1]=='u' || funcao[i+1]=='U') && (funcao[i+2]=='m' || funcao[i+2]=='M')) {
			valor = SUM(desc,funcao);
			flag = 1;
		}		
		else if((funcao[i]=='a' || funcao[i]=='A') && (funcao[i+1]=='v' || funcao[i+1]=='V') && (funcao[i+2]=='g' || funcao[i+2]=='G')) {
			valor = AVG(desc,funcao);
			flag = 1;
		}
		else if((funcao[i]=='m' || funcao[i]=='M') && (funcao[i+1]=='a' || funcao[i+1]=='A') && (funcao[i+2]=='x' || funcao[i+2]=='X')) {
			valor = MAX(desc,funcao);
			flag = 1;
		}
		else if((funcao[i]=='m' || funcao[i]=='M') && (funcao[i+1]=='i' || funcao[i+1]=='I') && (funcao[i+2]=='n' || funcao[i+2]=='N')) {
			valor = MIN(desc,funcao);
			flag = 1;
		}
		else {
			flag=0;
			valor = 0;
		}
	}
	return valor;
}

float ConstroiListaGen(char equacao[], TpDesc *desc);

float VerificaFuncao (char funcao[], TpDesc *desc){
	char linha[10];
	MatEsp *auxF;
	int i;
	float valor;
	if(((funcao[0]>='a' && funcao[0]<='z') || (funcao[0]>='A' && funcao[0]<='Z')) && funcao[1]>='0' && funcao[1]<='9') {
		for(i=1; i<strlen(funcao); i++) 
			linha[i-1] = funcao[i];
		linha[i-1] = '\0';
		VerificaOcupado(desc->plinha,atof(linha),funcao[0],&auxF);
		if(auxF) 
			return VerificaFuncao(auxF->valor,desc);
		else 
			return 0;
	}
	if(funcao[0]>='0' && funcao[0]<='9')
		valor = atof(funcao);
	else
		if(funcao[0]!='=' && strlen(funcao)<13)
			valor = CalculaFuncao(funcao,desc);
		else
			valor = ConstroiListaGen(funcao,desc);
	return valor;
}

float Calcula(ListaGen *lista, TpDesc *desc) { //da free nas caixinhas
	PilhaFloat *Pv; //Pilha de valores
	PilhaChar *Po; //Piha de operadores
	initFloat(&Pv);
	initChar(&Po);
	char linha[10];
	char opAux;
	float num1, num2, valor;
	float resultado;
	int i;
	ListaGen *aux;
	MatEsp *auxF;
	while(lista!=NULL) {	
		if(lista->terminal=='V' || lista->terminal=='F') {
			if(lista->terminal=='F') 
				valor = VerificaFuncao(lista->no.funcao, desc);
			else
				valor = lista->no.valor;
			pushFloat(&Pv,valor);
		}
		else {
			if(lista->no.operador == '*' || lista->no.operador == '/')
				pushChar(&Po,lista->no.operador);
			else {
				if(!isEmptyChar(Po)) {
					while(!isEmptyChar(Po)) {
						popChar(&Po,&opAux);
						popFloat(&Pv,&num1);
						popFloat(&Pv,&num2);
						resultado = CalculaSinal(num2, num1, opAux);
						pushFloat(&Pv,resultado);
					}
				}
				pushChar(&Po,lista->no.operador);
			}		
		}
		aux = lista;
		lista = lista->cauda;
		free(aux);
	}
	while(!isEmptyFloat(Pv) && !isEmptyChar(Po)) {
		popChar(&Po,&opAux);
		popFloat(&Pv,&num1);
		popFloat(&Pv,&num2);
		resultado = CalculaSinal(num2, num1, opAux);
		pushFloat(&Pv,resultado);
	}
	popFloat(&Pv,&resultado);
	return resultado;
}

float CalcularListaGen(Pilha *P, ListaGen **L, TpDesc *desc) {
	ListaGen *atual;
	float resultado;
	while(!isEmpty(P)) {
		pop(&P,&atual);
		if(atual!=*L)
			atual->no.valor = Calcula(atual->cabeca,desc);
		else
			resultado = Calcula(atual,desc);
	}
	return resultado;
}

float ConstroiListaGen(char equacao[], TpDesc *desc) {
	ListaGen *L;
	Pilha *P1, *P2;
	ListaGen *nova = NULL;
	ListaGen *atual = NULL;
	Lista *lista = SeparaTermos(equacao);
	Lista *aux = lista;
	char dir = 'H'; //'H' = Horizontal
	init(&P1);
	init(&P2);
	L = NULL;
	while(lista!=NULL) {
		if(lista->info[0]!='(' && lista->info[0]!=')') {	//numero, operador, celula, funcao
			if(lista->info[0]>='0' && lista->info[0]<='9') 
				nova = ConsV(lista->info); //cons valor	
			else if(lista->info[0]=='+' || lista->info[0]=='-' || lista->info[0]=='*' || lista->info[0]=='/')
				nova = ConsO(lista->info); //cons operador
			else
				nova = ConsF(lista->info); //pode ser funcao ou celula... Resolver funcao e celula
			if(L==NULL) {
				L = atual = nova;
				push(&P2,atual);
			}
			else {
				if(dir == 'H') {
					atual->cauda = nova;
					atual = atual->cauda;
				}
				else {
					atual->cabeca = nova;
					atual = atual->cabeca;
				}
				dir = 'H';		
			}				
		}
		else {
			if(lista->info[0]=='(') {
				nova = ConsV("0");
				if(L==NULL) 
					L = atual = nova;		
				else {
					if(dir == 'H') {
						atual->cauda = nova;
						atual = atual->cauda;
					}
					else {
						atual->cabeca = nova;
						atual = atual->cabeca;
					}
					push(&P1,atual);		
				}
				push(&P2,atual);		
				dir = 'V'; //'V' = Vertical
			}
			else 	
				pop(&P1,&atual);
		}
		aux = lista;
		lista = lista->prox;
		free(aux);
	}
	return CalcularListaGen(P2,&L,desc);
}
