// Hotel.cpp - Programa de Hotel para prova de Estrutura de Dados - 29/09/2016
// FATEC - Mogi das Cruzes
#include "Hotel.h"
//
// Fun��o principal
//
void main(void)
{
	int  i,									// indice gen�rico
		nQtdeAndares,						// para receber a quantidade de andares do hotel
		nQtdeQuartosPorAndar,				// para receber a quantidade de quartos por andar
		nAndarHotel,						// para receber o n�mero do andar
		nOpcao,								// para a op��o do servi�o
		nQuartoHotel;						// para receber o n�mero do quarto dentro do andar

	char cOpcaoOper,						// local para receber a op��o escolhida do menu
		cNomeHospede[41],					// local para receber o nome do h�spede do hotel
		cWork[200],							// para formata��o de sprintf
		cNomeAluno[] = "Marco Aur�lio Macei Duarte";

	double dValor;							// local para receber o valor dos servi�os do hotel

	QUARTO stQuarto;						// para conter os dados de um quarto
	
	SYSTEMTIME stTime;						// para buscar data e hora

	ACENTUACAO;					//Acentua��o PT - BR

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
			"\n\tFATEC-MC - Sistema de Hotel %02d/%02d/%04d �s %02d:%02d:%02d\n\t\tOperador: %s Turma: noite",
			stTime.wDay, stTime.wMonth, stTime.wYear,
			stTime.wHour, stTime.wMinute, stTime.wSecond,
			cNomeAluno);
		cout << cWork << endl;

		// Mostrar o menu de op��es para o operador
		cout << FAZER_CHECK_IN << " - Check-in de um hospede" << endl
			<< FAZER_CHECK_OUT << " - Check-out de um hospede" << endl
			<< LANCAR_SERVICO << " - Lan�ar servi�o em um dado quarto" << endl
			<< LANCAR_DIARIA << " - Lancar diarias em todos os quartos ocupados" << endl
			<< LISTAR_UM_QUARTO << " - Listar um quarto" << endl
			<< LISTAR_QUARTOS_OCUPADOS << " - Listar todos os quartos ocupados" << endl
			<< SAIR_DO_PROGRAMA << " - Sair do programa" << endl
			<< "\tSelecione: ";
		cin	 >> cOpcaoOper;					// op��o escolhida pelo operador

		cOpcaoOper = toupper(cOpcaoOper);	// converte para upper case a op��o escolhida

		switch(cOpcaoOper)					// avaliar a op��o que foi escolhida pelo operador
		{
			case FAZER_CHECK_IN:
				// 1 - Chamar a fun��o PedirAndarQuarto passando os endere�o de nAndarHotel para receber
				//		o n�mero do andar e o endere�o de nQuartoHotel para receber o n�mero do quarto no
				//			andar e indicar que a transa��o � "Check-in de h�spede"
				if(!PedirAndarQuarto(&nAndarHotel, &nQuartoHotel, "Check-in de h�spede"))
				{
					break;					//Volta ao menu
				}
				
				// 2 - Se retornou true indicando que foram informados andar e quarto v�lidos
				//	   invocar o m�todo que verifica se o quarto est� ocupado (VerificarQuartoLivre)
				if(objHotel.VerificarQuartoLivre(nAndarHotel, nQuartoHotel, &stQuarto))
				{
					//Quarto ocupado
					cout << endl << "Quarto ocupado \nNome: " << stQuarto.szNomeHospede << endl << endl;
					PAUSA;
					break;
				}

				// 4 - Se n�o estiver ocupado Pedir o nome do h�spede considerando que tem nome e sobrenome
				// 5 - Invocar o m�todo CheckInHospede
				cin.ignore(1, EOF);
				cout << "Informe o nome do H�spede: ";
				cin.getline(stQuarto.szNomeHospede, EXTEN_NOME + 1, '\n');

				if(objHotel.CheckInHospede(nAndarHotel, nQuartoHotel, stQuarto.szNomeHospede))
				{
					cout << endl << endl << "\t\tH�spede cadastrado com sucesso!" << endl << endl;
					PAUSA;
					break;
				}
				break;						// para sair do switch e voltar ao menu

			case FAZER_CHECK_OUT:
				// 1 - Pedir o andar e o quarto da mesma forma que no item anterior indicando que a transa��o 
				//		� "Check-out"
				if(!PedirAndarQuarto(&nAndarHotel, &nQuartoHotel, "Check-out de h�spede"))
				{
					break;					//Volta ao menu
				}
				
				// 3 - Verificar se o quarto est� ocupado e se estiver exibir o nome do h�spede e
				//		o total de di�rias do quarto
				if(objHotel.VerificarQuartoLivre(nAndarHotel, nQuartoHotel, &stQuarto))
				{
					//Quarto ocupado
					cout << endl << "Quarto ocupado \nNome: " << stQuarto.szNomeHospede 
						<< "\nValor Total: " << stQuarto.dVlrTotalGeral << endl << endl;

					// 4 - Pedir a confirma��o do operador com um S ou N
					//		Se for indicado 'S' ou 's' invocar o m�todo CheckOutHospede
					cout << "Deseja realizar o checkOut? (S - sim ou N - n�o): ";
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
				}//Fim do m�todo

				cout << endl << "\t\tQuarto desocupado!" << endl << endl;
				PAUSA;
				break;						// para sair do switch e voltar ao menu

			case LANCAR_DIARIA:
				// <<<< 05 >>>> 
				// 1 - Pedir a data e hora do sistema e apresentar ao operador
				GetLocalTime(&stTime);
				sprintf_s(cWork, sizeof(cWork), "\t\tLan�ar Di�rias %02d/%02d/%04d �s %02d:%02d:%02d", 
					stTime.wDay, stTime.wMonth, stTime.wYear, stTime.wHour, stTime.wMinute, stTime.wSecond);
				cout << endl << cWork << endl;

				cout << endl << "\tDeseja realmente lan�ar as di�rias? (S - sim ou N - n�o): ";
				cin >> cOpcaoOper;

				// 2 - Confirmar com o operador se ele realmente deseja lan�ar as di�rias no hor�rio
				//		apresentado
				// 3 - Se o operador confirmar invocar o m�todo LancarDiarias
				if(cOpcaoOper == 's' || cOpcaoOper == 'S')
					//Chamar o m�todo para lan�ar di�rias
					objHotel.LancarDiarias();
				
				// 4 - Caso o operador n�o confirme voltar ao menu.
				break;						// para sair do switch e voltar ao menu

			case LISTAR_UM_QUARTO:
				// <<<< 06 >>>> 
				// 1 - Pedir Andar e Quarto conforme itens anteriores
				if(!PedirAndarQuarto(&nAndarHotel, &nQuartoHotel, "Listar um quarto"))
				{
					break;					//Volta ao menu
				}
				
				// 2 - Verificar se o quarto est� ocupado passando o endere�o da struct stQuarto
				if(objHotel.VerificarQuartoLivre(nAndarHotel, nQuartoHotel, &stQuarto))
				{
					//Quarto ocupado
					sprintf_s(cWork, sizeof(cWork), 
						"Andar: %d \nQuarto: %d \nH�spede: %s \nVlr.Di�ria: %.2f \nTot. Servi�o %.2f \nTot. Geral: %.2f", 
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
				if(!PedirAndarQuarto(&nAndarHotel, &nQuartoHotel, "Lan�ar Servi�o"))
				{
					break;					//Volta ao menu
				}
			
				// 2 - Verificar se o quarto est� ocupado passando o endere�o da struct stQuarto
				if(!objHotel.VerificarQuartoLivre(nAndarHotel, nQuartoHotel, &stQuarto))
				{
					//Quarto desocupado
					cout << endl << endl << "\t\tQuarto desocupado!" << endl << endl;
					PAUSA;
					break;
				}

				//Quarto ocupado
				sprintf_s(cWork, sizeof(cWork), "Andar: %d \nQuarto: %d \nH�spede: %s \nVlr.Di�ria: %.2f \nTot. Geral: %.2f", 
					stQuarto.nAndar, stQuarto.nQuarto, stQuarto.szNomeHospede, stQuarto.dValorDiaria, stQuarto.dVlrTotalGeral);

				cout << endl << cWork << endl;
				
				//Pedir servi�o
				//Loop
				do
				{
					cout << endl << "Informe o servi�o desejado" << endl
						<< RESTAURANTE << " - RESTAURANTE" << endl
						<< LAVANDERIA << " - LAVANDERIA" << endl
						<< BAR << " - BAR" << endl
						<< PISCINA << " - PISCINA" << endl
						<< BOATE << " - BOATE" << endl
						<< "Selecione: ";
					cin >> nOpcao;					//Recebendo a op��o de servi�o

				}while(nOpcao < RESTAURANTE || nOpcao > BOATE);

				//Servi�o informado, perdir valores
				cout << endl << "Informe o valor do servi�o: ";
				cin >> dValor;					//Recebendo o valor informado pelo operador

				//Chamar a fun��o de lan�ar servi�o
				if(objHotel.LancarServico(nAndarHotel, nQuartoHotel, nOpcao, dValor))
				{
					//Servi�o inserido
					//ACHO QUE PRECISA FAZER ALGO AQUI
					break;
				}

				cout << endl << endl << "\t\tServi�o n�o inserido!" << endl << endl;
				PAUSA;
				break;

			case LISTAR_QUARTOS_OCUPADOS:
				// <<<< 08 >>>>>
				//	1 - Invocar o m�todo ListarTodosQuartos
				objHotel.ListarTodosQuartos();
				
				break;
			case SAIR_DO_PROGRAMA:
				cout << "\nDeseja sair realmente? (S ou N): ";
				cin >> cOpcaoOper;
				if(cOpcaoOper == 'S' || cOpcaoOper == 's')		// sair realmente?
					return;
				break;						// para sair do switch e decidir se vai sair realmente
			default:
				// Avisar que foi escolhida uma op��o errada
				cout << "\nOpcao inv�lida!" << endl;
				PAUSA;
		} // switch
	} // while(true)
} // main
//
// Fun��o que pede o andar e  quarto v�lido
//	Par�metros:
//		Entrada: int *ptrAndar - ponteiro de uma inteira que receber� o andar
//				 int *ptrQuarto - ponteiro de uma inteira que receber� o quarto dentro do andar
//				 char *ptrTransacao - ponteiro de um string que cont�m a transa��o que est� sendo
//					executada.
//		Retorno: bool - true - indica que foram informados o andar e o quarto v�lido
//						false - indica que foi informado um andar zero ou quarto zero para
//									cancelar a transa��o
//
bool	PedirAndarQuarto(int *ptrAndar, int *ptrQuarto, char *ptrTransacao)
{
	cout << "\n\t" << ptrTransacao << endl;				// exibe a transa��o que est� sendo executada

	do
	{
		cout << "\nAndar entre 1 e " << QUANTIDADE_ANDARES << " ou zero para cancelar: ";
		cin >> *ptrAndar;					// recebe o andar

		if(*ptrAndar == 0)					// cancelar?
			return 0;					// indica que deve cancelar

	} while(*ptrAndar < 1 || *ptrAndar > QUANTIDADE_ANDARES); // loop at� andar v�lido

	do
	{
		cout << "\nQuarto entre 1 e " << QUANTIDADE_QUARTOS << " ou zero para cancelar: ";
		cin >> *ptrQuarto;							// recebe o quarto

		if(*ptrQuarto == 0)						// cancelar?
			return 0;							// indica que deve cancelar

	} while(*ptrQuarto < 1 || *ptrQuarto > QUANTIDADE_QUARTOS); // loop at� quarto v�lido

	return true;							// para n�o dar erro
}