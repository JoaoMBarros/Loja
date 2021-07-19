//Dupla: Ingrid Miranda dos Santos e João Pedro Monteiro

#include <stdio.h>
#include <string.h>
#define MAX 100

typedef struct{
	int tetoBonus;
	float valorBonus;
	float valorBonificar;
} eBonus, tBonus;
typedef struct {
	float ultimasemBonus;
	float valorUltima;
	int compraCancelada;
	tBonus bonus;
} tCompra;
typedef struct {
	char nome[50];
	char CPF[12];
	char telefone[15];
	char email[30];
	int bonus;
	int antBonus;
	float totalCompras;
	tCompra ultimaCompra;
} tCliente;

int procuraCPF(tCliente vetCliente[], char CPF[12], int quant) { //procura pelo cliente do cpf informado
	int i;
	int achou = -1; //variavel que serve de indicador se o cpf foi ou nao encontrado
	for (i = 0; i < quant; i++){
		if (strcmp(CPF, vetCliente[i].CPF) == 0) {
			achou = i; //variavel assume a posicao do vetor em que o cpf foi encontrado
			break;
		} 
	}
	return achou; //retorna -1 caso o cpf nao tenha sido encontrado e != -1 caso tenha encontrado
}
void listaCompra(tCliente vetCliente[], int i){ //funcao que imprime dados do cliente (ta aqui porque eu chamo ela em outras funcoes)

	printf("\nNome: %s", vetCliente[i].nome); //o valor i indica qual cliente vai ter as informacoes impressas
	printf("CPF: %s\n", vetCliente[i].CPF);
	printf("Telefone: %s\n", vetCliente[i].telefone);
	printf("Email: %s\n", vetCliente[i].email);
	printf("Bonus: %d\n", vetCliente[i].bonus);
	printf("Total gasto em compras: %.2f\n", vetCliente[i].totalCompras);

}
void efetivarCompra(tCliente vetCliente[], eBonus *bonusGeral, int pos) { //funcao que efetua a compra
	int aux;
	int i;
	float compra;
	float comprasemBonus;
	float pagamentoCliente;
	float adicao;
	float troco;
	char CPF[12];
	int bonusProv = 0;
	int bonusProv2;

	printf("Qual o CPF do cliente? ");
	scanf("%s", CPF);
	aux = procuraCPF(vetCliente, CPF, pos); //aux recebe a posicao do cliente cadastrado que retorna da funcao de procurar cpf
	if (aux == -1) {
		printf("ERRO: CPF nao cadastrado.\n"); //se o valor retornado for -1 o cliente do cpf informado nao foi encontrado
		return;
	} else {
		printf("BONUS = %d.\n", vetCliente[aux].bonus); //informa quanto de bonus o cliente tem
		printf("VALOR CORRESPONDENTE = R$%.2f.\n", (vetCliente[aux].bonus) * (bonusGeral->valorBonus)); //informa o valor referente a quantidade de bonus que o cliente tem
	}
    printf("Qual o valor da compra? R$");
	do{
		scanf("%f", &compra); //variavel compra recebe o valor de compra informada
		if (compra < 0)
			printf ("Erro: valor negativo. Digite novamente: R$ ");
	} while (compra < 0); //do while que obriga a digitar um valor positivo para o valor de compra
	comprasemBonus = compra;
	if (vetCliente[aux].bonus > 0) { //se o cliente tiver pelo menos 1 bonus, pergunta se ele quer utilizar
    	printf("Deseja utilizar seu bonus?\n1-Sim\n2-Nao\n");
		printf("Opcao desejada: ");
    	scanf("%d", &i);
		fflush(stdin);
   		if (i == 1) { //se ele escolher um valor diferente de 1 nao entra nesse if
				if ((vetCliente[aux].bonus*bonusGeral->valorBonus) >= compra){ //se o valor do bonus do cliente for maior que a compra que ele quer fazer
					bonusProv = (compra/bonusGeral->valorBonus); // expressao pra calcular quanto de bonus vai sobrar
					compra = 0; // compra é zerada pelo fato de se ter mais em bonus do que o valor da compra
				}
				else { // se o valor da compra for superior ao bonus
					compra = compra - ((vetCliente[aux].bonus)*(bonusGeral->valorBonus)); // calcula quanto o cliente ainda vai ter que pagar
					bonusProv = vetCliente[aux].bonus; // bonus é totalmente utilizado e, por isso, zerado
				}
			bonusProv2 = compra / bonusGeral->valorBonificar;
			printf("BONUS A SER GANH0 = %d\n", bonusProv2);
			printf("VALOR DA COMPRA ATUALIZADO = R$%.2f\n", compra);
		}
	}
	printf("Qual o valor do pagamento do cliente? R$");
	do {
		scanf("%f", &pagamentoCliente);
		if (pagamentoCliente < 0)
			printf("Erro: valor negativo. Digite novamente:\nR$");
	} while (pagamentoCliente < 0); //obriga o cliente a digitar um pagamento maior que 0
	if (pagamentoCliente < compra) { //se o pagamento do cliente for inferior ao valor da compra
		do {
			printf("Erro: Valor do pagamento inferior ao valor da compra. ");
			printf("Deseja fornecer mais dinheiro?\n1-Sim\n2-Nao\n");
            printf("Opcao desejada: ");
			scanf("%d", &i);
			fflush(stdin);
			if (i != 1) { //se o cliente resolver nao adicionar mais dinheiro
				printf("COMPRA NAO EFETIVADA. Valor devolvido ao cliente: R$ %.2f.\n", pagamentoCliente);
				return;
			} else{
				printf("Valor a ser adicionado\nR$");
				scanf("%f", &adicao); //valor que cliente deseja adicionar ao pagamento
				pagamentoCliente += adicao; //valor digitado pelo cliente é adicionado ao valor do pagamento
			}
		}while (pagamentoCliente < compra); //repete ate o valor do pagamento do cliente ser maior ou igual o valor da compra
	} if(pagamentoCliente == compra || pagamentoCliente > compra){
		troco = pagamentoCliente - compra; //calcula o troco do cliente 
		vetCliente[aux].antBonus = vetCliente[aux].bonus;
		vetCliente[aux].bonus -= bonusProv; //retira o bonus usado na compra
		vetCliente[aux].ultimaCompra.valorUltima = compra; //atualiza o valor da ultima compra do cliente (depois de usar o bonus)
		vetCliente[aux].ultimaCompra.ultimasemBonus = comprasemBonus; //atualiza o valor dau ultima compra do cliente (antes de usar o bonus)
		vetCliente[aux].totalCompras += compra; //adiciona a compra ao valor total de compras do cliente
		if (bonusGeral->tetoBonus > vetCliente[aux].bonus){
		vetCliente[aux].bonus += compra/bonusGeral->valorBonificar; //calcula quanto de bonus o cliente deve ganhar
			if (vetCliente[aux].bonus > bonusGeral->tetoBonus){ 
				vetCliente[aux].bonus = bonusGeral->tetoBonus; //bonus assume o valor maximo (valor do teto)
			}
		}
		vetCliente[aux].ultimaCompra.compraCancelada = 0; //ultima compra recebe o indicador de nao estar cancelada
		printf("COMPRA EFETIVADA COM SUCESSO!\n");
		printf("TROCO = R$ %.2f\n", troco);
	}
}
void cancelarCompra(tCliente vetCliente[], eBonus *bonusGeral, int pos) { //funcao que cancela uma compra 
	char CPF[12];
	int aux;
	int i;
	int j;
	int k=0;
	int bonusCancelado = 0;
	int bonusCanceladoUlt = 0;
	int bonusCanceladoOther = 0;
	int bonusUsado;
	int novoBonusCancelado=0;
	float valorCancelar;

	printf("Qual o CPF do cliente? ");
	scanf("%s", CPF);
	aux = procuraCPF(vetCliente, CPF, pos);
	if (aux == -1){
		printf("Erro: CPF nao cadastrado.\n");
		return;
	} else {
		bonusCancelado = vetCliente[aux].ultimaCompra.valorUltima / vetCliente[aux].ultimaCompra.bonus.valorBonificar;
		if(bonusCancelado > vetCliente[aux].ultimaCompra.bonus.tetoBonus)
		bonusCancelado = vetCliente[aux].ultimaCompra.bonus.tetoBonus - vetCliente[aux].antBonus;
		do {
			printf("Qual compra deseja cancelar?\n");
			printf("1 - Ultima compra\n2 - Outra compra\n");
			printf("Opcao desejada: ");
			scanf("%d", &i);
			fflush(stdin);
				if(i != 1 && i != 2)
					printf("Opcao invalida. Tente novamente: ");
		} while(i != 1 && i != 2); //do while que força a escolha de uma das opcoes do menu
		switch(i) {
			case 1: //caso o usuario escolha cancelar a ultima compra feita
				if (vetCliente[aux].ultimaCompra.compraCancelada) { //se a compra ja estiver cancelada
					printf("ERRO: Ultima compra ja cancelada\n");
					return; //volta ao menu 
				} else {
					bonusUsado = (vetCliente[aux].ultimaCompra.ultimasemBonus - vetCliente[aux].ultimaCompra.valorUltima) / vetCliente[aux].ultimaCompra.bonus.valorBonus;
					bonusCanceladoUlt = bonusCancelado;
					printf("Valor da compra: R$%.2f\n", vetCliente[aux].ultimaCompra.ultimasemBonus);
					printf("Valor pago: %.2f\n", vetCliente[aux].ultimaCompra.valorUltima);
					printf("Bonus usados: %d\n", bonusUsado);
					printf("Bonus ganhos: %d\n", bonusCanceladoUlt);
					printf("Tem certeza que deseja cancelar essa compra?\n1-Sim\n2-Nao\n");
					printf("Opcao desejada: ");
					scanf("%d", &j);
					fflush(stdin);
					if (j == 1) {
						vetCliente[aux].bonus = vetCliente[aux].antBonus; 
						vetCliente[aux].totalCompras -= vetCliente[aux].ultimaCompra.valorUltima;
						vetCliente[aux].ultimaCompra.compraCancelada = 1;
						printf("COMPRA CANCELADA!\n");
					} else {
						printf("COMPRA NAO CANCELADA!\n");
						return;
					}
				}
				break;
			case 2: // caso o cliente queira cancelar outra compra 
				do{
				printf("Qual o valor pago da compra a ser cancelada?\n");
				scanf("%f", &valorCancelar);
					if(valorCancelar > vetCliente[aux].totalCompras) 
						printf("Valor informado excede o valor total de compras do cliente. Tente novamente\n");
				}while(valorCancelar > vetCliente[aux].totalCompras);
				bonusCanceladoOther = valorCancelar / bonusGeral->valorBonificar;
				printf("Valor da compra: R$%.2f\n", valorCancelar);
				if (bonusCanceladoOther > vetCliente[aux].bonus){
					printf("Valor informado excede o bonus do cliente.\n");
					printf("Valor em bonus informado: %d\n", bonusCanceladoOther);
					printf("Bonus do cliente: %d\n", vetCliente[aux].bonus);
				    bonusCanceladoOther = vetCliente[aux].bonus;
				}
				printf("Bonus a ser retirado do cliente: %d\n", bonusCanceladoOther);
				printf("Vendedor, deseja informar um valor menor de bonus?\n1-Sim\n2-Nao\n");
				printf("Opcao desejada: ");
				scanf("%d", &k);
			    if (k==1){
					do{
                       printf("Digite o valor do bonus a ser retirado do cliente (menor ou igual a %d): ", bonusCanceladoOther);
					   scanf("%d", &novoBonusCancelado);
					}while (novoBonusCancelado>bonusCanceladoOther);
				}
				printf("Tem certeza que quer cancelar essa compra?\n1-Sim\n2-Nao\n");
				printf("Opcao desejada: ");
				scanf("%d", &i);
				fflush(stdin);
				if (i==1) {
					if (novoBonusCancelado>vetCliente[aux].bonus||bonusCanceladoOther>vetCliente[aux].bonus){
						vetCliente[aux].bonus=0;
						vetCliente[aux].antBonus = 0;
						if (vetCliente[aux].totalCompras<valorCancelar){
							vetCliente[aux].totalCompras=0;
							printf("COMPRA CANCELADA!\n");  
						} else {
							vetCliente[aux].totalCompras -= valorCancelar; // diminui do total de compras o valor que foi cancelado
					        printf("COMPRA CANCELADA!\n");  
						}
						return;
					}
					if (k==1){
						vetCliente[aux].bonus -= novoBonusCancelado; //diminui do total de bonus o novo bonus que foi informado
						vetCliente[aux].antBonus = vetCliente[aux].bonus - bonusCancelado;
					    vetCliente[aux].totalCompras -= valorCancelar;
						printf("COMPRA CANCELADA!\n");
					} else {
					    vetCliente[aux].bonus -= bonusCanceladoOther; //diminui do total de bonus o bonus que foi cancelado
						vetCliente[aux].antBonus = vetCliente[aux].bonus - bonusCancelado;
					    vetCliente[aux].totalCompras -= valorCancelar; // diminui do total de compras o valor que foi cancelado
					    printf("COMPRA CANCELADA!\n");
					}	
				} else {
					printf("COMPRA NAO CANCELADA!\n");
					return;
				}
				break;
		}
	}
}
void cadastro(tCliente vetCliente[], int *pos, eBonus *bonusGeral) { //cadastro de um cliente
	int j = *pos; //guarda a posicao atual do ponteiro

	printf("Cliente %d\n", j+1);
	printf("Qual o nome do cliente? ");
	fgets(vetCliente[j].nome, 50, stdin); //le a string
	fflush(stdin);
	printf("Qual o CPF? ");
	scanf("%s", vetCliente[j].CPF);
	if (procuraCPF(vetCliente, vetCliente[j].CPF, j) != -1) {
		printf("\nERRO: CPF ja cadastrado.\n");
		return;
	} //se o CPF for encontrado a funcao para de rodar
	printf("Qual o telefone? ");
	scanf("%s", vetCliente[j].telefone);
	printf("Qual o email? ");
	scanf("%s", vetCliente[j].email);

	vetCliente[j].ultimaCompra.compraCancelada = 2;
	vetCliente[j].totalCompras = 0;
	vetCliente[j].ultimaCompra.valorUltima = 0;
	vetCliente[j].bonus = 0;
	vetCliente[j].ultimaCompra.bonus.tetoBonus = bonusGeral->tetoBonus;
	vetCliente[j].ultimaCompra.bonus.valorBonificar = bonusGeral->valorBonificar;
	vetCliente[j].ultimaCompra.bonus.valorBonus = bonusGeral->valorBonus;

	printf("CLIENTE CADASTRADO COM SUCESSO!\n");

	*pos = j+1; //avanÃ§a o vetor para a proxima posicao a ser cadastrada
}
void alterar(tCliente vetCliente[], int quant) { //altera os dados de um determinado cliente
	int aux;
	int aux2; //auxiliar que vai guardar a posicao dos cpf encontrados durante a funcao
	int escolha; //variavel opcao menu
	char CPF[12]; //cpf a ser procurado e cpf a ser substituido

	printf("Qual o CPF do cliente? ");
	scanf("%s", CPF);
	aux = procuraCPF (vetCliente, CPF, quant); //aux assume valor de retorno da funcao

	if (aux == -1) {
		printf("ERRO. CPF nao cadastrado.\n");
		return;
	} else {
		do {
		printf("1 - Alterar CPF\n");
		printf("2 - Alterar nome\n");
		printf("3 - Alterar telefone\n");
		printf("4 - Alterar email\n");
		printf("0 - Voltar ao menu principal\n");
		printf("Opcao desejada: ");
		scanf("%d", &escolha);
		fflush(stdin);
		switch(escolha) {
			case 1:
				printf("Qual o novo CPF? ");
				scanf("%s", CPF);
				aux2 = procuraCPF(vetCliente, CPF, quant); //auxiliar recebe novo valor referente a posica do cliente cujo cpf foi encontrado
				if (aux2 != -1) {
					printf("ERRO. CPF ja cadastrado. ");
					break;
				} else {
					strcpy(vetCliente[aux].CPF, CPF); //funcao pra copiar o novo cpf digitado no lugar do cpf encontrado
					printf("CPF atualizado com sucesso!\n");
				}
				break;
			case 2:
				printf("Qual o novo nome? ");
				fgets(vetCliente[aux].nome, 50, stdin);
				fflush(stdin); 
				printf("Nome atualizado com sucesso!\n");
				break;
			case 3:
				printf("Qual o novo telefone? ");
				scanf("%s", vetCliente[aux].telefone);
				printf("Telefone atualizado com sucesso!\n");
				break;
			case 4:
				printf("Qual o novo email? ");
				scanf("%s", vetCliente[aux].email);
				printf("Email atualizado com sucesso!\n");
				break;
			case 0:
				break;
			default:
				printf("Tente novamente\n");
				break;
			}
		} while (escolha != 0);
	}
}
void consultar(tCliente vetCliente[], int pos){ //acessa os dados de um cliente e imprime as informaçoes 
	int aux;
	char CPF[12];
	printf("Qual o CPF do cliente? ");
	scanf("%s", CPF);
	fflush(stdin);
	aux = procuraCPF(vetCliente, CPF, pos);
	if (aux != -1){
		listaCompra(vetCliente, aux);
		if (vetCliente[aux].ultimaCompra.compraCancelada==1 || vetCliente[aux].ultimaCompra.compraCancelada==0){
        	printf("Dados da ultima compra:\n");
            printf("Valor realmente pago: %.2f\n", vetCliente[aux].ultimaCompra.valorUltima);
            printf("Valor total da compra: %.2f\n", vetCliente[aux].ultimaCompra.ultimasemBonus);
			if (vetCliente[aux].ultimaCompra.compraCancelada == 1)
		    printf("Ultima compra cancelada: Sim\n");
			else if(vetCliente[aux].ultimaCompra.compraCancelada == 0)
			printf("Ultima compra cancelada: Nao\n");
            return;
        }else{
			printf("Cliente nao realizou nenhuma compra.\n");
		}
        return;
	} else {
		printf("CPF nao encontrado\n");
		return;
	}
}
void remover(tCliente vetCliente[], int *pos){ //remove os dados de um cliente
	int i = *pos;
	int escolha;
	int aux; // assume a posiçao do cliente
	char CPF[12];
	printf("Qual o CPF do cliente? ");
	scanf("%s", CPF);
	aux = procuraCPF(vetCliente, CPF, i);
	if (aux != -1) {
		listaCompra(vetCliente, aux);
		printf("Tem certeza que deseja remover esse cliente?\n1-Sim\n2-Nao\n");
		printf("Opcao desejada: ");
		scanf("%d", &escolha);
		fflush(stdin);
		if (escolha == 1) {
			strcpy(vetCliente[aux].nome, vetCliente[aux+1].nome);
			strcpy(vetCliente[aux].CPF, vetCliente[aux+1].CPF);
			strcpy(vetCliente[aux].telefone, vetCliente[aux+1].telefone);
			strcpy(vetCliente[aux].email, vetCliente[aux+1].email);
			vetCliente[aux].bonus = vetCliente[aux+1].bonus;
			vetCliente[aux].totalCompras = vetCliente[aux+1].totalCompras;
			vetCliente[aux].ultimaCompra.valorUltima = vetCliente[aux+1].ultimaCompra.valorUltima;
			vetCliente[aux].ultimaCompra.compraCancelada = vetCliente[aux+1].ultimaCompra.compraCancelada;
			vetCliente[aux].ultimaCompra.bonus.tetoBonus = vetCliente[aux+1].ultimaCompra.bonus.tetoBonus;
			vetCliente[aux].ultimaCompra.bonus.valorBonus = vetCliente[aux+1].ultimaCompra.bonus.valorBonus;
			*pos -= 1; // diminui uma posicao no total se o cliente for excluido
			printf("CADASTRO DO CLIENTE EXCLUIDO!\n");
		} else {
			printf("CADASTRO DO CLIENTE NAO EXCLUIDO!\n");
			return;
		}
	} else{
		printf("CPF nao cadastrado!\n");
	}
}
void exibirBonus(tCliente vetCliente[], int pos, eBonus *bonusGeral){ //exibe o bonus de um determinado cliente
	int aux;
	char CPF[12];
	printf("Qual o CPF do cliente?\n");
	scanf("%s", CPF);
	aux = procuraCPF(vetCliente, CPF, pos);
	if (aux == -1){
		printf("Erro: CPF nao cadastrado.\n");
		return;
	} else {
		printf("BONUS = %d\n", vetCliente[aux].bonus);
		printf("VALOR CORRESPONDENTE = %.2f\n", (vetCliente[aux].bonus) * (bonusGeral->valorBonus));
	}
}
void configurarBonus(eBonus *bonusGeral) { //configura os valores de bonus (teto, valor de cada bonus e valor para ganhar 1 unidade de bonus)
	int escolha;
	int novoTeto;
	float novoBonus;
	float novoBonificar;
	do {
		printf("1 - Alterar teto\n");
		printf("2 - Alterar valor do bonus\n");
		printf("3 - Alterar valor para receber bonus\n");
		printf("0 - Sair\n");
		printf("Opcao desejada: ");
		scanf("%d", &escolha);
		fflush(stdin);
		switch(escolha){
			case 1:
				printf("Qual o novo valor do teto? ");
				do {
					scanf("%d", &novoTeto);
					if (novoTeto < 0)
						printf("Erro: valor negativo. Digite novamente: ");
				} while(novoTeto < 0);
				bonusGeral->tetoBonus = novoTeto;
				printf("Alterado com sucesso!\n");
				break;
			case 2:
				printf("Qual o novo valor de 1 bonus? ");
				do {
					scanf("%f", &novoBonus);
					if (novoBonus < 0)
						printf("Erro: valor negativo. Digite novamente: ");						
				} while(novoBonus<0);
				bonusGeral->valorBonus = novoBonus;
				printf("Alterado com sucesso!\n");
				break;
			case 3:
				printf("Qual o novo valor para receber bonus? ");
				do{
					scanf("%f", &novoBonificar);
					if (novoBonificar < 0)
						printf("Erro: valor negativo. Digite novamente: ");
				} while(novoBonificar < 0);
				bonusGeral->valorBonificar = novoBonificar;
				printf("Alterado com sucesso!\n");
				break;
			case 0:
				break;
			default:
				printf("\nTente novamente: ");
		}
	} while(escolha != 0);
}
void listarComprasInferiores(tCliente vetCliente[], int quant, float valor){ //lista compras com valores inferiores a informada
	int i;
	for(i = 0; i < quant; i++){
		if(vetCliente[i].totalCompras < valor){
			listaCompra(vetCliente, i);
		} 
	}
}
void listarComprasIguais(tCliente vetCliente[], int quant, float valor){ //lista compras com valores iguais a informada
	int i;
	for (i = 0; i < quant; i++){
		if (vetCliente[i].totalCompras == valor){
			listaCompra(vetCliente, i);
		} 
	}
}
void listarComprasSuperiores(tCliente vetCliente[], int quant, float valor){ //lista compras com valores superiores a informada
	int i;
	for(i = 0;i < quant; i++){
		if (vetCliente[i].totalCompras > valor){
			listaCompra(vetCliente, i);
		}
	}
}
void listarCompras(tCliente vetCliente[], int quant){ //menu para escolher se quer listar compras inferiores, superiores ou iguais a informada
	int escolha;
	float valor;
	do {
		printf("Qual valor desejado? R$");
		scanf("%f", &valor);
			if (valor < 0)
				printf ("Tente novamente\n");
	} while(valor < 0);
	do {
		printf("1 - Valor total superior ao informado\n");
		printf("2 - Valor total igual ao informado\n");
		printf("3 - Valor total inferior ao informado\n");
		printf("0 - Voltar\n");
		printf("Opcao desejada: ");
		scanf("%d", &escolha);
		fflush(stdin);
		switch(escolha){
			case 1:
				listarComprasSuperiores(vetCliente, quant, valor);
				break;
			case 2:
				listarComprasIguais(vetCliente, quant, valor);
				break;
			case 3:
				listarComprasInferiores(vetCliente, quant, valor);
				break;
			case 0:
				return;
			default:
				printf("Tente novamente ");
		}
	} while(escolha != 0);
}
void listarBonus(tCliente vetCliente[], int quant, eBonus *bonusGeral){ //lista o bonus de todos os clientes cadastrados
	int i;
	for(i=0; i<quant;i++){
		printf("Cliente %s\n", vetCliente[i].nome);
		printf("Bonus: %d\n", vetCliente[i].bonus);
		printf("Valor correspondente: %.2f\n", vetCliente[i].bonus*bonusGeral->valorBonus);
	}
}
void listarClientes(tCliente vetCliente[], int quant){ //lista todos os clientes cadastrados
	int i;
	for(i = 0; i < quant; i++){
		listaCompra(vetCliente, i);
	}
}
void menuRelatorios(tCliente vetCliente[], int pos, eBonus *bonusGeral){ //menu de relatorios
	int escolha;	
	do{
		printf("MENU RELATORIOS\n");
		printf("1 - Listar clientes\n");
		printf("2 - Listar bonus\n");
		printf("3 - Listar cliente por compras\n");
		printf("0 - Sair\n");
		printf("Opcao desejada: ");
		scanf("%d", &escolha);
		fflush(stdin);
		switch(escolha) {
			case 1:
				listarClientes(vetCliente, pos);
				break;
			case 2:
				listarBonus(vetCliente, pos, bonusGeral);
				break;
			case 3:
				listarCompras(vetCliente, pos);
				break;
			case 0:
				return;
			default:
				printf("Tente novamente: ");
		}
	} while(escolha!=0);
}
void menuBonus(tCliente vetCliente[], int pos, eBonus *bonusGeral) { //menu de bonus
	int escolha;
	do {
		printf("MENU BONUS\n");
		printf("1 - Configurar\n");
		printf("2 - Exibir\n");
		printf("0 - Sair\n");
		printf("Opcao desejada: ");
		scanf("%d", &escolha);
		fflush(stdin);
		switch(escolha){
			case 1:
				configurarBonus(bonusGeral);
				break;
			case 2:
				exibirBonus(vetCliente, pos, bonusGeral);
				break;
			case 0:
				break;
			default:
				printf("Tente novamente ");
		}
	} while(escolha!=0);
}
void menuCompra(tCliente vetCliente[], int pos, eBonus *bonusGeral){ //menu de compras
	int escolha;
	do {
		printf("MENU COMPRA\n");
		printf("1 - Efetivar\n");
		printf("2 - Cancelar\n");
		printf("0 - Sair\n");
		printf("Opcao desejada: ");
		scanf("%d", &escolha);
		fflush(stdin);
		switch(escolha){
			case 1:
				efetivarCompra(vetCliente, bonusGeral, pos);
				break;
			case 2:
				cancelarCompra(vetCliente, bonusGeral, pos);
				break;
			case 0:
				break;
			default:
				printf("Tente novamente ");
		}
	} while (escolha != 0);
}
void menuCliente(tCliente vetCliente[], int *i, eBonus *bonusGeral) { //menu de cliente
	int pos; //guarda a posicao atual do ponteiro
	int escolha; //variavel da escolha do menu de cliente	
	do {
		pos = *i;
		printf("MENU CLIENTE:\n");
		printf("1 - Cadastrar\n");
		printf("2 - Alterar\n");
		printf("3 - Consultar\n");
		printf("4 - Remover\n");
		printf("0 - Sair\n");
		printf("Opcao desejada: ");
		scanf("%d", &escolha);
		fflush(stdin);
		switch(escolha) {
			case 1:
				cadastro(vetCliente, &*i, bonusGeral);
				break;
			case 2:
				alterar(vetCliente, pos);
				break;
			case 3:
				consultar(vetCliente, pos);
				break;
			case 4:
				remover(vetCliente, &*i);
				break;
			case 0:
				break;
			default :
				printf("Tente novamente\n");
				break;
		}
	} while (escolha != 0);
}
void menuPrincipal(tCliente vetCliente[], eBonus *bonusGeral) { //menu principal
	int pos=0; //posicao iniciar do vetor
	int escolha; //variavel da escolha do menu principal
	do {
		printf("MENU PRINCIPAL:\n");
		printf("1 - Cliente\n");
		printf("2 - Compra\n");
		printf("3 - Bonus\n");
		printf("4 - Relatorios\n");
		printf("0 - Sair\n");
		printf("Opcao desejada: ");
		scanf("%d", &escolha);
		fflush(stdin);
		switch(escolha) {
			case 1:
				menuCliente(vetCliente, &pos, bonusGeral);
				break;
			case 2:
				menuCompra(vetCliente, pos, bonusGeral);
				break;
			case 3:
				menuBonus(vetCliente, pos, bonusGeral);
				break;
			case 4:
				menuRelatorios(vetCliente, pos, bonusGeral);
				break;
			case 0:
				break;
			default:
				printf("Tente novamente\n");
				break;
		}
	}while (escolha != 0);
}
void bonusInicial(eBonus *bonusGeral) { //os bonus iniciais
	bonusGeral->tetoBonus = 100;
	bonusGeral->valorBonificar = 100.00;
	bonusGeral->valorBonus = 0.50;
}
int main() { //main...
	tCliente vetCliente[MAX];
 	eBonus *ptr;
    eBonus bonusGeral;
    ptr = &bonusGeral;
    bonusInicial(ptr);
    menuPrincipal(vetCliente, ptr);
	return 0;
}