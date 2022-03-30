#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cctype>
#include <windows.h>
#include <conio2.h>
using namespace std;

typedef struct noAluno{
    string nome;
    int idade, matricula;
    struct noAluno *proxAluno, *antAluno;
}tipoAluno;

typedef struct noClasse{
    string professor, etapa;
    char turma;
    int serie;
    struct noClasse *proxClasse;
    tipoAluno *primeiroAluno, *ultimoAluno;
    int qtdAlunos;
}tipoClasse;

typedef struct gerencLista{
    tipoClasse *inicio, *fim;
    int qtdClasses;
}tipoLista;

//------------------------ Protótipos ------------------------

void waitAndClear(int tempo);
void inicializa(tipoLista *lista);
void lerNumero(int& numero);
void menuCadastroClasse(tipoLista* lista);
int cadastrarClasse(tipoLista *lista, string prof, string etapa, char turma, int serie);
void matriculaAlunoNaClasse(tipoLista* lista);
tipoClasse* pesquisaClasse(tipoLista* lista);
tipoAluno* pedeDadosAluno();
int vinculaAlunoAClasse(tipoAluno* aluno, tipoClasse* classe);
void removeAlunoDaClasse(tipoLista* lista);
tipoAluno* pesquisaAluno(tipoClasse* classe);
int desvinculaAlunoDaClasse(tipoAluno* aluno, tipoClasse* classe);
void exibirListaClasses(tipoLista *lista);
void exibirClasse(tipoClasse* classe);

//------------------------------------------------------------

int main(){
    tipoLista lista;
    int opcao;

    //system("chcp 65001>nul"); // UTF-8
    system("chcp 1252>nul");    // Padrao windows

    inicializa(&lista);
    do{
        printf("\n============================ Menu ============================\n\n");
        printf("1  - Cadastrar classe\n");
        printf("2  - Matricular aluno em determinada classe\n");
        printf("3  - Remover aluno de classe\n");
        printf("4  - Exibir listagem de alunos matriculados em uma classe\n");
        printf("5  - Exibir lista de classes\n");
        printf("6  - Remover classe\n");
        printf("7  - Pesquisar e exibir qual a classe de um aluno\n");
        printf("99 - Para sair do programa\n");
        printf("\n==============================================================\n\n");
        printf("-> Escolha uma opção: ");
        printf("\n\n\n==============================================================\n\n");
        gotoxy(23, 15);
        lerNumero(opcao);

        switch(opcao){
        case 1:
            waitAndClear(750);
            menuCadastroClasse(&lista);
            break;
        case 2:
            waitAndClear(750);
            matriculaAlunoNaClasse(&lista);
            break;
        case 3:
            waitAndClear(750);
            removeAlunoDaClasse(&lista);
            break;
        case 4:
            waitAndClear(750);
            break;
        case 5:
            if (lista.qtdClasses == 0){
                printf("\n-> Não existem classes cadastradas!");
                break;
            }
            waitAndClear(750);
            exibirListaClasses(&lista);
            break;
        case 6:
            waitAndClear(750);
            break;
        case 7:
            waitAndClear(750);
            break;
        case 99:
            printf("\n-> Fechando o programa...");
            break;
        default:
            printf("\n-> Opção inválida. Digite outra opção!");
            break;
        }
        waitAndClear(0);
    }while (opcao != 99);

    return 0;
}

void waitAndClear(int tempo){
    Sleep(tempo);
    system("cls");
}

void inicializa(tipoLista *lista){
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->qtdClasses = 0;
}

void lerNumero(int& numero){
    char aux[30];
    cin >> aux;
    cin.ignore(100, '\n');
    numero = atoi(aux);
}

void menuCadastroClasse(tipoLista* lista){
    string etapasEnsino[3] = {"Ensino Fundamental I", "Ensino Fundamental II", "Ensino Médio"};
    string prof, etapa;
    char turma;
    int serie, nEtapa;
    printf("\n===============================================================");
    printf("\nCadastro de classe:");
    printf("\n===============================================================\n");
    //Professor
    printf("\n* Digite o nome do professor: ");
    getline(cin, prof);
    printf("\n===============================================================\n");
    //Etapa de estudo
    printf("\n>> Qual a etapa de estudo?\n");
    printf("      1 - Ensino Fundamental\n");
    printf("      2 - Ensino Médio\n");
    printf("-> Escolha uma opção: ");
    do{
        gotoxy(23, 13); printf("                    ");
        gotoxy(23, 13);
        lerNumero(nEtapa);
    }while (nEtapa != 1 && nEtapa != 2);
    printf("\n===============================================================\n");
    //Serie
    printf("\n* Digite a série (1°, 2°, 3°, ...) <Apenas o número>: ");
    do{
        gotoxy(55, 17); printf("                    ");
        gotoxy(55, 17);
        lerNumero(serie);
        if (nEtapa == 2 && (serie < 1 || serie > 3))
            serie = 0;
    }while (serie < 1 || serie > 9);
    //Atribui a string correta de acordo com a etapa
    if (nEtapa == 1){
        if (serie >= 1 && serie <= 5)
            etapa = etapasEnsino[0];
        else if (serie >= 6 && serie <= 9)
            etapa = etapasEnsino[1];
    }else{
        etapa = etapasEnsino[2];
    }
    printf("\n>>> Esta classe pertence ao %s.\n", etapa.c_str());
    printf("\n===============================================================\n");
    //Turma
    printf("\nDigite a turma (A, B, C, ...): ");
    turma = cin.get();
    cin.ignore(100, '\n');
    turma = toupper(turma);
    printf("\n===============================================================\n");
    //Status
    if (cadastrarClasse(lista, prof, etapa, turma, serie)){
        printf("\nClasse cadastrada com sucesso!\n");
    }else{
        printf("\nFalha ao cadastrar classe!!!!\n");
    }
    printf("\n===============================================================\n");
}

