struct pilhafloat {
	float info;
	struct pilhafloat *prox;
};
typedef struct pilhafloat PilhaFloat;

void initFloat(PilhaFloat **P) {
	*P = NULL;
}

char isEmptyFloat(PilhaFloat *P) {
	return P==NULL;
}

float topFloat(PilhaFloat *P) {
	if(!isEmptyFloat(P))
		return P->info;
	else
		return -1;
}

void pushFloat(PilhaFloat **P, float x) {
	PilhaFloat *nova;
	nova = (PilhaFloat*)malloc(sizeof(PilhaFloat));
	nova->info = x;
	nova->prox = *P;
	*P = nova;	
}

void popFloat(PilhaFloat **P, float *x) {
	PilhaFloat *aux;
	if(!isEmptyFloat(*P)) {
		*x = (*P)->info;
		aux = *P;
		*P = (*P)->prox;
		free(aux);
	}
	else 
		*x = -1;
}


