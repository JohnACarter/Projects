/* This program is just a copy of sample_1.dsl. You should write your FCAL program to multiply two
2-dimensional matrices and save it
in a file with this name. */

main () { 
  int lower_bound;
  lower_bound = 1;
  int upper_bound;
  upper_bound = 25;
  float mean;
  mean = 0.0;
		boolean a;
		a = True;
		if (a == True) {
				a = False;
		}
  while (lower_bound <= upper_bound) {
    mean = mean + lower_bound;
    lower_bound = lower_bound + 1;
  }
  mean = mean / upper_bound;
  float std_dev;
  std_dev = 0.0;
  lower_bound = 1;
  while (lower_bound <= upper_bound) {
    std_dev = std_dev + (lower_bound - mean) * (lower_bound - mean);
    lower_bound = lower_bound + 1;
  }
  std_dev = std_dev / upper_bound;
  std_dev = sqrt(std_dev);
  print("The mean is :\n");
  print(mean);
  print("\n");
  print("The standard deviation is :\n");
  print(std_dev);
}



