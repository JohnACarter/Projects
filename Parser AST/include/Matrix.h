/*******************************************************************************
 * Name            : Matrix.h
 * Project         : 
 * Module          : 
 * Description     : Header file for C/C++ translations from FCAL
 * Original Author : John Harwell
 * Modifications by: John Carter, Rohith Anil
 * Copyright       : 
 ******************************************************************************/

#ifndef INCLUDE_MATRIX_H_
#define INCLUDE_MATRIX_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
class matrix {
 public:
  matrix(int i, int j);
  matrix(const matrix& m);
  ~matrix();
  matrix() : rows(0), cols(0), data(0) { }

  int n_rows();
  int n_cols();

  float* access(const int i, const int j) const;
  friend std::ostream& operator<<(std::ostream &os, matrix &m);
  matrix& operator=(const matrix& m);
  matrix operator*(const matrix& m1);

  static matrix matrix_read(std::string filename);

 private:
  int rows;
  int cols;

     /*! Your implementation of "data" may vary.  There are ways in
      *  which data can be an array of arrays and thus simplify the
      *  access method, at the cost of complicating the process of
      *  allocating space for data.  The choice is entirely up to
      *  you. 
      */
  float **data;
};

#endif  // INCLUDE_MATRIX_H_
