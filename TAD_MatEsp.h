struct matesp{
	int linha;
	char coluna, valor[50];
	struct matesp *pc, *pl;
};
typedef struct matesp MatEsp;

struct coluna {
	char coluna;
	struct matesp *primC;
	struct coluna *prox;
};
typedef struct coluna Coluna;

struct linha {
	int linha;
	struct matesp *primL;
	struct linha *prox;
};
typedef struct linha Linha;

struct tpdesc {
	struct coluna *pcoluna;
	struct linha *plinha;
};
typedef struct tpdesc TpDesc;

struct arquivo {
	int linha;
	char coluna;
	char valor[100];
};
typedef struct arquivo Arquivo;

void IniciarDescritor(TpDesc **desc) {
	(*desc) = (TpDesc*)malloc(sizeof(TpDesc));	
	(*desc)->pcoluna = NULL;
	(*desc)->plinha = NULL;
}

Coluna *CriaCaixaColuna(char c) {
	Coluna *Caixa = (Coluna*)malloc(sizeof(Coluna));
	Caixa->coluna = c;
	Caixa->prox = NULL;
	Caixa->primC = NULL;
	return Caixa;
}

Linha *CriaCaixaLinha(int l) {
	Linha *Caixa = (Linha*)malloc(sizeof(Linha));
	Caixa->linha = l;
	Caixa->prox = NULL;
	Caixa->primL = NULL;
	return Caixa;
}

void CriarCaixaMatEsp(MatEsp **celula, int l, char c, char valor[]) {
	*celula = (MatEsp*)malloc(sizeof(MatEsp));
	(*celula)->linha = l;
	(*celula)->coluna = c;
	strcpy((*celula)->valor,valor);
	(*celula)->pl = (*celula)->pc = NULL;	
}

float ConstroiListaGen(char equacao[], TpDesc *desc);

Coluna *BuscarColuna(TpDesc *desc, char c) {
	Coluna *coluna = desc->pcoluna;
	while(coluna!=NULL && c>coluna->coluna)
			coluna = coluna->prox;
	if(coluna!=NULL && c==coluna->coluna)
		return coluna;
	return NULL;
}

Linha *BuscarLinha(TpDesc *desc, int l) {
	Linha *linha = desc->plinha;
	while(linha!=NULL && l>linha->linha)
			linha = linha->prox;
	if(linha!=NULL && l==linha->linha)
		return linha;
	return NULL;
}

void CriaColuna(TpDesc **desc, char c, Coluna **auxC) {
	Coluna *CaixaCol = BuscarColuna(*desc,c);
	if (CaixaCol==NULL) {
		CaixaCol = CriaCaixaColuna(c);
		if((*desc)->pcoluna==NULL) 
			(*desc)->pcoluna = CaixaCol;
		else {
			if(c<(*desc)->pcoluna->coluna) {
				CaixaCol->prox = (*desc)->pcoluna;
				(*desc)->pcoluna = CaixaCol;
			}
			else {				
				Coluna *antCol = (*desc)->pcoluna;
				Coluna *atualCol = (*desc)->pcoluna->prox;
				while(atualCol!=NULL && c > atualCol->coluna) {
					antCol = atualCol;
					atualCol = atualCol->prox;
				}	
				antCol->prox = CaixaCol;
				CaixaCol->prox = atualCol;
			}
		}
	}
	*auxC = CaixaCol;
}



void CriaLinha(TpDesc **desc, int l, Linha **auxL) {
	Linha *CaixaLin = BuscarLinha(*desc,l);
	if (CaixaLin==NULL) {
		CaixaLin = CriaCaixaLinha(l);
		if((*desc)->plinha==NULL) {
			(*desc)->plinha = CaixaLin;
		}
		else {
			if(l<(*desc)->plinha->linha) {
				CaixaLin->prox = (*desc)->plinha;
				(*desc)->plinha = CaixaLin;
			}
			else {				
				Linha *antLin = (*desc)->plinha;
				Linha *atualLin = (*desc)->plinha->prox;
				while(atualLin!=NULL && l > atualLin->linha) {
					antLin = atualLin;
					atualLin = atualLin->prox;
				}	
				antLin->prox = CaixaLin;
				CaixaLin->prox = atualLin;
			}
		}
	}
	*auxL = CaixaLin;
}

