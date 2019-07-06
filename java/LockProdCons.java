import java.util.ArrayList;

public class LockProdCons{
    public static char[] workspace;
    public static Integer[] locks;  //Vetor de objetos para serem usados como lock nos blocos synchronized
    public static void main (String[] args){
        int nThreads = 1;
        String inputString = "";
        String baseString = "a";
        for(int i = 0; i < 30000; i++){
            inputString = inputString + baseString;
        }
        
        if (args.length > 0) {
            nThreads = Integer.parseInt(args[0]);
        }
        //Inicializa o workspace e locks
        LockProdCons.workspace = new char[nThreads];
        LockProdCons.locks = new Integer[nThreads];
        for (int i = 0; i < LockProdCons.locks.length; i++){
            LockProdCons.locks[i] = new Integer('0');
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
            synchronized(LockProdCons.locks[this.index]){
                if(LockProdCons.workspace[this.index] == Character.MIN_VALUE){  //Posição está vazia; pode produzir
                    LockProdCons.workspace[this.index] = this.inputString.charAt(currentChar);  //Produz um char
                    //System.out.println("Produtor " + this.index + " > " + this.inputString.charAt(currentChar));
                    currentChar++;  //Passa para o próximo char
                }
            }
        }

        System.out.println("Produtor " + this.index + " > " + "Done!");
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
            synchronized(LockProdCons.locks[this.index]){
                if(LockProdCons.workspace[this.index] != Character.MIN_VALUE){  //Posição tem dados; pode consumir
                    consumedString = consumedString + LockProdCons.workspace[this.index];   //Consome um char
                    //System.out.println("Consumidor " + this.index + " > " + LockProdCons.workspace[this.index]);
                    LockProdCons.workspace[this.index] = Character.MIN_VALUE;
                }
            }
        }

        System.out.println("Consumidor " + this.index  + " > " + consumedString);
    }
}