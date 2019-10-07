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
        return 0; //arvore vazia

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

    return 0;
}

//Codigo Tem que existir na arvore
int retorna_pos_dado(int codigo)
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

    link_folhas(arq, cab->pos_raiz, -1);

    fclose(arq);
}

void arruma_pai(FILE *arq, int pos, int codigo, int pos_esq, int pos_dir)
{
    // == 1- ADICIONAR O ITEM CRIANDO O OVERFLOW ==//
    noBmais *no_atual = (noBmais *)malloc(sizeof(noBmais));
    no_atual = le_no_codigo(arq, pos);

    int i;
    for (i = 0; i <= no_atual->numChaves - 1; i++)
    {
        if (codigo < no_atual->chave[i])
            break;
    }
    //atualizar o no movendo as casas para seus novos lugares
    for (int j = ORDEM; j > i; j--)
    {
        no_atual->chave[j] = no_atual->chave[j - 1];
        no_atual->ponteiro[j] = no_atual->ponteiro[j - 1];
    }

    no_atual->numChaves++;
    no_atual->chave[i] = codigo;
    no_atual->ponteiro[i] = pos_esq;
    no_atual->ponteiro[i + 1] = pos_dir;

    escreve_no_codigo(arq, no_atual, pos);
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

        aux->ponteiro[ORDEM - 1] = -1;
        aux->ponteiro[ORDEM] = -1;

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
            { //atingiu o limite de chaves

                // == 1- ADICIONAR O ITEM CRIANDO O OVERFLOW ==//

                int i;
                for (i = 0; i < no_atual->numChaves; i++)
                {
                    if (codigo < no_atual->chave[i])
                        break;
                }
                //atualizar o no movendo as casas para seus novos lugares
                for (int j = ORDEM; j > i; j--)
                {
                    no_atual->chave[j] = no_atual->chave[j - 1];
                    no_atual->ponteiro[j] = no_atual->ponteiro[j - 1];
                }

                //colocar a nova chave e ponteiro na posicao correta

                no_atual->chave[i] = codigo;
                no_atual->ponteiro[i] = pos_dado;
                no_atual->numChaves++;

                int meio = ORDEM / 2;
                // == 2 - CRIAR NOVA PAGINA A ESQUERDA DO NO ATUAL E COPIANDO O VALORES QUE VEM ANTES DO MEIO NELE == //

                noBmais *new = (noBmais *)malloc(sizeof(noBmais));

                new->eh_folha = no_atual->eh_folha;
                new->pai = pai;
                new->numChaves = 0;

                int b;

                for (b = 0; b < ORDEM; b++)
                {
                    if (b < meio)
                    {
                        new->chave[b] = no_atual->chave[b];
                        new->ponteiro[b] = no_atual->ponteiro[b];

                        new->numChaves++;
                    }
                    else
                    {
                        new->chave[b] = -1;
                        new->ponteiro[b] = -1;
                    }
                }

                new->ponteiro[ORDEM - 1] = pos;
                new->ponteiro[ORDEM] = -1;

                // == 3- ARRUMAR O NO_ATUAL == //

                int c;

                for (b = 0, c = meio; b < ORDEM; b++, c++)
                {
                    if (b <= meio)
                    {
                        no_atual->chave[b] = no_atual->chave[c];
                        no_atual->ponteiro[b] = no_atual->ponteiro[c];
                    }
                    else
                    {

                        no_atual->chave[b] = -1;
                        no_atual->ponteiro[b] = -1;
                    }
                }
                no_atual->numChaves = ORDEM - meio;
                no_atual->ponteiro[ORDEM - 1] = no_atual->ponteiro[ORDEM];
                no_atual->ponteiro[ORDEM] = -1;

                // == 4 - ESCREVER O FILHO ATUAL NA SUA POSICAO RECEBIDA PELOS PARAMETROS NO ARQUIVO == //

                escreve_no_codigo(arq, no_atual, pos);

                // == 5 - ESCREVER O NOVO NO (NEW) EM UMA POSICAO LIVRE OU NO TOPO== //

                Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
                cab = le_cabecalho_codigo(arq);

                int pos_new;

                if (cab->pos_livre != -1)
                { //colocar ele na posicao livre
                    pos_new = cab->pos_livre;
                    noBmais *livre = (noBmais *)malloc(sizeof(noBmais));

                    livre = le_no_codigo(arq, cab->pos_livre);

                    cab->pos_livre = livre->eh_folha; //encadeamento de nos excluidos

                    escreve_no_codigo(arq, new, pos_new);
                    escreve_cabecalho_codigo(arq, cab);

                    free(livre);
                    free(cab);
                }
                else
                { //colocar ele no topo
                    pos_new = cab->pos_topo;

                    cab->pos_topo++;

                    escreve_no_codigo(arq, new, pos_new);
                    escreve_cabecalho_codigo(arq, cab);

                    free(cab);
                }

                // == 6 - ARRUMAR O PAI OU CRIAR O PAI (CRIAR SOMENTE SE ESTIVER DANDO SPLIT NA RAIZ) == //

                if (pai != -1)
                { // arruma o pai e retorna -1

                    arruma_pai(arq, pai, no_atual->chave[0], pos_new, pos);

                    return -1;
                }
                else
                { //criar o pai , escreve ele e depois retona sua posicao

                    noBmais *no_pai = (noBmais *)malloc(sizeof(noBmais));
                    Cabecalho_BMais *cab2 = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
                    cab2 = le_cabecalho_codigo(arq);

                    no_pai->numChaves = 1;
                    no_pai->chave[0] = no_atual->chave[0]; //colocar o valor do meio anteriormente decidido na cabeca dessa pagina
                    no_pai->eh_folha = 0;
                    no_pai->pai = -1;

                    int b;

                    for (b = 1; b <= ORDEM; b++)
                    {
                        no_pai->chave[b] = -1;
                        no_pai->ponteiro[b] = -1;
                    }

                    no_pai->ponteiro[1] = pos;
                    no_pai->ponteiro[0] = pos_new;

                    int pos_pai;

                    if (cab2->pos_livre != -1)
                    { //colocar o no criado anteriormente na posicao livre
                        pos_pai = cab2->pos_livre;
                        noBmais *livre2 = (noBmais *)malloc(sizeof(noBmais));

                        livre2 = le_no_codigo(arq, cab2->pos_livre);

                        cab2->pos_livre = livre2->eh_folha; //encadeamento de nos excluidos

                        escreve_no_codigo(arq, no_pai, pos_pai);
                        escreve_cabecalho_codigo(arq, cab2);

                        free(livre2);
                        free(cab2);
                    }
                    else
                    { //colocar o no criado anteriormente no top
                        pos_pai = cab2->pos_topo;

                        cab2->pos_topo++;

                        escreve_no_codigo(arq, no_pai, pos_pai);
                        escreve_cabecalho_codigo(arq, cab2);

                        free(cab2);
                    }

                    free(no_pai);

                    return pos_pai;
                }
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
                for (int j = no_atual->numChaves; j > i; j--)
                {
                    no_atual->chave[j] = no_atual->chave[j - 1];
                    no_atual->ponteiro[j] = no_atual->ponteiro[j - 1];
                }

                //colocar a nova chave e ponteiro na posicao correta
                no_atual->numChaves++;
                no_atual->chave[i] = codigo;
                no_atual->ponteiro[i] = pos_dado;

                escreve_no_codigo(arq, no_atual, pos);

                return pos;
            }
        }
        else
        { //navegue ate mais baixo

            int i;
            int ponteiro = 0;

            for (i = 0; i < no_atual->numChaves; i++)
            {
                if (codigo < no_atual->chave[i])
                {
                    break;
                }
            }

            int response = insereCodigo_termino(arq, pos, no_atual->ponteiro[i], codigo, pos_dado);

            if (response == -1)
            { //ele foi atualizado , precisa ser lido e se tiver overflow da split , arruma o pai e retorna -1
                no_atual = le_no_codigo(arq, pos);

                if (no_atual->numChaves >= ORDEM)
                { //TEM OVERFLOw

                    Cabecalho_BMais *cab3 = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
                    cab3 = le_cabecalho_codigo(arq);

                    if (pai == cab3->pos_raiz)
                    { //alterar
                        int meio = ORDEM / 2;
                        // == 2 - CRIAR NOVA PAGINA A ESQUERDA DO NO ATUAL E COPIANDO O VALORES QUE VEM ANTES DO MEIO NELE == //

                        noBmais *new = (noBmais *)malloc(sizeof(noBmais));

                        new->eh_folha = no_atual->eh_folha;
                        new->pai = pai;
                        new->numChaves = 0;

                        int b;

                        for (b = 0; b < ORDEM; b++)
                        {
                            if (b <= meio)
                            {
                                if (b < meio)
                                {
                                    new->chave[b] = no_atual->chave[b];
                                    new->numChaves++;
                                }
                                else
                                {
                                    new->chave[b] = -1;
                                }
                                new->ponteiro[b] = no_atual->ponteiro[b];
                            }
                            else
                            {
                                new->chave[b] = -1;
                                new->ponteiro[b] = -1;
                            }
                        }
                        new->ponteiro[ORDEM] = -1;

                        // == 3- ARRUMAR O NO_ATUAL == //

                        int c;

                        int MEIO_QUE_SOBE = no_atual->chave[meio];

                        for (b = 0, c = meio + 1; b < ORDEM; b++, c++)
                        {
                            if (b < meio)
                            {
                                no_atual->chave[b] = no_atual->chave[c];
                            }
                            else
                            {
                                no_atual->chave[b] = -1;
                            }
                        }

                        for (b = 0, c = meio + 1; b < ORDEM + 1; b++, c++)
                        {
                            if (b <= meio)
                            {
                                no_atual->ponteiro[b] = no_atual->ponteiro[c];
                            }
                            else
                            {
                                no_atual->ponteiro[b] = -1;
                            }
                        }

                        no_atual->numChaves = ORDEM - meio - 1;

                        // == 4 - ESCREVER O FILHO ATUAL NA SUA POSICAO RECEBIDA PELOS PARAMETROS NO ARQUIVO == //

                        escreve_no_codigo(arq, no_atual, pos);

                        // == 5 - ESCREVER O NOVO NO (NEW) EM UMA POSICAO LIVRE OU NO TOPO== //

                        Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
                        cab = le_cabecalho_codigo(arq);

                        int pos_new;

                        if (cab->pos_livre != -1)
                        { //colocar ele na posicao livre
                            pos_new = cab->pos_livre;
                            noBmais *livre = (noBmais *)malloc(sizeof(noBmais));

                            livre = le_no_codigo(arq, cab->pos_livre);

                            cab->pos_livre = livre->eh_folha; //encadeamento de nos excluidos

                            escreve_no_codigo(arq, new, pos_new);
                            escreve_cabecalho_codigo(arq, cab);

                            free(livre);
                            free(cab);
                        }
                        else
                        { //colocar ele no topo
                            pos_new = cab->pos_topo;

                            cab->pos_topo++;

                            escreve_no_codigo(arq, new, pos_new);
                            escreve_cabecalho_codigo(arq, cab);

                            free(cab);
                        }

                        // == 6 - ARRUMAR O PAI OU CRIAR O PAI (CRIAR SOMENTE SE ESTIVER DANDO SPLIT NA RAIZ) == //

                        if (pai != -1)
                        { // arruma o pai e retorna -1

                            arruma_pai(arq, pai, MEIO_QUE_SOBE, pos_new, pos);
                            return -1;
                        }
                        else
                        { //criar o pai , escreve ele e depois retona sua posicao

                            noBmais *no_pai = (noBmais *)malloc(sizeof(noBmais));
                            Cabecalho_BMais *cab2 = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
                            cab2 = le_cabecalho_codigo(arq);

                            no_pai->numChaves = 1;
                            no_pai->chave[0] = MEIO_QUE_SOBE; //colocar o valor do meio anteriormente decidido na cabeca dessa pagina
                            no_pai->eh_folha = 0;
                            no_pai->pai = -1;

                            int b;

                            for (b = 1; b <= ORDEM; b++)
                            {
                                no_pai->chave[b] = -1;
                                no_pai->ponteiro[b] = -1;
                            }

                            no_pai->ponteiro[1] = pos;
                            no_pai->ponteiro[0] = pos_new;

                            int pos_pai;

                            if (cab2->pos_livre != -1)
                            { //colocar o no criado anteriormente na posicao livre
                                pos_pai = cab2->pos_livre;
                                noBmais *livre2 = (noBmais *)malloc(sizeof(noBmais));

                                livre2 = le_no_codigo(arq, cab2->pos_livre);

                                cab2->pos_livre = livre2->eh_folha; //encadeamento de nos excluidos

                                escreve_no_codigo(arq, no_pai, pos_pai);
                                escreve_cabecalho_codigo(arq, cab2);

                                free(livre2);
                                free(cab2);
                            }
                            else
                            { //colocar o no criado anteriormente no top
                                pos_pai = cab2->pos_topo;

                                cab2->pos_topo++;

                                escreve_no_codigo(arq, no_pai, pos_pai);
                                escreve_cabecalho_codigo(arq, cab2);

                                free(cab2);
                            }

                            free(no_pai);

                            return pos_pai;
                        }
                    }
                    else
                    {
                        int meio = ORDEM / 2;
                        // == 2 - CRIAR NOVA PAGINA A ESQUERDA DO NO ATUAL E COPIANDO O VALORES QUE VEM ANTES DO MEIO NELE == //

                        noBmais *new = (noBmais *)malloc(sizeof(noBmais));

                        new->eh_folha = no_atual->eh_folha;
                        new->pai = pai;

                        int b;

                        for (b = 0; b < ORDEM; b++)
                        {
                            if (b <= meio)
                            {
                                if (b < meio)
                                {
                                    new->chave[b] = no_atual->chave[b];
                                    new->numChaves++;
                                }
                                else
                                {
                                    new->chave[b] = -1;
                                }
                                new->ponteiro[b] = no_atual->ponteiro[b];
                            }
                            else
                            {
                                new->chave[b] = -1;
                                new->ponteiro[b] = -1;
                            }
                        }
                        new->ponteiro[ORDEM] = -1;

                        // == 3- ARRUMAR O NO_ATUAL == //

                        int c;

                        int MEIO_QUE_SOBE = no_atual->chave[meio];

                        for (b = 0, c = meio + 1; b < ORDEM; b++, c++)
                        {
                            if (b < meio)
                            {
                                no_atual->chave[b] = no_atual->chave[c];
                            }
                            else
                            {
                                no_atual->chave[b] = -1;
                            }
                        }

                        for (b = 0, c = meio + 1; b < ORDEM + 1; b++, c++)
                        {
                            if (b <= meio)
                            {
                                no_atual->ponteiro[b] = no_atual->ponteiro[c];
                            }
                            else
                            {
                                no_atual->ponteiro[b] = -1;
                            }
                        }

                        no_atual->numChaves = ORDEM - meio - 1;

                        // == 4 - ESCREVER O FILHO ATUAL NA SUA POSICAO RECEBIDA PELOS PARAMETROS NO ARQUIVO == //

                        escreve_no_codigo(arq, no_atual, pos);

                        // == 5 - ESCREVER O NOVO NO (NEW) EM UMA POSICAO LIVRE OU NO TOPO== //

                        Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
                        cab = le_cabecalho_codigo(arq);

                        int pos_new;

                        if (cab->pos_livre != -1)
                        { //colocar ele na posicao livre
                            pos_new = cab->pos_livre;
                            noBmais *livre = (noBmais *)malloc(sizeof(noBmais));

                            livre = le_no_codigo(arq, cab->pos_livre);

                            cab->pos_livre = livre->eh_folha; //encadeamento de nos excluidos

                            escreve_no_codigo(arq, new, pos_new);
                            escreve_cabecalho_codigo(arq, cab);

                            free(livre);
                            free(cab);
                        }
                        else
                        { //colocar ele no topo
                            pos_new = cab->pos_topo;

                            cab->pos_topo++;

                            escreve_no_codigo(arq, new, pos_new);
                            escreve_cabecalho_codigo(arq, cab);

                            free(cab);
                        }

                        // == 6 - ARRUMAR O PAI OU CRIAR O PAI (CRIAR SOMENTE SE ESTIVER DANDO SPLIT NA RAIZ) == //

                        if (pai != -1)
                        { // arruma o pai e retorna -1

                            arruma_pai(arq, pai, no_atual->chave[0], pos_new, pos);
                            return -1;
                        }
                        else
                        { //criar o pai , escreve ele e depois retona sua posicao

                            noBmais *no_pai = (noBmais *)malloc(sizeof(noBmais));
                            Cabecalho_BMais *cab2 = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
                            cab2 = le_cabecalho_codigo(arq);

                            no_pai->numChaves = 1;
                            no_pai->chave[0] = MEIO_QUE_SOBE; //colocar o valor do meio anteriormente decidido na cabeca dessa pagina
                            no_pai->eh_folha = 0;
                            no_pai->pai = -1;

                            int b;

                            for (b = 1; b <= ORDEM; b++)
                            {
                                no_pai->chave[b] = -1;
                                no_pai->ponteiro[b] = -1;
                            }

                            no_pai->ponteiro[1] = pos;
                            no_pai->ponteiro[0] = pos_new;

                            int pos_pai;

                            if (cab2->pos_livre != -1)
                            { //colocar o no criado anteriormente na posicao livre
                                pos_pai = cab2->pos_livre;
                                noBmais *livre2 = (noBmais *)malloc(sizeof(noBmais));

                                livre2 = le_no_codigo(arq, cab2->pos_livre);

                                cab2->pos_livre = livre2->eh_folha; //encadeamento de nos excluidos

                                escreve_no_codigo(arq, no_pai, pos_pai);
                                escreve_cabecalho_codigo(arq, cab2);

                                free(livre2);
                                free(cab2);
                            }
                            else
                            { //colocar o no criado anteriormente no top
                                pos_pai = cab2->pos_topo;

                                cab2->pos_topo++;

                                escreve_no_codigo(arq, no_pai, pos_pai);
                                escreve_cabecalho_codigo(arq, cab2);

                                free(cab2);
                            }

                            free(no_pai);

                            return pos_pai;
                        }
                    }
                }
                else
                { //NAO TEM OVERFLOW E NAO PRECISA FAZER SPLIT
                    return pos;
                }
            }
            else
            {

                return pos;
            }
        }
    }
}

