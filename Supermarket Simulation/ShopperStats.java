
public class ShopperStats {					//Borrowed some code from CarStat Class from lecture
    static double lastArrivalTime;			//Shopper statistic variables
    static double totalCheckingTime;
    static double totalRunTime=0;
    static double averageWaitline;
    static double startCheckout;
    static double lastWaitlineCount;
    static double count;
    static double arrivalWaitTime;
    static double totalHoursWorking = 0;
    static double totalArrivalTime;
    static double totalPreviousTimeIn;
    static double totalCheckOutTime;
    static double arrivaltimes;
    static int maxWaitline;
    static double maxWaitTime;
    //end Shopper statistic variables

    public static void arrivalToStartCheckout(double in, double start){
        if (start-in>maxWaitTime){						//Calculates Shopper wait times
                maxWaitTime = start - in;
        }
        arrivalWaitTime += in;
        startCheckout += start;
    }
//end arrivalToStartCheckout

    public static void checkerIdleTime(double newIn,double last){
                                                                                            //Calculates checker idle time
        if (newIn - last > 0){
                totalArrivalTime += newIn;
                totalPreviousTimeIn += last;
        }
    }
//end checkerIdleTime

    public static void totalCheckOutTime(double total){  //Calculates total time spent working
        totalCheckOutTime += total;
    }

//end totalCheckOutTime

    public static double updateCount(){
        count += 1;						//updates how many shoppers have been checkedout
        return count;
    }
//end updateCount

    public static void maxWaitline(int max, double time){
                                                                                                                    //calculates max wait line and avg wait line
        if (max > maxWaitline){
        maxWaitline = max;
        }

        totalRunTime+= time - lastArrivalTime;
        averageWaitline += lastWaitlineCount*(time-lastArrivalTime);
        lastArrivalTime = time;
        lastWaitlineCount = max;
    }
//end maxWaitline

    public static String whichBagging(){		//creates "employee bagging" or "customer bagging" for display Statistics
        if (ShopperSim.bagging == 9){
                return "Customer Bagging";
        }
        else {
                return "Employee Bagging";
        }
    }
//end whichBagging


    public static void displayStats(){				//displays final statistics of simulation

        double totalWaitingTime = startCheckout -arrivalWaitTime;
        double totalIdleTime = totalArrivalTime - totalPreviousTimeIn;

        System.out.println("**Simulation Results**\n");

        System.out.printf("Arrival interval:              %d \n",ShopperSim.arrivalInterval);
        System.out.printf("Number of regular checkers:    %d \n",ShopperSim.numberRegularCheckers);
        System.out.printf("Number of express checkers:    %d \n",ShopperSim.numberExpressCheckers);
        System.out.print("Bagging method:                "+whichBagging()+"\n\n");

        System.out.printf("Period of simulation time:     %.2f  minutes\n",totalRunTime/60);
        System.out.printf("Total working time:            %.2f minutes\n",totalHoursWorking/60);
        System.out.printf("Total employee idle time:      %.2f minutes\n",totalIdleTime/60 );
        System.out.printf("Total employee working time:   %.2f minutes\n",totalCheckOutTime/60);
        System.out.printf("Average checkout time          %.2f  minutes\n", ((totalCheckOutTime/count) )/60);

        System.out.printf("Total customer waiting time:   %.2f  minutes\n",
                                                (totalWaitingTime)/60 );
        System.out.printf("Maximum customer wait time:    %.2f  minutes\n",maxWaitTime/60);
        System.out.printf("Average customer wait time:    %.2f  minutes\n"
                                                ,(((totalWaitingTime/count) ))/60);

        System.out.printf("Maximum checkout waitline;     %d\n",maxWaitline);
        System.out.printf("Average checkout waitline:     %.2f\n",(averageWaitline/totalRunTime)/ShopperSim.numberCheckers);

    }
//end displayStats
}
//end ShopperStats