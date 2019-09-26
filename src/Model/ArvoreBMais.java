package Model;

import java.io.File;

public class ArvoreBMais {
	//Numero de filhos que um NoBMais pode ter;
	//ORDEM - 1 =  numero de chaves que um NoBMais pode ter;
	public static final int ORDEM = 5;
	public NoBMais raiz;
	
	public  ArvoreBMais() {
		System.out.println("Iniciando Arvore B+");
	}
			
	
	//Adiciona um codigo na arvore
	//code nao pode ser um codigo existente na arvore e !=null
	public void insere_codigo(int code , int posicao_dado) {
		
	}
	
	//Exclui um codigo da arvore
	//code nao pode ser um codigo nao existente na arvore e !=null
	//Retorna a posicao dos dados do livro a ser retirado no arquivo de dados
	public int remove_codigo(int code) {
		
		return 0;
	}
	
	//Retorna a posicao dos dados do livro a ser procurado no arquivo de dados
	//code nao pode ser um codigo nao existente na arvore e !=null
	public int posicao_dado(int code) {
		
		return 0;
	}
	
	//Retorna se o codigo existe dentro do arquivo de arvore
	//code !=null
	public int existe_codigo(int code) {
		
		return 0;
	}
	
	//imprime na tela arvore em niveis
	public void imprimir_arvore() {
		
	}
	
	//retorna todos os codigos de livros validos
	public int codigos_validos() {
		
		return 0;
	}
	
	//le o cabecalo do arquivo de codigos
	//arquivo de codigos deve existir
	//arquivo deve estar aberto para leitura 
	//retorna o cabecalo escrito do arquvio de codigos
	public Cabecalho le_cabecalho(File arq) {
		Cabecalho cabecalho = new Cabecalho();
		
		return cabecalho;
	}
	
	//escreve o cabecalo no arquivo de codigos
	//arquivo deve estar aberto para escrita 
	public void escreve_cabecalho(File arq, Cabecalho cab) {
		
	}
	
	//le o no de codigo na posicao "pos" dentro do arquivo de codigos
	//arquivo deve estar aberto para leitura
	//pos deve ser uma posicao valida
	public NoBMais le_no(File arq, int pos) {
		NoBMais no = new NoBMais();
			
		return no;
	}
		
	//escreve um no no arquivo de codigos
	//arquivo deve estar aberto para escrita
	//no deve ser != null
	//pos deve ser uma posicao valida
	public void escreve_no(File arq, NoBMais no , int pos) {
		
	}
		
	
	
	

}