void VerificaOcupado(Linha *linha, int l, char c, MatEsp **aux) {
	MatEsp *atual;
	while(linha!=NULL && linha->linha != l)
		linha = linha->prox;
	if(linha!=NULL && linha->linha == l) {
		atual = linha->primL;
		if(atual == NULL) 
			*aux = NULL;
		else {
			while(atual!=NULL && atual->coluna!=c)
				atual = atual->pc;
			if(atual!=NULL && atual->coluna == c)
				*aux = atual;
			else
				*aux = NULL;
		}
	}
	else
		*aux=NULL;
}

void insereCelula(TpDesc **desc, int l, char c, char valor[]) {	
	MatEsp *ant, *atual, *caixa;
	Linha *auxL;
	Coluna *auxC;
	CriaLinha(&(*desc),l,&auxL);
	CriaColuna(&(*desc),c,&auxC);
	VerificaOcupado((*desc)->plinha,l,c,&atual);
	if(atual!=NULL) 
		strcpy(atual->valor, valor);
	else {
		CriarCaixaMatEsp(&caixa,l,c,valor);
		if(auxL->primL==NULL) 
			auxL->primL = caixa;
		else {
			if(c < auxL->primL->coluna) {
				caixa->pc = auxL->primL;
				auxL->primL = caixa;
			}
			else {
				ant = auxL->primL;
				atual = auxL->primL->pc;
				while(atual!=NULL && c > atual->coluna) {
					ant = atual;
					atual = atual->pc;
				}
				caixa->pc = atual;
				ant->pc = caixa;
			}
		}
		if(auxC->primC==NULL) 
			auxC->primC = caixa;
		else {
			if(l < auxC->primC->linha) {
				caixa->pl = auxC->primC;
				auxC->primC = caixa;
			}
			else {
				ant = auxC->primC;
				atual = auxC->primC->pl;
				while(atual!=NULL && l > atual->linha) {
					ant = atual;
					atual = atual->pl;
				}
				caixa->pl = atual;
				ant->pl = caixa;
			}
		}
	}
}

void DeletarMatriz(TpDesc **desc) {
	MatEsp *ant, *atual;
	Coluna *auxC;
	Linha *auxL;
	if((*desc)!=NULL) {
		Coluna *colunas = (*desc)->pcoluna;
		Linha *linhas = (*desc)->plinha;
		auxC = colunas;
		auxL = linhas;
		while(auxC!=NULL) {
			ant = auxC->primC;
			atual = ant->pl;
			while(atual!=NULL) {
				free(ant);
				ant = atual;
				atual=atual->pl;
			}
			if(ant!=NULL)
				free(ant);
			auxC = auxC->prox;
		}
		auxC = colunas;
		colunas = colunas->prox;
		while(colunas!=NULL) {
			free(auxC);
			auxC = colunas;
			colunas = colunas->prox;
		}
		if(auxC!=NULL)
			free(auxC);
		auxL = linhas;
		linhas = linhas->prox;
		while(linhas!=NULL) {
			free(auxL);
			auxL = linhas;
			linhas = linhas->prox;
		}
		if(auxL!=NULL)
			free(auxL);
	}
}


void ExibirMatriz(TpDesc *desc) {
	Linha *aux = desc->plinha;
	MatEsp *atual;
	while(aux!=NULL) {
		printf("%d\t",aux->linha);
		atual = aux->primL;
		while(atual!=NULL) {
			printf("%d, %c: %s\t",atual->linha,atual->coluna,atual->valor);
			atual = atual->pc;
		}
		printf("\n");
		aux = aux->prox;	
	}
}

