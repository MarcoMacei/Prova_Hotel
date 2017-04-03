// Hotel.cpp - Programa de Hotel para prova de Estrutura de Dados - 29/09/2016
// FATEC - Mogi das Cruzes
#include "Hotel.h"
//
// Função principal
//
void main(void)
{
	int  i,									// indice genérico
		nQtdeAndares,						// para receber a quantidade de andares do hotel
		nQtdeQuartosPorAndar,				// para receber a quantidade de quartos por andar
		nAndarHotel,						// para receber o número do andar
		nOpcao,								// para a opção do serviço
		nQuartoHotel;						// para receber o número do quarto dentro do andar

	char cOpcaoOper,						// local para receber a opção escolhida do menu
		cNomeHospede[41],					// local para receber o nome do hóspede do hotel
		cWork[200],							// para formatação de sprintf
		cNomeAluno[] = "Marco Aurélio Macei Duarte";

	double dValor;							// local para receber o valor dos serviços do hotel

	QUARTO stQuarto;						// para conter os dados de um quarto
	
	SYSTEMTIME stTime;						// para buscar data e hora

	ACENTUACAO;					//Acentuação PT - BR

	// comandos para estabelecer o numero de casas decimais
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);						// duas casas decimais

	clHotel objHotel;						// foi criado o objeto
	
	while(true)								// loop infinito
	{
		LIMPAR_TELA;

		GetLocalTime(&stTime);				//Para pegar a hora do sistema

		sprintf_s(cWork, sizeof(cWork), 
			"\n\tFATEC-MC - Sistema de Hotel %02d/%02d/%04d às %02d:%02d:%02d\n\t\tOperador: %s Turma: noite",
			stTime.wDay, stTime.wMonth, stTime.wYear,
			stTime.wHour, stTime.wMinute, stTime.wSecond,
			cNomeAluno);
		cout << cWork << endl;

		// Mostrar o menu de opções para o operador
		cout << FAZER_CHECK_IN << " - Check-in de um hospede" << endl
			<< FAZER_CHECK_OUT << " - Check-out de um hospede" << endl
			<< LANCAR_SERVICO << " - Lançar serviço em um dado quarto" << endl
			<< LANCAR_DIARIA << " - Lancar diarias em todos os quartos ocupados" << endl
			<< LISTAR_UM_QUARTO << " - Listar um quarto" << endl
			<< LISTAR_QUARTOS_OCUPADOS << " - Listar todos os quartos ocupados" << endl
			<< SAIR_DO_PROGRAMA << " - Sair do programa" << endl
			<< "\tSelecione: ";
		cin	 >> cOpcaoOper;					// opção escolhida pelo operador

		cOpcaoOper = toupper(cOpcaoOper);	// converte para upper case a opção escolhida

		switch(cOpcaoOper)					// avaliar a opção que foi escolhida pelo operador
		{
			case FAZER_CHECK_IN:
				// 1 - Chamar a função PedirAndarQuarto passando os endereço de nAndarHotel para receber
				//		o número do andar e o endereço de nQuartoHotel para receber o número do quarto no
				//			andar e indicar que a transação é "Check-in de hóspede"
				if(!PedirAndarQuarto(&nAndarHotel, &nQuartoHotel, "Check-in de hóspede"))
				{
					break;					//Volta ao menu
				}
				
				// 2 - Se retornou true indicando que foram informados andar e quarto válidos
				//	   invocar o método que verifica se o quarto está ocupado (VerificarQuartoLivre)
				if(objHotel.VerificarQuartoLivre(nAndarHotel, nQuartoHotel, &stQuarto))
				{
					//Quarto ocupado
					cout << endl << "Quarto ocupado \nNome: " << stQuarto.szNomeHospede << endl << endl;
					PAUSA;
					break;
				}

				// 4 - Se não estiver ocupado Pedir o nome do hóspede considerando que tem nome e sobrenome
				// 5 - Invocar o método CheckInHospede
				cin.ignore(1, EOF);
				cout << "Informe o nome do Hóspede: ";
				cin.getline(stQuarto.szNomeHospede, EXTEN_NOME + 1, '\n');

				if(objHotel.CheckInHospede(nAndarHotel, nQuartoHotel, stQuarto.szNomeHospede))
				{
					cout << endl << endl << "\t\tHóspede cadastrado com sucesso!" << endl << endl;
					PAUSA;
					break;
				}
				break;						// para sair do switch e voltar ao menu

			case FAZER_CHECK_OUT:
				// 1 - Pedir o andar e o quarto da mesma forma que no item anterior indicando que a transação 
				//		é "Check-out"
				if(!PedirAndarQuarto(&nAndarHotel, &nQuartoHotel, "Check-out de hóspede"))
				{
					break;					//Volta ao menu
				}
				
				// 3 - Verificar se o quarto está ocupado e se estiver exibir o nome do hóspede e
				//		o total de diárias do quarto
				if(objHotel.VerificarQuartoLivre(nAndarHotel, nQuartoHotel, &stQuarto))
				{
					//Quarto ocupado
					cout << endl << "Quarto ocupado \nNome: " << stQuarto.szNomeHospede 
						<< "\nValor Total: " << stQuarto.dVlrTotalGeral << endl << endl;

					// 4 - Pedir a confirmação do operador com um S ou N
					//		Se for indicado 'S' ou 's' invocar o método CheckOutHospede
					cout << "Deseja realizar o checkOut? (S - sim ou N - não): ";
					cin >> cOpcaoOper;

					if(cOpcaoOper == 'S' || cOpcaoOper == 's')
					{
						if(objHotel.CheckOutHospede(nAndarHotel, nQuartoHotel))
						{
							cout << endl << endl << "\t\tCheck Out realizado com sucesso!" << endl << endl;
							PAUSA;
							break;
						}//Fim if do check out

						cout << endl << endl << "\t\tProblema no Check Out!" << endl << endl;
						PAUSA;
						break;
					}//Fim do if
				}//Fim do método

				cout << endl << "\t\tQuarto desocupado!" << endl << endl;
				PAUSA;
				break;						// para sair do switch e voltar ao menu

			case LANCAR_DIARIA:
				// <<<< 05 >>>> 
				// 1 - Pedir a data e hora do sistema e apresentar ao operador
				GetLocalTime(&stTime);
				sprintf_s(cWork, sizeof(cWork), "\t\tLançar Diárias %02d/%02d/%04d às %02d:%02d:%02d", 
					stTime.wDay, stTime.wMonth, stTime.wYear, stTime.wHour, stTime.wMinute, stTime.wSecond);
				cout << endl << cWork << endl;

				cout << endl << "\tDeseja realmente lançar as diárias? (S - sim ou N - não): ";
				cin >> cOpcaoOper;

				// 2 - Confirmar com o operador se ele realmente deseja lançar as diárias no horário
				//		apresentado
				// 3 - Se o operador confirmar invocar o método LancarDiarias
				if(cOpcaoOper == 's' || cOpcaoOper == 'S')
					//Chamar o método para lançar diárias
					objHotel.LancarDiarias();
				
				// 4 - Caso o operador não confirme voltar ao menu.
				break;						// para sair do switch e voltar ao menu

			case LISTAR_UM_QUARTO:
				// <<<< 06 >>>> 
				// 1 - Pedir Andar e Quarto conforme itens anteriores
				if(!PedirAndarQuarto(&nAndarHotel, &nQuartoHotel, "Listar um quarto"))
				{
					break;					//Volta ao menu
				}
				
				// 2 - Verificar se o quarto está ocupado passando o endereço da struct stQuarto
				if(objHotel.VerificarQuartoLivre(nAndarHotel, nQuartoHotel, &stQuarto))
				{
					//Quarto ocupado
					sprintf_s(cWork, sizeof(cWork), 
						"Andar: %d \nQuarto: %d \nHóspede: %s \nVlr.Diária: %.2f \nTot. Serviço %.2f \nTot. Geral: %.2f", 
						stQuarto.nAndar, stQuarto.nQuarto, stQuarto.szNomeHospede, 
						stQuarto.dValorDiaria, stQuarto.dVlrServicos, stQuarto.dVlrTotalGeral);

					cout << endl << cWork << endl;
					PAUSA;
					break;
				}

				//Quarto desocupado
				cout << endl << endl << "\t\tQuarto desocupado!" << endl << endl;
				PAUSA;
				break;

			case LANCAR_SERVICO:
				// <<<< 07 >>>> 
				// 1 - Pedir Andar e Quarto conforme itens anteriores
				if(!PedirAndarQuarto(&nAndarHotel, &nQuartoHotel, "Lançar Serviço"))
				{
					break;					//Volta ao menu
				}
			
				// 2 - Verificar se o quarto está ocupado passando o endereço da struct stQuarto
				if(!objHotel.VerificarQuartoLivre(nAndarHotel, nQuartoHotel, &stQuarto))
				{
					//Quarto desocupado
					cout << endl << endl << "\t\tQuarto desocupado!" << endl << endl;
					PAUSA;
					break;
				}

				//Quarto ocupado
				sprintf_s(cWork, sizeof(cWork), "Andar: %d \nQuarto: %d \nHóspede: %s \nVlr.Diária: %.2f \nTot. Geral: %.2f", 
					stQuarto.nAndar, stQuarto.nQuarto, stQuarto.szNomeHospede, stQuarto.dValorDiaria, stQuarto.dVlrTotalGeral);

				cout << endl << cWork << endl;
				
				//Pedir serviço
				//Loop
				do
				{
					cout << endl << "Informe o serviço desejado" << endl
						<< RESTAURANTE << " - RESTAURANTE" << endl
						<< LAVANDERIA << " - LAVANDERIA" << endl
						<< BAR << " - BAR" << endl
						<< PISCINA << " - PISCINA" << endl
						<< BOATE << " - BOATE" << endl
						<< "Selecione: ";
					cin >> nOpcao;					//Recebendo a opção de serviço

				}while(nOpcao < RESTAURANTE || nOpcao > BOATE);

				//Serviço informado, perdir valores
				cout << endl << "Informe o valor do serviço: ";
				cin >> dValor;					//Recebendo o valor informado pelo operador

				//Chamar a função de lançar serviço
				if(objHotel.LancarServico(nAndarHotel, nQuartoHotel, nOpcao, dValor))
				{
					//Serviço inserido
					//ACHO QUE PRECISA FAZER ALGO AQUI
					break;
				}

				cout << endl << endl << "\t\tServiço não inserido!" << endl << endl;
				PAUSA;
				break;

			case LISTAR_QUARTOS_OCUPADOS:
				// <<<< 08 >>>>>
				//	1 - Invocar o método ListarTodosQuartos
				objHotel.ListarTodosQuartos();
				
				break;
			case SAIR_DO_PROGRAMA:
				cout << "\nDeseja sair realmente? (S ou N): ";
				cin >> cOpcaoOper;
				if(cOpcaoOper == 'S' || cOpcaoOper == 's')		// sair realmente?
					return;
				break;						// para sair do switch e decidir se vai sair realmente
			default:
				// Avisar que foi escolhida uma opção errada
				cout << "\nOpcao inválida!" << endl;
				PAUSA;
		} // switch
	} // while(true)
} // main
//
// Função que pede o andar e  quarto válido
//	Parâmetros:
//		Entrada: int *ptrAndar - ponteiro de uma inteira que receberá o andar
//				 int *ptrQuarto - ponteiro de uma inteira que receberá o quarto dentro do andar
//				 char *ptrTransacao - ponteiro de um string que contém a transação que está sendo
//					executada.
//		Retorno: bool - true - indica que foram informados o andar e o quarto válido
//						false - indica que foi informado um andar zero ou quarto zero para
//									cancelar a transação
//
bool	PedirAndarQuarto(int *ptrAndar, int *ptrQuarto, char *ptrTransacao)
{
	cout << "\n\t" << ptrTransacao << endl;				// exibe a transação que está sendo executada

	do
	{
		cout << "\nAndar entre 1 e " << QUANTIDADE_ANDARES << " ou zero para cancelar: ";
		cin >> *ptrAndar;					// recebe o andar

		if(*ptrAndar == 0)					// cancelar?
			return 0;					// indica que deve cancelar

	} while(*ptrAndar < 1 || *ptrAndar > QUANTIDADE_ANDARES); // loop até andar válido

	do
	{
		cout << "\nQuarto entre 1 e " << QUANTIDADE_QUARTOS << " ou zero para cancelar: ";
		cin >> *ptrQuarto;							// recebe o quarto

		if(*ptrQuarto == 0)						// cancelar?
			return 0;							// indica que deve cancelar

	} while(*ptrQuarto < 1 || *ptrQuarto > QUANTIDADE_QUARTOS); // loop até quarto válido

	return true;							// para não dar erro
}