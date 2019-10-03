#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDEM 5
#define file "arvore.bin"

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

Cabecalho_BMais *le_cabecalho_codigo(FILE *arq)
{
    Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
    fseek(arq, 0, SEEK_SET); // posiciona no in ́ıcio do arquivo
    fread(cab, sizeof(Cabecalho_BMais), 1, arq);
    return cab;
}

noBmais *le_no_codigo(FILE *arq, int pos)
{
    noBmais *x = malloc(sizeof(noBmais));
    fseek(arq, sizeof(Cabecalho_BMais) + pos * sizeof(noBmais), SEEK_SET);
    fread(x, sizeof(noBmais), 1, arq);
    return x;
}

void escreve_cabecalho_codigo(FILE *arq, Cabecalho_BMais *cab)
{
    fseek(arq, 0, SEEK_SET); //posiciona no in ́ıcio do arquivo
    fwrite(cab, sizeof(Cabecalho_BMais), 1, arq);
}

void escreve_no_codigo(FILE *arq, noBmais *x, int pos)
{
    fseek(arq, sizeof(Cabecalho_BMais) + pos * sizeof(noBmais), SEEK_SET);
    fwrite(x, sizeof(noBmais), 1, arq);
}

//verifica se o codigo existe na arvore
int existe_codigo(int codigo)
{
    FILE *arq = fopen(file, "rb+");
    Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
    cab = le_cabecalho_codigo(arq);

    if (cab->pos_raiz == -1)
        return -1; //arvore vazia

    noBmais *aux = (noBmais *)malloc(sizeof(noBmais));
    aux = le_no_codigo(arq, cab->pos_raiz);
    int i = 0;

    while (aux->eh_folha != 1)
    {

        for (i = 0; i < aux->numChaves; i++)
        {
            if (codigo < aux->chave[i])
            {
                break;
            }
        }

        aux = le_no_codigo(arq, aux->ponteiro[i]);
    }

    fclose(arq);

    for (i = 0; i < aux->numChaves; i++) //aux ja eh folha
    {
        if (codigo == aux->chave[i])
            return 1;
    }

    return -1;
}

//Codigo Tem que existir na arvore
int pos_dado(int codigo)
{
    FILE *arq = fopen(file, "rb+");
    Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
    cab = le_cabecalho_codigo(arq);

    if (cab->pos_raiz == -1)
        return -1; //arvore vazia

    noBmais *aux = (noBmais *)malloc(sizeof(noBmais));
    aux = le_no_codigo(arq, cab->pos_raiz);
    int i = 0;

    while (aux->eh_folha != 1)
    {

        for (i = 0; i < aux->numChaves; i++)
        {
            if (codigo < aux->chave[i])
            {
                break;
            }
        }

        aux = le_no_codigo(arq, aux->ponteiro[i]);
    }

    fclose(arq);

    for (i = 0; i < aux->numChaves; i++) //aux ja eh folha
    {
        if (codigo == aux->chave[i])
            return aux->ponteiro[i];
    }

    return -1;
}

//Inicia o a operacao de insercao do codigo
void insereCodigo_inicio(int codigo, int pos_dado)
{
    FILE *arq = fopen(file, "rb+");

    Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
    cab = le_cabecalho_codigo(arq);

    //apontando raiz
    int raiz;
    raiz = insereCodigo_termino(arq, -1, cab->pos_raiz, codigo, pos_dado);

    cab = le_cabecalho_codigo(arq);
    cab->pos_raiz = raiz;

    escreve_cabecalho_codigo(arq, cab);

    fclose(arq);
}

void arruma_pai(FILE *arq, int pai, int nova_chave, int pos_esq, int pos_dir)
{
    noBmais *no_atual = (noBmais *)malloc(sizeof(noBmais));

    no_atual = le_no_codigo(arq, pai);

    //adiciona no pai independente se for ficar overflow

    int i;
    for (i = 0; i < no_atual->numChaves; i++)
    {
        if (nova_chave < no_atual->chave[i])
            break;
    }

    no_atual->ponteiro[ORDEM + 1] = no_atual->ponteiro[ORDEM];

    for (int j = (ORDEM); j >= i; j--)
    {
        no_atual->chave[j] = no_atual->chave[j - 1];
        no_atual->chave[j] = no_atual->chave[j - 1];
    }

    no_atual->ponteiro[i] = pos_esq;
    no_atual->ponteiro[i + 1] = pos_dir;

    escreve_no_codigo(arq, no_atual, pai);
}

