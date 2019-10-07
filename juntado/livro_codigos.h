#ifndef LIVRO_CODIGO
#define LIVRO_CODIGO
#include <stdio.h>


#define ORDEM 5
#define file "bdcodigos.bin"

typedef struct nodeBMais
{
    int ponteiro[ORDEM + 1]; //mais um somente para nao quebrar na funcao de arrumar o pai
    int chave[ORDEM];        //a ultima chave eh para overflow
    int pai;
    int eh_folha; //se este no estiver numa posicao que foi apagada , eh_folha vai ter a posicao do proximo no apagado para fazer encadeamento
    int numChaves;
} noBmais;

typedef struct Cabecalho_BMais
{
    int pos_raiz;
    int pos_topo;
    int pos_livre;
} Cabecalho_BMais;


//Escreve um no em uma determinada posi ̧c~ao do arquivo
//Precondicao: arquivo deve estar aberto e ser um arquivo de arvore
//pos deve ser uma posicao valida do arquivo
//Pos-condicao: no escrito no arquivo
void escreve_no_codigo(FILE *arq, noBmais *x, int pos);

//Escreve um cabecalho no arquivo
//Precondicao: arquivo deve estar aberto e ser um arquivo de arvore
//cab nao pode ser nulo
//Pos-condicao: cabecalho escrito no arquivo
void escreve_cabecalho_codigo(FILE *arq, Cabecalho_BMais *cab);

//inicia um arquivo de codigo vazio
//Precondicao: nenhuma
//Pos-condicao: arquivo de codigo sem nada na arvore criado
void inicia_arvore();

//le um no em uma determinada posicao do arquivo
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//pos deve ser uma posicao v ́alida da lista
//Pos-condicao: ponteiro para no lido  ́e retornado
noBmais *le_no_codigo(FILE *arq, int pos);

//Le o cabecalho do arquivo contendo as informacoes da lista
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: retorna o ponteiro para o cabe ̧calho lido
Cabecalho_BMais *le_cabecalho_codigo(FILE *arq);




//Insere um novo codigo dentro da arvore de codigos
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: codigo inserido dentro da arvore de codigos
void insereCodigo_inicio(int codigo, int pos_dado);


//imprime a arvore inteira em forma de lista
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: imprime a arvore inteira em forma de lista
void imprimi_lista_codigo(FILE *arq);

//imprime a arvore inteira de forma binaria
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: imprime a arvore inteira e forma binaria
void imprimir_arvore_binaria_na_notacao(FILE *arq, int pos);

//imprime a arvore inteira de forma binaria
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: imprime a arvore inteira e forma binaria
void imprime_tudo_notacao();

//Excluir um codigo dentro da arvore em uma pos exata
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//pos deve ser uma posicao valida
//Pos-condicao: codigo excluido da arvore de codigos


//Excluir um codigo dentro da arvore em uma pos exata
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//pos deve ser uma posicao valida
//Pos-condicao: codigo excluido da arvore de codigos
void printa_arvore();

//Printa o codigo de uma determinada posicao na arvore
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: codigo mostrada na tela
void printa_no(noBmais *no, int pos);

//Printa os codigo de um nivel da arvore
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: codigos de tal nivel mostrados na tela
void printa_nivel(FILE *arq, int pos, int contagem);

//Funcao auxiliar para printar nivel da arvore
//Pre-condicao nenhuma
//Pos-Condicao nenhuma
void printa_chaves(noBmais *aux, int pos);

//Percorre a árvore e devolve o total de livros presente na estrutura
//Pre-condição: arquivo deve estar aberto e pos deve ser diferente de -1
//Pos-condição: retorna a quantidade de livros
int qtdLivros(FILE *arq, int pos, int qtd);

//Printa a arvore de codigos por niveis
//Pre-condicao: nenhuma
//Pos-condicao: arvore printada no console por niveis
void printa_arvore_por_nivel();

//Verifica se um codigo existe no arquivo de codigo
//Precondicao: arquivo deve estar aberto e ser um arquivo de arvore
//Pos-condicao: retorna se o codigo existe na arvore
int existe_codigo(int codigo);



//Gera um arquivo com toda as informações dos livros separados por ordem alfabética
//Pre-condição: arquivo deve estar aberto
//Pos-condição: nenhuma
void gerarListagemporTitulo();

#endif
