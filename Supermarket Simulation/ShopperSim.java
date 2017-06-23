
public class ShopperSim {
	
	static PQ agenda = new PQ();
	static Checker regurlarCheckers[];
	static Checker expressCheckers[];
	static int bagging;
	static  int regularIndex = 0;
	static int expressIndex = 0;
	static final int customerBagging = 9;
	static final int employeeBagging = 5;
	static int arrivalInterval;
	static int numberRegularCheckers;
	static int numberExpressCheckers;
	static int totalRunTime;			//used to calculate total simulation time
	static int numberCheckers;
	
	public static void main(String args[]) {
		
		//variables that can change for simulation
		bagging = employeeBagging;
		arrivalInterval = 60;
		numberRegularCheckers = 7;
		numberExpressCheckers = 0;
		totalRunTime = 14400;
		//end variable		
		
		numberCheckers = numberRegularCheckers + numberExpressCheckers;
		regurlarCheckers = new Checker[numberRegularCheckers];			//2 arrays to hold regular and express checkers
		expressCheckers = new Checker[numberExpressCheckers];
		
		
		for (int i = 0; i<numberRegularCheckers; i++){					//creates new instances of checkers depending on 
			regurlarCheckers[i]=new Checker();							//how many checkers specified
		}
		for (int q = 0; q<numberExpressCheckers;q++){
			expressCheckers[q] = new Checker();
		}
		
		agenda.add(new ShopperMaker(arrivalInterval,bagging), 0);		//runs simulation loop until time runs out
		
        while (agenda.getCurrentTime()<totalRunTime){
            
        	agenda.remove().run();
        	
        }
        
       ShopperStats.displayStats();								//displays statistics for simulation
       
    }  // main method
}
//end ShopperSimulation