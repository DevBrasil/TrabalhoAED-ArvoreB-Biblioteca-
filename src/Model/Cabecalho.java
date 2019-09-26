package Model;

public class Cabecalho {
	protected int raiz;
	protected int topo;
	protected int livre;
	
	
	//retorna a informacao em raiz
	protected int getRaiz() {
		return raiz;
	}
	
	//insere a informacao em raiz
	protected void setRaiz(int raiz) {
		this.raiz = raiz;
	}
	
	//retorna a informacao em topo
	protected int getTopo() {
		return topo;
	}
	
	//insere a informacao em topo
	protected void setTopo(int topo) {
		this.topo = topo;
	}
	
	//retorna a informacao em livre
	protected int getLivre() {
		return livre;
	}
	
	//insere a informacao em livre
	protected void setLivre(int livre) {
		this.livre = livre;
	}
}
