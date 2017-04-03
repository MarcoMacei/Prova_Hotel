// Declarações da prova de Estrutura - N1 - 29/09/2016
#include <iostream>
#include <Windows.h>
using namespace std;
//
//Definições do sistema
#define PAUSA							system("pause")
#define LIMPAR_TELA						system("cls")
#define ACENTUACAO						setlocale(LC_ALL, "portuguese_brazil")
#define EXTEN_NOME						30					// extensão do nome do hóspede
//
// opções do menu
#define FAZER_CHECK_IN					'I'					// fazer o check-in do hóspede
#define FAZER_CHECK_OUT					'O'					// fazer o check-out do hóspede
#define LISTAR_UM_QUARTO				'L'					// listar os dados de um dado quarto
#define LANCAR_DIARIA					'D'					// lançar diárias nos quartos que estão ocupados
#define LANCAR_SERVICO					'S'					// lançar servico em um dado quarto
#define LISTAR_QUARTOS_OCUPADOS			'T'					// Listar todos os quartos ocupados
#define SAIR_DO_PROGRAMA				'X'					// sair do programa	
//
// Valores das diárias para cada quarto de acordo com o andar em que está
#define DIARIA_QUARTO_SIMPLES			165.00				// Diária do quarto simples
#define DIARIA_QUARTO_MEDIO				308.00				// Diária do quarto do tipo médio
#define DIARIA_QUARTO_SUPER				610.00				// Diária do quarto do tipo super
#define DIARIA_QUARTO_SUITE				990.00				// Diária do quarto do tipo suite
// Observações:
// 1 - Os quartos dos andares 1º ao 2º - são quartos do tipo simples
// 2 - Os quartos dos andares 3º ao 4º - são quartos do tipo médio
// 3 - Os quartos dos andares 5º ao 8º - são quartos do tipo super
// 4 - Os quartos dos andares 9º e acima - são quartos do tipo suite
//
// Limitações do programa
#define QUANTIDADE_ANDARES				20					// quantidade de andares do hotel
#define QUANTIDADE_QUARTOS				10					// quantidade de quartos por andar
//
// definições de tipos de serviços
#define RESTAURANTE						1					// transação de restaurante
#define LAVANDERIA						2					// transação de lavanderia
#define BAR								3					// transação de bar
#define PISCINA							4					// transação de bar da piscina
#define BOATE							5					// transação de bar da boate
//
//Protótipo de funções
bool	PedirAndarQuarto(int *ptrAndar, int *ptrQuarto, char *ptrTransacao);
// struct da data e hora
typedef struct tagDATA_HORA
{
	int nDia, nMes,								// data com dia/mes
		nHora, nMinuto;							// hora com minuto
} DATA_HORA;
//
// struct de uma despesa
typedef struct tagSERVICO
{
	int nCodigo;								// código do serviço conforme tabela 
	DATA_HORA stDtHora;							// data e hora do serviço
	double dVlrServico;							// valor do serviço prestado ao hóspede do quarto
	tagSERVICO *ptrProximo;						// ponteiro para o próximo serviço prestado ao quarto
} SERVICO;
//
// struct do quarto do hotel
typedef struct tagQUARTO
{
	int	nAndar,									// andar do quarto 1 a QUANTIDADE_ANDARES
		nQuarto;								// quarto dentro do andar 1 a QUANTIDADE_QUARTOS
	bool flgLivreOcupado;						// false - indica que o quarto está livre e true - ocupado
	DATA_HORA stHoraOcupacao;					// indicar a data e hora da ocupação do quarto
	char szNomeHospede[EXTEN_NOME + 1];			// nome do hóspede que está ocupando o quarto
	double dValorDiaria,						// valor da diária do quarto
		dVlrServicos,							// valor dos serviços sem as diárias
		dVlrTotalGeral;							// valor total de total dos serviços com diárias inclusive
	SERVICO *ptrPrimeiro,						// ponteiro para o primeiro serviço do quarto
		*ptrUltimo;								// ponteiro para o último serviço do quarto
} QUARTO;
//
// classe do hotel
//
class clHotel
{
	// Atributos e métodos private do sistema
private:
	QUARTO vetQuartos[QUANTIDADE_ANDARES * QUANTIDADE_QUARTOS],		// vetor de quartos
		stQuarto;						// para conter os dados de um quarto

