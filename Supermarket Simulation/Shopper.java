
public class Shopper {							//Used Car class from lecture as a template
	
	private double arrivalTime;
	private double numberOfItems;
	private int baggingMethod;
	Checker currentChecker;
	
	
	public Shopper(double arr, double it, int bagging, Checker c) {
	    
		arrivalTime = arr;						//Adds shopper at current time with random number of items
	    numberOfItems = it;						
	    baggingMethod = bagging;
	    
	    currentChecker = c;
	    
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