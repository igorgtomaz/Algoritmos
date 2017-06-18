#include "locadora.h"

/* Funções do Sistema */

// Cadastro de clientes

// Pergunta se deseja cadastrar o que está faltando (Ex: Filial, veiculo, etc...)
int valida_nov_cad()
{
    int repeat_cad = 0;

    do
    {
        scanf("%d", &repeat_cad);
        getchar();

        if (repeat_cad == 0)
        {
            system("clear");
            printf("Cadastro finalizado.\n");
            break;
        }
        else if (repeat_cad == 1)
        {
            system("clear");
        }
        else
        {
            printf("\nOpção inválida, favor digitar (1 - Sim / 0 - Não)\n");
        }
    }
    while (repeat_cad != 0 && repeat_cad != 1);

    return repeat_cad;
}

//Valida cpf
int valida_cpf(char cpf_teste[11])
{
    int i, cpf_t[11], soma = 0, dig[2], cont = 10, cont_num = 0, aux = 0;
    char query_sql[200], *erro = NULL;

    strcpy(recebe_select, "");
    sprintf(query_sql, "SELECT cpf FROM cliente WHERE cpf = '%s';", cpf_teste);
    sqlite3_exec(db, query_sql, callback, NULL, &erro);

    if (erro != NULL)
    {
        printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
        grava_erro("Valida id: ");
    }
    else
    {
        if (strcmp(recebe_select, cpf_teste) == 0)
        {
            return 2;
            strcpy(recebe_select, "");
        }
        else if (strcmp(recebe_select, "") == 0 || strcmp(recebe_select, cpf_teste) != 0)
        {
            aux = cpf_teste[0];

            for (i = 1; i < 11; i++)
            {
                if (aux == cpf_teste[i])
                {
                    cont_num++;
                }
                aux = cpf_teste[i];
            }

            if (cont_num >= 10)
            {
                return 0;
            }
            else
            {
                for (i = 0; i < 11; i++)
                {
                    cpf_t[i] = (cpf_teste[i] - 48);
                }

                dig[0] = cpf_t[9];
                dig[1] = cpf_t[10];

                for(i = 0; i < 9; i++)
                {
                    soma += cpf_t[i] * cont;
                    cont--;
                }

                cont = 11;

                if (soma%11 < 2)
                {
                    cpf_t[9] = 0;
                }
                else
                {
                    cpf_t[9] = (11 - (soma%11));
                }

                soma = 0;

                for(i = 0; i < 10; i++)
                {
                    soma += cpf_t[i] * cont;
                    cont--;
                }

                if (soma%11 < 2)
                {
                    cpf_t[10] = 0;
                }
                else
                {
                    cpf_t[10] = (11 - (soma%11));
                }

                if (cpf_t[9] == dig[0] && cpf_t[10] == dig[1])
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    return 0;
}

//Verifica id
int valida_id(char recebe_id[5])
{
    int id_teste[5], i, cont = 9, soma = 0, dig_teste = 0;
    char query_sql[200], *erro = NULL;
    strcpy(recebe_select, "");

    sprintf(query_sql, "SELECT id FROM cliente WHERE id = '%s';", recebe_id);
    sqlite3_exec(db, query_sql, callback, NULL, &erro);

    if (erro != NULL)
    {
        printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
        grava_erro("Valida id: ");
    }
    else
    {
        if (strcmp(recebe_id, recebe_select) == 0)
        {
            return 2;
            strcpy(recebe_select, "");
        }
        else if (strcmp(recebe_select, "") == 0 || strcmp(recebe_id, recebe_select) != 0)
        {
            strcpy(recebe_select, "");
            for (i = 0; i < 5; i++)
            {
                id_teste[i] = (recebe_id[i] - 48);
            }

            for(i = 0; i < 4; i++)
            {
                soma += id_teste[i] * cont;
                cont--;
            }

            if (soma%11 >= 10)
            {
                dig_teste = 0;
            }
            else
            {
                dig_teste = soma%11;
            }

            if (dig_teste == id_teste[4])
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else if (strcmp(recebe_select, "") == 0)
        {
            return 2;
        }

    }
    return 0;
}

// Cadastro de novos clientes
void cad_cliente()
{
    system("clear");
    char p_nome[15], u_nome[20], recebe_cpf[20], recebe_id[5], recebe_senha[6], endereco[100], query_sql[200], *erro_cad = NULL;
    int verif_cad = 0, repeat_cad = 0, i, resp = 0;

    do
    {
        printf("----------------- Cadastro de novos clientes -----------------\n\n");

        printf("Clientes cadastrados:\n");
        exibe_cadastros(1);

        printf("Digite o primeiro nome: ");
        fgets(p_nome, 15, stdin);
        for (i = 0; i < 15; i++)
        {
            if (p_nome[i] == '\n')
                p_nome[i] = '\0';
        }

        printf("Digite o ultimo nome: ");
        fgets(u_nome, 20, stdin);
        for (i = 0; i < 15; i++)
        {
            if (u_nome[i] == '\n')
                u_nome[i] = '\0';
        }

        printf("Digite o CPF: ");
        do
        {
            scanf("%12s", recebe_cpf);

            // Verifica se o cpf digitado possui 11 digitos
            if (strlen(recebe_cpf) == 11)
            {
                //Verifica se o cpf é válido
                recebe_valor = valida_cpf(recebe_cpf);

                if (recebe_valor == 2)
                {
                    printf("\nEste CPF já está cadastrado, digite um CPF diferente: ");
                    verif_cad = 0;
                }
                else if (recebe_valor == 1)
                {
                    verif_cad = 1;
                }
                else
                {
                    printf("CPF inválido, digite novamente: ");
                    verif_cad = 0;
                }
            }
            else
            {
                printf("O CPF deve conter 11 digitos (sem pontuações), digite novamente: ");
            }
        }
        while (verif_cad == 0);
        verif_cad = 0;
        recebe_valor = 0;

        printf("Digite a identificacao: ");
        do
        {
            scanf("%s", recebe_id);

            //Verifica se o tamanho da id digitada é 5
            if (strlen(recebe_id) == 5)
            {
                //Verifica se a id é valida ou se consta no banco
                recebe_valor = valida_id(recebe_id);

                if (recebe_valor == 2)
                {
                    printf("O id já está cadastrado, digite um novo id: ");
                    verif_cad = 0;
                }
                else if (recebe_valor == 1)
                {
                    verif_cad = 1;
                }
                else
                {
                    printf("Id inválido, favor digitar um válido: ");
                }
            }
            else
            {
                printf("A id deve conter 5 digitos, digite uma id válida: ");
            }
        }
        while (verif_cad == 0);

        printf("Digite a senha: ");
        do
        {
            scanf("%s", recebe_senha);
            getchar();

            // Verifica se a senha contem 6 digitos
            if (strlen(recebe_senha) == 6)
            {
                resp = 1;
            }
            else
            {
                printf("A senha deve conter 6 números, favor digitar novamente: ");
            }
        }
        while (resp == 0);

        printf("Digite o endereço: ");
        fgets(endereco, 100, stdin);
        for (i = 0; i < 100; i++)
        {
            if (endereco[i] == '\n')
                endereco[i] = '\0';
        }

        // Insere o cadastro no banco
        sprintf(query_sql, "INSERT INTO cliente (p_nome, u_nome, cpf, id, senha, endereco) values ('%s', '%s', '%s', '%s', %s, '%s');", p_nome, u_nome, recebe_cpf, recebe_id, recebe_senha, endereco);
        sqlite3_exec(db, query_sql, NULL, NULL, &erro_cad);

        printf("\nSalvando cadastro...\n");
        if (erro_cad != NULL)
        {
            printf("ERRO ao realizar INSERT. Verifique o log_erro para saber mais.");
            grava_erro("006 - Cadastrando cliente: ");
        }
        else
        {
            system("clear");
            printf("Cadastro salvo com sucesso.\n\n");
        }

        printf("Deseja cadastrar outro usuário? (1 - Sim / 0 - Não)\n");

        // Verifica se deseja cadastrar ou não.
        repeat_cad = valida_nov_cad();
    }
    while (repeat_cad == 1);
}

// Cadastra novas filiais
void cad_filial()
{

    char nome_filial[100], end_filial[100], query_sql[200], *erro_cad = NULL;
    int i, repeat_cad = 0;

    do
    {
        system("clear");
        printf("----------------- Cadastro de novas filiais -----------------\n\n");

        printf("\nFiliais cadastradas:\n");
        exibe_cadastros(3);

        printf("Digite o nome da filial: ");
        fgets(nome_filial, 100, stdin);
        //Impede do nome e do endereço captar o \n
        for (i = 0; i < 100; i++)
        {
            if (nome_filial[i] == '\n')
                nome_filial[i] = '\0';
        }

        printf("Digite o endereco da filial: ");
        fgets(end_filial, 100, stdin);
        for (i = 0; i < 100; i++)
        {
            if (end_filial[i] == '\n')
                end_filial[i] = '\0';
        }

        // Insere no banco o cadastro
        sprintf(query_sql, "INSERT INTO filial (nome, endereco) values ('%s', '%s');", nome_filial, end_filial);
        sqlite3_exec(db, query_sql, NULL, NULL, &erro_cad);

        printf("\nSalvando cadastro...\n");
        if (erro_cad != NULL)
        {
            printf("ERRO ao realizar INSERT. Verifique o log_erro para saber mais.");
            grava_erro("007 - Cadastrando filial: ");
        }
        else
        {
            printf("\nCadastro salvo com sucesso.\n\n");
        }

        printf("Deseja cadastrar outra filial? (1 - Sim / 0 - Não)\n");

        repeat_cad = valida_nov_cad();
    }
    while (repeat_cad == 1);
}

// Cadastro realizado dentro de outras funções
void perg_cad_filial()
{
    int resp = 0;

    printf("\nFilial não está cadastrada. Deseja cadastrar? (1 - Sim / 0 - Não)\n");
    scanf("%d", &resp);
    getchar();

    if (resp == 1)
    {
        cad_filial();
    }
}

// Cadastra novos veiculos
void cad_veiculo()
{
    system("clear");
    char recebe_placa[8], modelo[20], estado[50], query_sql[200], *erro_cad_v = NULL, resposta[5];
    int i, ano, repeat_cad = 1, resp = 0;
    float recebe_km;

    printf("----------------- Cadastro de novos veiculos -----------------\n\n");

    printf("Veiculos cadastrados:\n");
    exibe_cadastros(2);

    do
    {
        // Verifica se a placa digitada tem 8 digitos, 7 + o h
        printf("Digite a placa do veiculo: ");
        do
        {
            scanf("%s", recebe_placa);

            if (strlen(recebe_placa) == 8)
            {
                resp = 1;
            }
            else
            {
                printf("Placa digitava inválida, digite no seguinte modelo: XXX-XXXX (com hífen).\n");
            }
        }
        while(resp == 0);

        //Torna sempre em letra maiúscula
        for(i = 0; i < 8; i++)
        {
            recebe_placa[i] = toupper(recebe_placa[i]);
        }

        //Verifica se a placa existe
        recebe_valor = verifica_placa(recebe_placa);

        if (recebe_valor == 0)
        {
            strcpy(recebe_select, "");

            getchar();
            printf("Digite o modelo do veiculo: ");
            fgets(modelo, 20, stdin);
            for (i = 0; i < 20; i++)
            {
                if (modelo[i] == '\n')
                    modelo[i] = '\0';
            }


            printf("Digite o ano do veiculo: ");
            scanf("%d", &ano);
            getchar();

            printf("Digite o estado do veiculo: ");
            fgets(estado, 50, stdin);
            for (i = 0; i < 100; i++)
            {
                if (estado[i] == '\n')
                    estado[i] = '\0';
            }

            printf("Digite a quilometragem do veiculo: ");
            scanf("%f", &recebe_km);
            getchar();

            // Aloca o carro cadastrado a uma filial
            printf("Digite o código da filial em que ficará alocado este veiculo: \n");
            exibe_lista_filial();
            printf("\nOpção: ");
            scanf("%d", &resp);
            getchar();
            resposta[0] = (resp+48);
            strcpy(recebe_select, "");

            // Insere o cadastro do veiculo na tabela veiculo
            sprintf(query_sql, "INSERT INTO veiculo (placa, modelo, ano, estado, km) values ('%s', '%s', %d, '%s', %f);", recebe_placa, modelo, ano, estado, recebe_km);
            sqlite3_exec(db, query_sql, NULL, NULL, &erro_cad_v);
            printf("\nSalvando cadastro...\n");
            if (erro_cad_v != NULL)
            {
                printf("ERRO ao realizar INSERT. Verifique o log_erro para saber mais.");
                grava_erro("008 - Cadastrando veiculo: ");
            }
            else
            {
                // Verifica se a filial esta cadastrada
                sprintf(query_sql, "SELECT cod FROM filial WHERE cod = %d;", resp);
                sqlite3_exec(db, query_sql, callback, NULL, &erro_cad_v);
                if (erro_cad_v != NULL)
                {
                    printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
                    grava_erro("009 - Cadastrando veiculo: ");
                }
                else
                {
                    if (strcmp(resposta, recebe_select) == 0)
                    {
                        // Aloca o carro cadastrado a uma filial na tabela transferencia
                        recebe_valor = insert_transf(resp, recebe_placa);

                        if (recebe_valor == 1)
                        {
                            printf("\nCadastro salvo com sucesso.\n\n");
                        }
                    }
                    else if (strcmp(recebe_select, "") == 0)
                    {
                        // Pergunta se deseja cadastrar a filial (após não encontrar a filial cadastrada)
                        perg_cad_filial();
                        printf("\nDeseja colocar o veiculo nesta filial? (1 - Sim / 0 - Não)\n");
                        scanf("%d", &repeat_cad);
                        strcpy(recebe_select, "");

                        if (repeat_cad == 1)
                        {
                            // Aloca o carro cadastrado a uma filial na tabela transferencia
                            insert_transf(resp, recebe_placa);
                        }
                        getchar();
                    }
                }
            }

        }
        else
        {
            strcpy(recebe_select, "");
            system("clear");
            printf("----------------- Cadastro de novos veiculos -----------------\n");
            printf("\nVeiculo já cadastrado, favor digitar uma placa diferente.\n");
            getchar();
        }

        printf("\nDeseja cadastrar outro veiculo? (1 - Sim / 0 - Não)\n");
        repeat_cad = valida_nov_cad();

        if (repeat_cad == 1)
        {
            system("clear");
            printf("----------------- Cadastro de novos veiculos -----------------\n\n");
        }
    }
    while (repeat_cad == 1);
}

//Pergunta se deseja cadastrar novo
int perg_cad_vei()
{
    int resp;

    printf("\nA placa digitada pertence a um veículo não cadastrado, deseja cadastrar?\n(1 - Sim / 0 - Não)\n");
    scanf("%d", &resp);
    getchar();

    if (resp == 1)
    {
        cad_veiculo();
        return 1;
    }
    else
    {
        return 0;
    }
}

// Aluga os carros
void aluga_carro()
{
    system("clear");
    char recebe_placa[8], query_sql[200], *erro_alug = NULL, resposta[5];
    char recebe_id[5], recebe_senha[6];
    int i, repeat_cad = 0, resp = 0, resp2 = 0, resp3 = 0;

    do
    {
        printf("----------------- Locação de carros -----------------\n\n");

        printf("Veiculos disponiveis:\n");
        exibe_cadastros(5);

        printf("Digite a placa do carro que deseja alugar: ");
        // Verifica se a placa digitada tem 8 digitos, 7 + o h
        do
        {
            scanf("%s", recebe_placa);

            if (strlen(recebe_placa) == 8)
            {
                resp = 1;
            }
            else
            {
                printf("Placa digitava inválida, digite no seguinte modelo: XXX-XXXX (com hífen).\n");
            }
        }
        while(resp == 0);

        //Torna sempre em letra maiúscula
        for(i = 0; i < 8; i++)
        {
            recebe_placa[i] = toupper(recebe_placa[i]);
        }

        // Verifica se o carro existe
        recebe_valor = verifica_placa(recebe_placa);

        if (recebe_valor == 1)
        {
            strcpy(recebe_select, "");

            // Verifica se o carro esta alugado ou não
            sprintf(query_sql, "SELECT ativo FROM aluguel WHERE data_atual = (SELECT MAX(data_atual) FROM aluguel WHERE placa_v = '%s') and placa_v = '%s';", recebe_placa, recebe_placa);
            sqlite3_exec(db, query_sql, callback, NULL, &erro_alug);
            strcpy(resposta, recebe_select);
            strcpy(recebe_select, "");

            if (erro_alug != NULL)
            {
                printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
                grava_erro("010 - Alugando veiculo: ");
            }
            else
            {
                if (strcmp(resposta, "") == 0 || strcmp(resposta, "0") == 0)
                {
                    getchar();

                    printf("\nClientes cadastrados:\n");
                    exibe_cadastros(1);

                    printf("Digite o id do cliente: ");

                    resp = 0;

                    do
                    {
                        strcpy(recebe_id, "");
                        scanf("%5s", recebe_id);

                        recebe_valor = verif_id(recebe_id);
                        if (recebe_valor == 1)
                        {
                            printf("Digite a senha: ");

                            do
                            {
                                resp2 = 0;

                                strcpy(recebe_select, "");
                                scanf("%6s", recebe_senha);

                                recebe_valor = verif_senha(recebe_id, recebe_senha);

                                if (recebe_valor == 1 && strlen(recebe_senha) == 6)
                                {
                                    strcpy(recebe_select, "");

                                    // Pega o cpf para colocar na transferencia
                                    sprintf(query_sql, "SELECT cpf FROM cliente WHERE id = '%s';", recebe_id);
                                    sqlite3_exec(db, query_sql, callback, NULL, &erro_alug);

                                    if (erro_alug != NULL)
                                    {
                                        printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
                                        grava_erro("013 - Alugando veiculo: ");
                                    }
                                    else
                                    {
                                        resp = 1;
                                        resp2 = 1;
                                        insert_alug(recebe_select, recebe_placa);
                                        strcpy(recebe_select, "");
                                        printf("\nVeiculo alugado com sucesso.\n");
                                        repeat_cad = 1;
                                    }

                                }
                                else
                                {
                                    system("clear");
                                    printf("Senha incorreta, favor digitar novamente: ");
                                }
                            }
                            while (resp2 == 0);
                        }
                        else
                        {
                            system("clear");
                            printf("ID não identificado, favor digitar o id corretamente: ");
                        }

                    }
                    while (resp == 0);
                }
                else
                {
                    strcpy(recebe_select, "");

                    // Pega o nome do cliente que alugou o carro
                    sprintf(query_sql, "SELECT p_nome FROM cliente c JOIN aluguel a on c.cpf = a.cpf_c WHERE data_atual = (SELECT MAX(data_atual) FROM aluguel WHERE placa_v = '%s') and placa_v = '%s';", recebe_placa, recebe_placa);
                    sqlite3_exec(db, query_sql, callback, NULL, &erro_alug);
                    if (erro_alug != NULL)
                    {
                        printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
                        grava_erro("014 - Alugando veiculo: ");
                    }
                    else
                    {
                        printf("\nO carro de placa %s, já esta alugado para o cliente %s.\n", recebe_placa, recebe_select);
                        strcpy(recebe_select, "");
                        repeat_cad = 1;
                    }

                }
            }
        }
        else
        {
            strcpy(recebe_select, "");
            resp3 = perg_cad_vei();

            if (resp3 == 1)
            {
                printf("\nDeseja alugar este carro? (1 - Sim / 0 - Não)\n");
                scanf("%d", &resp3);

                if (resp3 == 1)
                {
                    do
                    {
                        printf("Digite a id do cliente: ");
                        scanf("%5s", recebe_id);

                        resp = verif_id(recebe_id);

                        if (resp == 1)
                        {
                            printf("Digite a senha: ");
                            scanf("%6s", recebe_senha);

                            resp = verif_senha(recebe_id, recebe_senha);

                            if (resp == 1)
                            {
                                // Insere na tabela o aluguel do carro
                                recebe_valor = insert_alug(recebe_id, recebe_placa);

                                if (recebe_valor == 1)
                                {
                                    printf("\nVeiculo alugado com sucesso.\n");
                                    repeat_cad = 1;
                                }
                                repeat_cad = 1;
                            }
                            else
                            {
                                printf("id inválida, favor digite uma senha correta.\n");
                            }
                        }
                        else
                        {
                            printf("id inválida, favor digite uma id correta.\n Deseja tentar outra vez? (1 - Sim / 0 - Não)");
                            scanf("%d", &resp3);
                        }
                    }
                    while (resp3 == 1);
                }
            }
        } // FIM verifica placa

        printf("\nDeseja realizar a locação de um outro veiculo? (1 - Sim / 0 - Não)\n");
        repeat_cad = valida_nov_cad();
        strcpy(recebe_select, "");

    }
    while (repeat_cad == 1);

    strcpy(recebe_select, "");
}

// Função que devolve carro
void devolve_car()
{
    char recebe_placa[8], query_sql[200], *erro_devol = NULL, cliente[15], resposta[5];
    int i, repeat_cad = 0, resp = 0;
    float recebe_km;

    system("clear");
    printf("----------------- Devolução de carros -----------------\n");

    printf("\nVeiculos alugados:\n");
    exibe_cadastros(6);

    do
    {

        printf("\nDigite a placa do carro que deseja devolver: ");

        do
        {
            // Verifica se a placa digitada tem 8 digitos, 7 + o h

            scanf("%s", recebe_placa);

            if (strlen(recebe_placa) == 8)
            {
                repeat_cad = 0;
            }
            else
            {
                printf("Placa digitava inválida, digite no seguinte modelo: XXX-XXXX (com hífen).\n");
                repeat_cad = 1;
            }

            if (repeat_cad == 0)
            {

                //Torna sempre em letra maiúscula
                for(i = 0; i < 8; i++)
                {
                    recebe_placa[i] = toupper(recebe_placa[i]);
                }

                //Verifica a placa na tabela veiculo
                recebe_valor = verifica_placa(recebe_placa);

                if (recebe_valor == 1)
                {
                    if (strcmp(recebe_placa, recebe_select) == 0)
                    {
                        strcpy(recebe_select, "");

                        //pesquisa se o status ta ativo ou não (alugado ou não)
                        sprintf(query_sql, "SELECT ativo FROM aluguel WHERE data_atual = (SELECT MAX(data_atual) FROM aluguel where placa_v = '%s') and placa_v = '%s';", recebe_placa, recebe_placa);
                        sqlite3_exec(db, query_sql, callback, NULL, &erro_devol);

                        if (erro_devol != NULL)
                        {
                            printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
                            grava_erro("015 - Devolvendo veiculo: ");
                        }
                        else
                        {
                            if (strcmp(recebe_select, "1") == 0)
                            {
                                strcpy(recebe_select, "");

                                //Atualiza a km usada
                                printf("Digite a quilometragem atual: ");
                                scanf("%f", &recebe_km);

                                // Pega o cliente que alugou o carro
                                sprintf(query_sql, "SELECT p_nome FROM cliente c JOIN aluguel a on c.cpf = a.cpf_c WHERE a.placa_v = '%s' and a.data_atual = (SELECT MAX(data_atual) from aluguel WHERE placa_v = '%s');", recebe_placa, recebe_placa);
                                sqlite3_exec(db, query_sql, callback, NULL, &erro_devol);
                                strcpy(cliente, recebe_select);

                                if (erro_devol != NULL)
                                {
                                    printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
                                    grava_erro("017 - Devolvendo veiculo: ");
                                }
                                else
                                {
                                    printf("\nO veiculo de placa %s encontra-se alugado para o cliente %s\n\n", recebe_placa, cliente);
                                    strcpy(recebe_select, "");
                                }

                                printf("Deseja devolver para qual filial?\n");
                                exibe_lista_filial();
                                printf("\nOpção: ");
                                scanf("%d", &resp);
                                resposta[0] = (resp+48);

                                //Verifica se a filial para qual vai devolver existe
                                sprintf(query_sql, "SELECT cod FROM filial WHERE cod = '%s';", resposta);
                                sqlite3_exec(db, query_sql, callback, NULL, &erro_devol);

                                if (erro_devol != NULL)
                                {
                                    printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
                                    grava_erro("018 - Devolvendo veiculo: ");
                                }
                                else
                                {
                                    if(strcmp(resposta, recebe_select) == 0)
                                    {
                                        // Inserte na tabela filial a transferencia
                                        recebe_valor = insert_transf(resp, recebe_placa);

                                        if (recebe_valor == 1)
                                        {
                                            // Pega o nome da filial para o qual está devolvendo
                                            recebe_valor = verifica_nome_filial(resp);
                                            if (recebe_valor == 1)
                                            {
                                                printf("\nVeículo devolvido para a filial %s com sucesso.\n\n", recebe_select);
                                                strcpy(recebe_select, "");
                                                repeat_cad = 1;
                                            }


                                            // Atualiza na tabela veiculo o km do carro
                                            sprintf(query_sql, "UPDATE veiculo SET km = %f WHERE placa = '%s';", recebe_km,recebe_placa);
                                            sqlite3_exec(db, query_sql, callback, NULL, &erro_devol);
                                            if (erro_devol != NULL)
                                            {
                                                printf("ERRO ao realizar UPDATE. Verifique o log_erro para saber mais.");
                                                grava_erro("016 - Devolvendo veiculo: ");
                                            }


                                            // Modifica o status do carro para não alugado
                                            sprintf(query_sql, "UPDATE aluguel SET ativo = 0 WHERE placa_v = '%s';", recebe_placa);
                                            sqlite3_exec(db, query_sql, callback, NULL, &erro_devol);

                                            if (erro_devol != NULL)
                                            {
                                                printf("ERRO ao realizar UPDATE. Verifique o log_erro para saber mais.");
                                                grava_erro("019 - Devolvendo veiculo: ");
                                            }
                                        }
                                    }
                                    else if (strcmp(recebe_select, "") == 0)
                                    {
                                        perg_cad_filial();
                                        repeat_cad = 1;
                                    }
                                    strcpy(recebe_select, "");
                                }

                            }
                            else if (strcmp(recebe_select, "0") == 0 || strcmp(recebe_select, "") == 0)
                            {
                                strcpy(recebe_select, "");

                                // pega o nome da filial onde o carro está
                                sprintf(query_sql, "SELECT nome FROM filial f JOIN transferencia t on f.cod = t.cod_f WHERE t.data_atual = (SELECT MAX(data_atual) FROM transferencia WHERE placa_v = '%s') AND t.placa_v = '%s';", recebe_placa, recebe_placa);
                                sqlite3_exec(db, query_sql, callback, NULL, &erro_devol);
                                if (erro_devol != NULL)
                                {
                                    printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
                                    grava_erro("020 - Devolvendo veiculo: ");
                                }
                                else
                                {
                                    printf("\nAtenção: O veiculo não foi alugado e encontra-se disponível na filial %s.\n\n", recebe_select);
                                    strcpy (recebe_select, "");
                                    repeat_cad = 1;
                                }
                            }
                        }
                    }
                    else if(strcmp(recebe_select, "") == 0)
                    {
                        perg_cad_vei();
                        repeat_cad = 1;
                    }
                }
                else
                {
                    strcpy(recebe_select, "");
                    printf("\nPlaca inválida, favor digitar uma placa diferente.\n\n");
                    repeat_cad = 1;
                    getchar();
                }

            }
        }
        while (repeat_cad == 0);

        printf("Deseja devolver um outro veiculo? (1 - Sim / 0 - Não)\n");
        repeat_cad = valida_nov_cad();
    }
    while (repeat_cad == 1);
    strcpy(recebe_select, "");
}

void mover_carros()
{
    system("clear");
    char recebe_placa[9], query_sql[200], *erro_mov = NULL, resposta[5];
    int i, repeat_cad = 0, resp = 0;

    do
    {
        printf("----------------- Movimentação de carros -----------------\n\n");

        printf("Veiculos disponiveis para movimentação:\n");
        exibe_cadastros(5);

        printf("\nDigite a placa do carro que deseja mover: ");

        do
        {
            // Verifica se a placa digitada tem 8 digitos, 7 + o h

            scanf("%s", recebe_placa);
            getchar();

            if (strlen(recebe_placa) == 8)
            {
                repeat_cad = 0;
            }
            else
            {
                printf("\nPlaca digitava inválida, digite no seguinte modelo: XXX-XXXX (com hífen).\n");
                repeat_cad = 1;
            }

            if (repeat_cad == 0)
            {

                //Torna sempre em letra maiúscula
                for(i = 0; i < 8; i++)
                {
                    recebe_placa[i] = toupper(recebe_placa[i]);
                }

                //Confere se o carro existe no banco
                recebe_valor = verifica_placa(recebe_placa);

                if (recebe_valor == 1)
                {
                    if (strcmp(recebe_placa, recebe_select) == 0)
                    {
                        strcpy(recebe_select, "");
                        printf("\nDeseja mover para qual filial?\n");
                        exibe_lista_filial();
                        printf("\nOpção: ");
                        scanf("%s", resposta);

                        resp = (resposta[0] - 48);

                        strcpy(recebe_select, "");

                        //Pega o codigo da ultima filial com aquela placa, se for a mesma não insere no banco
                        sprintf(query_sql, "SELECT cod_f FROM transferencia t WHERE t.data_atual = (SELECT MAX(data_atual) FROM transferencia WHERE placa_v = '%s') AND t.placa_v = '%s';", recebe_placa, recebe_placa);
                        sqlite3_exec(db, query_sql, callback, NULL, &erro_mov);
                        if (erro_mov != NULL)
                        {
                            printf("ERRO ao realizar SELECT. Verifique o log_erro para saber mais.");
                            grava_erro("021 - Movimentação de veiculo: ");
                        }
                        else
                        {
                            if (strcmp(resposta, recebe_select) == 0)
                            {
                                strcpy(recebe_select, "");
                                printf("\nO veiculo já encontra-se nesta filial.\n");
                                repeat_cad = 1;
                            }
                            else if (strcmp(recebe_select, "") == 0)
                            {
                                recebe_valor = verifica_placa(recebe_placa);

                                if (recebe_valor == 1)
                                {
                                    if (strcmp(recebe_select, "") == 0)
                                    {
                                        perg_cad_vei();
                                    }
                                    else if (strcmp(recebe_placa, recebe_select) == 0)
                                    {
                                        strcpy(recebe_select, "");

                                        // Inserte na tabela filial a transferencia
                                        recebe_valor = insert_transf(resp, recebe_placa);

                                        if (recebe_valor == 1)
                                        {
                                            // Pega o nome da filial para qual foi movido o veiculo
                                            recebe_valor = verifica_nome_filial(resp);
                                            if (recebe_valor == 1)
                                            {
                                                printf("\nVeiculo movido para a filial %s com sucesso.\n", recebe_select);
                                                strcpy(recebe_select, "");
                                                repeat_cad = 1;
                                            }
                                        }

                                    }
                                }
                            }
                            else if (strcmp(resposta, recebe_select) != 0 || strcmp(resposta, "") != 0)
                            {
                                strcpy(recebe_select, "");
                                // Inserte na tabela filial a transferencia
                                recebe_valor = insert_transf(resp, recebe_placa);

                                if (recebe_valor == 1)
                                {
                                    // Pega o nome da filial para qual foi movido o veiculo
                                    recebe_valor = verifica_nome_filial(resp);
                                    if (recebe_valor == 1)
                                    {
                                        printf("\nVeiculo movido para a filial %s com sucesso.\n", recebe_select);
                                        strcpy(recebe_select, "");
                                        repeat_cad = 1;
                                    }
                                }
                            }
                        }

                    }
                    else if (strcmp(recebe_select, "") == 0)
                    {
                        perg_cad_vei();
                    }
                }
                else
                {
                    strcpy(recebe_select, "");
                    printf("\nPlaca inválida, favor digitar uma placa diferente.\n");
                    repeat_cad = 1;
                }

            }
        }
        while (repeat_cad == 0);

        printf("\nDeseja mover outro veiculo? (1 - Sim / 0 - Não)\n");
        repeat_cad = valida_nov_cad();
    }
    while (repeat_cad == 1);
}

void menu()
{
  int menu = 0;
  system("clear");

  printf("\n\n----------------- Bem vindo a LoCar -----------------\n");

  while (menu != 10)
  {

      printf("\n----------------- Menu de Opções -----------------\n");

      printf("\nSelecione a opção que deseja:\n\n1 - Cadastro de novos clientes\n2 - Cadastro de novos veiculos\n3 - Cadastro de novas filiais\n4 - Locação de carros\n5 - Movimentar carros\n6 - Devolver carros\n7 - Listar clientes\n8 - Listar veiculos\n9 - Listar filiais\n10 - Sair\n");
      printf("\nOpção: ");
      scanf("%d", &menu);
      getchar();

      if (menu == 1)
      {
          cad_cliente();
      }
      else if (menu == 2)
      {
          cad_veiculo();
      }
      else if (menu == 3)
      {
          cad_filial();
      }
      else if (menu == 4)
      {
          aluga_carro();
      }
      else if (menu == 5)
      {
          mover_carros();
      }
      else if (menu == 6)
      {
          devolve_car();
      }
      else if (menu == 7)
      {
          system("clear");
          printf("Clientes cadastrados:\n");
          exibe_cadastros(1);
      }
      else if (menu == 8)
      {
          system("clear");
          printf("Veiculos cadastrados:\n");
          exibe_cadastros(4);
      }
      else if (menu == 9)
      {
          system("clear");
          printf("Filiais cadastradas:\n");
          exibe_cadastros(3);
      }
      else if (menu == 10)
      {
          system("clear");
          printf("Programa finalizado.\n");
          sqlite3_close(db);
          break;
      }
      else
      {
          printf("Opção inválida, favor digitar uma opção correta.\n");
      }
  }
}

/* Fim das funções dos sistema */
