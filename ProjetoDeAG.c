#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include<string.h>


#define TAMANHO  500 //tamanho da entrada de dados, é so mudar aqui
#define SUPERINDIVIUOS 150 // quantidade de individuos que deseja criar 
#define myrand ((float)(random())/(float)(RAND_MAX)) 
#define N 8 //quantas numeros da entrada de dados deseja, para compor um individuo
#define valor_do_N 28// este numero deve ser mudado sempre que o N mudar tbm, é nele que inserimos o resultado de cada 
// disctancia entre as varias entradas de dados que compoem o individuo. o calculo deve ser feito a mao, ou voce pode 
// executar a funcao "fatorialdemais" que ela te dara o resultado de acordo com N. o calculo a mao nada mais é que um
// fatorail simples, comelando com o valor de N, mas nao se multiplica, e sim somasse.
#define geracoes 300// numeros de gerações.

typedef struct Sindividuos{
	float individuo[N];//onde fica os sub individuos (entrada de dados)
	float indi[valor_do_N];//onde fica cada uma das suas diferenças
	float total_diferenca;// somatorio de todas as diferenças desse individuo
}Individuo;


void vetor_aleatorio(float vetor[TAMANHO]){
		int var; 
		clock_t inicio;
			
		for(int d = 0; d < TAMANHO; ++d){
			inicio = clock();//faz com que a funcao rand gere numeros aleatorios a cada milisegundo
			srand((unsigned) time(NULL) + inicio);
			var = rand() % 500;
			vetor[d] = myrand * var;
			
		}
}
void escreve_arquivo( float vetor[TAMANHO]){

	FILE *arq;
	int result = 0;

	arq = fopen("arqui.bin", "wb");

	if (arq == NULL){
    	printf("Problemas na abertura do arquivo\n");
    	return;
	}

		result = result + fwrite (&vetor[0], sizeof(float), TAMANHO, arq);
	
	

	printf("Nro de elementos gravados: %d\n", result);
    fclose(arq);
}
void ler_arquivo( float vetor[TAMANHO]){

	FILE *arq;
  	int result = 0;

	// Abre um arquivo BINÁRIO para LEITURA
	arq = fopen("arqui.bin", "rb");

	if (arq == NULL){  // Se houve erro na abertura
	   	printf("Problemas na abertura do arquivo\n");
	    return;
	}
		result = result + fread(&vetor[0], sizeof(float), TAMANHO, arq);	
	
	
	printf("Nro de elementos lidos: %d\n", result);
	
	fclose(arq);	
}

int fatorialdemais(){
	int aux = 0;
	for (int i = N; i > 0; i--){
		aux = aux + (i-1);// fiz essa função pra saber o valor total do tamanho do vetor que eu devo guardar
		//pois se quero 4 individuos preciso guardar 6, se quero 5, 10.
	}

	return aux;
}

void cria_individuos(float vetor[TAMANHO], Individuo indivi[SUPERINDIVIUOS]){
	int   alea[N], cont2 = 0, aux = 0;
	float var[N];
	clock_t inicio;
	
	for(int h = 0; h < SUPERINDIVIUOS; h++){
		cont2 = 0; aux = 0;
		do{
			inicio = clock();//faz com que a funcao rand gere numeros aleatorios a cada milisegundo
			srand((unsigned) time(NULL) + inicio);
			
			alea[cont2] = rand() % TAMANHO; //gera um numero aleatorio para pegar no vetor de sub_individuos
			var[cont2] = vetor[alea[cont2]];// pego o sub do vetor e boto em var
			
			cont2++;

			if(cont2 == N){
				for (int i = 0; i < N; ++i){
					if(aux == alea[i]){
						cont2 = 0;	
					}
					aux = alea[i];
				}
			}

			
		}while(cont2 < N);
		
		for(int i = 0; i < N; ++i){
			indivi[h].individuo[i] = var[i];
		}
	}
}//funcionanado;

