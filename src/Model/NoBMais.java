package Model;


//PONTEIRO = INT QUE APONTA PARA O SEU "FILHO" CASO NAO SEJA FOLHA;
//PONTEIRO = INT QUE TEM ENDERECO DO DADO DO LIVRO CASO SEJA FOLHA;

//CHAVE = INT QUE REPRESENTA SEU CODIGO;
//PAI = INT QUE APONTA PARA A POSICAO DE SEU PAI , ( = 0 SE FOR RAIZ);
//EH_FOLHA = BOOLEAN QUE 1 = FOLHA , 0 = NAO FOLHA;
//NUM_CHAVES = INT NUMERO DE CHAVES NA PAGINA;


public class NoBMais {
	protected int ponteiro[];
	protected int chave[];
	protected int pai;
	protected Boolean eh_folha;
	protected int num_chaves;
	
	//retona a informacao guardada em ponteiro
	//index deve ser >=0 e <=ORDEM
	public int getPonteiro(int index) {
		return this.ponteiro[index];
	}
	
	//insere a informacao guardada em ponteiro
	//index deve ser >=0 e <=ORDEM
	public void setPonteiro(int index , int posicao) {
		this.ponteiro[index] = posicao;
	}
	
	//retona a informacao guardada na chave na posicao index
	//index deve ser >=0 e <ORDEM
	public int getChave(int index) {
		return this.chave[index];
	}
	
	//insere a informacao guardada na chave na posicao index
	//index deve ser >=0 e <ORDEM
	public void setChave(int index, int posicao) {
		this.chave[index] = posicao;
	}

	//retorna 1 se for folha e 0 se nao for
	public Boolean getEh_folha() {
		return eh_folha;
	}

	//insere informacao dizendo se eh ou nao folha
	public void setEh_folha(Boolean eh_folha) {
		this.eh_folha = eh_folha;
	}

	//retorna o numero de chaves na pagina
	public int getNum_chaves() {
		return num_chaves;
	}

	//insere o numero de chaves atual na pagina
	public void setNum_chaves(int num_chaves) {
		this.num_chaves = num_chaves;
	}
}
