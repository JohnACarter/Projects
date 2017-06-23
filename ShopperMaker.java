
public class ShopperMaker implements Event {
	
	//two arrays hold the random variables for choosing number of items and arrival time
	int arrayNumberItems [] = {10,20,30,40,50,60,70,80,90,100,10,20,30,40,50,60,70,80,10,20,30,40,50,60,70,20,30,40,50,30};
	double arrayIntervals [] = {.75,.75,.5,.5,.5,.2,.2,.2,.2,0,0,-.2,-.2,-.2,-.2,-.5,-.5,-.5,-.75,-.75};
	private double arrivalInterval;
	private int baggingMethod;
	
	Checker checker;  //instance of checker used for new shopper
	
	public ShopperMaker(double interv, int bagging){		//creates shoppermaker with pre-specified arrival interval
		arrivalInterval = interv;							//and pre-specified employee or customer bagging
		baggingMethod = bagging;
		
		ShopperStats.arrivaltimes = ShopperSim.agenda.getCurrentTime();	//updates arrival time statistics
	}
//end constructor	
	
	private double randomInterval(){
		double arrayIntervalRatio =  arrayIntervals[(int) Math.floor(Math.random()*20)];
		
		return   (arrivalInterval + arrayIntervalRatio*arrivalInterval);			//chooses random interval time for shopper
	}
//end randomInterval	
	
	private int numberItems(){
		int index = (int) Math.floor(Math.random()*30);				//chooses random number of items for shopper
		int items = (int) Math.floor(Math.random()*10 + 1);
		
		return arrayNumberItems[index] - 10 + items;
		
	}
//end numberItems	
	
	public void run() {
		 
		//creates new ShopperMaker and adds to queue
		ShopperSim.agenda.add(new ShopperMaker(arrivalInterval,baggingMethod),randomInterval());
		
		//checks for 10 items or less for new shopper to choose regular or express lane
		if (baggingMethod>10 || ShopperSim.numberExpressCheckers == 0){
		Shopper shopper = new Shopper(ShopperSim.agenda.getCurrentTime(),numberItems(),
										baggingMethod,ShopperSim.regurlarCheckers[ShopperSim.regularIndex]);
		
			if (ShopperSim.regularIndex == ShopperSim.numberRegularCheckers-1){
			    	ShopperSim.regularIndex = 0;
			    }														//updates index holding checker instances
			else{
			    ShopperSim.regularIndex+=1;
			    }
		}
		else {
			
			//if shopper items are 10 or less, finds shortest line, express lane or regular lane
			
			if(ShopperSim.expressCheckers[ShopperSim.expressIndex].length() <= ShopperSim.regurlarCheckers[ShopperSim.regularIndex].length()){
				
				Shopper shopper = new Shopper(ShopperSim.agenda.getCurrentTime(),numberItems(),
						baggingMethod,ShopperSim.expressCheckers[ShopperSim.expressIndex]);
				
				if (ShopperSim.numberExpressCheckers == 1){
					ShopperSim.expressIndex = 0;
				}
				else if (ShopperSim.numberExpressCheckers-1 == ShopperSim.expressIndex){
					ShopperSim.expressIndex = 0;
				}
				else {
					ShopperSim.expressIndex +=1;
				}
				
			}
			else {
				
				Shopper shopper = new Shopper(ShopperSim.agenda.getCurrentTime(),numberItems(),
						baggingMethod,ShopperSim.regurlarCheckers[ShopperSim.regularIndex]);
				
				if (ShopperSim.regularIndex == ShopperSim.numberRegularCheckers-1){
			    	ShopperSim.regularIndex = 0;
			    }
				else{
					ShopperSim.regularIndex+=1;
			    }
			}
		}
		
	}
//end run method	

}
//end ShopperMaker