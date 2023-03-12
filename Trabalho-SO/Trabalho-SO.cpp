#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>
#include <time.h>
#include <process.h>
#include <windows.h>

using namespace std;

void ImprimeMenu();
void ImprimeResultados();
void GerarMatriz(int linhas, int colunas, int**& mat);
void PreencherMatriz(int linhas, int colunas, int**& mat);
bool ConfirmarEscolha(const string op);
bool VerificaPrimalidade(int n);

int main()
{
    int opt , n_threads = 0;
    int linhaMat = 0, colunaMat = 0;
    int tamSub = 0;
    int** mat = NULL;
    bool confirmar;
    bool sementeGerada = false;
    do
    {
        ImprimeMenu();
        cout << "Selecione uma opção:\n> ";
        cin >> opt;
        switch (opt)
        {
            case 1: //Definir tamanho da Matriz
                confirmar = ConfirmarEscolha("o tamanho da matriz");
                if (confirmar)
                {
                    cout << ">>Tamanho atual da matriz:" << linhaMat << "X" << colunaMat << endl;
                    cout << "Informe o número de linhas:\n>";
                    cin >> linhaMat;
                    cout << "Informe o número de colunas:\n>";
                    cin >> colunaMat;
                    if (linhaMat != 0 || colunaMat != 0)
                        free(mat);
                    GerarMatriz(linhaMat, colunaMat, mat);
                    cout << ">>Tamanho da matriz alterada para: " << linhaMat << "X" << colunaMat << endl;
                }
                else
                    cout << ">>O tamanho da matriz não foi alterada." << endl;
                system("PAUSE");
                break;
            case 2: //Definir semente do gerador
                confirmar = ConfirmarEscolha("semente aleatoria");
                if (confirmar)
                {
                    srand(time(NULL));
                    cout << ">>Semente aleatória alterada com sucesso!" << endl;
                    sementeGerada = true;
                }
                else
                    cout << ">>Semente aleatória não foi alterada." << endl;
                system("PAUSE");
                break;
            case 3: //Preencher matriz
                if (sementeGerada)
                {
                    confirmar = ConfirmarEscolha("o conteúdo da matriz");
                    if (confirmar)
                    {
                        cout << ">>Preenchendo matriz com valores aleatórios..." << endl;
                        PreencherMatriz(linhaMat, colunaMat, mat);
                        cout << ">>A matriz preenchida com valores aleatorios." << endl;
                    }
                    else
                        cout << ">>O conteúdo da matriz não foi alterado." << endl;
                }
                else
                    cout << ">>Gere uma semente aleatória antes de preencher a matriz." << endl;
                system("PAUSE");
                break;
            case 4: //Definir tamanho das submatrizes
                confirmar = ConfirmarEscolha("o tamanho das submatrizes");
                if (confirmar)
                {
                    cout << ">>O tamanho atual das submatrizes é de " << tamSub << " elementos." << endl;
                    cout << "Informe o tamanho das submatrizes:\n>";
                    cin >> tamSub;
                    cout << ">>Tamanho das submatrizes alterado para " << tamSub << " elementos." << endl;
                }
                else
                    cout << ">>O tamanho das submatrizes não foi alterado." << endl;
                system("PAUSE");
                break;
            case 5: //Definir número de Thread
                confirmar = ConfirmarEscolha("o número de threads");
                if (confirmar)
                {
                    cout << "Entre com o número de threads(n° threads atual: " << n_threads << "):\n>";
                    cin >> n_threads;
                    cout << ">>Numero de threads alteradas para" << n_threads << "!" << endl;
                }
                else
                    cout << ">>O numero de threads não foi alterado." << endl;
                system("PAUSE");
                break;
            case 6: //Executar
                cout << ">>Processando..." << endl;
                //TODO
                cout << ">>Processamento concluido com sucesso!" << endl;
                system("PAUSE");
                break;
            case 7: //Ver resultados
                cout << ">>Gerando relatório dos resultados obtidos..." << endl;
                //TODO
                ImprimeResultados();
                break;
            case 8: //Encerrar
                cout << ">>Finalizando..." << endl;
                free(mat);
                break;
            default: //Caso padrão, opção inválida
                cout << ">>Por favor, entre com uma opção válida." << endl;
                system("PAUSE");
                break;
        }
    }while (opt != 8);
    return 0;
}

void ImprimeMenu() 
{
    cout << flush;
    system("CLS");
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "1- Definir tamanho da Matriz" << "\t\t" << "5 - Definir número de Thread" << endl;
    cout << "2- Definir semente do gerador" << "\t\t" << "6- Executar" << endl;
    cout << "3- Preencher matriz" << "\t\t\t" << "7- Ver resultados" << endl;
    cout << "4- Definir tamanho das submatrizes" << "\t" << "8- Encerrar" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
}

void ImprimeResultados()
{
    //TODO
}

bool ConfirmarEscolha(string op)
{
    char escolha;
    do
    {
        cout << "Deseja mudar " << op << "?[S/N]" << endl;
        if (op == "o tamanho da matriz")
            cout << "ATENÇÂO: isso implicará na perda dos elementos da matriz!" << endl;
        if (op == "o conteúdo da matriz")
            cout << "ATENÇÂO: caso caso tenha mudado a semente aleatória essa operação irá sobrescrever os dados anteriores." << endl;
        cout << ">";
        cin >> escolha;
        escolha = toupper(escolha);
        if (escolha == 'S')
            return true;
        if (escolha == 'N')
            return false;
    } while (escolha != 'Y' && escolha != 'N');
    return false;
}

void GerarMatriz(int linhas, int colunas, int**& mat)
{
    mat = (int**)malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++)
        mat[i] = (int*)malloc(colunas * sizeof(int));
}

void PreencherMatriz(int linhas, int colunas, int**& mat)
{
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            mat[i][j] = rand();
            cout << mat[i][j] << "  ";
        }
        cout << endl;
    }
}

bool VerificaPrimalidade(int n)
{
    if (n <= 1)
        return false;
    if (n == 2 || n == 3)
        return true;
    if (!(n & 1)) //Verifica se o número é par utilizando operador And bit a bit
        return false;
    for (int i = 3; i <= sqrt(n); i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    return true;
}