#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>

using namespace std;

const string NOME_ARQUIVO = "lms-jun22qtr-csv.csv";
/*A finalidade deste trabalho é implementar um
 sistema que converta um arquivo CSV em binário e,
 posteriormente, trabalhe com ordenação em memória secundária*/
/*as operações devem ser realizadas no arquivo binário,
sendo permitido apenas carregar registros individuais
para a memória. Ou seja, não é permitido carregar o arquivo
inteiro na memória para realizar qualquer das operações solicitadas.
*/
// Nosso grupo o trabalho tem que ser em ordem decrescente
void imprimirMenu();
int lerInteiro();
int main()
{
    
    int comando = -1;
    bool encerrarPrograma = false;
    try
    {
        do
        {
            imprimirMenu();
            comando = lerInteiro();
            switch (comando)
            {
            case 1:

                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:

                break;
            case 0:
                cout << "Para confirmar o encerramento do arquivo aperte -1"
                     << " ou outro inteiro para voltar ao menu\n";
                comando = lerInteiro();
                if (comando == -1)
                    encerrarPrograma = true;
                break;
            default:
                cout << "Opcão Não encontrada digite novamente outro comando\n";
                break;
            }
        } while (!encerrarPrograma);
    }
    catch (const std::exception &e)
    {
        cout << e.what() << '\n';
    }
    return 0;
}
void imprimirMenu()
{
    cout << "=============================================\n";
    cout << "           MENU DE MANIPULACAO BINARIA       \n";
    cout << "=============================================\n";
    cout << "1. Inserir elemento em posicao especifica\n";
    cout << "2. Visualizar registros entre duas posicoes\n";
    cout << "3. Alterar dados de um registro especifico\n";
    cout << "4. Trocar dois registros de posicao (swap)\n";
    cout << "5. Imprimir todos os registros\n";
    cout << "0. Sair do programa\n";
    cout << "---------------------------------------------\n";
    cout << "Digite a opcao desejada: ";
}
int lerInteiro()
{
    int valor;
    bool valorInvalido = true;
    while (valorInvalido)
    {
        cin >> valor;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Entrada inválida. Digite um inteiro e tente novamente.\n";
        }
        else
        {
            valorInvalido = false;
        }
    }
    cin.ignore(1000, '\n');
    // limpa \n que ficou no cin, importante para rogramas que usam getline
    return valor;
}