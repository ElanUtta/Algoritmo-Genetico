#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include<string.h>


#define geracoes 200 

float precisao(int vetor[], float local_aptidao[]){

	int  var;
	float E;

	var = pow(2.0,16.0);

	E = 5.0/(var - 1);
	
	for (int i = 0; i < 20; ++i){
		local_aptidao[i] = E * vetor[i];
	}
}

void aptidao(int vetor_de_x[][2], float vetor_de_aptdao[20]){

	float var1, var2;

	for (int i = 0; i < 20; ++i){
		
		
		var1 = sin(3.14*(vetor_de_x[i][0] - 3))/3.14*(vetor_de_x[i][0] - 3);
		if(var1 < 0){
			var1 = var1 * (-1);
		}

		var2 = sin(3.14*(vetor_de_x[i][1] - 3)) / 3.14*(vetor_de_x[i][1] - 3);
		if(var2 < 0){
			var2 = var2 * (-1);
		}

		vetor_de_aptdao[i] = var1 * var2;// vamos por os valores de forma organizada para que nao troquemos os valores dos x
	}
}


void aleato(int pont[][16]){
	
	srand(time(NULL));
	
	for(int x=0;x<20;x++){
		for (int i = 0; i < 16; ++i){
			pont[x][i] = rand() % 2;
		}	
	}
}
/*
int dectobin(float var, int *pont){
	
	int i, bin[16];
	printf("valor passo no var: %f\n" ,var);

	for(i = 16; i >= 0; i--) {
	    // Executa a operação shift right até a 
	    // última posição da direita para cada bit.
	    bin[i] = var >> i;
	         
	    // Por meio do "e" lógico ele compara se o valor 
	    // na posição mais à direita é 1 ou 0 
	    // e imprime na tela até reproduzir o número binário.
	    if(bin[i] & 1) {
	        bin[i] = 1;
	    } else {
	        bin[i] = 0;
	    }
	 }
}
*/
int bintodec(int *bin){
	int var = 0;
	double cont = 16;
	
	for(int x = 0; x < 16; x++){
		cont = cont - 1;
		if(bin[x] == 1){
			var = var + pow(2.0,cont);
		}
	}
	return var;
}


float melhor(float *pont){
	float var = 0;
	for(int x = 0; x < 20; x++){
		if(var < pont[x])
			var = pont[x];
	}
	return var;
}

int roleta(float *apt){
    int valor_aleatorio, sorte, total = 0, acumulado = 0;
  
    for(int i = 0; i < 20; ++i){
        total += apt[i];
    }

    while(1){
    	
    	sorte = rand() % 100;
    	valor_aleatorio = rand() % total;
    	

    	for(int i = 0; i < 20; ++i){
    		acumulado += apt[i];
        	if(acumulado > valor_aleatorio){
        		if(sorte >= 80)
            		break;
            	return i;
            	break;
        	}
    	}   
	}
}


void cruzamento(int matriz_bit[][16], int *posicao, int novo_povo[][16], int *posi_novo){
	int ponto_de_corte[2];

	ponto_de_corte[0] = rand() % 15;
	ponto_de_corte[0] += 1;

	ponto_de_corte[1] = rand() % 15;
	ponto_de_corte[1] += 1;	

	for (int i = 0; i < 16; ++i){
		novo_povo[*posi_novo][i] = matriz_bit[posicao[0]][i];
		novo_povo[*posi_novo + 1][i] = matriz_bit[posicao[1]][i];
		// passamos o valor do individuo escolhido para o vetor novo_povo, pois ele ira mudar de acordo
		//com o cruzamento, achei essa forma mais facil, logo abaixo vemos eles cruzando, pois assim ja fica
		//tudo no novo vetor e nao precisamos mais cuidar dele.
	}

	for (int i = 0; i < 16; ++i){
		novo_povo[*posi_novo][ponto_de_corte[0]] = novo_povo[*posi_novo][ponto_de_corte[0]];
		
		ponto_de_corte[0]++;
	}

	for (int i = 0; i < 16; ++i){
		novo_povo[*posi_novo+1][ponto_de_corte[1]] = novo_povo[*posi_novo+1][ponto_de_corte[1]];
		
		ponto_de_corte[1]++;
	}
	if(*posi_novo == 18)
		*posi_novo = 0;
		
	else
		*posi_novo += 2;
}	

void mutacao(int matriz_bit[][16]){

	int sorte, qual_bit, cont;

	for(cont = 0; cont < 20; cont++){
	
		sorte = rand() % 100;
		qual_bit = rand() % 16;


		if(sorte <= 20){
			if(matriz_bit[cont][qual_bit] == 0)
				matriz_bit[cont][qual_bit] = 1;
			else
				matriz_bit[cont][qual_bit] = 0;
		}
	}
}

void geracional(int antigo[][16], int novo[][16]){

	for (int i = 0; i < 20; ++i)	{
		for (int d = 0; d < 16; ++d){
			antigo[i][d] = novo[i][d];
		}
	}

}

float melhor_dos_melhores(float melhor[geracoes]){
	float var = 0;
	for (int i = 0; i < geracoes; ++i){
		if(melhor[i] > var)
			var = melhor[i];
	}
	return var;
}


int main(){

int  povo[20][16], valor_do_individuo[20], posicao_dos_pais[2], novo_povo[20][16], posi_novo = 0;
float  mlhor[200], aptidao[20], melhordosmelhores;




aleato(povo);//cria a população de forma aleatorio

for(int d = 0; d < geracoes; d++){
	//printf("0\n");
	for (int i = 0; i < 20; ++i)
		valor_do_individuo[i] = bintodec(povo[i]);// transforma todo mundo em decimal para poder calcular a aptidao
	//printf("1\n");
	precisao(valor_do_individuo, aptidao); // calcula a aptidao de todos e bota no vetor aptidao;
	//printf("2\n");
	for (int i = 0; i < 20; ++i){	
	//	printf("3\n");
		posicao_dos_pais[0] = roleta(aptidao);// geramos a possibilidade do individuo ser escolhido pro cruzamento.
		posicao_dos_pais[1] = roleta(aptidao);// assim como sua possibilidade de 80% de ocorrer.
	//	printf("4\n");
		cruzamento(povo, posicao_dos_pais, novo_povo, &posi_novo);// cruzamos o individuo 
	//	printf("5\n");
	}

	mutacao(novo_povo);// mutamos o individuo com o bit flip e vemos se ela ocorre ou nao, sendo 20% a chance
	//printf("6\n");
	mlhor[d] = melhor(aptidao);// pegamos o valor da melhor aptidoa feita. 
	
	geracional(povo, novo_povo);// botamos todos os valores de novo_povo em povo;
	//printf("7\n");
	printf("Valor da melhor aptidao :%f\n", mlhor[d]);
}

	melhordosmelhores = melhor_dos_melhores(mlhor);
	printf("\nMelhor dos melhores: %f\n",melhordosmelhores );
}