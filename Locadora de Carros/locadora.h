//Banco de dados

sqlite3_stmt *stmt;
sqlite3 *db;

int grava_erro(char recebe_s[300]);
int callback(void *arg, int argc, char **argv, char **colName);
int setback (void *data, int argc, char **argv, char **azColName);
int fileExists(char *cpfileName);
void cria_bd();
void exibe_cadastros(int answer);
void exibe_lista_filial();
int verif_id(char recebe_id[5]);
int verif_senha(char recebe_id[5], char recebe_senha[6]);
int verifica_nome_filial(int resp);
int insert_transf(int resp, char recebe_placa[8]);
int verifica_placa(char recebe_placa[8]);
int insert_alug(char cpf[11], char recebe_placa[8]);

//Fim das funções do banco

//Funções do Sistema

int valida_nov_cad();
int valida_cpf(char cpf_teste[11]);
int valida_id(char recebe_id[5]);
void cad_cliente();
void cad_filial();
void perg_cad_filial();
void cad_veiculo();
int perg_cad_vei();
void aluga_carro();
void devolve_car();
void mover_carros();
void menu();

//Fim das funções do sistema