float distancia_euclidiana(Individuo indivi[SUPERINDIVIUOS], int posisao){

	int  cont = 0, cont2 = 0, cont3 = 0, aux = 0; 

	
	for (int posisao = 0; posisao < SUPERINDIVIUOS; ++posisao){
		cont = 0; cont2 = 0; cont3 = 0; aux = 1;
		while(cont2 < (N-1)){
			for (int cont3 = aux; cont3 < N; ++cont3){
					
				
				indivi[posisao].indi[cont] = sqrt((indivi[posisao].individuo[cont2] - indivi[posisao].individuo[cont3])*(indivi[posisao].individuo[cont2] - indivi[posisao].individuo[cont3]));// euclidiana de x1 e x2
				
				if(indivi[posisao].indi[cont] < 0)
					indivi[posisao].indi[cont] = indivi[posisao].indi[cont] * (-1);
				
			
				cont++;
			}
			
			if(cont == fatorialdemais())
				break;
			
			cont2++;
			aux = cont2 + 1;
		}
	}		
}//funciona

void aptidao(Individuo indivi[SUPERINDIVIUOS], int posisao){

	indivi[posisao].total_diferenca = 0;
	for (int d = 0; d < valor_do_N; ++d){
		indivi[posisao].total_diferenca = indivi[posisao].indi[d] + indivi[posisao].total_diferenca;//somatorio do Super Individuo;
	}
	
}//funcionando

int roleta(Individuo indivi[SUPERINDIVIUOS]){
  
    int valor_aleatorio, sorte;
  	float total_da_porcentagem[SUPERINDIVIUOS], porcentagem, acumulado = 0, total = 0;
  
    for(int i = 0; i < SUPERINDIVIUOS; ++i){
        total += indivi[i].total_diferenca;// gera o total de aptidao que a populacao tem
    }

    //printf("\n\n%f\n",total);

    for (int i = 0; i < SUPERINDIVIUOS; ++i){
    	total_da_porcentagem[i] = indivi[i].total_diferenca/total;	// pega a porcentagem que cada individuo tem na apt geral da populacao
    }


    while(1){
	    acumulado = 0;
	    porcentagem = myrand;// gera um numero de 0|1 para se achar o individuo, é de acordo com sua apt

	   	for(int i = 0; i < SUPERINDIVIUOS; ++i){
	       	acumulado += total_da_porcentagem[i];
	       	if(acumulado > porcentagem){
	        	return i;// me retorna a posisao em que o pai esta ///
	       		break;
	       	}
	   	}   
	} 
}

void cruzamento(Individuo indivi[SUPERINDIVIUOS], int posisao[2]){
	int ponto_de_corte[2], div;
	Individuo sup[2];

	ponto_de_corte[0] = rand() % valor_do_N - 1;
	if(ponto_de_corte[0] == -1)
		ponto_de_corte[0] = 0;
	
	ponto_de_corte[1] = rand() % valor_do_N - 1;
	if(ponto_de_corte[1] == -1)
		ponto_de_corte[1] = 0;

	
	for (int i = 0; i < valor_do_N; ++i){
		sup[0].individuo[i] = indivi[posisao[0]].individuo[i];
		sup[1].individuo[i] = indivi[posisao[1]].individuo[i];
		// passamos o valor do individuo escolhido para o vetor novo_povo, pois ele ira mudar de acordo
		//com o cruzamento, achei essa forma mais facil, logo abaixo vemos eles cruzando, pois assim ja fica
		//tudo no novo vetor e nao precisamos mais cuidar dele.
	}

	div = valor_do_N/3;
	for (int i = 0; i < div; ++i){
		indivi[posisao[0]].individuo[i] = sup[1].individuo[i];
	}

	for (int i = div; i < div*2+1; ++i){
		indivi[posisao[0]].individuo[i] = sup[0].individuo[i];
	}

	for (int i = div*2+1; i < valor_do_N; ++i){
		indivi[posisao[0]].individuo[i] = sup[1].individuo[i];
	}
	for (int i = 0; i < div; ++i){
		indivi[posisao[1]].individuo[i] = sup[0].individuo[i];
	}

	for (int i = div; i < div*2+1; ++i){
		indivi[posisao[1]].individuo[i] = sup[1].individuo[i];
	}

	for (int i = div*2+1; i < valor_do_N; ++i){
		indivi[posisao[1]].individuo[i] = sup[0].individuo[i];
	}
	/*
	for (int i = 0; i < N; ++i){
	printf("%f ",sup[0].individuo[i] );
	}
	printf("\n");
	for (int i = 0; i < N; ++i){
	printf("%f ",sup[1].individuo[i] );
	}
	printf("\n\n");
	for (int i = 0; i < N; ++i){
	printf("%f ",indivi[posisao[0]].individuo[i] );
	}
	printf("\n");
	for (int i = 0; i < N; ++i){
	printf("%f ",indivi[posisao[1]].individuo[i] );
	}
	*/
}	

