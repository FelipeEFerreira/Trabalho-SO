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
#include <locale>

//manipulação de vetores
#include <vector>

//Medir tempo gasto
#include <ctime>

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

//Structs utilizadas

struct InfoMatriz
{
    int linhas = 0;
    int colunas = 0;
};

struct InfoSub
{
    int linhas = 0;
    int colunas = 0;
};

struct Parametros
{
    int idThread = 0;
    InfoMatriz* matriz = NULL;
    InfoSub* subMatriz = NULL;
};

//Declaração das funções utilizadas

//Funções da interface com usuário
void ImprimeMenu(); //Imprime o menu de opções
void ImprimeResultados(const InfoMatriz* matriz, const InfoSub* subMatriz, const int numThreads, const int semente, const double tempoExecucao); //Imprime os resultados obtidos
int LerInteiro(); //Garante a leitura de um inteiro
bool ConfirmarEscolha(const string op); //Pergunta ao usuário para confirmar sua ação
bool PodeExecutar(const InfoMatriz* matriz, const InfoSub* subMatriz, const int numThreads, const bool matPreenchida); // Verifica se tudo está adequado para execução

//Manipulação da matriz
void GerarMatriz(const InfoMatriz* matriz); //Aloca dinâmicamente uma matriz
void PreencherMatriz(const InfoMatriz* matriz, const int semente); //Preenche a matriz com valores aleatórios
void LiberarMatriz(const int linhas); //Libera memória alocada dinâmicamente para matriz

//Contagem dos números primos
void ContarPrimos(void* parametrosFunc); //Função multithreading para contar os primos
bool VerificaPrimalidade(const int n); //Verifica se um número n é primo

//Variaveis globais
int** mat;
int totalPrimos = 0;
int subVerificadas = 0;

//MUTEX para sessão crítica
HANDLE hmutex1;
HANDLE hmutex2;

