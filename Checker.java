
public class Checker {
	Q1 waitline = new Q1();
	
	int lineCount=0;  				// used to show how many shopper in queue
	
	double lastCheckoutTime = 0;    // used to find previous checker checkout time
	int maxLineCount = 0;			// used to find maximum line count
	public boolean busy;			// shows if checker busy or idle
	
	public Checker(){
		
	}
//end constructor
		
	public int length(){
		return waitline.length();
	}
//end length	
	
	public void enter(Shopper s){				//adds shopper to this checkers queue
		
		waitline.add(s);
	}
//end enter shopper to checker	
	
	public boolean isBusy(){					// determines whether this checker is busy or idle
		
		return busy;
	}
//end isBusy
	
}
//end Checker