void mutacao(float vetor[TAMANHO], Individuo indivi[SUPERINDIVIUOS]){
	float porcentagem;
	int alea, alea2;
	clock_t inicio;

	for (int i = 0; i < SUPERINDIVIUOS; ++i){
		porcentagem = myrand;
		if(porcentagem >= 0.85){// 15% de chance de ocorre uma substicao num individuo
			
			inicio = clock();//faz com que a funcao rand gere numeros aleatorios a cada milisegundo
			srand((unsigned) time(NULL) + inicio);
			alea = rand() % N;// ver qual dos N subindividuos será substituido
			alea2 = rand() % TAMANHO;// ver qual da total de população ira substituir 

			indivi[i].individuo[alea] = vetor[alea2];// substitui um sub individuo por outro, de forma aleatoria
		}
	}
}

void melhor_da_geracao(Individuo indivi[SUPERINDIVIUOS], float valores_dos_X[N], float *difere){

	for (int i = 0; i < SUPERINDIVIUOS; ++i){
		if(*difere < indivi[i].total_diferenca){
			//printf("valor %f\n", indivi[i].total_diferenca);
			*difere = indivi[i].total_diferenca;
			//printf("valor %f\n", *difere);
			for (int d = 0; d < N; ++d){
				valores_dos_X[d] = indivi[i].individuo[d]; 
			}
		}
	}
}



int main(){

int pais[2];
float vetor_alea[TAMANHO], Sorte_Muta, sorte, aux=0, valores_dos_X[N], difere = 0;
Individuo indivi[SUPERINDIVIUOS];

vetor_aleatorio(vetor_alea);
cria_individuos(vetor_alea, indivi);// pegamos os subindividuos aleatoriamente e criamos o individuo com N subindiviuos

for (int i = 0; i < SUPERINDIVIUOS; ++i){
	distancia_euclidiana(indivi, i);// calcula o valor de cada diferença dos pontos, indo de 2 em dois, passando por todos
	aptidao(indivi, i);// faz um somatio com todas as diferenças geradas, passa por cada diferença dos subindividuos	
}

for (int d = 0; d < geracoes; ++d){
	
	for (int i = 0; i < SUPERINDIVIUOS; ++i){
		melhor_da_geracao(indivi, valores_dos_X, &difere);//pegamos os Xs das melhores aptidoes, de cada geração.
		sorte = myrand;
		if(sorte >= 0.2){
			pais[0] = roleta(indivi);
			pais[1] = roleta(indivi);
			cruzamento(indivi, pais);
			mutacao(vetor_alea, indivi);
			//Aqui será um estacionario, pois cada vez que os pais forem substituidos pelos filhos
			//eles já irão para a seleção e cruzamento juntos. 
			distancia_euclidiana(indivi, pais[0]);// calcula o valor de cada diferença dos pontos, indo de 2 em dois, passando por todos
			aptidao(indivi, pais[0]);// faz um somatio com todas as diferenças geradas, passa por cada diferença dos subindividuos
			//fazemos denono pois, como o individuo foi modificado, precisamos calcular a aptidao do memso de novo
			//e como sao dois indiviuos, temos que chamar cada função duas vezes
			distancia_euclidiana(indivi, pais[1]);
			aptidao(indivi, pais[1]);
		}
	}
}
printf("Melhores valores dos X's\n");
for (int i = 0; i < N; ++i){
	printf("%f ",valores_dos_X[i]);
}
printf("\n\nA diferença desses valores de X: %f\n", difere);

}