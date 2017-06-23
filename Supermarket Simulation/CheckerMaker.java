
public class CheckerMaker implements Event{
											//CheckerMaker removes shopper and gets stats and creates new 
	private double checkOutTime;			//CheckerMaker for simulation
	public Checker checker;
	
	public CheckerMaker(){
		checkOutTime = 0;
	}
//end constructor
	
	public CheckerMaker(double ch,Checker check){
		checkOutTime = ch;
		checker = check;
	}
//end constructor for holding checkout time and checker instance
	
	public CheckerMaker(Checker check){
		checker = check;
	}
//end constructor for creating new CheckerMaker and specific checker instance	
	
	public void run(){
		Shopper shopper;
		
		if (checker.waitline.length() == 0){		//checks to see if any shoppers waiting
			checker.busy = false;
			
		}
		else {									//if shopper waiting-Run
			
			checker.busy = true;
			double lastCheckoutTime = checker.lastCheckoutTime;
			
			ShopperStats.updateCount();
			ShopperStats.maxWaitline(checker.waitline.length(), ShopperSim.agenda.getCurrentTime());
			
			shopper = (Shopper) checker.waitline.remove();	//remove shopper from queue and calculate statistics
			checker.lineCount += 1;
			
			ShopperStats.totalHoursWorking+=shopper.getCheckoutTime()*shopper.getBaggingTime()+ShopperSim.agenda.getCurrentTime()-lastCheckoutTime;
			checker.lastCheckoutTime = shopper.getCheckoutTime()*shopper.getBaggingTime() + ShopperSim.agenda.getCurrentTime();
			ShopperStats.totalCheckOutTime(shopper.getCheckoutTime()*shopper.getBaggingTime());
			ShopperStats.checkerIdleTime(shopper.getArrivalTime(),lastCheckoutTime);
			ShopperStats.arrivalToStartCheckout(shopper.getArrivalTime(), ShopperSim.agenda.getCurrentTime());
			checkOutTime = shopper.getCheckoutTime()*shopper.getBaggingTime();
			
			ShopperSim.agenda.add(new CheckerMaker(checkOutTime,checker),checkOutTime);
		}			//Creates new CheckerMaker and adds to simulation queue
	}
//end run method	
}
//end CheckerMaker