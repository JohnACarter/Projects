
// CheckerMaker removes a shopper when finished, retrieves stat,
// and creates a new shopper to be checkedout.
public class CheckerMaker implements Event {
    Shopper shopper;										
    private double checkOutTime;			
    public Checker checker;

    public CheckerMaker() {
            checkOutTime = 0;
    }
//end constructor
	
    public CheckerMaker(double time, Checker check) {
            checkOutTime = time;
            checker = check;
    }
//end constructor for holding checkout time and checker instance
	
    public CheckerMaker(Checker check) {
            checker = check;
    }
//end constructor for creating new CheckerMaker and specific checker instance	
	
    public void run() {
    
        if (checker.waitline.length() == 0) {		//checks to see if any shoppers waiting
            checker.busy = false;
        }
        else {									//if shopper is waiting -- Run
            checker.busy = true;
            double lastCheckoutTime = checker.lastCheckoutTime;

            ShopperStats.updateCount();
            ShopperStats.maxWaitline(checker.waitline.length(), ShopperSim.agenda.getCurrentTime());

            shopper = (Shopper) checker.waitline.remove();	//remove shopper from queue and calculate statistics
            checker.lineCount += 1;

            ShopperStats.totalHoursWorking += shopper.getCheckoutTime() * shopper.getBaggingTime() +
                                              ShopperSim.agenda.getCurrentTime() - lastCheckoutTime;
           
            checker.lastCheckoutTime = (shopper.getCheckoutTime() * shopper.getBaggingTime()) + 
                                        ShopperSim.agenda.getCurrentTime();
            
            ShopperStats.totalCheckOutTime(shopper.getCheckoutTime() * shopper.getBaggingTime());
            
            ShopperStats.checkerIdleTime(shopper.getArrivalTime(),lastCheckoutTime);
            
            ShopperStats.arrivalToStartCheckout(shopper.getArrivalTime(), ShopperSim.agenda.getCurrentTime());
            
            checkOutTime = shopper.getCheckoutTime() * shopper.getBaggingTime();
            
            //Creates new CheckerMaker and adds to simulation queue
            ShopperSim.agenda.add(new CheckerMaker(checkOutTime,checker),checkOutTime);
    }			
}
//end run method	
}
//end CheckerMaker