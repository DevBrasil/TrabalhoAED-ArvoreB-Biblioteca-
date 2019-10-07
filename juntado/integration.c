#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livro_codigos.h"
#include "livros_dados.h"

char *remover_espacos_frase(char *s)
{
  int n = 0;
  while (n < strlen(s))
  {
    if ((s[n] == ' ') && (s[n - 1] == ' '))
    {
      while (s[n] == ' ')
      {
        s[n] = '\a';
        n++;
      }
    }
    n++;
  }
  return s;
}

void adiciona_livro(Dados_Livro livro)
{
  int posicao;
  FILE *arq_livro = fopen("bd.bin", "rb+");
  posicao = insere_livro(arq_livro, livro);
  fclose(arq_livro);
  insereCodigo_inicio(livro.codigo, posicao); //adiciona na arvore B com a posiçao do livro
}

void adiciona_livro_entradas()
{
  printf("\n \t Adicionar Livro\n\n");

  Dados_Livro livro;

  int aux_codigo, aux = 0;
  printf("Insira um codigo: ");
  scanf("%d", &aux_codigo);

  aux = existe_codigo(aux_codigo);
  printf("%d", aux);
  while (aux == 1)
  {
    printf("\nCodigo existente!!");
    printf("\nInsira um codigo novamente: ");
    scanf("%d", &aux_codigo);
    aux = existe_codigo(aux_codigo);
  }
  livro.codigo = aux_codigo;

  printf("\nInsira um titulo: ");
  scanf("%*c%[^\n]%*c", livro.titulo);

  printf("\nInsira um autor: ");
  scanf("%[^\n]", livro.autor);
  //printf("Livro titulo: %s\n",remover_espacos_frase(livro.titulo));

  printf("\nInsira o numero de exemplares: ");
  scanf("%d", &livro.exemplares);

  adiciona_livro(livro);
  printf("Livro cadastrado com sucesso!\n");
}
/*
void exclui_livro()
{

  int codigo;
  printf("\nEscreva o codigo do livro que deseja excluir = ");
  scanf("%d", &codigo);

  FILE *arquivo_codigo;
  arquivo_codigo = fopen("bdcodigos.bin", "rb+");

  Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
  cab = le_cabecalho_codigos(arquivo_codigo);

  if (existe_codigo(arquivo_codigo, codigo, cab->pos_raiz))
  {
    int posicao = posicao_do_livro(arquivo_codigo, cab->pos_raiz, codigo);
    fclose(arquivo_codigo);

    iniciar_exclusao_Codigo(codigo);

    FILE *arquivo_dados = fopen("bd.bin", "rb+");
    retira(arquivo_dados, posicao);
    printf("Removido com sucesso!!\n ");
    fclose(arquivo_dados);
  }
  else
  {
    printf("\nlivro nao existente\n");
    fclose(arquivo_codigo);
  }


  free(cab);
}
*/
void imprime_em_ordem_de_codigo()
{

  FILE *arq;
  Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
  cab = le_cabecalho_codigo(arq);

  noBmais *aux = (noBmais *)malloc(sizeof(noBmais));

  aux = le_no_codigo(arq, cab->pos_raiz);

  while (aux->eh_folha != 1)
  {

    aux = le_no_codigo(arq, aux->ponteiro[0]);
  }

  while (1)
  {

    for (int j = 0; j < aux->numChaves; j++)
    {

      imprime_livro(aux->ponteiro[j]);
    }

    if (aux->ponteiro[ORDEM - 1] == -1)
      break;
    aux = le_no_codigo(arq, aux->ponteiro[ORDEM - 1]);
  }
  fclose(arq);
}

void atualizar_exmplares()
{
  printf("\n \t Atualizar exemplares \n\n");

  FILE *arq;
  arq = fopen("bdcodigos.bin", "rb+");
  Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
  cab = le_cabecalho_codigo(arq);

  if (cab->pos_raiz == -1)
  {
    printf("Nao existem livros cadastrados \n");
  }
  else
  {
    int aux = 1;
    int aux_codigo;

    printf("\nInsira um codigo: ");
    scanf("%d", &aux_codigo);

    aux = existe_codigo(aux_codigo);

    while (aux != 1)
    {
      printf("\nCodigo nao existente!!");
      printf("\nInsira um codigo novamente: ");
      scanf("%d", &aux_codigo);
      aux = existe_codigo(aux_codigo);
    }

    int posicao = retorna_pos_dado(aux_codigo);

    FILE *teste2;
    teste2 = fopen("bd.bin", "rb+");

    imprime_livro(posicao); //retorna dados do livro

    No_livro *livro = (No_livro *)malloc(sizeof(No_livro));
    livro = le_no_livro(teste2, posicao);

    printf("\nInsira a nova quantidade de exemplares do livro: ");
    scanf("%d", &livro->livro.exemplares);

    escreve_livro_no(teste2, livro, posicao);

    fclose(teste2);
  }
  fclose(arq);
}

void buscar_dados_do_livro()
{
  printf("\n \t Buscar dados do Livro\n\n");

  FILE *arq;
  arq = fopen("bdcodigos.bin", "rb+");
  Cabecalho_BMais *cab = (Cabecalho_BMais *)malloc(sizeof(Cabecalho_BMais));
  cab = le_cabecalho_codigo(arq);

  if (cab->pos_raiz == -1)
  {
    printf("Nao existem livros cadastrados \n");
  }
  else
  {

    int aux = 1;
    int aux_codigo;

    printf("\nInsira um codigo: ");
    scanf("%d", &aux_codigo);

    aux = existe_codigo(aux_codigo); //Insere o codigo dos livros

    if (aux == 0)
    {
      printf("Livro nao existente no sistema\n");
    }
    else
    {
      int posicao = retorna_pos_dado(aux_codigo);

      FILE *teste2;
      teste2 = fopen("bd.bin", "rb+");
      imprime_livro(posicao);
      fclose(teste2);
    }
  }

  fclose(arq);
}

void inserir_via_arquivo_txt()
{

  Dados_Livro livro;

  FILE *file1;
  file1 = fopen("dados.txt", "r");

  char type;
  if (file1 == NULL)
  {
    printf("Problemas na LEITURA do arquivo\n");
  }
  else
  {
    printf("ARQUIVO ABERTO COM SUCESSO LEITURA SENDO FEITA ...\n");
    while (!feof(file1))
    {
      fscanf(file1, "%d%*c", &livro.codigo);
      fscanf(file1, "%[^;]%*c", livro.titulo);
      fscanf(file1, "%[^;]%*c", livro.autor);
      fscanf(file1, "%d%*c", &livro.exemplares);
      *livro.autor = *remover_espacos_frase(livro.autor);
      *livro.titulo = *remover_espacos_frase(livro.titulo);
      adiciona_livro(livro);
    }
  }
  printf("LEITURA FEITA COM SUCESSO\n");

  fclose(file1);
}