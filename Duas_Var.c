	#include<stdlib.h>
	#include<stdio.h>
	#include<time.h>
	#include<math.h>
	#include<string.h>


#define geracoes 200 
#define myrand ((float)(random())/(float)(RAND_MAX)) 

void precisao(int debinparadec[][2], float vetor_precisao[][2]){

	int  var;
	float E;

	var = pow(2.0,16.0);

	E = 5.0/(var - 1);
	
	for (int i = 0; i < 20; ++i){
		vetor_precisao[i][0] = E * debinparadec[i][0];
		vetor_precisao[i][1] = E * debinparadec[i][1];
	}// feito//feito
}//feito

void aptidao(float vetor_de_xy[][2], float vetor_de_aptdao[20]){

	float var1, var2;

	for (int i = 0; i < 20; ++i){		
		
		var1 = sin(3.14*(vetor_de_xy[i][0] - 3)) / (3.14*(vetor_de_xy[i][0] - 3));
		
		if(var1 < 0)
			var1 = var1 * (-1);
		

		var2 = sin(3.14*(vetor_de_xy[i][1] - 3)) / (3.14*(vetor_de_xy[i][1] - 3));
		if(var2 < 0)
			var2 = var2 * (-1);
		

		vetor_de_aptdao[i] = var1 * var2;// vamos por os valores de forma organizada para que nao troquemos os valores dos x
	}
}//feito

void aleato(int pont[][16]){
	
	srand(time(NULL));
	
	for(int x=0;x<20;x++){
		for (int i = 0; i < 16; ++i){
			pont[x][i] = rand() % 2;
		}	
	} //feito 
}//feito

void bintodec(int bin[][16], int matriz_de_x_y[][2]){
	int var = 0;
	double cont = 8;
		
	for (int i = 0; i < 20; ++i){
		for(int x = 0; x < 8; x++){
			cont = cont - 1;
			if(bin[i][x] == 1){
				var = var + pow(2.0,cont);
			}
		}

		matriz_de_x_y[i][0] = var;

		var = 0;
		cont = 16;

		for(int x = 8; x < 16; x++){
			cont = cont - 1;
			if(bin[i][x] == 1){
				var = var + pow(2.0,cont);
			}
		}
		
		matriz_de_x_y[i][1] = var;	
	}// feito
}//feito


float melhor(float *pont){
	float var = 0;
	for(int x = 0; x < 20; x++){
		if(var < pont[x])
			var = pont[x];
	}
	printf("Melhor de um geração: %f\n",var);
	return var;
}

int roleta(float apt[20]){
    int valor_aleatorio, sorte;
  	float total_da_porcentagem[20], porcentagem, acumulado = 0, total = 0;

    for(int i = 0; i < 20; ++i){
        total += apt[i];// gera o total de aptidao que a populacao tem
    }

    for (int i = 0; i < 20; ++i){
    	total_da_porcentagem[i] = apt[i]/total;	// pega a porcentagem que cada individuo tem na apt geral da populacao
    }


    while(1){

	    sorte = rand() % 100; // gera um numero para dizer se o individou sera esolhido ou nao, msm se a roleta disse que sim
	    acumulado = 0;
	    porcentagem = myrand;// gera um numero de 0|1 para se achar o individuo, é de acordo com sua apt

	   	for(int i = 0; i < 20; ++i){
	       	acumulado += total_da_porcentagem[i];
	       	if(acumulado > porcentagem){
	       		if(sorte >= 80)
	           		break;
	        return i;// me retorna a posisao em que o pai esta ///
	       	break;
	       	}
	   	}   
	} 
}//feito

void cruzamento(int matriz_bit[][16], int posisao[2]){
	int ponto_de_corte[2], teste[16];

	ponto_de_corte[0] = rand() % 15;// gera a posição do vetor de onde será cortado o cruzamento
	ponto_de_corte[0] += 1;

	ponto_de_corte[1] = rand() % 15;
	ponto_de_corte[1] += 1;	


	// passamos o valor do individuo escolhido para o vetor novo_povo, pois ele ira mudar de acordo
	//com o cruzamento, achei essa forma mais facil, logo abaixo vemos eles cruzando, pois assim ja fica
	//tudo no novo vetor e nao precisamos mais cuidar dele.
	
	for (int i = 0; i < 8; ++i){
		matriz_bit[posisao[0]][ponto_de_corte[0]] = matriz_bit[posisao[1]][ponto_de_corte[0]];
		
		if(ponto_de_corte[0] == 16)
			break;
		ponto_de_corte[0]++;
	}

	for (int i = 8; i < 16; ++i){
		matriz_bit[posisao[1]][ponto_de_corte[1]] = matriz_bit[posisao[0]][ponto_de_corte[1]];
		

		if(ponto_de_corte[1] == 16)
			break;

		ponto_de_corte[1]++;
	}

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
		sorte = rand() % 100;
		if(sorte <= 20){
			if(matriz_bit[cont][qual_bit] == 0)
				matriz_bit[cont][qual_bit] = 1;
			else
				matriz_bit[cont][qual_bit] = 0;
		}
	}

	//faço duas vezes aqui, pois sao duas variaveis em um so individuo
}

void geracional(int antigo[][16], int novo[][16]){

	for (int i = 0; i < 20; ++i)	{
		for (int d = 0; d < 16; ++d){
			antigo[i][d] = novo[i][d];
		}
	}
}

void melhor_dos_melhores(float melhor[geracoes]){
	float var = 0;
	for (int i = 0; i < geracoes; ++i){
		if(melhor[i] > var)
			var = melhor[i];
	}
	printf("\nMelhor dos melhores: %f", var);
}


int main(){

int  povo[20][16], matriz_de_x_y[20][2], pais[2];
float matriz_de_precisao[20][2], vetor_de_aptidao[20], melhores[geracoes];



aleato(povo);//cria a população de forma aleatorio
for (int i = 0; i < geracoes; ++i){
	

	bintodec(povo, matriz_de_x_y); //transforma para decimal
	precisao(matriz_de_x_y, matriz_de_precisao);// da a precisao escolhida, que no caso é de 0 a 5
	aptidao(matriz_de_precisao, vetor_de_aptidao);// gera a aptidao da funcao desejada

//	for (int d = 0; d < 20; ++d){
		do{
			pais[0] = roleta(vetor_de_aptidao);
			pais[1] = roleta(vetor_de_aptidao);
		}while(pais[0] == pais[1]);
		cruzamento(povo, pais);
//	}

	mutacao(povo);
	melhores[i] = melhor(vetor_de_aptidao);
	
}

melhor_dos_melhores(melhores);


}