void SalvarPlanilha(TpDesc *desc, char nomeArquivo[]) {	
	Linha *aux = desc->plinha;
	MatEsp *atual;
	Arquivo arquivo;
	FILE *ptr = fopen(nomeArquivo, "w+b");
	if(ptr) {
		while(aux!=NULL) {
			atual = aux->primL;
			while(atual!=NULL) {
				arquivo.linha = atual->linha;
				arquivo.coluna = atual->coluna;
				strcpy(arquivo.valor,atual->valor);
				fwrite(&arquivo , sizeof(arquivo), 1, ptr);
				atual = atual->pc;
			}
			aux = aux->prox;	
		}
		fclose(ptr);
	}
	else
		printf("Erro de abertura!\n");	
}

void EditarCelula(int l, char c, TpDesc **desc, char valor[]) {
	if(*desc==NULL) 
		IniciarDescritor(&(*desc));
	insereCelula(&(*desc),l,c,valor);
}

void AbrirPlanilha (TpDesc **desc, char nomePlanilha[]) {
	Arquivo arquivo;
	FILE *ptr = fopen(nomePlanilha, "rb");
	if(ptr) {
		DeletarMatriz(&(*desc));
		*desc=NULL;
		fread(&arquivo, sizeof(Arquivo), 1, ptr);
		while(!feof(ptr)) {	
			EditarCelula(arquivo.linha,arquivo.coluna,&(*desc),arquivo.valor);
			fread(&arquivo, sizeof(Arquivo), 1, ptr);
		}
		fclose(ptr);
	}
	else
		printf("Erro de abertura!\n");
}

void BuscarCelulas(char valor[], int *l1, int *l2, char *c1, char *c2) {
	char *num = (char *)malloc(3 * sizeof(char));
	for(int i=0; valor[i]!='\0'; i++) {
		if((valor[i]=='s' || valor[i]=='S') && (valor[i+1]=='u' || valor[i+1]=='U') && (valor[i+2]=='m' || valor[i+2]=='M'))
			i+=3;
		else if((valor[i]=='a' || valor[i]=='A') && (valor[i+1]=='v' || valor[i+1]=='V') && (valor[i+2]=='g' || valor[i+2]=='G'))
			i+=3;
		else if((valor[i]=='m' || valor[i]=='M') && (valor[i+1]=='a' || valor[i+1]=='A') && (valor[i+2]=='x' || valor[i+2]=='X'))
			i+=3;
		else if((valor[i]=='m' || valor[i]=='M') && (valor[i+1]=='i' || valor[i+1]=='I') && (valor[i+2]=='n' || valor[i+2]=='N'))
			i+=3;
		if((valor[i]>='A' && valor[i]<='Z') || (valor[i]>='a' && valor[i]<='z')) {
			if(valor[i]>='a' && valor[i]<='z')
				valor[i] = valor[i]-32;
			if(*c1==0) 
				*c1 = valor[i];
			else if(*c2==0)
				*c2 = valor[i];
		}
		else if(valor[i]>='0' && valor[i]<='9') {
			if(valor[i+1]>='0' && valor[i+1]<='9') {
				num[0] = valor[i];
				num[1] = valor[i+1];
				num[2] = '\0';
				if(*l1==0)
					*l1 = atoi(num);
				else
					*l2 = atoi(num);
				i++;
			}
			else {
				if(*l1==0)
					*l1 = (int)valor[i]-48;
				else if(*l2==0)
					*l2 = (int)valor[i]-48;
			}
		}
	}
}

void CalculaSumCol(TpDesc *desc, float *soma, char cMenor, char cMaior, int linha) {
	char *auxF;
	MatEsp *aux;
	float result;
	for(int i=(int)cMenor; i<=cMaior; i++) {
		VerificaOcupado(desc->plinha,linha,i,&aux);
		if(aux!=NULL) {
			if(aux->valor[0]>='0' && aux->valor[0]<='9')
				result = strtof(aux->valor,&auxF);
			else 
				result = ConstroiListaGen(aux->valor,desc);
			*soma = *soma+result;
		}
	}		
}