int main()
{
    setlocale(LC_CTYPE, "pt_BR.UTF-8"); //Troca a codificação dos caracteres para imprimir caracteres acentudados corretamente
    int opt, semente = 0, numThreads = 0, threadsCriadas = 0;
    bool confirmar;
    bool sementeGerada = false;
    bool matrizGerada = false;
    bool matPreenchida = false;
    clock_t start, end;
    double tempoExecucao = 0.0;

    InfoMatriz matriz;
    InfoSub subMatriz;

    vector<HANDLE> hthreads;
    vector<Parametros> vetorParametro;

    hmutex1 = CreateMutex(NULL, FALSE, NULL);
    hmutex2 = CreateMutex(NULL, FALSE, NULL);

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
                    cout << ">>Tamanho atual da matriz:" << matriz.linhas << "X" << matriz.colunas << endl;
                    if (matriz.linhas != 0) 
                    {
                        cout << ">>Desalocando memória, aguarde..." << endl;
                        LiberarMatriz(matriz.linhas);
                        matPreenchida = false;
                    }
                    do
                    {
                        cout << "Informe o número de linhas:\n>";
                        matriz.linhas = LerInteiro();
                        cout << "Informe o número de colunas:\n>";
                        matriz.colunas = LerInteiro();

                        if(matriz.linhas <= 0 || matriz.colunas <= 0)
                            cout << ">>Entre com um valor válido." << endl;

                    }while(matriz.linhas <= 0 || matriz.colunas <= 0);
                    GerarMatriz(&matriz);
                    matrizGerada = true;
                    cout << ">>Tamanho da matriz alterada para: " << matriz.linhas << "X" << matriz.colunas << endl;
                }
                else
                    cout << ">>O tamanho da matriz não foi alterada." << endl;
                system("PAUSE");
                break;

            case 2: //Definir semente do gerador
                cout << "Entre com uma semente:\n>";
                semente = LerInteiro();
                sementeGerada = true;
                cout << ">>Semente aleatória alterada com sucesso!" << endl;
                system("PAUSE");
                break;

            case 3: //Preencher matriz
                if (sementeGerada && matrizGerada)
                {
                    confirmar = ConfirmarEscolha("o conteúdo da matriz");
                    if (confirmar)
                    {
                        cout << ">>Preenchendo matriz com valores aleatórios..." << endl;
                        PreencherMatriz(&matriz, semente);
                        matPreenchida = true;
                        cout << ">>A matriz preenchida com valores aleatorios." << endl;
                    }
                    else
                        cout << ">>O conteúdo da matriz não foi alterado." << endl;
                }
                else
                    cout << ">>É necessário gerar uma matriz e uma semente aleatória para realizar esta operação!" << endl;
                system("PAUSE");
                break;

            case 4: //Definir tamanho das submatrizes
                cout << ">>O tamanho atual das submatrizes é de " << subMatriz.linhas << "X" << subMatriz.colunas << endl;
                do{
                    cout << "Informe o número de linhas das submatrizes:\n>";
                    subMatriz.linhas = LerInteiro();
                    cout << "Informe o número de colunas das submatrizes:\n>";
                    subMatriz.colunas = LerInteiro();
                    if(subMatriz.linhas <= 0 || subMatriz.colunas <= 0)
                        cout << ">>Entre com um tamanho válido para as submatrizes" << endl;
                    if (subMatriz.linhas > matriz.linhas || subMatriz.colunas > matriz.colunas)
                        cout << ">>O numero de linha ou colunas da submatriz não pode ser maior que o da matriz original(Matriz original: " << matriz.linhas << "X" << matriz.colunas << ")" << endl;
                }while(subMatriz.linhas <= 0 || subMatriz.colunas <= 0 || subMatriz.linhas > matriz.linhas || subMatriz.colunas > matriz.colunas);
                cout << ">>Tamanho das submatrizes alterado para " << subMatriz.linhas << "X" << subMatriz.colunas << endl;
                system("PAUSE");
                break;

            case 5: //Definir número de Thread
                cout << "Entre com o número de threads(n° threads atual: " << numThreads << "):\n>";

                numThreads = LerInteiro();
                threadsCriadas = (int)vetorParametro.size();

                if (numThreads > threadsCriadas) //Cria mais threads se necessário
                    for (int i = threadsCriadas; i < numThreads; i++)
                        vetorParametro.push_back({ i, &matriz, &subMatriz });
                else //Diminui o numero de threads
                    for (int i = threadsCriadas; i > numThreads; i--)
                            vetorParametro.pop_back();

                cout << ">>Numero de threads alteradas para " << numThreads << "!" << endl;

                system("PAUSE");
                break;

            case 6: //Executar
                
                if (PodeExecutar(&matriz, &subMatriz, numThreads, matPreenchida)) {

                    if (totalPrimos != 0 || subVerificadas != 0) {
                        totalPrimos = 0;
                        subVerificadas = 0;
                    }
                    cout << ">>Executando para " << numThreads << " threads, aguarde..." << endl;

                    start = clock();
                    for (int i = 0; i < numThreads; i++)
                        hthreads.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ContarPrimos, &vetorParametro[i], 0, NULL));

                    WaitForMultipleObjects(numThreads, hthreads.data(), TRUE, INFINITE);
                    end = clock();

                    tempoExecucao = double(end - start) / CLOCKS_PER_SEC;

                    for (int i = 0; i < numThreads; i++) //Fecha os HANDLE das threads
                        CloseHandle(hthreads[i]);

                    hthreads.clear(); //Remove as threads

                    cout << ">>Processamento concluido com sucesso!" << endl;
                }
                else
                    cout << ">>Não foi possível executar!" << endl;
                system("PAUSE");
                break;

            case 7: //Ver resultados
                cout << ">>Gerando relatório dos resultados obtidos..." << endl;
                ImprimeResultados(&matriz, &subMatriz, numThreads, semente, tempoExecucao);
                system("PAUSE");
                break;

            case 8: //Encerrar
                cout << ">>Desalocando memória, aguarde..." << endl;
                LiberarMatriz(matriz.linhas);
                cout << ">>Finalizando..." << endl;
                CloseHandle(hmutex1);
                CloseHandle(hmutex2);
                break;

            default: //Caso padrão, opção inválida
                cout << ">>Por favor, entre com uma opção válida." << endl;
                system("PAUSE");
                break;
        }
    }while (opt != 8);
    return 0;
}


//Definição das funções utilizadas

/* Declaração das funções utilizadas */

void ImprimeMenu() 
{
    system("CLS");
    cout << "--------------------------------------------------------------------------------" << endl
         << "1- Definir tamanho da Matriz" << "\t\t" << "5- Definir número de Thread" << endl
         << "2- Definir semente do gerador" << "\t\t" << "6- Executar" << endl
         << "3- Preencher matriz" << "\t\t\t" << "7- Ver resultados" << endl
         << "4- Definir tamanho das submatrizes" << "\t" << "8- Encerrar" << endl
         << "--------------------------------------------------------------------------------" << endl;
}

void ImprimeResultados(const InfoMatriz* matriz, const InfoSub* subMatriz, const int numThreads, const int semente, const double tempoExecucao)
{
    if (tempoExecucao != 0.0) 
    {
        cout << "\n----------------------------------RESULTADOS----------------------------------" << endl
            << ">Tamanho da matriz: " << matriz->linhas << "X" << matriz->colunas << endl
            << ">Tamanho da submatriz: " << subMatriz->linhas << "X" << subMatriz->colunas << endl
            << ">Número de threads: " << numThreads << endl
            << ">Semente aleatória: " << semente << endl
            << ">Quantidade de primos: " << totalPrimos << endl
            << ">Tempo decorrido: " << tempoExecucao << " segundos" << endl
            << "------------------------------------------------------------------------------" << endl;
    }
    else
    {
        cout << ">>É necessário pelo menos uma execução para gerar um ralatório!" << endl;
    }
}