int cadastrarClasse(tipoLista *lista, string prof, string etapa, char turma, int serie){
    tipoClasse *novaClasse;
    novaClasse = new tipoClasse;
    if (novaClasse == NULL)
        return 0;
    novaClasse->professor = prof;
    novaClasse->etapa = etapa;
    novaClasse->turma = turma;
    novaClasse->serie = serie;
    novaClasse->primeiroAluno = NULL;
    novaClasse->ultimoAluno = NULL;
    novaClasse->qtdAlunos = 0;
    novaClasse->proxClasse = NULL;
    if (lista->inicio == NULL){
        lista->inicio = novaClasse;
    }
    if (lista->fim != NULL){
        lista->fim->proxClasse = novaClasse;
    }
    lista->fim = novaClasse;
    lista->qtdClasses++;
    return 1;
}

void matriculaAlunoNaClasse(tipoLista* lista){
    //Pesquisar a classe
    tipoClasse* classe;
    char op;
    do{
        waitAndClear(500);
        op = ' ';
        classe = pesquisaClasse(lista);
        if (classe == NULL){
            printf("\n# Esta classe não existe!!! #\n");
            printf("\n>> Aperte <ENTER> para pesquisar novamente.\n");
            printf("\n>> Aperte qualquer outra tecla para voltar ao menu principal.\n");
            op = getch();
        }
    }while (op == '\r');

    if (classe == NULL)
        return;

    waitAndClear(750);
    //Pedir dados do aluno
    tipoAluno* aluno = pedeDadosAluno();
    if (aluno == NULL){
        printf("\n# Algo de errado ocorreu ao tentar cadastrar o aluno. #");
        return;
    }

    //Vincula aluno a classe
    printf("\n===============================================================\n");
    if (vinculaAlunoAClasse(aluno, classe)){
        printf("\nAluno matriculado com sucesso!\n");
    }else{
        printf("\nNão foi possível matricular este aluno!\n");
    }
    printf("\n===============================================================\n");
}

tipoClasse* pesquisaClasse(tipoLista* lista){
    //Interface
    string etapasEnsino[3] = {"Ensino Fundamental I", "Ensino Fundamental II", "Ensino Médio"};
    string etapa;
    int serie, nEtapa;
    char turma;
    printf("\n===============================================================\n");
    printf("\n>> Qual a etapa de estudo?\n");
    printf("      1 - Ensino Fundamental\n");
    printf("      2 - Ensino Médio\n");
    printf("-> Escolha uma opção: ");
    do{
        gotoxy(23, 7); printf("                    ");
        gotoxy(23, 7);
        lerNumero(nEtapa);
    }while (nEtapa != 1 && nEtapa != 2);

    printf("\n===============================================================\n");
    printf("\n>>> Qual a série do aluno? ");
    do{
        gotoxy(28, 11); printf("                    ");
        gotoxy(28, 11);
        lerNumero(serie);
        if (nEtapa == 2 && (serie < 1 || serie > 3))
            serie = 0;
    }while (serie < 1 || serie > 9);
    //Atribui a string correta de acordo com a etapa
    if (nEtapa == 1){
        if (serie >= 1 && serie <= 5)
            etapa = etapasEnsino[0];
        else if (serie >= 6 && serie <= 9)
            etapa = etapasEnsino[1];
    }else{
        etapa = etapasEnsino[2];
    }
    printf("\n===============================================================\n");
    printf("\n>>> Qual é a turma? ");
    turma = cin.get();
    cin.ignore(100, '\n');
    turma = toupper(turma);
    printf("\n===============================================================\n");

    tipoClasse* atual = lista->inicio;
    while (atual != NULL){
        if (atual->etapa == etapa && atual->serie == serie && atual->turma == turma){
            return atual;
        }
        atual = atual->proxClasse;
    }
    return NULL;
}

