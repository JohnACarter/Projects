
public class Shopper {	
    
	
    private double arrivalTime;
    private double numberOfItems;
    private int baggingMethod;
    Checker currentChecker;
/*@Return shopper
    
    */

    public Shopper(double arrive, double items, int bagging, Checker check) {

        arrivalTime = arrive;						//Adds shopper at current time with random number of items
        numberOfItems = items;						
        baggingMethod = bagging;

        currentChecker = check;

        currentChecker.enter(this);				//Adds shopper to one instance of a checker depending on total checkers used

        if (!currentChecker.isBusy()){
           new CheckerMaker(currentChecker).run();  
        }
    }
    //end constructor
	
	public double getArrivalTime() {
	    return arrivalTime;
	}
    //end get arrival time
	
	public double getCheckoutTime() {
	    return numberOfItems;
	}
    //end get checkout time
	
	public int getBaggingTime(){
		return baggingMethod;
	}
    //end get number of items	         
	    
}
//End Shopper class