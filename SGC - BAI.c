 /*Projecto - Grupo nº 7
 * Elementos:
 * 			- Barquisse Fulai David, nº 221075
 * 			- Albano Patrício, nº 228778
 * 			- Sílvio Correia, nº 223161
 * 	Cadeira: Programação I
 * 	Docente: Lukau Garcia
 * 	
 * 	Objetivo: De formas a melhorar a gestão e controlo de seus clientes e seus
respectivos dinheiros, foram convidados os alunos do curso de
Informática de Gestão Financeira do ISAF a desenvolver um sistema
que vai ajudar o BAI nas suas actividades
*/

//Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <locale.h>



//Registos
struct pessoa {
	char nome[50];
	char bi[15];
	double nif;
	int codigo;
	int telefone;
};

struct conta{
	int num_conta;
	double saldo;
	int codigo_cliente;
	int tipo_conta;
	char estado[20];
};

typedef struct pessoa PESSOA;
typedef struct conta CONTA;

//Variaveis e constantes para vectores
int TotalClientes = 0;
int TotalContas = 0;


//Vectores

PESSOA clientes[1000];
CONTA contas[1000];

//Funções auxiliares

//Esta função procura identificar se o cliente existe na base de dados, por intermédio do seu código pessoal
int identificar_cliente(int codigo_pessoa){
	int espaco;
	
	for (espaco=0; espaco<TotalClientes; espaco++){
		PESSOA p = clientes[espaco];
		
		if (codigo_pessoa == p.codigo){
			return espaco;
		}
	}
 return -1;
}

// Esta função procurar identificar se a conta do cliente existe, por intermédio do seu código pessoal
int identificar_conta(int codigo_cliente){
	int espaco;
	
	for (espaco=0; espaco<TotalContas; espaco++){
		CONTA c= contas[espaco];
		
		if (c.codigo_cliente == codigo_cliente){
			return espaco;
		}	
	}
return -1;
}
 
