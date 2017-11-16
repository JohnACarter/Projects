
public class Checker {
    Q1 waitline = new Q1();

    int lineCount = 0;  			// used to show how many shopper in queue

    double lastCheckoutTime = 0;                // used to find previous checker checkout time
    int maxLineCount = 0;			// used to find maximum line count
    public boolean busy;			// shows if checker busy or idle

    public Checker(){}
// End constructor
		
    
    public int length(){
            return waitline.length();
    }
// End length	

// Adds shopper to this checkers queue
    public void enter(Shopper s){				

            waitline.add(s);
    }
// End enter shopper to checker	
	
    public boolean isBusy(){					
        return busy;
    } 
// End isBusy
	
}
// End Checker