void CalculaSumLin(TpDesc *desc, float *soma, int lMenor, int lMaior, char coluna) {
	char *auxF;
	MatEsp *aux;
	float result;
	for(int i=lMenor; i<=lMaior; i++) {
		VerificaOcupado(desc->plinha,i,coluna,&aux);
		if(aux!=NULL) {
			if(aux->valor[0]>='0' && aux->valor[0]<='9')
				result = strtof(aux->valor,&auxF);
			else 
				result = ConstroiListaGen(aux->valor,desc);
			*soma = *soma+result;
		}
	}		
}

float SUM(TpDesc *desc, char valor[]) {
	int l1=0, l2=0;
	char c1=0, c2=0;
	float soma=0.0;

	BuscarCelulas(valor,&l1,&l2,&c1,&c2);
	if(l2==l1) {
		if(c1<c2) 
			CalculaSumCol(desc,&soma,c1,c2,l1);			
		else if(c2<c1) 
			CalculaSumCol(desc,&soma,c2,c1,l1);					
	}
	else if(c1==c2) {
		if(l1<l2) 
			CalculaSumLin(desc,&soma,l1,l2,c1);
		if(l2<l1) 
			CalculaSumLin(desc,&soma,l2,l1,c1);	
	}
	return soma;
}

void CalculaAvgCol(TpDesc *desc, float *media, char cMenor, char cMaior, int linha, float *cont) {
	char *auxF;
	MatEsp *aux;
	float result;
	for(int i=(int)cMenor; i<=cMaior; i++) {
		VerificaOcupado(desc->plinha,linha,i,&aux);
		if(aux!=NULL) {
			if(aux->valor[0]>='0' && aux->valor[0]<='9') 
				result = strtof(aux->valor,&auxF);
			else 
				result = ConstroiListaGen(aux->valor,desc);	
			*media = *media+result;
			(*cont)++;
		}
	}		
}

void CalculaAvgLin(TpDesc *desc, float *media, int lMenor, int lMaior, char coluna, float *cont) {
	char *auxF;
	MatEsp *aux;
	float result;
	for(int i=lMenor; i<=lMaior; i++) {
		VerificaOcupado(desc->plinha,i,coluna,&aux);
		if(aux!=NULL) {
			if(aux->valor[0]>='0' && aux->valor[0]<='9') 
				result = strtof(aux->valor,&auxF);
			else 
				result = ConstroiListaGen(aux->valor,desc);	
			*media = *media+result;
			(*cont)++;
		}
	}		
}

float AVG(TpDesc *desc, char valor[]) {
	int l1=0, l2=0;
	char c1=0, c2=0;	
	float media=0.0, cont=0.0;
	
	BuscarCelulas(valor,&l1,&l2,&c1,&c2);
	if(l2==l1) {
		if(c1<c2) 
			CalculaAvgCol(desc,&media,c1,c2,l1,&cont);		
		else if(c2<c1) 
			CalculaAvgCol(desc,&media,c2,c1,l1,&cont);
	}
	else if(c1==c2) {
		if(l1<l2) 
			CalculaAvgLin(desc,&media,l1,l2,c1,&cont);	
		else if(l2<l1) 
			CalculaAvgLin(desc,&media,l2,l1,c1,&cont);	
	}
	return media/cont;
}

void CalculaMaxCol(TpDesc *desc, float *maior, char cMenor, char cMaior, int linha) {
	MatEsp *aux;
	char *auxF;
	float result;
	for(int i=(int)cMenor; i<=cMaior; i++) {
		VerificaOcupado(desc->plinha,linha,i,&aux);
		if(aux!=NULL) {
			if(aux->valor[0]>='0' && aux->valor[0]<='9') 
				result = strtof(aux->valor,&auxF);
			else 
				result = ConstroiListaGen(aux->valor,desc);				
			if(result>*maior)
				*maior = result;
		}
	}	
}

