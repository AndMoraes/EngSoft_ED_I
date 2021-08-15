//<Bibliotecas>________________________
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
//_______________________</Bibliotecas>

//<Constantes>_________________________
#define CUSTOPORDIGITO 100
#define INFINITO 9999
#define MAXCONEX 2
//________________________</Constantes>

//<Variáveis Globais>__________________
int i, j;
//_________________</Variáveis Globais>

//<Tipos definidos>____________________
bool verdade = false;
typedef struct Vert{
	//uso geral
	int iDestino;
	struct Vert *prox;
	//uso na criação das vértices
	int escolhidoOrigem;
	int escolhidoDestino;
	//uso no cálculo Dijkstra
	bool visitado;
	int origemMenor;
	int manterMenor;
}vrt;
typedef struct Arestas{
	int id;
	int origem;
	int destino;
	int custo;
	struct Arestas *prox;
}are;
//___________________</Tipos definidos>

//<Assinaturas>________________________
void CriarVertices(vrt *v, int n);
void IniciarVertice(vrt *v);
void IniciarAresta(are *a);
vrt* AlocarVertice(int indice);
void Liberar(vrt *v, are *a);
void ImprimirResumo(vrt *v, are *a);
void ImprimirSimples(are *a);
void ImprimirMelhorRota(vrt *v, int destino);
int BuscarMenor(vrt *v, int destino, int *custo, int *tam);
void ConectarArestas(vrt *v, are *a, int n, int *tamanho, int *numAresta, int *opcao);
int ValidarOrigem(vrt *v, int n, int indice);
int ValidarDestino(vrt *v, are *a, int n, int vOrigem, int indice);
int asc(int n);
void CriarArestas(are *a, int origem, int destino, int numA, int custo);
are* AlocarAresta(int origem, int destino, int numA, int custo);
void Dijkstra(are *a, vrt *v, int n, int maxArestas);
void AtualizarMenor(are *a, vrt *v, int *atual, int nDestino);
bool BuscarAresta(are *a, int atualOrigem, int atualDestino, int *tmpCusto);
int BuscarProximo(vrt *v, int n);
void rep(int num, int x);
int menu(int n);
void Aviso();
//_______________________</Assinaturas>
int main(void){
	setlocale(LC_ALL, "Portuguese");
	int nVert = 0;
	vrt *vertice;
	are *aresta;
	int op;
	int tamanho;
	int opcao = -1;
	int numAresta = 1;
	while(1){
		op = menu(nVert);
		switch(op){
			case 0:
				printf("\n\tObrigado! Saindo...\n\n");
				if(nVert > 0) Liberar(vertice, aresta);
				return 0;
			case 1:
				vertice = (vrt *)malloc(sizeof(vrt));
	   	   	   	aresta = (are *)malloc(sizeof(are));
				if(!vertice || !aresta){
					printf("Sem memória\n");
					exit(1);
				}else{
					IniciarVertice(vertice);
					IniciarAresta(aresta);
	   	   	   	}
				system("cls");
				printf("\nGostaria de rodar automaticamente conforme o enunciado?\n");
				while(opcao != 0 && opcao != 1){
					printf("Digite 1 para SIM ou 0 para NÃO\n> ");
					scanf("%d", &opcao);
				}
				if(opcao == 1){
					nVert = 5;
				}else{
					system("cls");
					printf("INICIANDO A CRIAÇÃO...\n\n");					
					printf("Digite o número de vértices.\n");
					while(nVert < 5 || nVert >= INFINITO){
						printf("Valor maior ou igual a 5!\n> ");
						scanf("%d",&nVert);
					}
					if(nVert > 5) Aviso();
				}
				CriarVertices(vertice, nVert);
				ConectarArestas(vertice, aresta, nVert, &tamanho, &numAresta, &opcao);
				break;
			case 2:
				opcao = -1;
				printf("\n\tImprimir:\n\n\t[0] Somente ARESTAS\n");
				printf("\n\t[1] Informações de VÉRTICES e ARESTAS\n\n");
				while(opcao != 0 && opcao != 1){
					printf("\tDigite 0 ou 1\n\t> ");
					scanf("%d",&opcao);
				}
				if(opcao == 0){
					ImprimirSimples(aresta);
				}else{
					ImprimirResumo(vertice, aresta);	
				}
				break;
			case 3:
				ImprimirMelhorRota(vertice, nVert);
				ImprimirSimples(aresta);
				break;
			default:
				printf("Oi?");
				break;
		}
	}
	Liberar(vertice, aresta);
	return 0;
}
int menu(int n){
	int opt = -1;
	int max, exceto;
	printf("\n\tSEJA BEM VINDO ao UNI-DIJ !\n\n");
	printf("\n\n\tESCOLHA UMA DAS OPÇÕES:\n");
	if(n == 0){
		printf("\n\t*As funções [indisponíveis] necessitam que\n\tprimeiro seja realizada a CRIAÇÃO (opção 1)\n\n");
		max = 1;
		exceto = -1;
	}else{
		printf("\n");
		max = 3;
		exceto = 1;
	}
	printf("\n");	
	printf("\t[  disponível  ]\t[ 0 ] SAIR & FECHAR PROGRAMA\n\n");
	printf("\t%s\t[ 1 ] CRIAR & CONECTAR VÉRTICES\n\n", n == 0 ? "[  disponível  ]" : "[ indisponível ]");
	printf("\t%s\t[ 2 ] IMPRIMIR VÉRTICES & ARESTAS\n\n", n == 0 ? "[ indisponível ]" : "[  disponível  ]");
	printf("\t%s\t[ 3 ] GERAR MELHOR ROTA\n\n", n == 0 ? "[ indisponível ]" : "[  disponível  ]");
	while((opt < 0 || opt > max) || opt == exceto){
		rep(9, 2);
		printf("\n\tDigite umas das opções disponíveis ");
		printf("[ 0 ] %s [ %d ] %s %c ", n == 0 ? "ou" : "a", max, exceto == 1 ? "*exceto [1]" : "",16);
		scanf("%d", &opt);
	}
	return opt;
}
void IniciarVertice(vrt *v){
	v->iDestino = 0;
	v->prox = NULL;
	v->escolhidoOrigem = 0;
	v->escolhidoDestino = 0;
	v->visitado = false;
	v->origemMenor = 0;
	v->manterMenor = INFINITO;
}
void IniciarAresta(are *a){
	a->id = 0;
	a->origem = 0;
	a->destino = 0;	
	a->custo = 0;
	a->prox = NULL;
}
void CriarVertices(vrt *v, int n){
	int indice = 0;
	for(i = 0; i < n; i++){
		indice++;
		vrt *novaVertice = AlocarVertice(indice);
		if(v->prox == NULL){
			v->prox = novaVertice;
		}else{
			vrt *tmp = v->prox;
			while(tmp->prox != NULL){
				tmp = tmp->prox;
			}
			tmp->prox = novaVertice;
		}
	}
}
vrt* AlocarVertice(int indice){
	vrt *novaVertice = (vrt*) malloc(sizeof(vrt));
	if(!novaVertice){
		printf("\nSem memória\n");
		exit(1);
	}else{
		IniciarVertice(novaVertice);
		novaVertice->iDestino = indice;
		return novaVertice;
	}
}
void ConectarArestas(vrt *v, are *a, int n, int *tamanho, int *numAresta, int *opcao){
	int maxArestas = (n * 2) - 3;
	int *ptrOrigem = NULL;
	int indiceOrigem = 0;
	int vrtOrigem = 0;
	bool numOK = false;
	int opcaoCusto;	
	int x = 0;
	//Enunciado_________________________
	int arrOrigem[7] = {1,1,2,2,3,3,4}; 
	int arrDestino[7] = {2,3,4,5,2,5,5};
	int arrCusto[7] = {2,0,0,7,6,4,1};
	//__________________________________	

	while(*numAresta <= maxArestas){
		*tamanho = 0;
		indiceOrigem = 0;
		for(i = 0; i < n-1; i++){
			indiceOrigem++;
			if(ValidarOrigem(v, n, indiceOrigem)){
				*tamanho+=1;
				ptrOrigem = (int *)realloc(ptrOrigem, *tamanho * sizeof(int));
				ptrOrigem[*tamanho-1] = indiceOrigem;
			}
		}
		system("cls");
		if(*opcao == 1){
			vrtOrigem = arrOrigem[*numAresta - 1];
		}else{
			printf("\n____________ Criação das ARESTAS # %d / %d ____________\n", *numAresta, maxArestas);
			printf("\nEscolha uma das VÉRTICES abaixo como [ORIGEM]\n\n");
			for(i = 0; i < *tamanho; i++){
				printf("[%0*d]%c", 4, ptrOrigem[i],asc(i));
			}
			numOK = false;
			printf("\n\nSomente o número, sem os zeros, ex.: 5\n\n[ORIGEM] :  ");	
			while(!numOK){
				scanf("%d", &vrtOrigem);
				for(i = 0; i < *tamanho; i++){
					if(ptrOrigem[i] == vrtOrigem){
						numOK = true;
					}
				}
				if(!numOK) printf("\nDigite um número válido!\n");
			}
		}
		int ultimo;
		vrt *tmp = v->prox;
		while(tmp != NULL){
			if(tmp->iDestino == n) ultimo = tmp->escolhidoDestino;
			if(vrtOrigem == tmp->iDestino){
				tmp->escolhidoOrigem++;  
			}
			tmp = tmp->prox;
		}
		int vrtDestino = 0;
	   	int indiceDestino = 1;
		int *ptrDestino = NULL;
		*tamanho = 0;
		for(i = 0; i < n; i++){
			indiceDestino++;
			if(indiceDestino != vrtOrigem && ValidarDestino(v, a, n, vrtOrigem, indiceDestino)){
				*tamanho+=1;
				ptrDestino = (int *)realloc(ptrDestino, *tamanho * sizeof(int));
				if(!ptrDestino){
			  		printf("Sem memória\n");
			  		exit(1);
				}else{	
					ptrDestino[*tamanho-1] = indiceDestino;
				}
			}
		}
		if(*opcao == 1){
			vrtDestino = arrDestino[*numAresta - 1];
		}else{
			printf("\nEscolha uma das VÉRTICES abaixo como [DESTINO]\n\n");
			for(i = 0; i < *tamanho; i++){
				printf("[%0*d]%c", 4, ptrDestino[i], asc(i));
			}
			numOK = false;
			printf("\nSomente o número, sem os zeros, ex.: 5\n\n");	
			while(!numOK){
				printf("[%0*d]--------->[DESTINO] : ", 4, vrtOrigem);
		   	   	scanf("%d", &vrtDestino);
				for(i = 0; i < *tamanho; i++){
					if(ptrDestino[i] == vrtDestino){
						numOK = true;
					}
				}
				if(!numOK) printf("\nDigite um número válido!\n");
			}
		}
		vrt *tmp2 = v->prox;
		while(tmp2 != NULL){
			if(vrtDestino == tmp2->iDestino){
				tmp2->escolhidoDestino++;  
			}
			tmp2 = tmp2->prox;
		}
		if(*opcao == 1 && x == 0){
			x++;
			printf("\nOk, você optou por usar o Enunciado.");
			printf("\n\nQuanto ao CUSTO, gostaria de digitar o R.A. manualmente? DIGITE: 0 \n");		
			printf("Ou utilizar (automaticamente) o R.A. do ALUNO, DIGITE: 1\n");
			opcaoCusto = -1;
			while(opcaoCusto != 0 && opcaoCusto != 1){
				printf("\nDigite 0 ou 1 > ");
				scanf("%d", &opcaoCusto);
			}
		}
		int custo = -1;
		if(*opcao == 1 && opcaoCusto == 1){
			custo = arrCusto[*numAresta - 1];
		}else{
			printf("\nDigite o dígito do R.A. Somente 1 dígito, valor de 0 a 9!\n");
			printf("O mesmo será convertido em [CUSTO] desta Aresta\n\n");	
			while (!((custo <= 9) && (custo >= 0))) {
			   	printf("[CUSTO] : ");
				scanf("%d", &custo); 
		    }
		}	
		CriarArestas(a, vrtOrigem, vrtDestino, *numAresta, custo);
		*numAresta+=1;
		system("cls");
		printf("ARESTA # %d / %d CRIADA COM SUCESSO!\n\n[ORIGEM]-------------[CUSTO]------------>[DESTINO]\n\n", *numAresta-1, maxArestas);
		printf("[%0*d]---------------[$ %d]--------------->[%0*d]\n", 4, vrtOrigem, custo*100, 4, vrtDestino);
		are *tmp3 = a->prox;
		printf("\n_________________________________________________________________________\n");
		printf("\nARESTAS CRIADAS ATÉ O MOMENTO:\n");
		while(tmp3 != NULL){
			printf("#%d Origem [%0*d]\tCusto: $ %d\tDestino [%0*d]\n",tmp3->id, 4, tmp3->origem, tmp3->custo, 4, tmp3->destino);
			tmp3 = tmp3->prox;
		}
		if(maxArestas - *numAresta == 2 && ultimo <= 1){
			printf("\nPara melhores resultados, recomendo que crie mais ligações com [%0*d]\n", 4, n);
		}
		printf("\npressione < ENTER > para sair");
		getchar(); 
		getchar();
		system("cls");
		free(tmp);
		free(tmp2);
		free(tmp3);
		
	}
	Dijkstra(a, v, n, maxArestas);
}
void Dijkstra(are *a, vrt *v, int n, int maxArestas){
	int itera = 0;
	int atualOrigem = 1;
	int tmpCusto;
	bool existe;
	system("cls");
	while(itera < maxArestas){
		vrt *tmpV = v->prox;
		while(tmpV != NULL){
			existe = BuscarAresta(a, atualOrigem, tmpV->iDestino, &tmpCusto);
			if(atualOrigem == tmpV->iDestino && !(tmpV->visitado)) tmpV->visitado = true;
			if(existe){
				itera++;
				if(tmpCusto <= tmpV->manterMenor){
					tmpV->origemMenor = atualOrigem;
					tmpV->manterMenor = tmpCusto;
				}
				system("cls");
			}
			tmpV = tmpV->prox;					
		}
		free(tmpV);		
		atualOrigem = BuscarProximo(v, n);
	}		
}
int BuscarProximo(vrt *v, int n){
	int tmpMenorCusto = INFINITO;
	int tmpProximo = INFINITO;
	vrt *tmp = v->prox;
	while(tmp != NULL){
		if(tmpMenorCusto > tmp->manterMenor && !(tmp->visitado) && tmp->iDestino != n){
			tmpMenorCusto = tmp->manterMenor;
			tmpProximo = tmp->iDestino;
		}
		tmp = tmp->prox;
	}
	return tmpProximo;
	free(tmp);
}
bool BuscarAresta(are *a, int atualOrigem, int atualDestino, int *tmpCusto){
	bool r = false;
	are *tmpA = a->prox;
	while(tmpA != NULL){
		if(atualOrigem == tmpA->origem && atualDestino == tmpA->destino){
			*tmpCusto = tmpA->custo;
			r = true;
			break;
		}
		tmpA = tmpA->prox;
	}
	return r;
}
void CriarArestas(are *a, int origem, int destino, int numA, int custo){
	are *novaAresta = AlocarAresta(origem, destino, numA, custo);
	if(a->prox == NULL){
		a->prox = novaAresta;
	}else{
		are *tmp = a->prox;
		while(tmp->prox != NULL){
			tmp = tmp->prox;
		}
		tmp->prox = novaAresta;
	}		
}
are* AlocarAresta(int origem, int destino, int numA, int custo){
	are *novaAresta = (are*) malloc(sizeof(are));
	if(!novaAresta){
		printf("\nSem memória\n");
		exit(1);
	}else{
		IniciarAresta(novaAresta);
		novaAresta->id = numA;
		novaAresta->origem = origem;
		novaAresta->destino = destino;
		novaAresta->custo = custo * CUSTOPORDIGITO;
		return novaAresta;
	}
}
int ValidarOrigem(vrt *v, int n, int indice){
	int x;
	vrt *tmp = v->prox;
	while(tmp != NULL){
		if(indice == tmp->iDestino){
			if(tmp->escolhidoOrigem < MAXCONEX){
				x = 1;
			}else{
				x = 0;
			}
		}
		tmp = tmp->prox;
	}
	return x;
	free(tmp);
}
int ValidarDestino(vrt *v, are *a, int n, int vOrigem, int indice){
	int x, y;
	vrt *tmp = v->prox;
	while(tmp != NULL){
		if(indice == tmp->iDestino){
			if(tmp->escolhidoDestino < (MAXCONEX + (indice == n ? 1 : 0))){
				x = 1;
			}else{
				x = 0;
			}
		}
		tmp = tmp->prox;
	}
	y = 0;
	are *tmp2 = a->prox;
	if(tmp2 == NULL){
		y = 0;
	}else{
		while(tmp2 != NULL){
			if((tmp2->origem == indice||tmp2->origem == vOrigem) && (tmp2->destino == vOrigem||tmp2->destino == indice)){
				y++;
			}
			tmp2 = tmp2->prox;
		}
	}
	if(x == 1 && y == 0){
		x = 1;
	}else{
		x = 0;
	}
	free(tmp);
	free(tmp2);
	return x;
}
void ImprimirMelhorRota(vrt *v, int destino){
	int aux = 0;
	int tam = 0;
	int vertice = destino;
	int custo = 0;
	int total = 0;
	int *arrVertice = NULL;
	int *arrCusto = NULL;
		do{
			vertice = BuscarMenor(v, vertice, &custo, &tam);
			arrVertice = (int *)realloc(arrVertice, tam * sizeof(int));
			arrCusto = (int *)realloc(arrCusto, tam * sizeof(int));
		   	if(!arrVertice || !arrCusto){
				printf("Sem memória\n");
				exit(1);
		   	}else{
				arrVertice[tam-1] = vertice;
				arrCusto[tam-1] = custo;
			}
		}while(vertice != 1);
		system("cls");
		printf("\n");
		rep(31, 13);
		rep(10, 2);
		printf("\n UNI-DIJ informa...\n\n");
		printf(" MELHOR ROTA ENCONTRADA !\n\n");
		rep(30, 13);
		printf("\n\nORIGEM\n");
		for(i = tam-1; i >= 0; i--){
		   rep(9, aux*3);printf("[ %0*d ] { $ %d } %c\n\n", 4, arrVertice[i], arrCusto[i], 16);
		   total += arrCusto[i];
		   aux++;
		}
		rep(9, aux*3);printf("DESTINO\n");
		rep(9, aux*3);printf("[ %0*d ]\n\n", 4, destino);
		printf("MELHOR CUSTO! >>> $$$ %d\n", total);
		rep(10, 2);
		printf("pressione < ENTER > para visualizar as ARESTAS");
		getchar(); 
		getchar();
		system("cls");
}
int BuscarMenor(vrt *v, int destino, int *custo, int *tam){
	int menor;
	vrt *tmp = v->prox;
	if(v->prox == NULL){
		printf("\nNão há vértices\n");
		exit(1);
	}else{
		while(tmp != NULL){
			if(tmp->iDestino == destino){
				menor = tmp->origemMenor;
				*custo = tmp->manterMenor;
				*tam += 1;
				break;
			}
			tmp = tmp->prox;
		}
	}
	return menor;
	free(tmp);
}
void ImprimirResumo(vrt *v, are *a){
	vrt *tmpV = v->prox;
	are *tmpA = a->prox;
	if(v->prox == NULL){
		printf("\nNão há vértices\n");
		exit(1);
	}else{
		system("cls");
		printf("\n===============RESUMO DAS VÉRTICES===============\n");
		while(tmpV != NULL){
			printf("\n[ Vértice: %d ]\n", tmpV->iDestino);
			printf("\n- Vértice Origem do MENOR CUSTO:\n\t> %d", tmpV->origemMenor);
			printf("\n- MENOR CUSTO:\n\t> %d", tmpV->manterMenor);
			printf("\n- Quantas vezes foi escolhida como ORIGEM ?\n\t> %d", tmpV->escolhidoOrigem);
			printf("\n- E quantas vezes foi escolhida como DESTINO?\n\t> %d", tmpV->escolhidoDestino);			
			printf("\n____________________________________________\n");			
			tmpV = tmpV->prox;
		}
		printf("\n=================================================\n");
	}
	printf("\npressione < ENTER > para RESUMO DAS ARESTAS");
	getchar(); 
	getchar();
	system("cls");
	if(a->prox == NULL){
	printf("\nNão há arestas\n");
	exit(1);
	}else{
		printf("\n===============RESUMO DAS ARESTAS===============\n");
		while(tmpA != NULL){
			printf("\n[ Aresta: %d ]\n", tmpA->id);
			printf("\n- Vértice ORIGEM:\n\t> %d", tmpA->origem);
			printf("\n- Vértice DESTINO:\n\t> %d", tmpA->destino);
			printf("\n- CUSTO do PERCURSO:\n\t> $ %d", tmpA->custo);			
			printf("\n____________________________________________\n");			
			tmpA = tmpA->prox;
		}
		printf("\n================================================\n");
	}
	printf("\npressione < ENTER > para sair");
	getchar(); 
	getchar();
	system("cls");
	free(tmpV);
	free(tmpA);
}
void Liberar(vrt *v, are *a){
    if(v->prox != NULL){
        vrt *tmp, *atual;
        atual = v->prox;
        while(atual != NULL){
            tmp = atual->prox;
            free(atual);
            atual = tmp;
        }
    }
    if(a->prox != NULL){
        are *tmp, *atual;
        atual = a->prox;
        while(atual != NULL){
            tmp = atual->prox;
            free(atual);
            atual = tmp;
        }
    }    
}
int asc(int n){
	int r;
	if((n+1) % 10 == 0){
		r = 10;
	}else{
		r = 9;
	}return r;
}
void Aviso(){
	printf("\n_________________________AVISO_________________________\n");
	printf("Você digitou um valor MAIOR do que 5 vértices,\n");	
	printf("caso a quantidade de dígitos do R.A. não seja suficiente,\n");	
	printf("deverá inserir alguns dígitos até atingir o necessário.\n");
	printf("pressione < ENTER > para sair\n\n> ");	
	getchar();
	getchar();
}
void rep(int num, int x){
	int i;
    for (i = 0; i != x; i++ ){
        printf("%c ", num);
    }
}
//--------------------------
void ImprimirSimples(are *a){
	are *tmpA = a->prox;
	if(a->prox == NULL){
		printf("\nNão há arestas!\n");
		exit(1);
	}else{
		system("cls");
		if(a->prox == NULL){
		printf("\nNão há arestas\n");
		exit(1);
		}else{
			printf("\n\t=================== ARESTAS ====================\n\n");
			printf("\n\t[ORIGEM] ------>\tCUSTO\t ------> [DESTINO]\n");
			while(tmpA != NULL){
				printf("\n\t[ %d ] --------> ", tmpA->origem);
				printf("\t$ %d", tmpA->custo);
				printf("\t --------> [ %d ]\n\n", tmpA->destino);						
				tmpA = tmpA->prox;
			}
			printf("\n\t================================================\n");
		}
		printf("\npressione < ENTER > para sair");
		getchar(); 
		getchar();
		system("cls");
		free(tmpA);
	}
}