int LerInteiro()
{
    int inteiro = 0;
    while (!(cin >> inteiro)) {
        cout << "ERRO: Entre com um número válido:\n>";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return inteiro;
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
            cout << "ATENÇÂO: caso tenha mudado a semente aleatória essa operação irá sobrescrever os dados anteriores." << endl;
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


bool PodeExecutar(const InfoMatriz* matriz, const InfoSub* subMatriz, const int numThreads, const bool matPreenchida)
{
    if (matriz->linhas <= 0 || matriz->colunas <= 0) //Número de linhas ou colunas da matriz inválido
    {
        cout << ">>Por favor defina um tamanho de matriz válido (Atual: " << matriz->linhas << "X" << matriz->colunas << ")." << endl;
        return false;
    }
    else if (subMatriz->linhas <= 0 || subMatriz->colunas <= 0) //Número de linhas ou colunas da submatriz inválido
    {
        cout << ">>Por favor defina um tamanho de submatriz válido (Atual: " << subMatriz->linhas << "X" << subMatriz->colunas << ")." << endl;
        return false;
    }
    else if (subMatriz->linhas > matriz->linhas || subMatriz->colunas > matriz->colunas) //Número de linhas ou colunas da submatriz maior que o da matriz
    {
        cout << ">>O numero de linha ou colunas da submatriz não pode ser maior que o da matriz original(Matriz original: " << matriz->linhas << "X" << matriz->colunas << ")" << endl;
        return false;
    }
    else if (numThreads == 0) //Número de threads não definido
    {
        cout << ">>Por favor defina a quantidade de threads!" << endl;
        return false;
    }
    else if (!matPreenchida) //Matriz não foi preenchida com valores
    {
        cout << ">>Por favor preencha a matriz antes de executar!" << endl;
        return false;
    }
    else
        return true; //Tudo OK
}


/* Manipulação da matriz */


void GerarMatriz(const InfoMatriz* matriz)
{
    mat = (int**)malloc(matriz->linhas * sizeof(int*));
    for (int i = 0; i < matriz->linhas; i++) 
        mat[i] = (int*)malloc(matriz->colunas * sizeof(int));
}

void PreencherMatriz(const InfoMatriz* matriz, const int semente)
{
    srand(semente);

    for (int i = 0; i < matriz->linhas; i++)
    {
        for (int j = 0; j < matriz->colunas; j++)
        {
            mat[i][j] = rand();
        }
    }
}

void LiberarMatriz(const int linhas) {
    for (int i = 0; i < linhas; i++)
        free(mat[i]);
    free(mat);
}

/* Contagem dos números primos */

bool VerificaPrimalidade(const int n)
{
    if (n <= 1)
        return false;
    if (n == 2 || n == 3)
        return true;
    if (!(n & 1)) //verifica se o número é par utilizando operador and bit a bit
        return false;
    for (int i = 3; i <= n/2; i += 2) //Verifica se é divisivel por algum numero impar
        if (n % i == 0)
            return false;
    return true;
}

void ContarPrimos(void* parametrosFunc) {
    //variaveis locais
    Parametros* parametros = (Parametros*)parametrosFunc;
    int linha, coluna, num;
    int contLocal = 0;
    bool temResto = false;
    bool primo = false;

    //Calcula quantidade de elementos
    int totalElementos = parametros->matriz->linhas * parametros->matriz->colunas;
    int totalSub = parametros->subMatriz->linhas * parametros->subMatriz->colunas;

    //calcula quantidade de submatrix
    int qntdSub = totalElementos / totalSub;
    int elementosRestante = totalElementos % totalSub;

    if (elementosRestante != 0) {
        qntdSub++;
        temResto = true;
    }

    //calcula iterações
    int elementoInicial = parametros->idThread * totalSub;
    int elementoFinal = elementoInicial + totalSub;

    if (elementoInicial >= totalElementos) //threads a mais que o necessário que seram inutilizadas
        _endthread();

    //Sessão critica p/ controlar a verificação de matrizes
    WaitForSingleObject(hmutex1, INFINITE);
    subVerificadas++;
    ReleaseMutex(hmutex1);

    while (subVerificadas <= qntdSub) {
        contLocal = 0;
        linha = elementoInicial / parametros->matriz->colunas; //Calcula linha inicial da matriz para cada submatriz
        coluna = elementoInicial % parametros->matriz->colunas; //Calcula coluna inicial da matriz para cada submatriz
        for (int i = elementoInicial; i < elementoFinal; i++) {
            num = mat[linha][coluna];
            primo = VerificaPrimalidade(num);
            if (primo)
                contLocal++;
            coluna++; //Percorre as colunas
            if (coluna == parametros->matriz->colunas) {
                coluna = 0;
                linha++; //Percorre as linhas
            }
        }

        //Sessão critica para armazenar resultado da thread e calcular proxima submatriz a ser verificada
        WaitForSingleObject(hmutex2, INFINITE);
        totalPrimos += contLocal;
        if (subVerificadas <= qntdSub) {
            elementoInicial = subVerificadas * totalSub;
            elementoFinal = elementoInicial + totalSub;
            ++subVerificadas;
            if (temResto)
                if (subVerificadas == qntdSub)
                    elementoFinal = elementoInicial + elementosRestante;
        }
        ReleaseMutex(hmutex2);
    }

    _endthread();
}