void CalculaMaxLin(TpDesc *desc, float *maior, int lMenor, int lMaior, char coluna) {
	MatEsp *aux;
	char *auxF;
	float result;
	for(int i=lMenor; i<=lMaior; i++) {
		VerificaOcupado(desc->plinha,i,coluna,&aux);
		if(aux!=NULL) {
			if(aux->valor[0]>='0' && aux->valor[0]<='9') 
				result = strtof(aux->valor,&auxF);
			else 
				result = ConstroiListaGen(aux->valor,desc);				
			if(result>*maior)
				*maior = result;
		}
	}	
}

float MAX(TpDesc *desc, char valor[]) {
	int l1=0, l2=0;
	char c1=0, c2=0;
	float maior=-9999.9;
	
	BuscarCelulas(valor,&l1,&l2,&c1,&c2);
	if(l2==l1) {
		if(c1<c2) 
			CalculaMaxCol(desc,&maior,c1,c2,l1);				
		else if(c2<c1) 
			CalculaMaxCol(desc,&maior,c2,c1,l1);							
	}
	else if(c1==c2) {
		if(l1<l2) 
			CalculaMaxLin(desc,&maior,l1,l2,c1);
		else if(l2<l1) 
			CalculaMaxLin(desc,&maior,l2,l1,c1);	
	}
	if(maior==-9999.9)
		maior=0;
	return maior;
}

void CalculaMinCol(TpDesc *desc, float *menor, char cMenor, char cMaior, int linha) {
	MatEsp *aux;
	char *auxF;
	float result;
	for(int i=(int)cMenor; i<=cMaior; i++) {
		VerificaOcupado(desc->plinha,linha,i,&aux);
		if(aux!=NULL) {
			if(aux->valor[0]>='0' && aux->valor[0]<='9') 
				result = strtof(aux->valor,&auxF);
			else 
				result = ConstroiListaGen(aux->valor,desc);					
			if(result<*menor)
				*menor = result;
		}
	}	
}

void CalculaMinLin(TpDesc *desc, float *menor, int lMenor, int lMaior, char coluna) {
	MatEsp *aux;
	char *auxF;
	float result;
	for(int i=lMenor; i<=lMaior; i++) {
		VerificaOcupado(desc->plinha,i,coluna,&aux);
		if(aux!=NULL) {
			if(aux->valor[0]>='0' && aux->valor[0]<='9') 
				result = strtof(aux->valor,&auxF);
			else 
				result = ConstroiListaGen(aux->valor,desc);					
			if(result<*menor)
				*menor = result;
		}
	}	
}

float MIN(TpDesc *desc, char valor[]) {
	int l1=0, l2=0;
	char c1=0, c2=0;
	float menor=9999.9, result;
	
	BuscarCelulas(valor,&l1,&l2,&c1,&c2);	
	if(l2==l1) {
		if(c1<c2) 
			CalculaMinCol(desc,&menor,c1,c2,l1);				
		else if(c2<c1) 
			CalculaMinCol(desc,&menor,c2,c1,l1);
	}
	else if(c1==c2) {
		if(l1<l2) 
			CalculaMinLin(desc,&menor,l1,l2,c1);
		else if(l2<l1) 
			CalculaMinLin(desc,&menor,l2,l1,c1);
	}
	if(menor==9999.9)
		menor=0;
	return menor;
}

int MaiorPalavra(MatEsp *celula, char coluna) {
	int tamanho=0;
	while(celula!=NULL && celula->coluna<=coluna) {
		if(strlen(celula->valor)>tamanho)
			tamanho = strlen(celula->valor);
		celula = celula->pc;
	}
	return tamanho;
}