int irmao_esq_pode_doar(FILE *arq, int pos, int ponteiro)
{
    noBmais *pai = (noBmais *)malloc(sizeof(noBmais));
    pai = le_no_codigo(arq, pos);

    if (ponteiro < pai->chave[0])
    {
        //nao tem fihos mais a esquerda

        return -1;
    }
    else
    {
        int i = 0;
        while (ponteiro > pai->chave[i])
        {
            i++;
        }

        noBmais *filho = (noBmais *)malloc(sizeof(noBmais));
        filho = le_no_codigo(arq, pai->ponteiro[i]);

        if (filho->numChaves > ORDEM / 2)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

int irmao_dir_pode_doar(FILE *arq, int pos, int ponteiro)
{
    noBmais *pai = (noBmais *)malloc(sizeof(noBmais));
    pai = le_no_codigo(arq, pos);

    if (ponteiro < pai->chave[0])
    {
        //nao tem fihos mais a esquerda

        return -1;
    }
    else if (ponteiro >= pai->chave[pai->numChaves - 1])
    {
        return -1;
    }
    else
    {
        int i = 0;
        while (ponteiro > pai->chave[i])
        {
            i++;
        }

        noBmais *filho = (noBmais *)malloc(sizeof(noBmais));
        filho = le_no_codigo(arq, pai->ponteiro[i + 2]);

        if (filho->numChaves > ORDEM / 2)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

void pega_emprestado_esq(FILE *arq, int pos_pai, int pos, int chave)
{
    noBmais *pai = (noBmais *)malloc(sizeof(noBmais));
    pai = le_no_codigo(arq, pos_pai);

    int i = 0;
    while (chave > pai->chave[i])
    {
        i++;
    }

    noBmais *filhoesq = (noBmais *)malloc(sizeof(noBmais));
    filhoesq = le_no_codigo(arq, pai->ponteiro[i]);

    noBmais *filhodir = (noBmais *)malloc(sizeof(noBmais));
    filhodir = le_no_codigo(arq, pos);

    pai->chave[i] = filhoesq->chave[filhoesq->numChaves - 1];
    filhodir->chave[0] = filhoesq->chave[filhoesq->numChaves - 1];

    filhoesq->chave[filhoesq->numChaves - 1] = -1;

    filhoesq->numChaves--;

    escreve_no_codigo(arq, pai, pos_pai);
    escreve_no_codigo(arq, filhodir, pos);
    escreve_no_codigo(arq, filhoesq, pai->ponteiro[i]);
}

int excluiCodigo_termino(FILE *arq, int pai, int pos, int codigo)
{
    noBmais *no_atual = (noBmais *)malloc(sizeof(noBmais));

    no_atual = le_no_codigo(arq, pos);

    if (no_atual->eh_folha == 1)
    {
        if (no_atual->numChaves > ORDEM / 2)
        { //lugar perfeito para excluir
            int i;

            for (i = 0; codigo != no_atual->chave[i]; i++)
            {
            }

            int b;

            for (b = 0; b < ORDEM - 2; b++)
            {
                if (b >= i)
                {
                    no_atual->chave[b] = no_atual->chave[b + 1];
                    no_atual->ponteiro[b] = no_atual->ponteiro[b + 1];
                }
            }

            no_atual->chave[b] = -1;
            no_atual->ponteiro[b] = -1;
            no_atual->numChaves--;

            escreve_no_codigo(arq, no_atual, pos);

            return pos;
        }
        else
        { // pagina imperfeita

            if (irmao_esq_pode_doar(arq, pai, no_atual->chave[0]) == 1)
            {
                int i;
                for (i = 0; codigo != no_atual->chave[i]; i++)
                {
                }

                int b;

                no_atual->chave[b] = -1;
                no_atual->ponteiro[b] = -1;

                for (b = 0; b < ORDEM - 2; b++)
                {
                    if (b < i)
                    {
                        no_atual->chave[b + 1] = no_atual->chave[b];
                        no_atual->ponteiro[b] = no_atual->ponteiro[b];
                    }
                }

                escreve_no_codigo(arq, no_atual, pos);

                //pega_emprestado_esq(arq, pai, pos, no_atual->chave[0]);
            }
            else if (irmao_dir_pode_doar(arq, pai, no_atual->chave[0]) == 1)
            {
                pega_emprestado_esq(arq, pai, pos, no_atual->chave[0]);
            }

            return pos;
        }
    }
    else
    {
        int i;
        int ponteiro = 0;

        for (i = 0; i < no_atual->numChaves; i++)
        {
            if (codigo < no_atual->chave[i])
            {
                break;
            }
        }

        int response = excluiCodigo_termino(arq, pos, no_atual->ponteiro[i], codigo);

        if (response != -1)
        { //nao alterou o pai
            return pos;
        }
        else
        {
            //ALTEROU O PAI , ler ele denovo e verificar se preicsa mudar

            no_atual = le_no_codigo(arq, pos);

            if (no_atual->numChaves >= ORDEM / 2)
            { //nao precisa mudar
                return pos;
            }
            else
            {
                //tem menos chaves que o minimo faca os casos e arrume
            }
        }
    }
}

void excluiCodigo_inicio(int codigo)
{
    FILE *arq = fopen(file, "rb+");

    Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
    cab = le_cabecalho_codigo(arq);

    //apontando raiz
    int raiz;
    raiz = excluiCodigo_termino(arq, -1, cab->pos_raiz, codigo);

    cab = le_cabecalho_codigo(arq);
    cab->pos_raiz = raiz;

    escreve_cabecalho_codigo(arq, cab);

    link_folhas(arq, cab->pos_raiz, -1);

    fclose(arq);
}

void printa_no(noBmais *no, int pos)
{
    printf("[");
    for (int i = 0; i < no->numChaves; i++)
    {
        if (i != 0)
        {
            printf(" ");
        }

        if (i == ORDEM - 1)
        {
            printf("[%d]", no->chave[i]);
        }
        else
        {
            printf("%d", no->chave[i]);
        }

        if (i + 1 < ORDEM)
        {
            printf(",");
        }
    }

    printf("]");

    /* if (no->eh_folha != 1)
    {
        printf(" Suns= [");
        for (int i = 0; i < ORDEM + 1; i++)
        {

            if (i != 0)
            {
                printf(" ");
            }

            if (i == ORDEM)
            {
                printf("[%d]", no->ponteiro[i]);
            }
            else
            {
                printf("%d", no->ponteiro[i]);
            }

            if (i < ORDEM)
            {
                printf(",");
            }
        }
        printf("]");
        printf(" pos = %d ", pos);
    }

    else
    {
        printf(" pos = %d ", pos);
    } */
}

void inicia_arvore()
{
    FILE *arq = fopen(file, "wb+");

    Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
    cab = le_cabecalho_codigo(arq);

    cab->pos_raiz = -1;
    cab->pos_livre = -1;
    cab->pos_topo = 0;

    escreve_cabecalho_codigo(arq, cab);

    fclose(arq);
}

void printa_chaves(noBmais *aux, int pos)
{

    printf("[");
    for (int i = 0; i < aux->numChaves; i++)
    {

        printf("%d", aux->chave[i]);
        if (i < aux->numChaves - 1)
        {
            printf(",");
        }
    }
    printf("]");
    printf(" pos = %d ", pos);
}

void printa_nivel(FILE *arq, int pos, int contagem)
{
    if (pos != -1)
    {
        noBmais *aux = (noBmais *)malloc(sizeof(noBmais));
        aux = le_no_codigo(arq, pos);

        if (contagem == 1)
        {
            printa_no(aux, pos);
            //printa_chaves(aux, pos);
            printf(" ");
        }
        else if (contagem > 1 && aux->eh_folha != 1)
        {
            for (int i = 0; i <= aux->numChaves; i++)
            {
                if (aux->ponteiro[i] != -1)
                {
                    printa_nivel(arq, aux->ponteiro[i], contagem - 1);
                }
            }
        }

        free(aux);
    }
}

void printa_arvore()
{
    FILE *arq = fopen(file, "rb+");
    Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
    cab = le_cabecalho_codigo(arq);

    if (cab->pos_raiz != -1)
    {
        for (int i = 1; i < 5; i++)
        {
            printa_nivel(arq, cab->pos_raiz, i);
            printf("\n");
        }
    }
    else
    {
        printf("sem raiz\n");
    }
    printf("\n");

    fclose(arq);
}

int link_folhas(FILE *arq, int pos, int link_position)
{
    noBmais *aux = (noBmais *)malloc(sizeof(noBmais));
    aux = le_no_codigo(arq, pos);

    if (aux->eh_folha == 1)
    { //EH FOLHA
        aux->ponteiro[ORDEM - 1] = link_position;

        escreve_no_codigo(arq, aux, pos);

        return pos;
    }
    else
    {
        int response = link_position;

        for (int i = aux->numChaves; i >= 0; i--)
        {
            response = link_folhas(arq, aux->ponteiro[i], response);
        }

        return response;
    }
}

int main()
{
    system("clear");

    inicia_arvore();
    //printa_arvore();

    insereCodigo_inicio(17, 10);
    insereCodigo_inicio(8, 10);
    insereCodigo_inicio(29, 10);
    insereCodigo_inicio(22, 10);
    insereCodigo_inicio(2, 10);
    insereCodigo_inicio(5, 10);
    insereCodigo_inicio(70, 10);
    insereCodigo_inicio(44, 10);
    insereCodigo_inicio(25, 10);
    insereCodigo_inicio(27, 10);
    insereCodigo_inicio(23, 10);
    insereCodigo_inicio(80, 10);
    insereCodigo_inicio(75, 10);
    insereCodigo_inicio(90, 10);
    insereCodigo_inicio(14, 10);
    insereCodigo_inicio(7, 10);
    insereCodigo_inicio(1, 10);
    printa_arvore();
    excluiCodigo_inicio(14);
    printa_arvore();
    excluiCodigo_inicio(7);
    /* excluiCodigo_inicio(80);
    excluiCodigo_inicio(44); */
    /*   insereCodigo_inicio(50, 10);
    insereCodigo_inicio(85, 10);
    insereCodigo_inicio(100, 10);
    insereCodigo_inicio(110, 10);
    insereCodigo_inicio(111, 10);
    insereCodigo_inicio(112, 10);
    insereCodigo_inicio(6, 10);
    insereCodigo_inicio(3, 10);
    insereCodigo_inicio(9, 10);
    insereCodigo_inicio(10, 10);
    insereCodigo_inicio(15, 10);
    insereCodigo_inicio(16, 10);
    insereCodigo_inicio(4, 10);
    insereCodigo_inicio(74, 10);
    insereCodigo_inicio(73, 10);
    insereCodigo_inicio(76, 10);
    insereCodigo_inicio(77, 10);
    insereCodigo_inicio(78, 10);
    insereCodigo_inicio(79, 10);
    insereCodigo_inicio(30, 10);
    insereCodigo_inicio(31, 10);
    insereCodigo_inicio(32, 10);
    insereCodigo_inicio(33, 10);
    insereCodigo_inicio(34, 10);
    insereCodigo_inicio(35, 10); */
    printa_arvore();

    /* insereCodigo_inicio(75, 6);
    printa_arvore(); */

    //printa_arvore();
}
