#include <iostream>
#include "Eigen/Dense"
#include <list>
#include <fstream>

using namespace Eigen;
using namespace std;
int main()
{
  // Open data file
  ifstream file("data.txt");
  ofstream output("output.txt");

  // Import data
  double input;
  list<VectorXd> data;
  int counter = 0;
  VectorXd sample(196);
  while(file >> input) {
    sample(counter) = input;
    counter++;
    if(counter == 196) {
      data.push_back(sample);
      sample = VectorXd(196);
      counter = 0;
    }
  }

  // Compute mean
  VectorXd mean(196);
  for(int i = 0; i < 196; i++) {
	  mean(i) = 0;
  }
  for(list<VectorXd>::iterator it = data.begin(); it != data.end(); it++) {
	  mean += *it;
  }
  mean /= data.size();

  // Compute scatter matrix
  MatrixXd scatterMatrix(196, 196);
  for(int i = 0; i < 196; i++) {
    for(int j = 0; j < 196; j++) {
      scatterMatrix(i, j) = 0;
    }
  }
  for(list<VectorXd>::iterator it = data.begin(); it != data.end(); it++) {
	  scatterMatrix += (*it - mean)*((*it - mean).transpose());
  }

  // Compute eigenvalues and eigenvectors
  SelfAdjointEigenSolver<MatrixXd> eigensolver(scatterMatrix);
  VectorXd evv = eigensolver.eigenvalues();
  MatrixXd evc= eigensolver.eigenvectors();

  // Output the eigenvalue that is needed
  double eigenvaluesPartialSum = 0;
  double eigenvaluesSum = 0;
  double order;
  for(int i = 0; i < 196; i++) {
    eigenvaluesSum += evv(i);
  }
  for(int i = 195; i >= 0; i--) {
    eigenvaluesPartialSum += evv(i);
    if(eigenvaluesPartialSum / eigenvaluesSum > 0.9) {
      order = i + 1;
      break;
    }
  }
  cout << "The smallest number N is of principal components (PCs) such that the proportion of variance explained larger than 90% is " << evv(order - 1) << endl;
  cout << "The number N is " << order << endl;

  // Output the eigenvector that is needed
  list<VectorXd> projectionVector;
  for(int i = 195; i >= order -1; i--) {
    VectorXd eigenvector(196);
    for(int j = 0; j < 196; j++) {
      eigenvector(j) = evc(j, i);
    }
    projectionVector.push_back(eigenvector);
  }

  // Output the projected vector
  list<VectorXd> outputVector;
  for(unsigned int i = 0; i < data.size(); i++) {
    VectorXd eigenvector(196);
    for(int j = 0; j < 196; j++) {
      eigenvector(j) = 0;
    }
    outputVector.push_back(eigenvector);
  }

  // Do projection
  for(list<VectorXd>::iterator it1 = projectionVector.begin(); it1 != projectionVector.end(); it1++) {
	  for(list<VectorXd>::iterator it2 = data.begin(), it3 = outputVector.begin(); it2 != data.end(); it2++, it3++) {
      double ak = (*it1).transpose() * (*it2 - mean);
	  *it3 += mean + ak * *it1;
    }
  }

  // Output data
  int count = 0;
  for(list<VectorXd>::iterator it = data.begin(); it != data.end(); it++) {
	  for(int i = 0; i < 196; i++) {
  	    output << (*it)(i) << ", ";
	  }
	  if(count < 1000 || (count >= 2000 && count < 2250))
        output << 0 << "\n";
	  if((count >= 1000 && count < 2000) || (count >= 2250 && count < 2500))
        output << 1 << "\n";
	  count++;
  }

  // Close file
  file.close();
  output.close();
}
