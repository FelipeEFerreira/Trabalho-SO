/*
* Disciplina: Sistemas Operacionais
* Curso: Engenharia de Computação - UFGD - FACET
* 
* Trabalho 1
* 
* Docente: Marcos Paulo Moro
* 
* Discente:
* Felipe Emanuel Ferreira   RGA: 20200712175441
*/

//Operações de I/O
#include <iostream>

//manipulação de vetores
#include <vector>

//Operações matemáticas
#include <cmath>

//Medir temnpo gasto
#include <time.h>

//Manipulação de string
#include <string>
#include <cctype>

//Multithread
#include <process.h>
#include <windows.h>

//Para limpar o buffer de leitura
#include <limits>
#undef max

using namespace std;

//Declaração das funções utilizadas

//Funções da interface com usuário
void ImprimeMenu(); //Imprime o menu de opções
int LerInteiro(); //Garante a leitura de um inteiro
bool ConfirmarEscolha(const string op); //Pergunta ao usuário para confirmar sua ação

//Manipulação da matriz
void GerarMatriz(const int linhas, const int colunas); //Aloca dinâmicamente uma matriz
void PreencherMatriz(const int linhas, const int colunas, const int semente); //Preenche a matriz com valores aleatórios

//Contagem dos números primos
void ContarPrimos(void* parametros);
bool VerificaPrimalidade(const int n); //Verifica se um número n é primo

//Imprime na tela
void ImprimeResultados(); //Imprime os resultados obtidos
void print(int** M, int linhas, int colunas); //Secrect function

struct Parametros {
    int linhas = 0;
    int colunas = 0;
    int tamSub = 0;
};

int** mat = NULL;

int main()
{
    int opt, semente, n_threads = 0;
    Parametros parametros;
    bool confirmar;
    bool sementeGerada = false;

    do
    {
        ImprimeMenu();
        cout << "Selecione uma opção:\n> ";
        opt = LerInteiro();
        switch (opt)
        {
            case 1: //Definir tamanho da Matriz
                confirmar = ConfirmarEscolha("o tamanho da matriz");
                if (confirmar)
                {
                    cout << ">>Tamanho atual da matriz:" << parametros.linhas << "X" << parametros.colunas << endl;
                    cout << "Informe o número de linhas:\n>";
                    parametros.linhas = LerInteiro();
                    cout << "Informe o número de colunas:\n>";
                    parametros.colunas = LerInteiro();
                    GerarMatriz(parametros.linhas, parametros.colunas);
                    cout << ">>Tamanho da matriz alterada para: " << parametros.linhas << "X" << parametros.colunas << endl;
                }
                else
                    cout << ">>O tamanho da matriz não foi alterada." << endl;
                system("PAUSE");
                break;
            case 2: //Definir semente do gerador
                confirmar = ConfirmarEscolha("semente aleatoria");
                if (confirmar)
                {
                    cout << "Entre com uma semente:\n>";
                    semente = LerInteiro();
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
                        PreencherMatriz(parametros.linhas, parametros.colunas, semente);
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
                    cout << ">>O tamanho atual das submatrizes é de " << parametros.tamSub << " elementos." << endl;
                    cout << "Informe o tamanho das submatrizes:\n>";
                    parametros.tamSub = LerInteiro();
                    cout << ">>Tamanho das submatrizes alterado para " << parametros.tamSub << " elementos." << endl;
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
                    n_threads = LerInteiro();
                    cout << ">>Numero de threads alteradas para " << n_threads << "!" << endl;
                }
                else
                    cout << ">>O numero de threads não foi alterado." << endl;
                system("PAUSE");
                break;
            case 6: //Executar
                if (parametros.linhas == 0 || parametros.colunas == 0)
                    cout << ">>Por favor defina um tamnaho de matriz válido (Atual: "<< parametros.linhas << "X" << parametros.colunas << ")." << endl;
                else if (parametros.tamSub == 0)
                    cout << ">>Por favor defina um tamanho de submatriz válido (Atual: " << parametros.tamSub << ")." << endl;
                else
                {
                    cout << ">>Processando..." << endl;
                    //TODO
                    cout << ">>Processamento concluido com sucesso!" << endl;
                }
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
            case -99:
                print(mat, parametros.linhas, parametros.colunas);
                system("PAUSE");
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
    system("CLS");
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "1- Definir tamanho da Matriz" << "\t\t" << "5- Definir número de Thread" << endl;
    cout << "2- Definir semente do gerador" << "\t\t" << "6- Executar" << endl;
    cout << "3- Preencher matriz" << "\t\t\t" << "7- Ver resultados" << endl;
    cout << "4- Definir tamanho das submatrizes" << "\t" << "8- Encerrar" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
}

void ImprimeResultados()
{
    //TODO
}

bool ConfirmarEscolha(const string op)
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

void GerarMatriz(const int linhas, const int colunas)
{
    mat = (int**)malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++)
        mat[i] = (int*)malloc(colunas * sizeof(int));
}

void PreencherMatriz(const int linhas, const int colunas, const int semente)
{
    srand(semente);

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

bool VerificaPrimalidade(const int n)
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

int LerInteiro() {
    int inteiro;
    while (!(cin >> inteiro)) {
        cout << "ERRO: o número deve ser inteiro! Entre com um número válido:\n>";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return inteiro;
}

void print(int** mat, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            cout << mat[i][j] << "  ";
        }
        cout << endl;
    }
}

void ContarPrimos(void* parametros) {

}