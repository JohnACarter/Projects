/*******************************************************************************
 * Name            : Matrix.cc
 * Project         : 
 * Module          : 
 * Description     : Source file for matrices in C/C++ translations from FCAL
 * Copyright       : 
 * Original Author : John Carter, Rohith Anil
 * Modifications by: 
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "include/Matrix.h"

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
matrix::matrix(int i, int j) {
  rows = i;
  cols = j;
  data = new float* [rows];
  for (int k = 0; k < rows; ++k) {
    data[k] = new float[cols];
  }
}



matrix::matrix(const matrix& m) {
  rows = m.rows;
  cols = m.cols;
  data = new float* [rows];
  for (int k = 0; k < rows; ++k) {
    data[k] = new float[cols];
  }
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      data[i][j] = m.data[i][j];
    }
  }
}

matrix::~matrix() {
  // if data has been allocated then delete
  if (data) {
    for (int i = 0; i < rows; i++) {
      delete[] data[i];
    }
    delete[] data;
    data = 0;
  }
}

int matrix::n_rows() {
  return rows;
}

int matrix::n_cols() {
  return cols;
}

float* matrix::access(const int i, const int j) const {
  return data[i] + j;
}

matrix matrix::matrix_read(std::string filename) {
  int r, c;
  std::ifstream input(filename.c_str());
  input >> r >> c;
  matrix m(r, c);
  for (int i = 0; i < m.n_rows(); ++i) {
    for (int j = 0; j < m.n_cols(); ++j) {
      input >> *(m.access(i, j));
    }
  }
  return m;
}

matrix& matrix::operator=(const matrix& m) {
  // check for self assignment
  if (this == &m) {
    return *this;
  }

  // if data exists delete it
  if (data) {
    for (int i = 0; i < rows; i++) {
      delete[] data[i];
    }
    delete[] data;
    data = 0;
  }

  // copy the values
  rows = m.rows;
  cols = m.cols;
  data = new float* [rows];
  for (int k = 0; k < rows; ++k) {
    data[k] = new float[cols];
  }
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      data[i][j] = m.data[i][j];
    }
  }
  return *this;
}

matrix matrix::operator*(const matrix& m1) {
  matrix m2(rows, m1.cols);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < m1.cols; ++j) {
      m2.data[i][j] = 0;
      for (int k = 0; k < cols; ++k) {
        m2.data[i][j] = m2.data[i][j] + data[i][k] * m1.data[k][j];
      }
    }
  }
  return m2;
}

std::ostream& operator<<(std::ostream &os, matrix &m) {
  os << m.rows << " " << m.cols << "\n";
  for (int i = 0; i < m.rows; ++i) {
    for (int j = 0; j < m.cols; ++j) {
      os << *(m.access(i, j)) << "  ";
    }
    os << "\n";
  }
  return os;
}


