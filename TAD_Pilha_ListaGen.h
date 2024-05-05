union info {
	char funcao[20];
	char operador; //3 cons, 1 para cada caso 
	float valor;
};

struct listagen {
	char terminal;
	union info no;
	struct listagen *cauda;
	struct listagen *cabeca;
};
typedef struct listagen ListaGen;

struct pilha {
	ListaGen *info;
	struct pilha *prox;	
};
typedef struct pilha Pilha;

void init(Pilha **P) {
	*P = NULL;
}

char isEmpty(Pilha *P) {
	return P==NULL;
}

ListaGen *top(Pilha *P) {
	if(!isEmpty(P))
		return P->info;
	else
		return NULL;
}

void push(Pilha **P, ListaGen *info) {
	Pilha *nova;
	nova = (Pilha*)malloc(sizeof(Pilha));
	nova->info = info;
	nova->prox = *P;
	*P = nova;	
}

void pop(Pilha **P, ListaGen **info) {
    Pilha *aux;
    if (!isEmpty(*P)) {
        aux = *P;
        *P = (*P)->prox;
        *info = aux->info;
        free(aux);
    } 
	else {
        *info = NULL;
    }
}
