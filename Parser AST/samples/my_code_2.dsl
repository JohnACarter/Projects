/* This program is just a copy of sample_1.dsl. 
You should write your second interesting FCAL
program and save it in a file with this name. */

main () { 
  matrix m1 = matrix_read("./samples/my_code_2_matrix_1.dat");
  matrix m2 = matrix_read("./samples/my_code_2_matrix_2.dat");
  print("Matrix 1: ");
  print(m1);
  print("Matrix 2: ");
  print(m2);
  if (n_cols(m1) == n_rows(m2)) {
    matrix m3 [n_rows(m1) : n_cols(m2)] i:j = 0;
    m3 = m1 * m2;
    print("Product of Matrix 1 and Matrix 2: ");
    print(m3);
  } else {
    print("Number of columns of Matrix 1 not equal to Number of rows of Matrix 2\n");
    print("Matrix Multiplication is not possible");
  }
  if (n_cols(m2) == n_rows(m1)) {
    matrix m4 [n_rows(m2) : n_cols(m1)] i:j = 0;
    m4 = m2 * m1;
    print("Product of Matrix 2 and Matrix 1: ");
    print(m4);
  } else {
    print("Number of columns of Matrix 2 not equal to Number of rows of Matrix 1\n");
    print("Matrix Multiplication is not possible");
  }
}



