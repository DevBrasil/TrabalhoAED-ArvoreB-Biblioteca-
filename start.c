#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDEM 5
#define file "arvore.bin"

typedef struct nodeBMais
{
    int ponteiro[ORDEM];
    int chave[ORDEM];//a ultima chave eh para overflow
    int pai;
    int eh_folha;
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
    fseek(arq, sizeof(noBmais) + pos * sizeof(noBmais), SEEK_SET);
    fread(x, sizeof(noBmais), 1, arq);
    return x;
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
            if (codigo <= aux->chave[i])
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
            if (codigo <= aux->chave[i])
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


void split(FILE *arq, int pos_pai, int pos){
    //pos = posicao do no que tem overflow ativo (precisa que seja dividido e seu pai assuma seu elemento mediao);
    noBmais *pai = (noBmais *)malloc(sizeof(noBmais));
    pai = le_no_codigo(arq,pos_pai);

   noBmais *aux1 = (noBmais *)malloc(sizeof(noBmais));
   noBmais *aux2 = (noBmais *)malloc(sizeof(noBmais));

    aux1 = le_no_codigo(arq,pos);

    if(aux1->eh_folha ==1) {
        aux2->eh_folha =1;
    }

    int q = ORDEM/2 -1; //POSICAO DO ELEMENTO MEDIANO

    int codigo_a_subir = aux1->chave[q];
    
    //criar a pagina mais a esquerda 
    for(int i=0; i < q; i++){
        aux2->chave[i] = aux1->chave[i];
        aux2->numChaves++;
    }

    //arrumar a pagina mais a direita
    for(int j = ORDEM, b=0 ; j >= q ; j--, b++){
        aux1->chave[b] = aux1->chave[j];
        aux1->numChaves--;
    }

    //neste ponto os dois filhos estao arrumados    
    




}


//codigo nao pode existir na arvore
int insere_codigo(FILE *arq, int pos_pai,int pos, int codigo, int pos_dado)
{
    noBmais *aux = (noBmais *)malloc(sizeof(noBmais));
    
    if( pos == -1){ //colocando na raiz

    }else { //colocando no meio do corpo ou navegando ate a folha 
        aux = le_no_codigo(arq,pos);
    
        if(aux->eh_folha == 1){ //encontrou a folha e vai add aqui 
            if(aux->numChaves == ORDEM) { //nao cabe mais nesse no
                split_filho(arq,pos_pai,pos,codigo,pos_dado);
            }

        
        }else {
            int i;

            for(i=0;i <aux->numChaves; i++ ){
                if(codigo <= aux->chave[i]) break;
            }

            return insere_codigo(arq,pos,aux->ponteiro[i], codigo , pos_dado);
        }
    }

    



}