tipoAluno* pedeDadosAluno(){
    //Interface
    string nome;
    int idade, matricula;
    printf("\n===============================================================\n");
    printf("\n* Digite o nome do aluno: ");
    getline(cin, nome);
    printf("\n===============================================================\n");
    printf("\n* Digite a idade do aluno: ");
    lerNumero(idade);
    printf("\n===============================================================\n");
    printf("\n* Digite o número da matrícula do aluno: ");
    lerNumero(matricula);

    tipoAluno* aluno = new tipoAluno;
    if (aluno == NULL)
        return NULL;
    aluno->nome = nome;
    aluno->idade = idade;
    aluno->matricula = matricula;
    aluno->antAluno = NULL;
    aluno->proxAluno = NULL;
    return aluno;
}

int vinculaAlunoAClasse(tipoAluno* aluno, tipoClasse* classe){
    if (classe->primeiroAluno == NULL){
        classe->primeiroAluno = aluno;
    }
    if (classe->ultimoAluno != NULL){
        aluno->antAluno = classe->ultimoAluno;
        classe->ultimoAluno->proxAluno = aluno;
    }
    classe->ultimoAluno = aluno;
    classe->qtdAlunos++;
    return 1;
}

void removeAlunoDaClasse(tipoLista* lista){
    //Pesquisa a classe na qual se encontra o aluno
    tipoClasse* classe;
    char op;
    do{
        waitAndClear(500);
        op = ' ';
        classe = pesquisaClasse(lista);
        if (classe == NULL){
            printf("\n# Esta classe não existe!!! #\n");
            printf("\n>> Aperte <ENTER> para pesquisar novamente.\n");
            printf("\n>> Aperte qualquer outra tecla para voltar ao menu principal.\n");
            op = getch();
        }
    }while (op == '\r');

    if (classe == NULL)
        return;

    waitAndClear(750);
    //Pesquisa o aluno a ser removido
    tipoAluno* aluno;
    do{
        waitAndClear(500);
        op = ' ';
        aluno = pesquisaAluno(classe);
        if (aluno == NULL){
            printf("\n# Não existe aluno com esta matrícula!!! #\n");
            printf("\n>> Aperte <ENTER> para pesquisar novamente.\n");
            printf("\n>> Aperte qualquer outra tecla para voltar ao menu principal.\n");
            op = getch();
        }
    }while (op == '\r');

    if (aluno == NULL)
        return;

    //Remover aluno
    printf("\n===============================================================\n");
    if (desvinculaAlunoDaClasse(aluno, classe)){
        printf("\nAluno removido com sucesso!\n");
    }else{
        printf("\nNão foi possível remover este aluno!\n");
    }
    printf("\n===============================================================\n");
}

tipoAluno* pesquisaAluno(tipoClasse* classe){
    int matricula;
    printf("\n===============================================================\n");
    printf("\n* Digite o número da matrícula do aluno a ser removido: ");
    lerNumero(matricula);

    tipoAluno* atual = classe->primeiroAluno;
    while (atual != NULL){
        if (atual->matricula == matricula){
            return atual;
        }
        atual = atual->proxAluno;
    }
    return NULL;
}

int desvinculaAlunoDaClasse(tipoAluno* aluno, tipoClasse* classe){
    if (classe->primeiroAluno != aluno)
        aluno->antAluno->proxAluno = aluno->proxAluno;
    if (classe->ultimoAluno != aluno)
        aluno->proxAluno->antAluno = aluno->antAluno;
    if (classe->primeiroAluno == aluno)
        classe->primeiroAluno = aluno->proxAluno;
    if (classe->ultimoAluno == aluno)
        classe->ultimoAluno = aluno->antAluno;
    classe->qtdAlunos--;
    delete aluno;
    aluno = NULL;

    return 1;
}

void exibirListaClasses(tipoLista *lista){
    tipoClasse* atual = lista->inicio;
    while (atual != NULL){
        exibirClasse(atual);
        atual = atual->proxClasse;
        if (atual != NULL)
            printf("\n\n-> Aperte qualquer tecla para mostrar a próxima classe!");
        else
            printf("\n\n-> Aperte qualquer tecla para voltar ao menu principal!");
        getch();
        waitAndClear(1000);
    }
}

void exibirClasse(tipoClasse* classe){
    printf("\n\nInformações sobre a classe:");
    printf("\n\n===========================================================================\n");
    printf("\n   Prof.: %s", classe->professor.c_str());
    printf("\n   Quantidade de alunos matriculados: %d", classe->qtdAlunos);
    printf("\n   Série: %d° ano", classe->serie);
    printf("\n   Turma: %c", classe->turma);
    printf("\n   Etapa de ensino: %s", classe->etapa.c_str());
    printf("\n\n===========================================================================\n");
}
