#include "locadora.h"

//BD
// Usado pra o resultado dos Selects
char recebe_select[200];

// Recebe o retorno das funções de BD
int recebe_valor = 0;

// Matriz com nomes das filiais
char nome_filial[100][20];

// Arquivo de Log
FILE *arquivo_log;

/* Fim das variaveis globais */

/* Logs*/
int grava_erro(char recebe_s[300])
{
    time_t curtime;
    char erro[500];

    time(&curtime);

    arquivo_log = fopen("log_erro.txt", "w");
    if (arquivo_log == NULL)
    {
        printf("Erro ao gerar log!");
    }

    sprintf(erro, "Local: %s. Erro: %s. Data: %s", recebe_s, sqlite3_errmsg(db), ctime(&curtime));
    fprintf(arquivo_log, "%s", erro);

    fclose(arquivo_log);
    return 0;
}

/* Funções do BD */

//Função que exibe o select feito.
int callback(void *arg, int argc, char **argv, char **colName)
{
    //Recebe o resultado do select
    strcat(recebe_select, *argv);
    return 0;
}

int setback (void *data, int argc, char **argv, char **azColName)
{
    int i;

    for(i=0; i<argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

// Função que verifica se já tem BD ou não
int fileExists(char *cpfileName)
{
    int errcode = access(cpfileName, F_OK);
    return errcode;
}

// Verifica se já existe um BD, se não cria um
void cria_bd()
{
    int resp_proc = fileExists("locadora.db");
    int resp_banco;
    char *erro_c = NULL, *erro_v = NULL, *erro_f = NULL, *erro_a = NULL, *erro_tr = NULL, *erro_th = NULL, *erro_h = NULL;

    if (resp_proc != 0)
    {
        // Criando o banco
        printf("Criando novo banco de dados...\n");
        resp_banco = sqlite3_open("locadora.db", &db);

        if (resp_banco)
        {
            printf("Não foi possivel criar o banco. Verifique o log_erro para saber mais.\n");
            grava_erro("000 Criando banco - ");
        }
        else
        {
            printf("Banco de dados criado com sucesso.\n");
        }

        // Criação das tabelas do banco
        sqlite3_exec(db, "CREATE TABLE cliente (p_nome CHAR(15) NOT NULL, u_nome CHAR(20) NOT NULL, cpf CHAR(11) NOT NULL, id CHAR(5) NOT NULL, senha CHAR(6) NOT NULL, endereco CHAR(100), CONSTRAINT PK_Cadastro_Cliente PRIMARY KEY (cpf, id));", NULL, NULL, &erro_c);
        sqlite3_exec(db, "CREATE TABLE veiculo (placa CHAR(8) NOT NULL, modelo CHAR(20) NOT NULL, ano INT NOT NULL, estado CHAR(50) NOT NULL, km REAL NOT NULL, CONSTRAINT PK_Cadastro_Veiculos PRIMARY KEY (placa));", NULL, NULL, &erro_v);
        sqlite3_exec(db, "CREATE TABLE filial (cod INTEGER PRIMARY KEY AUTOINCREMENT, nome CHAR(100) NOT NULL, endereco CHAR(100) NOT NULL);", NULL, NULL, &erro_f);
        sqlite3_exec(db, "CREATE TABLE aluguel (cod_a INTEGER PRIMARY KEY AUTOINCREMENT, cpf_c CHAR(11) NOT NULL, placa_v CHAR(8) NOT NULL, ativo INT, data_atual CHAR(50) NOT NULL, CONSTRAINT FK_Aluguel_Cliente FOREIGN KEY(cpf_c) REFERENCES cliente (cpf), CONSTRAINT FK_Aluguel_Veiculo FOREIGN KEY(placa_v) REFERENCES veiculo (placa));", NULL, NULL, &erro_a);
        sqlite3_exec(db, "CREATE TABLE transferencia (cod_t INTEGER PRIMARY KEY AUTOINCREMENT, cod_f INT NOT NULL, placa_v CHAR(8) NOT NULL, data_atual CHAR(50) NOT NULL, CONSTRAINT FK_Transf_Filial FOREIGN KEY (cod_f) REFERENCES filial (cod), CONSTRAINT FK_Transf_Veiculo FOREIGN KEY (placa_v) REFERENCES veiculo (placa));", NULL, NULL, &erro_tr);

        // Aviso de erro na criação das tabelas
        if (erro_c != NULL)
        {
            printf("Não foi possível criar a tabela cliente. Verifique o log_erro para saber mais.\n");
            grava_erro("001 - Criando tabela cliente - ");
        }
        if (erro_v != NULL)
        {
            printf("Não foi possível criar a tabela veiculo. Verifique o log_erro para saber mais.\n");
            grava_erro("001 - Criando tabela veiculo -");
        }
        if (erro_f != NULL)
        {
            printf("Não foi possível criar a tabela filial. Verifique o log_erro para saber mais.\n");
            grava_erro("001 - Criando tabela filial -");
        }
        if (erro_a != NULL)
        {
            printf("Não foi possível criar a tabela aluguel. Verifique o log_erro para saber mais.\n");
            grava_erro("001 - Criando tabela aluguel -");
        }
        if (erro_tr != NULL)
        {
            printf("Não foi possível criar a tabela transferencia. Verifique o log_erro para saber mais.\n");
            grava_erro("001 - Criando tabela transferencia -");
        }
        if (erro_th != NULL)
        {
            printf("Não foi possível criar a tabela tipo_historico. Verifique o log_erro para saber mais.\n");
            grava_erro("001 - Criando tabela tabela tipo_historico -");
        }
        if (erro_h != NULL)
        {
            printf("Não foi possível criar a tabela historico. Verifique o log_erro para saber mais.\n");
            grava_erro("001 - Criando tabela historico -");
        }
        if (erro_h != NULL)
        {
            printf("Não foi possível inserir os valores na tabela tipo_historico. Verifique o log_erro para saber mais.\n");
            grava_erro("001 - Inserindo valores na tabela -");
        }
    }
    else
    {
        printf("\nBanco de dados existente - Conectando ao banco...");
        resp_banco = sqlite3_open("locadora.db", &db);
    }
}

//Exibe lista de cadastros
void exibe_cadastros(int answer)
{
    char query_sql[300], *erro = NULL;

    printf("\n-------------------------------------------------------------\n\n");
    if (answer == 1)
    {
        sprintf(query_sql, "SELECT p_nome as Nome, u_nome as Sobrenome, cpf as CPF, id as ID, senha as Senha, endereco as Endereço FROM cliente;");
        sqlite3_exec(db, query_sql, setback, NULL, &erro);
    }
    else if (answer == 2)
    {
        sprintf(query_sql, "SELECT placa as Placa, modelo as Modelo, ano as Ano, estado as Estado, km as quilometragem from veiculo;");
        sqlite3_exec(db, query_sql, setback, NULL, &erro);
    }
    else if (answer == 3)
    {
        sprintf(query_sql, "SELECT cod as Codigo, nome as Nome, endereco as Endereço FROM filial;");
        sqlite3_exec(db, query_sql, setback, NULL, &erro);
    }
    else if (answer == 4)
    {
        sprintf(query_sql, "SELECT v.placa as Placa, v.modelo as Modelo, v.ano as Ano, v.km as Quilometragem, (SELECT CASE (a.ativo) WHEN 1 THEN 'Sim' ELSE 'Não' END) as Alugado FROM veiculo v LEFT JOIN aluguel a on v.placa = a.placa_v;");
        sqlite3_exec(db, query_sql, setback, NULL, &erro);
    }
    else if (answer == 5)
    {
        sprintf(query_sql, "SELECT v.placa as Placa, v.modelo as Modelo, v.ano as Ano, v.estado as Estado, v.km as Quilometragem FROM veiculo v LEFT JOIN aluguel a on v.placa = a.placa_v WHERE a.ativo = 0 or a.ativo is null;");
        sqlite3_exec(db, query_sql, setback, NULL, &erro);
    }
    else if (answer == 6)
    {
        sprintf(query_sql, "SELECT v.placa as Placa, v.modelo as Modelo, v.ano as Ano, v.estado as Estado, v.km as Quilometragem FROM veiculo v LEFT JOIN aluguel a on v.placa = a.placa_v WHERE a.ativo = 1;");
        sqlite3_exec(db, query_sql, setback, NULL, &erro);
    }

    printf("-------------------------------------------------------------\n\n");

}

// Exibe as filiais cadastradas
void exibe_lista_filial()
{
    char query_sql[200], *erro = NULL;

    int cont = 1;

    printf("\nFiliais: \n");
    while (1)
    {
        sprintf(query_sql, "SELECT nome FROM filial WHERE cod = %d;", cont);
        sqlite3_exec(db, query_sql, callback, NULL, &erro);

        if (strcmp(recebe_select, "") != 0)
        {
            printf("%d - %s\n", cont, recebe_select);
            cont++;
            strcpy(recebe_select, "");
        }
        else
        {
            strcpy(recebe_select, "");
            break;
        }
    }
}

// Verifica se o ID existe no banco
int verif_id(char recebe_id[5])
{
    char *erro, query_sql[200];
    strcpy(recebe_select, "");

    sprintf(query_sql, "SELECT id FROM cliente WHERE id = '%s';", recebe_id);
    sqlite3_exec(db, query_sql, callback, NULL, &erro);

    if (erro != NULL)
    {
        printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
        grava_erro("011 - Alugando veiculo: ");
    }
    else
    {
        if (strcmp(recebe_select, recebe_id) == 0 && strlen(recebe_id) == 5)
        {
            return 1;
            strcpy(recebe_select, "");
        }
        else
        {
            return 0;
            strcpy(recebe_select, "");
        }
    }
    return 0;
}

// Verifica se a senha bate com o ID digitado
int verif_senha(char recebe_id[5], char recebe_senha[6])
{
    char *erro, query_sql[200];
    strcpy(recebe_select, "");

    sprintf(query_sql, "SELECT senha FROM cliente WHERE id = '%s';", recebe_id);
    sqlite3_exec(db, query_sql, callback, NULL, &erro);

    if (erro != NULL)
    {
        printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
        grava_erro("011 - Alugando veiculo: ");
    }
    else
    {
        if (strcmp(recebe_select, recebe_senha) == 0)
        {
            return 1;
            strcpy(recebe_select, "");
        }
        else
        {
            return 0;
            strcpy(recebe_select, "");
        }
    }
    return 0;
}

// Busca nome da filial
int verifica_nome_filial(int resp)
{
    char query_sql[200], *erro;
    strcpy(recebe_select, "");

    sprintf(query_sql, "SELECT nome FROM filial WHERE cod = %d;", resp);
    sqlite3_exec(db, query_sql, callback, NULL, &erro);
    if (erro != NULL)
    {
        printf("ERRO ao realizar INSERT. Verifique o log_erro para saber mais.");
        grava_erro("002 - Verificando nome da filial - ");
        return 0;
    }
    else
    {
        return 1;
    }
}

// Insere na tabela transferencia
int insert_transf(int resp, char recebe_placa[8])
{
    char query_sql[200], *erro;

    sprintf(query_sql, "INSERT INTO transferencia (cod_f, placa_v, data_atual) values ('%d', '%s', DATETIME(CURRENT_TIMESTAMP, 'LOCALTIME'));", resp, recebe_placa);
    sqlite3_exec(db, query_sql, callback, NULL, &erro);

    if (erro != NULL)
    {
        printf("ERRO ao realizar INSERT. Verifique o log_erro para saber mais.");
        grava_erro("003 - Inserindo transferência - ");
        return 0;
    }
    else
    {
        return 1;
    }
}

// Verifica se a placa consta no banco
int verifica_placa(char recebe_placa[8])
{
    char query_sql[200], *erro;
    strcpy(recebe_select, "");
    sprintf(query_sql, "SELECT placa FROM veiculo WHERE placa = '%s';", recebe_placa);
    sqlite3_exec(db, query_sql, callback, NULL, &erro);

    if (erro != NULL)
    {
        printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
        grava_erro("004 - Verificando placa -");
        return 0;
    }
    else
    {
        if (strcmp(recebe_placa, recebe_select) == 0)
        {
            return 1;
        }
        else
            return 0;
    }
}

// Insere na tabela o registro do aluguel do carro
int insert_alug(char cpf[11], char recebe_placa[8])
{
    char query_sql[200], *erro;

    sprintf(query_sql, "INSERT INTO aluguel (cpf_c, placa_v, ativo, data_atual) values ('%s', '%s', 1, DATETIME(CURRENT_TIMESTAMP, 'LOCALTIME'));", cpf, recebe_placa);
    sqlite3_exec(db, query_sql, callback, NULL, &erro);

    if (erro != NULL)
    {
        printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
        grava_erro("005 - Inserindo aluguel");
        return 0;
    }
    else
        return 1;
}

/* Fim das funções do BD */