int insereCodigo_termino(FILE *arq, int pai, int pos, int codigo, int pos_dado)
{
    if (pos == -1)
    { //arvore vazia ( Adicionar na raiz)
        noBmais *aux = (noBmais *)malloc(sizeof(noBmais));

        //popular a primeira chave
        aux->numChaves = 1;
        aux->chave[0] = codigo;
        aux->eh_folha = 1;
        aux->pai = -1;
        aux->ponteiro[0] = pos_dado;

        int b;

        for (b = 1; b < ORDEM; b++)
        {
            aux->chave[b] = -1;
            aux->ponteiro[b] = -1;
        }

        aux->ponteiro[b] = -1;

        Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
        cab = le_cabecalho_codigo(arq);

        if (cab->pos_livre != -1)
        { //tem posicao livre;
            noBmais *livre = (noBmais *)malloc(sizeof(noBmais));
            livre = le_no_codigo(arq, cab->pos_livre); //pega a informacao do no livre
            int pos_livre = cab->pos_livre;            //guarda a posicao do no livre
            cab->pos_livre = livre->eh_folha;          //atualiza a lista de nos livres

            cab->pos_raiz = pos_livre; //atualiza a posicao da raiz

            escreve_no_codigo(arq, aux, pos_livre);
            escreve_cabecalho_codigo(arq, cab);

            free(aux);
            free(livre);
            free(cab);

            return pos_livre;
        }
        else
        { //nao tem posicao livre

            int pos_topo = cab->pos_topo; //guarda a posicao do topo

            escreve_no_codigo(arq, aux, cab->pos_topo);
            cab->pos_raiz = cab->pos_topo;
            cab->pos_topo++;

            escreve_cabecalho_codigo(arq, cab);

            free(aux);
            free(cab);

            return pos_topo;
        }
    }
    else
    { //arvore nao vazia , percorrer e adicionar

        noBmais *no_atual = (noBmais *)malloc(sizeof(noBmais));

        no_atual = le_no_codigo(arq, pos);

        if (no_atual->eh_folha)
        { //adiciona aqui
            if (no_atual->numChaves == ORDEM - 1)
            { //fazer split depois que adicionar

                int i = 0;

                for (i = 0; i < ORDEM - 1; i++)
                { //para quando acahar a posicao que deve adicionar o novo codigo
                    if (codigo < no_atual->chave[i])
                        break;
                }

                int j = 0;

                no_atual->ponteiro[ORDEM + 1] = no_atual->ponteiro[ORDEM]; //passar a referencia da folha irma para o ponteiro overflow

                for (j = ORDEM; j > i; j--) //colocar pra frente as casas que estao a frente da onde vai ser inserido
                {
                    no_atual->chave[j] = no_atual->chave[j - 1];
                    no_atual->chave[j] = no_atual->chave[j - 1];
                }

                no_atual->chave[j] = codigo; //colocar a nova chave na posicao idal
                no_atual->chave[j] = pos;    //colocar o novo ponteiro na posicao ideal

                noBmais *aux = (noBmais *)malloc(sizeof(noBmais));

                aux->ponteiro[ORDEM] = pos; //criando o encadeamento de folhas  irmas

                for (int y = 0; y < i - 1; y++) //copiar as chaves e ponteiros ate o meio-1 copiando o no_atual
                {
                    aux->chave[y] = no_atual->chave[y];
                    aux->ponteiro[y] = no_atual->ponteiro[y];
                    aux->numChaves++;
                    no_atual->numChaves--;
                }

                int k = 0;
                int c = 0;

                for (c = 0, k = i - 1; k < ORDEM; k++, c++)
                {
                    no_atual->chave[c] = no_atual->chave[k + 1];
                    no_atual->ponteiro[c] = no_atual->ponteiro[+1];
                }

                no_atual->ponteiro[c] = no_atual->ponteiro[k + 1]; //colocar o ultimo ponteiro que faltou no lugar certo

                //escrever no arquivo o no atual e o no criado
                Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
                cab = le_cabecalho_codigo(arq);

                if (cab->pos_livre != -1)
                { //tem posicao livre;
                    noBmais *livre = (noBmais *)malloc(sizeof(noBmais));
                    livre = le_no_codigo(arq, cab->pos_livre); //pega a informacao do no livre
                    int pos_livre = cab->pos_livre;            //guarda a posicao do no livre
                    cab->pos_livre = livre->eh_folha;          //atualiza a lista de nos livres

                    escreve_no_codigo(arq, no_atual, pos);
                    escreve_no_codigo(arq, aux, pos_livre);
                    escreve_cabecalho_codigo(arq, cab);

                    free(aux);
                    free(livre);
                    free(cab);

                    arruma_pai(arq, pai, no_atual->chave[i], pos_livre, pos);
                }
                else
                { //nao tem posicao livre

                    int pos_topo = cab->pos_topo; //guarda a posicao do topo

                    escreve_no_codigo(arq, no_atual, pos);
                    escreve_no_codigo(arq, aux, cab->pos_topo);
                    cab->pos_topo++;

                    escreve_cabecalho_codigo(arq, cab);

                    arruma_pai(arq, pai, no_atual->chave[i], pos_topo, pos);

                    free(no_atual);
                    free(aux);
                    free(cab);
                }

                return -1;
            }
            else
            { //folha perfeita para adicionar
                int i;
                for (i = 0; i < no_atual->numChaves; i++)
                {
                    if (codigo < no_atual->chave[i])
                        break;
                }

                //atualizar o no movendo as casas para seus novos lugares
                for (int j = ORDEM - 1; j > i; j--)
                {
                    no_atual->chave[j] = no_atual->chave[j - 1];
                    no_atual->ponteiro[j] = no_atual->ponteiro[j - 1];
                }

                //colocar a nova chave e ponteiro na posicao correta

                no_atual->chave[i] = codigo;
                no_atual->chave[i] = pos_dado;

                escreve_no_codigo(arq, no_atual, pos);

                return pos;
            }
        }
        else
        { //navegue ate mais baixo
            int i;

            for (i = 0; i < no_atual->numChaves; i++)
            {
                if (codigo < no_atual->chave[i])
                    break;
            }

            int resposta = insereCodigo_termino(arq, pos, no_atual->ponteiro[i], codigo, pos_dado);

            if (resposta != -1)
            { // esse no voltou normal
                no_atual->ponteiro[i] = resposta;
                escreve_no_codigo(arq, no_atual, pos);

                return pos;
            }
            else
            { //esse no foi atualizado , leia ele dnv ,
                //se o no atual tiver overflow arrume ele e devolva -1
                //se nao tiver overflow devolva a posicao atual

                no_atual = le_no_codigo(arq, pos);

                if (no_atual->numChaves <= ORDEM - 1) //nao tem overflow
                {
                    return pos;
                }
                else
                { //tem overflow

                    int i = 0;

                    for (i = 0; i < ORDEM - 1; i++)
                    { //para quando acahar a posicao que deve adicionar o novo codigo
                        if (codigo < no_atual->chave[i])
                            break;
                    }

                    int j = 0;

                    no_atual->ponteiro[ORDEM + 1] = no_atual->ponteiro[ORDEM]; //passar a referencia da folha irma para o ponteiro overflow

                    for (j = ORDEM; j > i; j--) //colocar pra frente as casas que estao a frente da onde vai ser inserido
                    {
                        no_atual->chave[j] = no_atual->chave[j - 1];
                        no_atual->chave[j] = no_atual->chave[j - 1];
                    }

                    no_atual->chave[j] = codigo; //colocar a nova chave na posicao idal
                    no_atual->chave[j] = pos;    //colocar o novo ponteiro na posicao ideal

                    noBmais *aux = (noBmais *)malloc(sizeof(noBmais));

                    aux->ponteiro[ORDEM] = pos; //criando o encadeamento de folhas  irmas

                    for (int y = 0; y < i - 1; y++) //copiar as chaves e ponteiros ate o meio-1 copiando o no_atual
                    {
                        aux->chave[y] = no_atual->chave[y];
                        aux->ponteiro[y] = no_atual->ponteiro[y];
                        aux->numChaves++;
                        no_atual->numChaves--;
                    }

                    int k = 0;
                    int c = 0;

                    for (c = 0, k = i - 1; k < ORDEM; k++, c++)
                    {
                        no_atual->chave[c] = no_atual->chave[k + 1];
                        no_atual->ponteiro[c] = no_atual->ponteiro[+1];
                    }

                    no_atual->ponteiro[c] = no_atual->ponteiro[k + 1]; //colocar o ultimo ponteiro que faltou no lugar certo

                    //escrever no arquivo o no atual e o no criado
                    Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
                    cab = le_cabecalho_codigo(arq);

                    if (cab->pos_livre != -1)
                    { //tem posicao livre;
                        noBmais *livre = (noBmais *)malloc(sizeof(noBmais));
                        livre = le_no_codigo(arq, cab->pos_livre); //pega a informacao do no livre
                        int pos_livre = cab->pos_livre;            //guarda a posicao do no livre
                        cab->pos_livre = livre->eh_folha;          //atualiza a lista de nos livres

                        escreve_no_codigo(arq, no_atual, pos);
                        escreve_no_codigo(arq, aux, pos_livre);
                        escreve_cabecalho_codigo(arq, cab);

                        free(aux);
                        free(livre);
                        free(cab);

                        arruma_pai(arq, pai, no_atual->chave[i], pos_livre, pos);
                    }
                    else
                    { //nao tem posicao livre

                        int pos_topo = cab->pos_topo; //guarda a posicao do topo

                        escreve_no_codigo(arq, no_atual, pos);
                        escreve_no_codigo(arq, aux, cab->pos_topo);
                        cab->pos_topo++;

                        escreve_cabecalho_codigo(arq, cab);

                        arruma_pai(arq, pai, no_atual->chave[i], pos_topo, pos);

                        free(no_atual);
                        free(aux);
                        free(cab);
                    }

                    return -1;
                }
            }
        }
    }
}

int main()
{
    printf("compilou");
}
