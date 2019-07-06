import java.util.ArrayList;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

public class SemProdCons{
    public static char[] workspace;
    public static Semaphore[] semaphores;  //Vetor de objetos para serem usados como lock nos blocos synchronized
    public static void main (String[] args){
        int nThreads = 1;
        String inputString = "";
        String baseString = "a";
        for(int i = 0; i < 30000; i++){ //Cria uma string grande para testar
            inputString = inputString + baseString;
        }

        if (args.length > 0) {
            nThreads = Integer.parseInt(args[0]);
        }
        //Inicializa o workspace e locks
        SemProdCons.workspace = new char[nThreads];
        SemProdCons.semaphores = new Semaphore[nThreads];
        for (int i = 0; i < SemProdCons.semaphores.length; i++){
            SemProdCons.semaphores[i] = new Semaphore(1);
        }

        long startTime = System.currentTimeMillis(); //Começa a contar o tempo

        Thread[] producersList = new Thread[nThreads];
        Thread[] consumersList = new Thread[nThreads];
        //Inicializa os produtores e consumidores
        for (int i = 0; i < nThreads; i++){
            Producer prod = new Producer(i, inputString);
            producersList[i] = new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        prod.produce();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });

            Consumer cons = new Consumer(i, inputString);
            consumersList[i] = new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        cons.consume();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
        }

        //Começa os produtores e consumidores
        for (int i = 0; i < nThreads; i++){
            producersList[i].start();
            consumersList[i].start();
        }

        //Espera todos os produtores e consumidores acabarem
        for (int i = 0; i < nThreads; i++){
            try{
                producersList[i].join();
                consumersList[i].join();
            }
            catch(Exception e){
                e.printStackTrace();
            }
        }

        long endTime = System.currentTimeMillis();   //Termina de contar o tempo

        System.out.println("SUCCESS! " + (endTime - startTime) + "ms"); //Terminou
    }

}

class Producer{
    private int index;
    private String inputString;

    public Producer(int index, String inputString){
        this.index = index;
        this.inputString = inputString;
    }

    public void produce(){
        int currentChar = 0;

        while(currentChar < this.inputString.length()){  //Passa por toda string de entrada, um caractere por vez
            boolean permit = false;
            try{
                permit = SemProdCons.semaphores[this.index].tryAcquire(1, TimeUnit.SECONDS);  //Tenta pegar o semáforo
                if(permit == true){
                    if(SemProdCons.workspace[this.index] == Character.MIN_VALUE){  //Posição está vazia; pode produzir
                        SemProdCons.workspace[this.index] = this.inputString.charAt(currentChar);  //Produz um char
                        //System.out.println("Produtor " + this.index + " > " + this.inputString.charAt(currentChar));
                        currentChar++;  //Passa para o próximo char
                    }
                }
            }
            catch (Exception e){
                e.printStackTrace();
            }
            finally{
                if(permit == true){
                    SemProdCons.semaphores[this.index].release();
                }
            }
        }

        //System.out.println("Produtor " + this.index + " > " + "Done!");
    }
}

class Consumer{
    private int index;
    private String inputString;

    public Consumer(int index, String inputString){
        this.index = index;
        this.inputString = inputString;
    }

    public void consume(){
        String consumedString = "";

        while(consumedString.length() < this.inputString.length()){ //Repete enquanto não tiver consumido a string de entrada inteira
            boolean permit = false;
            try{
                permit = SemProdCons.semaphores[this.index].tryAcquire(1, TimeUnit.SECONDS);  //Tenta pegar o semáforo
                if(permit == true){
                    if(SemProdCons.workspace[this.index] != Character.MIN_VALUE){  //Posição tem dados; pode consumir
                        consumedString = consumedString + SemProdCons.workspace[this.index];   //Consome um char
                        //System.out.println("Consumidor " + this.index + " > " + SemProdCons.workspace[this.index]);
                        SemProdCons.workspace[this.index] = Character.MIN_VALUE;
                    }
                }
            }
            catch (Exception e){
                e.printStackTrace();
            }
            finally{
                if(permit == true){
                    SemProdCons.semaphores[this.index].release();
                }
            }
        }

        //System.out.println("Consumidor " + this.index  + " > " + consumedString);
    }
}