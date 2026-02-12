#ifndef MM_MULT_H
#define MM_MULT_H

#include <vector>

using namespace std;

void matrixMultiplyBaseline(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C, int n);

void matrixMultiplyInterchanged(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C, int n);

void matrixMultiplyTiled(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C, int n, int tileSize);

class MatrixSoA {
private:
    vector<double> data;
    int n;
public:
    MatrixSoA(int size) : n(size), data(size* size) {}
    double& operator()(int i, int j);
    const double& operator()(int i, int j) const;
    int size() const { return n; }
    void clear() { fill(data.begin(), data.end(), 0.0); }
};

void multiplySoA(const MatrixSoA& A, const MatrixSoA& B, MatrixSoA& C, int n);
void multiplySoAVectorized(const MatrixSoA& A, const MatrixSoA& B, MatrixSoA& C, int n);
void matrixMultiplyVectorized(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C, int n);
void matrixMultiplyParallel(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C, int n);
void matrixMultiplyHybrid(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C, int n, int tileSize);

bool verifyResult(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    const vector<vector<double>>& C, int n);

bool verifyResultSoA(const MatrixSoA& A,
    const MatrixSoA& B,
    const MatrixSoA& C, int n);

void initializeMatrix(vector<vector<double>>& matrix, int n);
void initializeMatrixSoA(MatrixSoA& matrix, int n);

#endif // MM_MULT_H