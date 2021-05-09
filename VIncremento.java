import java.util.Scanner;
//classe da estrutura de dados (recurso) compartilhado entre as threads
class V {
  //recurso compartilhado
	private int tam =50;
	private int vetor[];
 
  //construtor
	public V(int N) {
		this.tam = N;
		this.vetor = new int [tam];
		for (int i=0; i<tam; i++)
			vetor[i] = 0; 
	}
  
  //operacao de escrita sobre o recurso compartilhado
	public void inc(int pos) { 
		this.vetor[pos]+=1;
	}
  
  //operacao de preenchimento do vetor
	public void preenche(int pos) { 
		this.vetor[pos]=pos;
	}
  //operacao de leitura sobre o recurso compartilhado
	public int get(int pos) { 
		return this.vetor[pos]; 
  }
	
	public void setTam(int N){
		this.tam=N;
  }
  
	public void teste(){
		for (int i=0; i<tam; i++){
			if (this.vetor[i]!= i+1)
			{
				System.out.println("Resultado incorreto");
				return;
			}
		}
		System.out.println("Resultado correto");
	} 
	
}
//classe que estende Thread e implementa a tarefa de cada thread do programa 
class T extends Thread {
   
   private int id, tam;
   private int nthreads;
   V v;
  
   //construtor
   public T(int tid, V v, int N, int tam) { 
      this.id = tid; 
      this.v = v;
      this.nthreads = N;
      this.tam = tam;
   }

   //metodo main da thread
   public void run() {
      for (int i=this.id; i<tam; i+=nthreads) {
         this.v.inc(i);  
      }
      
   }
}

//classe da aplicacao
class VIncremento {
	
	
	
	static int N = 0;
	static int tam=0;
	public static void main (String[] args) {
  
	//leitura dos dados de entrada
    Scanner ler = new Scanner(System.in);
    System.out.printf("Informe o tamanho do vetor: ");
    tam = ler.nextInt();
    System.out.printf("Informe o número de threads: ");
    N = ler.nextInt();
    
    if(N>tam)
		N=tam;
    
    Thread[] threads = new Thread[N];
    V v = new V(tam);
    
    
	for (int i=0; i<tam; i++)
		v.preenche(i);
		
      //cria as threads da aplicacao
	for (int i=0; i<threads.length; i++) {
         threads[i] = new T(i, v,N, tam);
      }

      //inicia as threads
	for (int i=0; i<threads.length; i++) {
         threads[i].start();
      }

      //espera pelo termino de todas as threads
	for (int i=0; i<threads.length; i++) {
         try { threads[i].join(); } catch (InterruptedException e) { return; }
      }
		
	//testa a corretude da saída
	v.teste();
   }
}