// Esta função procura identificar se a conta do cliente, por intermédio do número de conta fornecido.
int procurar_conta(int num_conta){
	int espaco;
	
	for (espaco=0; espaco<TotalContas; espaco++){
		CONTA c= contas[espaco];
		
		if (c.num_conta == num_conta){
			return espaco;
		}
	}
return -1;
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void continuar(){
	printf("\n\nPressione em qualquer tecla para continuar.");
	_getch();
	fflush(stdin);
}


//Criação de Ficheiro
FILE *ficheiro;


void CriarFicheiro(){
	ficheiro = fopen("backup_isaf.txt", "r");
	if (ficheiro == NULL){
		
		ficheiro = fopen("backup_isaf.txt", "w");
			if (ficheiro == NULL){
				printf("Não foi possível criar o arquivo");
				exit(EXIT_FAILURE);
			}
		printf("Arquivo criado com sucesso.\n");
		
		sleep(1);
		fclose(ficheiro);
	}
	else {
		
		fclose(ficheiro);
	}
	
}

//Funções de Ficheiro

//Função que escreve as informações no ficheiro
void copia_de_seguranca(){
	ficheiro = fopen("backup_isaf.txt", "w");
		
		if (ficheiro == NULL){
			printf("Não foi possível abrir o ficheiro.");
			return;
		}
		
		int i;
		for (i=0; i<TotalClientes; i++){
			
			fprintf(ficheiro, "%d ;%s;%s; %lf ; %d ;", clientes[i].codigo, clientes[i].nome, clientes[i].bi, clientes[i].nif, clientes[i].telefone);
			
			int espaco = identificar_conta(clientes[i].codigo);
			
			if (espaco != -1){
				fprintf(ficheiro, "%d ; %d ; %lf ; %d ;%s;", contas[espaco].codigo_cliente, contas[espaco].num_conta, contas[espaco].saldo, contas[espaco].tipo_conta, contas[espaco].estado);
			
			}
			fprintf(ficheiro,"\n");
		}
	
	fclose(ficheiro);		
}

//Função que lê as informações existentes no ficheiro
void restaurar_copia_de_seguranca(){
    
    ficheiro = fopen("backup_isaf.txt", "r");
    if (ficheiro == NULL){
        printf("Erro ao abrir o ficheiro");
        exit(EXIT_FAILURE);
    }else if (ficheiro != NULL){
    	printf("Iniciando o programa."); 
    	sleep(1);
			}
    PESSOA p;
    CONTA c;

		char linha[1000];
		while(fscanf(ficheiro, "%[^\n]\n", &linha) == 1){
			//fflush(stdin);
			
			char *dados = strtok(linha, ";");
			char *linha_separar[10];
			

			int i = 0;
			while (dados != NULL) {
				linha_separar[i] = dados;
				dados = strtok(NULL, ";");
				i++;
			}
			
			
			p.codigo = atoi(linha_separar[0]);
			strcpy(p.nome, linha_separar[1]);
				
			strcpy(p.bi, linha_separar[2]);
			sscanf(linha_separar[3], "%lf", &p.nif);
		//	p.nif = atof(linha_separar[3]);
			sscanf(linha_separar[4], "%d", &p.telefone);
			fflush(stdin);

			clientes[TotalClientes]=p;
			TotalClientes++;

			if (i == 5){
				continue;
			}else{	
				CONTA c;
				
				sscanf(linha_separar[5], "%d", &c.codigo_cliente);
				fflush(stdin);
				sscanf(linha_separar[6], "%d", &c.num_conta);
				fflush(stdin);
				
				sscanf(linha_separar[7], "%lf", &c.saldo);
				fflush(stdin);
				//c.codigo_cliente = atoi(linha_separar[7]);
				 sscanf(linha_separar[8],"%d", &c.tipo_conta);
				 fflush(stdin);
				//c.tipo_conta = atoi(linha_separar[8]);
				strcpy(c.estado, linha_separar[9]);
				
				contas[TotalContas]=c;
				TotalContas++;	
			}	
		}	
    fclose(ficheiro);
}

//MÓDULOS PRINCIPAIS


//FUNÇÃO DE CADASTRAR CLIENTES
bool cadastro_de_clientes(PESSOA p)
{
	
	clientes[TotalClientes]=p;
	TotalClientes++;
	return true;
}


//FUNÇÃO ABERTURA DE CONTA
bool abertura_de_conta (PESSOA p, int tipo_de_conta){
	CONTA c;
	
	int aberta = identificar_conta(p.codigo);
	if(aberta ==-1 ){
		
		c.num_conta=TotalContas+1;
		c.saldo=0;
		c.codigo_cliente = p.codigo;
		c.tipo_conta=tipo_de_conta;
		strcpy(c.estado, "Desativado");
	
		
		printf("\n================================= \n");
		printf("ID cliente: %d", c.codigo_cliente);
		printf("\nNúmero da Conta: %d", c.num_conta);
		printf("\nSaldo:      AKZ %.2f", c.saldo);
		printf("\nTipo de conta: %d", c.tipo_conta);
		printf("\nEstado da Conta:%s ", c.estado);
		printf("\n================================= \n");
		
		contas[TotalContas]=c;
		TotalContas++;
		return true;
	} else {
		printf("Este cliente já possui uma conta");
	}
}


//PROCEDIMENTO LISTAR CLIENTES
void listar_cliente(){
	int contador, espaco;
	
	for(contador=0; contador<TotalClientes; contador++){
		PESSOA p=clientes[contador];
		printf("=============================================\n");
		printf("ID:%d", p.codigo);
		printf("\nNome:%s", p.nome);
		printf("\nBI:%s", p.bi);
		printf("\nNIF:%.lf", p.nif);
		
		printf("\nTelefone:%d", p.telefone);
		
		espaco=identificar_conta(p.codigo);
		
		if (espaco!=-1){
			CONTA c = contas[espaco];
			
			printf("\nNúmero da conta: %d", c.num_conta);
			printf("\nID Conta: %d", c.codigo_cliente );
			printf("\nSaldo:           AKZ %.2lf", c.saldo);
			printf("\nTipo de Conta: %d", c.tipo_conta);
			printf("\nEstado da Conta:%s", c.estado);
			fflush(stdin);
			printf("\n=====================================\n");
		} else{
			printf("\n\nO cliente cadastrado não possui uma conta.\nPor favor, prossiga com a abertura da conta do cliente.");
			printf("\n=====================================\n");
		}
		
	}
}

//FUNÇÃO DEPOSITAR DINHEIRO
bool depositar_dinheiro(int num_conta, double valor){
	int espaco;
	
	//Verifica se a conta existe e só depois procede ao depósito de dinheiro
	espaco = procurar_conta(num_conta);
	if(espaco!=-1){
		contas[espaco].saldo=contas[espaco].saldo+valor;
		strcpy(contas[espaco].estado, "Ativado");	
		return true;
	}
	
return false;
}

//FUNÇÃO LEVANTAR DINHEIRO
bool levantar_dinheiro(int num_conta, double valor){
	//verifica se a conta existe e só depois procede ao levantamento do dinheiro
	int espaco = procurar_conta(num_conta);
	
	if (espaco!=-1){
		//Verificar se o saldo é suficiente para fazer a transação
		if(contas[espaco].saldo>=valor){
			contas[espaco].saldo= contas[espaco].saldo-valor;
			return true;
		}else{
			printf("\nSaldo insuficiente.\nImpossível realizar operação. ");
			return false;
		}
	}else{
		printf("\nTente novamente");
		return false;
	}
}

double consultar_saldo(int num_conta){
	int espaco = procurar_conta(num_conta);
	
	if (espaco != -1){
		return contas[espaco].saldo;
	}
return -1;
}

bool transferir_dinheiro(int num_conta_enviar, int num_conta_receber, double valor){
	int espacoEnviar = procurar_conta(num_conta_enviar);
	int espacoReceber = procurar_conta(num_conta_receber);
	
	if (espacoEnviar != -1 && espacoReceber!=-1){
		//Verificar se as contas encontram ativadas e se o saldo da conta emissora é suficiente.
		if ( strcmp(contas[espacoEnviar].estado, "Ativado")==0 && contas[espacoEnviar].saldo>=valor && strcmp(contas[espacoReceber].estado, "Ativado")==0){
			
			contas[espacoEnviar].saldo= contas[espacoEnviar].saldo-valor;
			
			contas[espacoReceber].saldo= contas[espacoReceber].saldo+valor;
			return true;
			
		} else if(strcmp(contas[espacoEnviar].estado, "Desativado")==0){
			printf("\nA conta a transferir está indisponível");
		} else if(strcmp(contas[espacoReceber].estado, "Desativado")==0){
			printf("\nA conta a receber está indisponível");
		} else if(strcmp(contas[espacoEnviar].estado, "Ativado")==0 && contas[espacoEnviar].saldo<valor){
			printf("\nO saldo da conta %d é insuficiente", contas[espacoEnviar].num_conta);
		}
	}
	
return false;
}

//Função de atualização de dados.
bool atualizar_dados(PESSOA p, int codigo_pessoa){
	fflush(stdin);
	int espaco = identificar_cliente(codigo_pessoa);
			if(espaco !=-1){
				int opcao;
		        do {
		            printf("Escolha o campo a ser atualizado:");
		            printf("\n1. BI\n");
		            printf("2. NIF\n");
		            printf("3. Nome\n");
		            printf("4. Telefone\n");
		            printf("0. Sair\n");
		            printf("Opção: ");
		            scanf("%d", &opcao);
		            fflush(stdin);
		
		            switch (opcao) {
		                case 1:
		                	system("cls");
		                	printf("===================Atualização de Dados=================\n");
		                    printf("\nInforme o BI:");
		                    scanf("%s", &clientes[espaco].bi);
		                    fflush(stdin);
						    break;
		                case 2:
		                	system("cls");
		                	printf("===================Atualização de Dados=================\n");
		                    printf("\nInforme o NIF: ");
		                    scanf("%lf", &clientes[espaco].nif);
		                    
		                    fflush(stdin);
		                    break;
		                case 3:
		                	system("cls");
		                	printf("===================Atualização de Dados=================\n");
		                    printf("\nInforme o nome:");
		                    scanf("%49[^\n]%*c", clientes[espaco].nome);
		                    fflush(stdin);
		                    break;
		                case 4:
		                	system("cls");
		                	printf("===================Atualização de Dados=================\n");
		                	printf("\nInforme o número de telefone: ");
		                	scanf("%d", &clientes[espaco].telefone);
		                	fflush(stdin); 
		                //	limparBufferEntrada();
		                	//printf("Telefone: %d\n", clientes[esapco].telefone);
		                	
		                	break;
		                case 0:
		                	system("cls");
		                	printf("===================Atualização de Dados=================\n");
		                    printf("Atualização concluída.\n");
		                    break;
		                default:
		                    printf("Opção inválida. Tente novamente.\n");
		            }
		        } while (opcao != 0);
		        
		        //clientes[espaco] = p;
		        return true;
		    
	    	} else {
	        printf("Pessoa não encontrada.\n");
	        sleep(1);
	        return false;
	    	}
}



int main(){
setlocale(LC_ALL, "Portuguese");

CriarFicheiro();
restaurar_copia_de_seguranca();

int opcao; 
bool resposta;

do{
	system("cls");
	
	printf("===============SISTEMA DE GESTÃO DE CLIENTES BAI=================\n");
	printf("======== Menu ========\n");
	printf("1: Cadastrar Cliente\n");
	printf("2: Listar Clientes\n");
	printf("3: Abertura de Conta\n");
	printf("4: Depositar Dinheiro\n");
	printf("5: Levantar Dinheiro\n");
	printf("6: Consultar Saldo\n");
	printf("7: Transferir Dinheiro\n");
	printf("8: Actualizar Dados do Cliente\n");
	printf("9: Terminar\n");
	printf("Escolha uma operação a realizar.\nOpção: ");
	scanf("%d", &opcao);
	fflush(stdin);
	
	switch(opcao){
		
		case 1: {
	        system("cls");
	        PESSOA p;
	        int escolha;
	
	        printf("===================Cadastro de Clientes=============\n\n");
	
	        printf("Insira o nome:");
	        scanf("%49[^\n]%*c", p.nome);
	     	fflush(stdin);
	        printf("\nQual documento usará para o cadastro?\n1-BI\n2-NIF\nEscolha: ");
	        scanf("%d", &escolha);
	        fflush(stdin);  // Limpar o buffer de entrada
	
	        switch (escolha) {
	            case 1:
	                printf("\nInsira o número do BI:");
	                scanf("%14s", p.bi);
	                fflush(stdin);
	                break;
	            case 2:
	                printf("\nInsira o número do NIF: ");
	                scanf("%lf", &p.nif);
	                fflush(stdin);  // Limpar o buffer de entrada
					strcpy(p.bi, "NoBI");
	                break;
	            default:
	                printf("\nOpção Inválida.\n");
	                break;
	        }
	        printf("\nInsira o número do telefone: ");
	        scanf("%d", &p.telefone);
	        fflush(stdin);  // Limpar o buffer de entrada
	        
				int ultimoCodigo = 0, i;
				for (i = 0; i < TotalClientes; i++){
				    if (clientes[i].codigo > ultimoCodigo){
				        ultimoCodigo = clientes[i].codigo;
				    }
				}
				// Atribuir o novo código
				p.codigo = ultimoCodigo + 1;
				
	        resposta = cadastro_de_clientes(p);
	        if (resposta == true){
	        	printf("\nOperação realizada com sucesso!");
			} else {
				printf("\nPor favor, tente novamente!");
			}
			sleep(1);
			
			system("cls");
			sleep(1);
			printf("===== Dados de cadastramento =====");
			printf("\n\nNome:%s", p.nome);
			if (escolha == 1){
				printf("\nNúmero de BI:%s", p.bi);
			}else if(escolha == 2){
				printf("\nNIF: %.lf", p.nif);	
			}
			
			printf("\nTelefone: %d", p.telefone);
			
			printf("\nID: %d", p.codigo);
			
			continuar();
       		break;
    	}	
    
		case 2:{
			system("cls");
			listar_cliente();
			continuar();
			break;
		}
				
		case 3:{
			system("cls");
			int codigo, tipo_conta, espaco;
			;printf("==================== Abertura de Conta ==================\n\n");
			printf("Digite o código ID do cliente: ");
			scanf("%d", &codigo);
			fflush(stdin);
			
			espaco = identificar_cliente(codigo);
			if (espaco != -1){
				printf("Que tipo de conta deseja abrir?\n");
				printf("0-Conta Particular\n1-Conta Empresarial\n");
				printf("Conta: ");
				scanf("%d", &tipo_conta);
				fflush(stdin);
			
			system("cls");
			printf("==================== Abertura de Conta ==================\n\n");
			resposta = abertura_de_conta(clientes[espaco], tipo_conta);
			}else{
				resposta = false;
			}
			
			if (resposta == true){
				printf("\nOperação realizada com sucesso");
			}else{
				printf("\nTente novamente.");
			}
			continuar();
			sleep(1);
			break;
		}
		case 4:{
			system("cls");
			int num_conta;
			double valor;
			printf("======================Depositar Dinheiro========================\n\n");
			
			printf("Insira o numero da conta: ");
			scanf("%d", &num_conta);
			fflush(stdin);
			
			printf("Digite o valor a depositar: ");
			scanf("%lf", &valor);
			fflush(stdin);
			
			resposta = depositar_dinheiro(num_conta, valor);
			
			if (resposta == true){
				printf("\nDepositou:     AKZ %.2lf", valor);
			}else{
				printf("\nPor favor, tente novamente");
			}
			
			continuar();
			sleep(1);
			break;
		}
		case 5: {
			system("cls");
			int codigo;
			double valor;
			printf("======================Levantar Dinheiro========================\n");
			
			printf("Insira o número da conta: ");
			scanf("%d", &codigo);
			fflush(stdin);
			
			printf("Digite o valor a levantar: ");
			scanf("%lf", &valor);
			fflush(stdin);
			
			resposta = levantar_dinheiro(codigo, valor);
			
			if (resposta == true){
				printf("\nLevantou:     AKZ %.2lf", valor);
			}else{
				printf("\nPor favor, tente novamente");
			}
			
			continuar();
			sleep(1);
			
			break;
		}
			
		case 6: {
			system("cls");
			int codigo;
			double retorno;
			
			printf("======================Consultar Saldo========================\n");
			
			printf("Insira o número da conta: ");
			scanf("%d", &codigo);
			
			retorno = consultar_saldo(codigo);
			
			if (retorno != -1){
				printf("\nNúmero da conta: %d", codigo);
				printf("\nSALDO: %.2f", retorno);
				resposta = true;
			}else if(retorno = -1){
				printf("\nPor favor, tente novamente!");
				resposta = false;							
			}
			
			continuar();
			sleep(1);
			
			break;
		}
		
		
		case 7:{
			system("cls");
			printf("=================Transferir Dinheiro=================\n");
			int codigoEnviar, codigoReceber;
			double valor;
			
			printf("Insira o número da conta de envio: ");
			scanf("%d", &codigoEnviar);
			fflush(stdin);
			
			printf("Insira o número de conta a receber: ");
			scanf("%d", &codigoReceber);
			fflush(stdin);
			
			printf("Insira o valor a transferir: ");
			scanf("%lf", &valor);
			fflush(stdin);
			
			resposta=transferir_dinheiro(codigoEnviar, codigoReceber, valor);
			
			if (resposta == true){
				printf("Transferiu AKZ %.2lf da conta nº %d para a conta nº %d", valor, codigoEnviar, codigoReceber);
			}else{
				printf("\nTente novamente!");
			}	
			
			continuar();
			sleep(1);
			break;
		}
		
		case 8:{
			system("cls");
			printf("===================Atualização de Dados=================\n");
			int codigo;
			PESSOA p;
			
			printf("Digite o código ID do cliente: ");
			scanf("%d", &codigo);
			fflush(stdin);
			
			resposta = atualizar_dados(p, codigo);
			
				break;
			}
			
		case 9:	{
			system("cls");
			printf("==== PROGRAMA TERMINADO ====");
			break;
		}
	
		default: {
				printf("\nOpção inválida");
			break;
		}
	}
	
	if (resposta == true){
		copia_de_seguranca();
	}
		
}while(opcao !=9);


return 0;
}
