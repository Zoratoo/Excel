struct pilhachar {
	char info;
	struct pilhachar *prox;
};
typedef struct pilhachar PilhaChar;

void initChar(PilhaChar **P) {
	*P = NULL;
}

char isEmptyChar(PilhaChar *P) {
	return P==NULL;
}

char topChar(PilhaChar *P) {
	if(!isEmptyChar(P))
		return P->info;
	else
		return -1;
}

void pushChar(PilhaChar **P, char x) {
	PilhaChar *nova;
	nova = (PilhaChar*)malloc(sizeof(PilhaChar));
	nova->info = x;
	nova->prox = *P;
	*P = nova;	
}

void popChar (PilhaChar **P, char *x) {
	PilhaChar *aux;
	if(!isEmptyChar(*P)) {
		aux = *P;
		*P = (*P)->prox;
		*x = aux->info;
		free(aux);
	}
	else 
		*x = 0;
}
