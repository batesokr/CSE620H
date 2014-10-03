/*
 * Copyright 2014 Keith Batesole
 */

#ifndef BATESOKR_HW3_H_
#define BATESOKR_HW3_H_
#endif

#include <iostream>
#include <vector>

class Matrix {
 public:
  // Constructors
  Matrix();
  Matrix(const int& r, const int& c);
  Matrix(const Matrix& src);
  ~Matrix();

  // Get operator
  double& operator()(const int& r, const int& c);

  // Additio operator
  Matrix operator+(const Matrix& other) const;

  // Subtraction operator
  Matrix operator-(const Matrix& other) const;

  // Multiplication operator
  Matrix operator*(const Matrix& other) const;

  // Scaling operator
  Matrix operator*(const double& factor) const;

  // Compound-multiplication operator
  Matrix& operator*=(const double& factor);

  // Division operator
  Matrix operator/(const double& other) const;

  // Compound-division operator
  Matrix& operator/=(const double& factor);

  // Transpose operator
  Matrix operator~() const;

  // Add-columns operator
  Matrix operator>>(const int& other) const;

  // Remove-columns operator
  Matrix operator<<(const int& other) const;

  // Set the format
  Matrix& setFormat(const int& width, const int& precision);

  // Output matrix
  friend std::ostream& operator<<(std::ostream& os, const Matrix& mat);

  // Input matrix
  friend std::istream& operator>>(std::istream& is, Matrix& mat);

 private:
  int r, c, width, precision;
  std::vector<std::vector<double>> matrix;
};
