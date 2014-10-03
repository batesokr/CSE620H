/**
 * Copyright 2014 Keith Batesole
 */

#include <iomanip>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <vector>
#include "batesokr_hw3.h"

Matrix::Matrix() {
  this->r = 1;
  this->c = 1;
  this->width = 5;
  this->precision = 5;
  this->matrix = std::vector<std::vector<double>> (1,
                    std::vector<double>(1, 0.0));
}

Matrix::Matrix(const int& r, const int& c) {
  this->r = r;
  this->c = c;
  this->width = 5;
  this->precision = 5;
  this->matrix = std::vector<std::vector<double>> (r,
                     std::vector<double>(c, 0.0));
}

Matrix::Matrix(const Matrix& src) {
  this->r = src.r;
  this->c = src.c;
  this->width = src.width;
  this->precision = src.precision;
  this->matrix = std::vector<std::vector<double>> (this->r,
                   std::vector<double>(this->c, 0.0));
  for (int i = 0; i < this->r; i++) {
    for (int j = 0; j < this->c; j++) {
      this->matrix[i][j] = src.matrix[i][j];
    }
  }
}

double& Matrix::operator()(const int& r, const int& c) {
  // Return a reference to a cell
  return this->matrix[r][c];
}

Matrix::~Matrix() {}

Matrix Matrix::operator+(const Matrix& other) const {
  // Check for invalid matrices being added together
  if (this->r != other.r || this->c != other.c) {
    return Matrix(0, 0);
  }

  // Perform addition
  Matrix mat = Matrix(this->r, this->c);
  for (int i = 0; i < this->r; i++) {
    for (int j = 0; j < this->c; j++) {
      mat(i, j) = this->matrix[i][j] + other.matrix[i][j];
    }
  }
  return mat;
}

Matrix Matrix::operator-(const Matrix& other) const {
  // Check for invalid matrices being subtracted together
  if (this->r != other.r || this->c != other.c) {
    return Matrix(0, 0);
  }

  // Perform subtraction
  Matrix mat = Matrix(this->r, this->c);
  for (int i = 0; i < this->r; i++) {
    for (int j = 0; j < this->c; j++) {
      mat(i, j) = this->matrix[i][j] - other.matrix[i][j];
    }
  }
  return mat;
}

Matrix Matrix::operator*(const Matrix& other) const {
  // Check for invalid matrices being multiplied together
  if (this->c != other.r) {
    return Matrix(0, 0);
  }

  // Peform multiplication
  Matrix mat(this->r, other.c);
  for (int i = 0; i < this->r; i++) {
    for (int j = 0; j < other.c; j++) {
      for (int k = 0; k < this->c; k++) {
        mat(i, j) += this->matrix[i][k] * other.matrix[k][j];
      }
    }
  }
  return mat;
}

Matrix Matrix::operator*(const double& factor) const {
  // Perform scaling
  Matrix mat(this->r, this->c);
  for (int i = 0; i < this->r; i++) {
    for (int j = 0; j < this->c; j++) {
      mat(i, j) = this->matrix[i][j] * factor;
    }
  }
  return mat;
}

Matrix& Matrix::operator*=(const double& factor) {
  // Perform scaling
  *this = (*this)*factor;
  return *this;
}

Matrix Matrix::operator/(const double& factor) const {
  // Perform scaling
  Matrix mat(this->r, this->c);
  for (int i = 0; i < this->r; i++) {
    for (int j = 0; j < this->c; j++) {
      mat(i, j) = this->matrix[i][j] / factor;
    }
  }
  return mat;
}

Matrix& Matrix::operator/=(const double& factor) {
  // Perform scaling
  *this = (*this)/factor;
  return *this;
}

Matrix Matrix::operator~() const {
  // Retrieve transpose matrix
  Matrix mat(this->c, this->r);
  for (int r = 0; r < this->r; r++) {
    for (int c = 0; c < this->c; c++) {
      mat(c, r) = this->matrix[r][c];
    }
  }
  return mat;
}

Matrix Matrix::operator>>(const int& n) const {
  // Add n columns to matrix
  Matrix mat(this->r, this->c+n);
  for (int r = 0; r < this->r; r++) {
    for (int c = 0; c < n; c++) {
      mat(r, c) = 0.0;
    }
    for (int c = n; c < mat.c; c++) {
      mat(r, c) = this->matrix[r][c-n];
    }
  }
  return mat;
}

Matrix Matrix::operator<<(const int& n) const {
  // Check if matrix should have no columnns left
  if (n >= this->c) {
    return Matrix(this->r, 0);
  }
  // Remove n columns from matrix
  Matrix mat(this->r, this->c-n);
  for (int r = 0; r < this->r; r++) {
    for (int c = 0; c < this->c-n; c++) {
      mat(r, c) = this->matrix[r][c+n];
    }
  }
  return mat;
}

Matrix& Matrix::setFormat(const int& width, const int& precision) {
  // Store parameters used to set format for printing matrix
  this->width = width;
  this->precision = precision;
  return *this;
}

std::ostream& operator<<(std::ostream& os, const Matrix& mat) {
  // Print out the matrix
  for (int r = 0; r < mat.r; r++) {
    for (int c = 0; c < mat.c; c++) {
      os << std::setw(mat.width) << std::setprecision(mat.precision)
         << mat.matrix[r][c];
    }
    os << std::endl;
  }
  return os;
}

std::istream& operator>>(std::istream& is, Matrix& mat) {
  // Read in a matrix
  int rows, cols;
  double d;
  is >> rows >> cols;
  mat = Matrix(rows, cols);
  for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
      is >> d;
      mat(i, j) = d;
    }
  }
  return is;
}
