//Bibliotecas
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <locale.h>
//Constantes
#define MIN 5 //m�nimo de arestas
#define MAX 9999 // Para c�lculo Dijkstra
#define OPCIONAL 3 // Arestas N�O obrigat�rias para o funcionamento 
#define XCUSTO 100 // Custo informado no MAPA
//Estruturas
typedef struct{
	int id;
	int custo;
}calc;
typedef struct{
	int idAre;
	int origem;
	int destino;
	int custo;
}Aresta;
typedef struct{
	int id;
	int visitado;
	int acumulado;
	int origemMenor;
	
	int nConex;
	calc *conex;
}Vertice;
//Vari�veis Globais
int pPartida = MAX;
int pChegada = MAX;
//Fun��es
/*----1� parte do c�digo-----------------------------------
prepara��o do cen�rio -------------------------------------*/
Aresta *Iniciar(Aresta *A, int *totalAre, int *totalVert);
int CheckDig(char tmp[], int menor, int maior, int igual);
int CalcComb(int n);
int Fator(int n, int p);
int BuscarCaminho(Aresta *A, int cont, int totalVertices);
void ImprimirArestas(Aresta *A, int cont);
int Liberar();
/*---------------------------------------------------------*/
/*----2� parte do c�digo-----------------------------------
c�lculo Dijkstra-------------------------------------------*/
void PartidaChegada(int totalVert);
Vertice *CriarVertice(Vertice *V, int indice);
void MenorRotaCfg(Aresta *A, int totalAre, int totalVert);
void MenorRotaCalc(Vertice *V, int totalVert);
int BuscarProxOrigem(Vertice *V, int totalVert);
void CalculoDijkstra(int origem, Vertice *V, int totalVert);
void Resultado(Vertice *V, int totalVertices);
/*---------------------------------------------------------*/
int main(void){
	setlocale(LC_ALL, "Portuguese");
	int totalAre = 0, totalVert = 0;
	Aresta *A = NULL;	
	A = Iniciar(A, &totalAre, &totalVert);
	MenorRotaCfg(A, totalAre, totalVert);
	free(A);
	printf("\n\nCalculo realizado com sucesso!\n\n<enter para fechar>\n");
	getchar();	
	getchar();
	return 0;
}
Aresta *Iniciar(Aresta *A, int *totalAre, int *totalVert){
	int cont = 0;
	int totalArestas;
	int sair, erros;
	system("cls");
   	char tmp[4] = "";
	memset(tmp, 0, 4); 		
	printf("\nINICIANDO!\n\nDigite a quantidade de V�rtices ( M�NIMO: %d ):\n", MIN);
	do{
		printf("> ");
		scanf("%s", tmp);
	}while( !CheckDig(tmp, MIN, MAX, 0) );
	int totalVertices = atoi(tmp);
	*totalVert = totalVertices;
	totalArestas = CalcComb(totalVertices);
	printf("\nSer�o geradas %d arestas para sua escolha.\n", totalArestas);
	printf("\nPara melhor an�lise, precisamos que no M�NIMO %d arestas sejam criadas (+ %d opcionais).\n", totalArestas - OPCIONAL, OPCIONAL);	
	printf("\nAp�s a cria��o de %.0f arestas, ser� avisado caso haja necessidade de mais op��es", floor(totalArestas/2));
	printf("\nde ORIGEM e/ou DESTINO.\n\nAperte <enter> para continuar...");	
	getchar();
	getchar();
	
	int i, j, k, x = 0;
	int matrizTmp[totalArestas][3];
	int indice = 0, origem = 1, destino = 2; 
	int escolhidas[totalArestas];
	j = 1, k = j + 1;
	for(i = 0; i < totalArestas; i++){
		matrizTmp[i][indice] = i + 1;
		matrizTmp[i][origem] = j;
		matrizTmp[i][destino] = k;
		k++;
		if(k > totalVertices){
			j++;
			k = (j+1);
		}
	}
	for(i = 0; i < totalArestas; i++) escolhidas[i] = 0;
	do{
		system("cls");
		printf("�ndice\tOrigem --------> Destino\n\n");
		for(i = 0; i < totalArestas; i++){
			for(j =0; j < totalArestas; j++){
				if(escolhidas[j] == matrizTmp[i][indice]) x++;
			}
			if(x > 0){
				printf("\tEM USO > { %d } ---------->{ %d }\n\n", 
					matrizTmp[i][origem], matrizTmp[i][destino]);
			}else{
				printf(" %d  > { %d } ---------->{ %d }\n\n", 
					matrizTmp[i][indice], matrizTmp[i][origem], matrizTmp[i][destino]);
			}
			x = 0;
		}
		printf("\nEscolha uma das arestas DISPON�VEIS acima.\n");
		int exct = 0;
		do{
			printf("\nDigite somente o �ndice:\n> ");
			scanf("%s", tmp);
			for(i = 0; i < totalArestas; i++) if(escolhidas[i] == atoi(tmp)) exct = atoi(tmp);
		}while( !CheckDig(tmp, 1, totalArestas, exct) ); 
		system("cls");
		printf("\nVoc� escolheu:\n");
		int linha;
		for(i = 0; i < totalArestas; i++){
			if(matrizTmp[i][indice] == atoi(tmp)){
				printf(" %d  >\t{ %d } ---------->{ %d }\n", 
						matrizTmp[i][indice], matrizTmp[i][origem], matrizTmp[i][destino]);
				linha = i;
				break;
			}
		}
		cont++;
		A = realloc(A, cont * sizeof(Aresta));
		if(A == NULL){
			printf("Erro!");
			exit(1);
		}
		escolhidas[cont-1] = atoi(tmp);
		printf("\nGostaria de inverter as pontas?\n\n");
		printf("\t{%d} ----> {%d}", matrizTmp[linha][origem], matrizTmp[linha][destino]);
		printf("\n\n  para:  \n\n");		
		printf("\t{%d} ----> {%d}\n\n", matrizTmp[linha][destino], matrizTmp[linha][origem]);
		do{
			printf("\n\t[1] SIM || [0] N�O\n\n\t> ");
			scanf("%s", tmp);
		}while( !CheckDig(tmp, 0, 1, 0) );	  		
		if(atoi(tmp)==1){
			A[cont-1].origem = matrizTmp[linha][destino];
			A[cont-1].destino = matrizTmp[linha][origem];
		}else{
			A[cont-1].origem = matrizTmp[linha][origem];
			A[cont-1].destino = matrizTmp[linha][destino];
		}
		printf("\nPara finalizar esta aresta,\n");
		printf("\nDigite o Custo da Aresta (m�ximo: $ %d).\nOu o d�gito do R.A.,", MAX-1);
		printf("por ex.:\n'200' (custo final) ou '2' (d�gito).\n");
		printf("D�gito x Custo, ex.:\n2*%d = 200, este ser� o custo da aresta.", XCUSTO);
		do{
			printf("\n> ");
			scanf("%s", tmp);
		}while( !CheckDig(tmp, 0, (MAX-1), 0) );
		if(strlen(tmp) == 1){
			A[cont-1].custo = atoi(tmp) * XCUSTO;
		}else{
			A[cont-1].custo = atoi(tmp);
		}
		memset(tmp, 0, 4);
		ImprimirArestas(A, cont);
		printf("\n\n<enter>");
		getchar();
		getchar();
		erros = BuscarCaminho(A, cont, totalVertices);
		if((cont >= floor(totalArestas/2)) && erros > 0){
			printf("\n\nForam encontradas %d necessidades\n", erros);
			printf("\n\n<enter>");
			getchar();
			getchar();
		}
		sair = 0;
		if(cont >= (totalArestas - OPCIONAL) && cont != totalArestas){
			if(erros == 0){
				printf("Faltam somente as %d opcionais.\nContinuar?\n", totalArestas - cont);
				do{
					printf("[1] SIM || [0] N�O\n> ");
					scanf("%s", tmp);
				}while( !CheckDig(tmp, 0, 1, 0) );
				if(atoi(tmp) == 0) sair = 1;
			}
		}else if(cont < (totalArestas - OPCIONAL)){
			printf("\nFaltam %d arestas + %d opcionais\n", (totalArestas - (cont + OPCIONAL)), OPCIONAL);
		}
	}while(cont < totalArestas && sair == 0);
	system("cls");
	if(cont >= (totalArestas - OPCIONAL) && erros == 0){
		*totalAre = cont;
		ImprimirArestas(A, cont);
		printf("\n\nTODAS ARESTAS CRIADAS\n");
	}else{
		ImprimirArestas(A, cont);
		printf("\n\nINFELIZMENTE AS %d NECESSIDADES N�O FORAM CORRIGIDAS\n", erros);
		printf("\nTENTE NOVAMENTE!\n\nPrograma encerrado...");
		exit(1);
	}
	printf("\n\n<enter>");
	getchar();
	getchar();
	return A;
}
int Fator(int n, int p){
	int f = n;
	while((n - 1) > p){
		n--;
		f *= n;
	};
	return f;
}
int CalcComb(int n){
	int k = 2;
	int c = Fator(n, (n - k)) / Fator(k, 0);
	return c; 
}
int CheckDig(char tmp[], int menor, int maior, int igual){
    int tamanho, i, r = 1;
    tamanho = strlen ( tmp );
    for (i = 0; i < tamanho; i++){
        if (!isdigit( tmp[i] )){
        	r = 0;
        	break;
        }
	}
	if( atoi(tmp) < menor || atoi(tmp) > maior ){
		r = 0;
	}
	if( igual > 0 && atoi(tmp) == igual ) r = 0;
	
	return r;
}
void ImprimirArestas(Aresta *A, int cont){
	int i;
	system("cls");
	printf("\nArestas criadas: %d \n\n", cont);
	for(i = 0; i < cont; i++){
		printf("\n%d. { %d }======{ %d }====== > { %d }\n", i+1, A[i].origem, A[i].custo, A[i].destino);
	}
}
int BuscarCaminho(Aresta *A, int cont, int totalVertices){
	int i, j, destino = 0, origem = 0, erros = 0;
	for(i = 1; i < totalVertices; i++){
		for(j = 0; j < cont; j++){
			if(A[j].origem == i) origem++; 
			if(A[j].destino == i+1) destino++;
		}
		if((origem == 0 || destino == 0) && erros == 0){
			printf("\n!ATEN��O!");
			printf("\nAlgumas V�rtices ainda n�o foram escolhidas como \n");
			printf("ORIGEM ou DESTINO. Para melhor an�lise, ser� necess�rio criar: \n");				
		}
		if(destino == 0){
			erros++;
			printf("\n{ X } ----> DESTINO: {%d} ", i+1);
		} 
		if(origem == 0){
			erros++;
			printf("\nORIGEM: {%d} ----> { X } ", i);
		}
		destino = 0;
		origem = 0;
	}
	return erros;
}
int Liberar(Vertice *V){
	free(V);
	return 1;
}
//-----------------------------2� PARTE--------------------------------------
void MenorRotaCfg(Aresta *A, int totalAre, int totalVert){
	Vertice *V = NULL;
	int indiceV, i, j;
	indiceV = 0;
	PartidaChegada(totalVert);
	for(i = 0; i < totalVert; i++){
		indiceV++;
		V = CriarVertice(V, indiceV);
		for(j = 0; j < totalAre; j++){
			if(A[j].origem == V[i].id){
				V[i].nConex++;
				V[i].conex = realloc(V[i].conex, V[i].nConex * sizeof(calc));
				V[i].conex[V[i].nConex - 1].id = A[j].destino;
				V[i].conex[V[i].nConex - 1].custo = A[j].custo;
			}
		}
	}
	MenorRotaCalc(V, totalVert);
}
Vertice *CriarVertice(Vertice *V, int indice){
	int menos = indice - 1;
    V = realloc(V, indice * sizeof(Vertice));
    if (!V){
		perror("Erro Malloc Criar");
		exit(EXIT_FAILURE); 
	}
	V[menos].id = indice;
	if(V[menos].id == pPartida){
	   	V[menos].acumulado = 0;		
	   	V[menos].visitado = 1;		
	}else if(V[menos].id == pChegada){
	   	V[menos].acumulado = MAX;		
		V[menos].visitado = 1;
	}else{
		V[menos].acumulado = MAX;		
		V[menos].visitado = 0;
	}
	V[menos].origemMenor = 0;
  	V[menos].nConex = 0;
  	V[menos].conex = NULL;
	return V;
}
void MenorRotaCalc(Vertice *V, int totalVert){
	int origem = pPartida;
	while(origem){
		CalculoDijkstra(origem, V, totalVert);
		origem = BuscarProxOrigem(V, totalVert);
	}
	Resultado(V, totalVert);
}
void CalculoDijkstra(int origem, Vertice *V, int totalVert){ 
	int w, c, r;
	for(w = 0; w < origem; w++){
		if(origem == V[w].id){
			V[w].visitado = 1;
			for(c = 0; c < V[w].nConex; c++){
				for(r = 0; r < totalVert; r++){
					if(V[w].conex[c].id == V[r].id){
						if(V[r].acumulado > (V[w].acumulado + V[w].conex[c].custo)){
							V[r].acumulado = V[w].acumulado + V[w].conex[c].custo;
							V[r].origemMenor = V[w].id;
							
						}
					break;						
					}
				}
			}
		}
   	}
}
int BuscarProxOrigem(Vertice *V, int totalVert){
	int prox = 0, i, menor = MAX;
	for(i = 0; i < totalVert; i++){
		if(V[i].acumulado < menor && V[i].visitado == 0){ 
			menor = V[i].acumulado;
			prox = V[i].id;
		}
	}
	return prox;
}
void Resultado(Vertice *V, int totalVertices){
	int i, j, k, anterior, indice = 0, total = 0;
	calc *caminho = NULL;
	system("cls");
	printf("* Relat�rio das V�RTICES *\n");
	for(i = 0; i < totalVertices; i++){
		printf("V�rtices: \n{ %d } para ---> ", V[i].id);
		for(j = 0; j < V[i].nConex; j++){
			printf("\n\t\t{ %d } ", V[i].conex[j].id);
			printf("\tCusto: $ %d ", V[i].conex[j].custo);
		}
		if(V[i].nConex == 0) printf("N�o escolhida como Origem!");
		printf("\n\t\tMenor v�rtice anterior: { %d }", V[i].origemMenor);
	   	printf("\n\t\tMenor Custo at�  { %d }: $ %d\n", V[i].id, V[i].acumulado);
	}
	printf("\n\n* MENOR CAMINHO *\n\n");
	anterior = pChegada;
	while(anterior != pPartida){
		for(i = 0; i < anterior; i++){
			if(V[i].id == anterior){
				if(anterior == pChegada) total = V[i].acumulado;
				indice++;
				caminho = realloc(caminho, indice * sizeof(calc));
				caminho[indice-1].id = V[i].origemMenor;
				anterior = V[i].origemMenor;
				for(j = 0; j < anterior; j++){
					if(V[j].id == anterior){
						for(k = 0; k < V[j].nConex; k++){
							if(V[j].conex[k].id == V[i].id){
								caminho[indice-1].custo = V[j].conex[k].custo;
								break;
							}
						}
					}
				}

			}
		}
	}
	for(i = indice-1; i >= 0; i--){
		printf("{ %d } custo: $ %d ---> ", caminho[i].id, caminho[i].custo);
	}
	printf("{ %d }.\nTOTAL MENOR CUSTO: $ %d .\n", pChegada, total);
	free(caminho);
}
void PartidaChegada(int totalVert){
	char tmp[4] = "";
	memset(tmp, 0, 4);
	system("cls");
	printf("\nDigite o ponto de\n");
	do{
		printf("\n\tPARTIDA > ");
		scanf("%s", tmp);
	}while( !CheckDig(tmp, 1, totalVert, 0) );
	pPartida = atoi(tmp);
	memset(tmp, 0, 4);
	do{
		printf("\n\tCHEGADA > ");
		scanf("%s", tmp);
	}while( !CheckDig(tmp, 1, totalVert, pPartida) );
	printf("\n");
	pChegada = atoi(tmp);
}