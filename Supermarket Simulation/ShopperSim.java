
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
    static int totalRunTime;                //used to calculate total simulation time
    static int numberCheckers;
	
public static void main(String args[]) {

 // Variables that can change for simulation
    bagging = employeeBagging;
    arrivalInterval = 60;         // Seconds
    numberRegularCheckers = 7;
    numberExpressCheckers = 0;
    totalRunTime = 14400;
		

    numberCheckers = numberRegularCheckers + numberExpressCheckers;
    regurlarCheckers = new Checker[numberRegularCheckers];			
    expressCheckers = new Checker[numberExpressCheckers];

 // Creates new instances of checkers dependent on # checkout lanes specified
    for (int i = 0; i<numberRegularCheckers; i++){					 
        regurlarCheckers[i]=new Checker();							
    }
    for (int q = 0; q<numberExpressCheckers;q++){
        expressCheckers[q] = new Checker();
    }
    
 // Runs simulation loop until time runs out
    agenda.add(new ShopperMaker(arrivalInterval,bagging), 0);		

    while (agenda.getCurrentTime()<totalRunTime){
        agenda.remove().run();
    }

 // Displays statistics for simulation
    ShopperStats.displayStats();

}  // End main method
}
// End ShopperSimulation