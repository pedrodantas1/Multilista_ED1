#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cctype>
#include <windows.h>
#include <conio2.h>
using namespace std;

typedef struct noAluno{
    string nome, matricula;
    int idade;
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
int menuPrincipal();
void menuCadastroClasse(tipoLista* lista);
int cadastrarClasse(tipoLista *lista, string prof, string etapa, char turma, int serie);
void matriculaAlunoNaClasse(tipoLista* lista);
tipoClasse* pedeClasse(tipoLista* lista);
tipoClasse* pesquisaClasse(tipoLista* lista);
tipoAluno* pedeDadosAluno();
int vinculaAlunoAClasse(tipoAluno* aluno, tipoClasse* classe);
void removeAlunoDaClasse(tipoLista* lista);
tipoAluno* pedeAluno(tipoClasse* classe);
tipoAluno* pesquisaAluno(tipoClasse* classe, string matricula);
string pedeMatricula();
int desvinculaAlunoDaClasse(tipoAluno* aluno, tipoClasse* classe);
void exibirListaAlunosDaClasse(tipoLista* lista);
void exibirAlunos(tipoClasse* classe);
void exibirListaClasses(tipoLista *lista);
void exibirClasse(tipoClasse* classe);
void removeClasse(tipoLista* lista);
int desvinculaClasseDaLista(tipoClasse* classe, tipoLista* lista);
void exibirClasseAlunoDesejado(tipoLista* lista);

//------------------------------------------------------------

int main(){
    tipoLista lista;
    int opcao;

    //system("chcp 65001>nul"); // UTF-8
    system("chcp 1252>nul");    // Padrao windows

    inicializa(&lista);
    do{
        opcao = menuPrincipal();
        int delay = 500;
        switch (opcao){
        case 1:
            waitAndClear(delay);
            menuCadastroClasse(&lista);
            break;
        case 2:
            if (lista.qtdClasses == 0){
                printf("\n\n\n-> Não existem classes cadastradas!");
                break;
            }
            waitAndClear(delay);
            matriculaAlunoNaClasse(&lista);
            break;
        case 3:
            if (lista.qtdClasses == 0){
                printf("\n\n\n-> Não existem classes cadastradas!");
                break;
            }
            waitAndClear(delay);
            removeAlunoDaClasse(&lista);
            break;
        case 4:
            if (lista.qtdClasses == 0){
                printf("\n\n\n-> Não existem classes cadastradas!");
                break;
            }
            waitAndClear(delay);
            exibirListaAlunosDaClasse(&lista);
            break;
        case 5:
            if (lista.qtdClasses == 0){
                printf("\n\n\n-> Não existem classes cadastradas!");
                break;
            }
            waitAndClear(delay);
            exibirListaClasses(&lista);
            break;
        case 6:
            if (lista.qtdClasses == 0){
                printf("\n\n\n-> Não existem classes cadastradas!");
                break;
            }
            waitAndClear(delay);
            removeClasse(&lista);
            break;
        case 7:
            if (lista.qtdClasses == 0){
                printf("\n\n\n-> Não existem classes cadastradas!");
                break;
            }
            waitAndClear(delay);
            exibirClasseAlunoDesejado(&lista);
            break;
        case 99:
            printf("\n\n\n-> Fechando o programa...");
            break;
        default:
            printf("\n\n\n-> Opção inválida. Digite outra opção!");
            break;
        }
        waitAndClear(1000);
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

int menuPrincipal(){
    int opcao;
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
    printf("\n\n==============================================================\n\n");
    gotoxy(23, 15);
    lerNumero(opcao);

    return opcao;
}

void menuCadastroClasse(tipoLista* lista){
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
            etapa = "Ensino Fundamental I";
        else if (serie >= 6 && serie <= 9)
            etapa = "Ensino Fundamental II";
    }else{
        etapa = "Ensino Médio";
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
    printf("\n>> Aperte qualquer tecla para voltar ao menu principal.");
    getch();
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
    tipoClasse* classe = pedeClasse(lista);

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
    printf("\n>> Aperte qualquer tecla para voltar ao menu principal.");
    getch();
}

tipoClasse* pedeClasse(tipoLista* lista){
    tipoClasse* classe;
    char op;
    do{
        waitAndClear(500);
        op = ' ';
        classe = pesquisaClasse(lista);
        if (classe == NULL){
            printf("\n# Esta classe não existe!!! #\n");
            printf("\n===============================================================\n");
            printf("\n>> Aperte <ENTER> para pesquisar novamente.\n");
            printf("\n>> Aperte qualquer outra tecla para voltar ao menu principal.\n");
            op = getch();
        }
    }while (op == '\r');

    return classe;
}

tipoClasse* pesquisaClasse(tipoLista* lista){
    //Interface
    string etapa;
    int serie, nEtapa;
    char turma;
    printf("\n===============================================================");
    printf("\nPesquisa de classe:");
    printf("\n===============================================================\n");
    printf("\n>> Qual a etapa de estudo?\n");
    printf("      1 - Ensino Fundamental\n");
    printf("      2 - Ensino Médio\n");
    printf("-> Escolha uma opção: ");
    do{
        gotoxy(23, 9); printf("                    ");
        gotoxy(23, 9);
        lerNumero(nEtapa);
    }while (nEtapa != 1 && nEtapa != 2);

    printf("\n===============================================================\n");
    printf("\n>>> Qual a série? ");
    do{
        gotoxy(19, 13); printf("                    ");
        gotoxy(19, 13);
        lerNumero(serie);
        if (nEtapa == 2 && (serie < 1 || serie > 3))
            serie = 0;
    }while (serie < 1 || serie > 9);
    //Atribui a string correta de acordo com a etapa
    if (nEtapa == 1){
        if (serie >= 1 && serie <= 5)
            etapa = "Ensino Fundamental I";
        else if (serie >= 6 && serie <= 9)
            etapa = "Ensino Fundamental II";
    }else{
        etapa = "Ensino Médio";
    }
    printf("\n===============================================================\n");
    printf("\n>>> Qual é a turma? ");
    turma = cin.get();
    cin.ignore(100, '\n');
    turma = toupper(turma);
    printf("\n===============================================================\n");

    //Pesquisa efetivamente a classe (se existir)
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
    string nome, matricula;
    int idade;
    printf("\n===============================================================");
    printf("\nDados do aluno:");
    printf("\n===============================================================\n");
    printf("\n* Digite o nome do aluno: ");
    getline(cin, nome);
    printf("\n===============================================================\n");
    printf("\n* Digite a idade do aluno: ");
    lerNumero(idade);
    printf("\n===============================================================\n");
    printf("\n* Digite o número da matrícula do aluno: ");
    getline(cin, matricula);

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
    tipoClasse* classe = pedeClasse(lista);
    if (classe == NULL)
        return;

    //Pesquisa o aluno a ser removido
    tipoAluno* aluno;
    if (classe->qtdAlunos > 0){
        waitAndClear(750);
        aluno = pedeAluno(classe);
    }else{
        printf("\n-> Não existem alunos matriculados nesta classe!!!\n");
        printf("\n===============================================================\n");
        printf("\n>> Aperte qualquer tecla para voltar ao menu principal.");
        getch();
        return;
    }
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
    printf("\n>> Aperte qualquer tecla para voltar ao menu principal.");
    getch();
}

tipoAluno* pedeAluno(tipoClasse* classe){
    tipoAluno* aluno;
    string matricula;
    char op;
    do{
        waitAndClear(500);
        op = ' ';
        printf("\n===============================================================");
        printf("\nPesquisa de aluno:");
        printf("\n===============================================================\n");
        matricula = pedeMatricula();
        aluno = pesquisaAluno(classe, matricula);
        if (aluno == NULL){
            printf("\n# Não existe aluno com esta matrícula!!! #\n");
            printf("\n===============================================================\n");
            printf("\n>> Aperte <ENTER> para pesquisar novamente.\n");
            printf("\n>> Aperte qualquer outra tecla para voltar ao menu principal.\n");
            op = getch();
        }
    }while (op == '\r');

    return aluno;
}

tipoAluno* pesquisaAluno(tipoClasse* classe, string matricula){
    tipoAluno* atual = classe->primeiroAluno;
    while (atual != NULL){
        if (atual->matricula == matricula){
            return atual;
        }
        atual = atual->proxAluno;
    }
    return NULL;
}

string pedeMatricula(){
    string matricula;
    printf("\n* Digite o número da matrícula do aluno: ");
    getline(cin, matricula);

    return matricula;
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

void exibirListaAlunosDaClasse(tipoLista* lista){
    //Pesquisa a classe que se deseja exibir os alunos
    tipoClasse* classe = pedeClasse(lista);
    if (classe == NULL)
        return;

    //Exibir alunos
    if (classe->qtdAlunos > 0){
        waitAndClear(750);
        exibirAlunos(classe);
    }else{
        printf("\n-> Não existem alunos matriculados nesta classe!!!\n");
        printf("\n===============================================================\n");
    }
    printf("\n>> Aperte qualquer tecla para voltar ao menu principal.");
    getch();
}

void exibirAlunos(tipoClasse* classe){
    printf("\n===========================================================================\n");
    printf("\nAlunos matriculados no %d° ano %c do %s:", classe->serie, classe->turma, classe->etapa.c_str());
    printf("\n\n===========================================================================\n\n");
    tipoAluno* atual = classe->primeiroAluno;
    while (atual != NULL){
        printf("   Nome do aluno: %s\n", atual->nome.c_str());
        printf("   Idade: %d\n", atual->idade);
        printf("   Número da matrícula: %s\n\n", atual->matricula.c_str());
        atual = atual->proxAluno;
    }
    printf("===========================================================================\n");
}

void exibirListaClasses(tipoLista *lista){
    tipoClasse* atual = lista->inicio;
    while (atual != NULL){
        exibirClasse(atual);
        atual = atual->proxClasse;
        if (atual != NULL)
            printf("\n-> Aperte qualquer tecla para mostrar a próxima classe!");
        else
            printf("\n-> Aperte qualquer tecla para voltar ao menu principal!");
        getch();
        waitAndClear(500);
    }
}

void exibirClasse(tipoClasse* classe){
    printf("\n===============================================================\n");
    printf("\nInformações sobre a classe:");
    printf("\n\n===============================================================\n");
    printf("\n   Prof.: %s", classe->professor.c_str());
    printf("\n   Quantidade de alunos matriculados: %d", classe->qtdAlunos);
    printf("\n   Série: %d° ano", classe->serie);
    printf("\n   Turma: %c", classe->turma);
    printf("\n   Etapa de ensino: %s", classe->etapa.c_str());
    printf("\n\n===============================================================\n");
}

void removeClasse(tipoLista* lista){
    //Pesquisa a classe que se quer remover
    tipoClasse* classe = pedeClasse(lista);
    if (classe == NULL)
        return;

    //Remove todos os aluno da classe (se houver)
    while (classe->primeiroAluno != NULL)
        desvinculaAlunoDaClasse(classe->primeiroAluno, classe);

    waitAndClear(750);
    //Remove a classe
    printf("\n===============================================================\n");
    if (desvinculaClasseDaLista(classe, lista)){
        printf("\nClasse removida com sucesso!\n");
    }else{
        printf("\nNão foi possível remover esta classe!\n");
    }
    printf("\n===============================================================\n");
    printf("\n>> Aperte qualquer tecla para voltar ao menu principal.");
    getch();
}

int desvinculaClasseDaLista(tipoClasse* classe, tipoLista* lista){
    tipoClasse* classeAnt = NULL;
    if (lista->inicio != classe){
        classeAnt = lista->inicio;
        while (classeAnt->proxClasse != classe)
            classeAnt = classeAnt->proxClasse;
        classeAnt->proxClasse = classe->proxClasse;
    }else{
        lista->inicio = classe->proxClasse;
    }
    if (lista->fim == classe)
        lista->fim = classeAnt;
    lista->qtdClasses--;
    delete classe;
    classe = NULL;
    return 1;
}

void exibirClasseAlunoDesejado(tipoLista* lista){
    //Perguntar de qual aluno deseja exibir sua classe
    printf("\n===============================================================\n");
    string matricula = pedeMatricula();
    printf("\n===============================================================\n");

    //Varrer todas as classes em busca do aluno
    tipoClasse* classe = lista->inicio;
    while (classe != NULL && pesquisaAluno(classe, matricula) == NULL)
        classe = classe->proxClasse;

    if (classe != NULL){
        waitAndClear(500);
        exibirClasse(classe);
        printf("\n>> Aperte qualquer tecla para voltar ao menu principal.");
        getch();
    }else{
        printf("\n# Não existe aluno com esta matrícula!!! #\n");
        printf("\n===============================================================\n");
        printf("\n>> Aperte qualquer tecla para voltar ao menu principal.");
        getch();
    }
}