	// Atributos e métodos públicos do sistema
public:
								
	// Método construtor da classe
	//	Parâmetros: 
	//		Entrada: void
	//	O que fazer:
	//		Inicializar o vetor de quartos indicando que não tem nenhum
	//			quarto ocupado (flgLivreOcupado), zerar o total de serviços e total geral.
	//			Para cada quarto colocar o seu andar e quarto dentro do andar e o valor
	//			 da diária corresponde ao quarto segundo a tabela. Zerar a lista ligada 
	//			 de serviços de cada quarto.
	clHotel(void)
	{
		int i = 0, j = 0;

		for(i = 0; i < QUANTIDADE_ANDARES * QUANTIDADE_QUARTOS; i++)
		{
			vetQuartos[i].flgLivreOcupado = false;
			vetQuartos[i].dVlrServicos = vetQuartos[i].dVlrTotalGeral = 0.0;
			vetQuartos[i].ptrUltimo = vetQuartos[i].ptrPrimeiro = NULL;

			//Inicializando os valores dos quartos
			if(i >= (QUANTIDADE_QUARTOS * 8))					//Do 9º a qtde de andares
				vetQuartos[i].dValorDiaria = DIARIA_QUARTO_SUITE;
			if(i < (QUANTIDADE_QUARTOS * 8))					//Do 5º ao 8º andar
				vetQuartos[i].dValorDiaria = DIARIA_QUARTO_SUPER;
			if(i < (QUANTIDADE_QUARTOS * 4))					//Do 3º ao 4º andar
				vetQuartos[i].dValorDiaria = DIARIA_QUARTO_MEDIO;
			if(i < (QUANTIDADE_QUARTOS * 2))					//Do 1º ao 2º andar
				vetQuartos[i].dValorDiaria = DIARIA_QUARTO_SIMPLES;

		}//Fim do for
		
		//Inicializando os números dos quartos e andares
		for (i = 1; i < QUANTIDADE_ANDARES + 1; i++)
		{
			for (j = 1; j < QUANTIDADE_QUARTOS + 1; j++)
			{
				vetQuartos[((i * j) - 1)].nAndar = i;
				vetQuartos[((i * j) - 1)].nQuarto = j;
			}
		}
	}
	// Método público que verifica se o quarto está livre ou ocupado
	//	Parâmetros:
	//		Entrada: int nAndar - andar do quarto entre 1 e QUANTIDADE_ANDARES
	//				 int nQuarto - quarto dentro do andar entre 1 e QUANTIDADE_QUARTOS
	//				 QUARTO *ptrStQuarto - ponteiro de um struct que conterá os dados
	//										do quarto se ele estiver ocupado
	//		Retorno: bool - true - indica que o quarto está ocupado e os seus dados 
	//								foram copiados na struct indica pelo invocador do
	//								método.
	//						false - indica que o quarto está livre
	//			O que fazer:
	//				Calcular o indice do vetor de quartos. Este indice é função do
	//				   andar, quantidade de quartos por andar e o quarto dentro do andar.
	//				Verificar se o quarto indexado está livre e se estiver retornar false.
	//				Se não estiver livre copiar o quarto indexado para a struct do 
	//					invocador do método e retornar true.
	bool VerificarQuartoLivre(int nAndar, int nQuarto, QUARTO *ptrStQuarto)
	{
		int indice = (nAndar - 1) * QUANTIDADE_QUARTOS + (nQuarto - 1);

		if (vetQuartos[indice].flgLivreOcupado)
		{
			memcpy_s(ptrStQuarto, sizeof(QUARTO), &vetQuartos[indice], sizeof(QUARTO));
			return true;			//Quarto ocupado
		}

		return false;			//Quarto desocupado
	}
	// Método público que faz o check_in do hóspede em um determinado andar e quarto
	//	Parâmetros:
	//		Entrada: int nAndar - andar do quarto entre 1 e QUANTIDADE_ANDARES
	//				 int nQuarto - quarto dentro do andar entre 1 e QUANTIDADE_QUARTOS
	//				 char *ptrNomeHospede - ponteiro de um string com o nome do hóspede
	//		Retorno: bool - true - o check_in foi realizado com sucesso
	//						false - o quarto já estava ocupado por outro hóspede
	//		O que fazer:
	//				 verificar se o quarto está livre e se não estiver devolver false.
	//				 Calcular o indice do vetor de quartos para tratá-lo indexado.
	//				 Se estiver livre indicar que está ocupado, copiar o nome do hóspede
	//					no quarto, indicar a data e hora da ocupação e zerar 
	//					total de serviços e total geral.
	bool CheckInHospede(int nAndar, int nQuarto, char *ptrNomeHospede)
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);

		int indice = (nAndar - 1) * QUANTIDADE_QUARTOS + (nQuarto - 1);

		if(vetQuartos[indice].flgLivreOcupado == false)  //quarto desocupado?
		{
			vetQuartos[indice].flgLivreOcupado = true;		//Passa a ser ocupado
			strcpy(vetQuartos[indice].szNomeHospede, ptrNomeHospede);
			vetQuartos[indice].dVlrServicos = 0.0;
			vetQuartos[indice].dVlrTotalGeral = 0.0;
			vetQuartos[indice].stHoraOcupacao.nMes = stTime.wMonth;
			vetQuartos[indice].stHoraOcupacao.nDia = stTime.wDay;
			vetQuartos[indice].stHoraOcupacao.nHora = stTime.wHour;
			vetQuartos[indice].stHoraOcupacao.nMinuto = stTime.wMinute;
			vetQuartos[indice].nAndar = nAndar;
			vetQuartos[indice].nQuarto = nQuarto;

			return true;			//Hóspede cadastrado
		}	

		return false;	//não cadastrado	
	}
	// Método público que faz o check-out do hóspede.
	//	Parâmetros:
	//		Entrada: int nAndar - andar do quarto entre 1 e QUANTIDADE_ANDARES
	//				 int nQuarto - quarto dentro do andar entre 1 e QUANTIDADE_QUARTOS
	//		Retorno: bool - true - o check-out foi realizado com sucesso
	//						false - o quarto não estava ocupado
	//		O que fazer:
	//				verificar se o quarto está ocupado e se não estiver retornar false.
	//				Calcular o indice do vetor de quartos para tratá-lo indexado.
	//				Se estiver ocupado simplesmente desocupar o quarto com um false.
	bool CheckOutHospede(int nAndar, int nQuarto)
	{
		int indice = (nAndar - 1) * QUANTIDADE_QUARTOS + (nQuarto - 1);

		if(vetQuartos[indice].flgLivreOcupado == true)			//Quarto ocupado?
		{
			vetQuartos[indice].flgLivreOcupado = false;		//Passa a ser desocupado
			return true;		//Check out realizado
		}
			
		return false;			//Erro no check out
	}
	// Método público que inclui um serviço em um dado quarto
	//	Parâmetros:
	//		Entrada: int nAndar - andar do quarto entre 1 e QUANTIDADE_ANDARES
	//				 int nQuarto - quarto dentro do andar entre 1 e QUANTIDADE_QUARTOS
	//				 int nCodServico - código do serviço prestado
	//				 double dVlrServico - valor do serviço prestado
	//		Retorno: bool - true - indica que foi tudo OK
	//						false - indica que não tem memória para incluir esse
	//								serviço na lista ligada de serviços
	//		O que fazer:
	//				 A verificação do quarto ocupado ou não deve ter sido feito
	//					anteriormente.
	//				 Buscar data e hora do sistema para complementar os dados do serviço
	//				 Calcular o indice do vetor de quartos para tratá-lo indexado.
	//				 Incluir este serviço na lista ligada que deve ser feita no
	//					último.
	//				 Somar este serviço nos totais de serviços e totais gerais do
	//					quarto.
	//					
	bool LancarServico(int nAndar, int nQuarto, int nCodServico, double dVlrServico)
	{
		SERVICO *ptrSalva;
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);

		int indice = (nAndar - 1) * QUANTIDADE_QUARTOS + (nQuarto - 1);

		if(vetQuartos[indice].ptrPrimeiro == NULL)			//Lista de serviços vazia?
			vetQuartos[indice].ptrUltimo = vetQuartos[indice].ptrPrimeiro = new SERVICO;//Alocando memória para o primeiro serviço
		else
		{
			//A lista já existe, alocar no último
			ptrSalva = vetQuartos[indice].ptrUltimo;			//Em caso de erro

			//Alocando memória para o próximo serviço, que será o último
			vetQuartos[indice].ptrUltimo = vetQuartos[indice].ptrUltimo->ptrProximo = new SERVICO;
		}

		//Depois de alocar, testar para ver se há memória
		if(vetQuartos[indice].ptrUltimo == NULL)
		{
			vetQuartos[indice].ptrUltimo = ptrSalva;			//Restaura a lista
			return false;					//Indica que houve erro
		}

		//Memória alocada, preencher o serviço
		vetQuartos[indice].ptrUltimo->dVlrServico = dVlrServico;
		vetQuartos[indice].dVlrServicos += dVlrServico;
		vetQuartos[indice].dVlrTotalGeral += dVlrServico;
		vetQuartos[indice].ptrUltimo->nCodigo = nCodServico;
		vetQuartos[indice].ptrUltimo->stDtHora.nMes = stTime.wMonth;
		vetQuartos[indice].ptrUltimo->stDtHora.nDia = stTime.wDay;
		vetQuartos[indice].ptrUltimo->stDtHora.nHora = stTime.wHour;
		vetQuartos[indice].ptrUltimo->stDtHora.nMinuto = stTime.wMinute;
		vetQuartos[indice].ptrUltimo->ptrProximo = NULL;			//Indica o último da lista
		return true;								// para não dar erro
	}
	// Método público que lista todos os quartos ocupados
	//	Parâmetros: nenhum
	//		O que fazer:
	//			Apagar a tela e emitir um cabeçalho indicando o tipo da listagem.
	//			Percorrer o vetor de quartos e se estiver ocupado exibir andar, quarto, nome do hóspede
	//				valor da diária, total de serviços e total geral.
	//			Percorrer a lista ligada exibindo os valores de cada serviço prestado 
	//				As linhas de impressão devem ser formatadas com sprintf_s.
	//
	//				Observe o layout abaixo: 
	//					Listagem dos quartos ocupados em dd/mm/aaaa às hh:mm (Cabeçalho da listagem - uma vez)
	//			(<<<<<As linhas abaixo repetem para cada quarto ocupado>>>>>>)
	//			Andar Quarto Hóspede                Dt.Ocupação Vlr.Diária Tot.Serviços Tot.Geral
	//			  AA	QQ	 HHHHHHHHHHHHHHHHHHHHHH	DD/MM HH:MM DDDDDDD.DD  SSSSSSS.SS  TTTTTT.TT
	//					 
	//											Serviço      Dt.Serviço   Vlr.Serviço
	//											NNNNNNNNNNNN DD/MM HH:MM  VSVSVSVS.VS
	//											NNNNNNNNNNNN DD/MM HH:MM  VSVSVSVS.VS
	//											NNNNNNNNNNNN DD/MM HH:MM  VSVSVSVS.VS .....quantos houver
	//
	//			(<<<<<<<na ultima linha da listagem apresentar o seguinte:>>>>>>>
	//													Total Geral: GGGGGGG.GG (somatória dos totais gerais)

	//			Onde:
	//			AA - andar
	//			QQ - quarto
	//			HHHHHH....H - Nome do hóspede
	//			DD/MM - dia e mes
	//			HH/MM - hora e minuto
	//			DDDDDD.DD - valor da diária
	//			SSSSSS.SS - total de serviços
	//			TTTTTT.TT - total geral de diárias e serviços
	//			NNNNNNNNN - nome do serviço
	//			VSVSVSVS.VS - valor do serviço
	//	
	void ListarTodosQuartos()
	{
		SERVICO *ptrWork;
		double somaTotal = 0;
		char cWork[300];
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		bool flagQuarto = true;					//True se nenhum quarto for listado

		char vetNomesServicos[5][15] = {"Restaurante",
									"Lavanderia",
									"Bar",
									"Piscina",
									"Boate"};

		LIMPAR_TELA;

		sprintf_s(cWork, sizeof(cWork), 
			"\t\tListagem dos quartos ocupados em %02d/%02d/%04d às %02d:%02d",
			stTime.wDay, stTime.wMonth, stTime.wYear, stTime.wHour, stTime.wMinute);

		cout << cWork << endl << endl;

		for(int i = 0; i < QUANTIDADE_ANDARES * QUANTIDADE_QUARTOS; i++)
		{
			if(vetQuartos[i].flgLivreOcupado)
			{
				//Quarto ocupado
				sprintf_s(cWork, sizeof(cWork), "Andar: %d - Quarto: %d - Hóspede: %s"
					"\nDt.Ocupação: %02d/%02d %02d:%02d - Vlr.Diária: %.2f - Tot.Serviços: %.2f - Tot. Geral: %.2f", 
					vetQuartos[i].nAndar, vetQuartos[i].nQuarto, vetQuartos[i].szNomeHospede,
					stTime.wDay, stTime.wMonth, stTime.wHour, stTime.wMinute,
					vetQuartos[i].dValorDiaria, vetQuartos[i].dVlrServicos,
					vetQuartos[i].dVlrTotalGeral);

				cout << endl << cWork << endl;

				//Soma do total geral
				somaTotal += vetQuartos[i].dVlrTotalGeral;
	
				//For para percorrer a lista de serviços
				for(ptrWork = vetQuartos[i].ptrPrimeiro; ptrWork; ptrWork = ptrWork->ptrProximo)
				{
					sprintf_s(cWork, sizeof(cWork), "Serviço: %s - Dt.Serviço: %02d/%02d %02d:%02d - Vlr.Serviço: %.2f", 
						vetNomesServicos[ptrWork->nCodigo - 1], ptrWork->stDtHora.nDia, ptrWork->stDtHora.nMes, ptrWork->stDtHora.nHora,
						ptrWork->stDtHora.nMinuto, ptrWork->dVlrServico);

					cout << endl << cWork;
				}//Fim do for dos serviços
				
				cout << endl;
				flagQuarto = false;			//Houve a listagem de pelo menos um quarto

			}//Fim do if
		}//Fim do for dos quartos

		//Exibindo a soma de todos os totais
		cout << endl << "A soma dos totais é: " << somaTotal << endl << endl;

		if(flagQuarto)				//Nenhum quarto listado
			cout << endl << "\t\tNenhum quarto ocupado!" << endl << endl;

		PAUSA;
	}

	// Método público que lança uma diária nos quartos ocupados
	//	Parâmetros: nenhum
	//		O que fazer:
	//			Percorrer o vetor de quartos e aquele que estiver ocupado somar o valor
	//				de sua diária no total de diárias e no total geral.
	void LancarDiarias()
	{
		bool flagQuarto = true;					//True se nenhum quarto for listado
		//Loop para percorrer o vetor
		for(int i = 0; i < QUANTIDADE_ANDARES * QUANTIDADE_QUARTOS; i++)
		{
			if(vetQuartos[i].flgLivreOcupado)			//Quarto ocupado
			{
				vetQuartos[i].dVlrTotalGeral += vetQuartos[i].dValorDiaria;
				flagQuarto = false;					//False se foi listado
			}
		}//Fim do for

		if(flagQuarto)				//Nenhum quarto listado
		{
			cout << endl << "\t\tNenhum quarto ocupado!" << endl << endl;
			PAUSA;
		}
		else
		{
			cout << endl << endl << "\t\tDiárias lançadas com sucesso!" << endl << endl;
			PAUSA;
		}
	}
};