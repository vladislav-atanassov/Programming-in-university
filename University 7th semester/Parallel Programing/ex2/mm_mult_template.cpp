#include "mm_mult.h"
#include <random>
#include <iostream>
#include <immintrin.h>
#include <omp.h>
#include <algorithm>

using namespace std;

void initializeMatrix(vector<vector<double>>& matrix, int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = dis(gen);
        }
    }
}

void initializeMatrixSoA(MatrixSoA& matrix, int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix(i, j) = dis(gen);
        }
    }
}

bool verifyResult(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    const vector<vector<double>>& C, int n) {
    const double tol = 1e-6;
    if (n > 512) return true;

    vector<vector<double>> ref(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += A[i][k] * B[k][j];
            }
            ref[i][j] = sum;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (abs(C[i][j] - ref[i][j]) > tol) {
                return false;
            }
        }
    }
    return true;
}

bool verifyResultSoA(const MatrixSoA& A, const MatrixSoA& B, const MatrixSoA& C, int n) {
    const double tol = 1e-6;
    if (n > 512) return true;

    MatrixSoA ref(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += A(i, k) * B(k, j);
            }
            ref(i, j) = sum;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (abs(C(i, j) - ref(i, j)) > tol) {
                return false;
            }
        }
    }
    return true;
}

void matrixMultiplyBaseline(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// MatrixSoA operators
double& MatrixSoA::operator()(int i, int j) {
    return data[i * n + j];
}

const double& MatrixSoA::operator()(int i, int j) const {
    return data[i * n + j];
}

// ==================== TODO Section ====================

// TODO 1.2: Loop Interchange
void matrixMultiplyInterchanged(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C, int n) {
    // TODO: Имплементирайте тук
    // Hint: Променете подредбата на циклите на i-k-j
    // Не забравяйте да инициализирате C на 0 преди да започнете!
    for (auto& row : C) fill(row.begin(), row.end(), 0.0);

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t k = 0; k < n; ++k)
        {
            for (size_t j = 0; j < n; ++j)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// TODO 1.2: Tiling
void matrixMultiplyTiled(const vector<vector<double>>& A,
                         const vector<vector<double>>& B,
                         vector<vector<double>>& C, int n, int tileSize) 
{
    // TODO: Имплементирайте тук
    // Hint: Използвайте 6 вложени цикъла
    // Внимавайте с границите - използвайте min(i + tileSize, n)

    for (auto& row : C) fill(row.begin(), row.end(), 0.0);

    for (size_t i = 0; i < n; i += tileSize) {
        for (size_t j = 0; j < n; j += tileSize) {
            for (size_t k = 0; k < n; k += tileSize) {

                size_t i_max = std::min((int)i + tileSize, n);
                size_t j_max = std::min((int)j + tileSize, n);
                size_t k_max = std::min((int)k + tileSize, n);

                for (size_t ii = i; ii < i_max; ++ii) {
                    for (size_t kk = k; kk < k_max; ++kk) {
                        double a_val = A[ii][kk];
                        for (size_t jj = j; jj < j_max; ++jj) {
                            C[ii][jj] += a_val * B[kk][jj];
                        }
                    }
                }

            }
        }
    }
}


// TODO 1.3: SoA умножение
void multiplySoA(const MatrixSoA& A, const MatrixSoA& B, MatrixSoA& C, int n) {
    // TODO: Имплементирайте тук
    // Hint: Използвайте подобна логика като при Loop Interchange
    // Но работете със SoA матрици: A(i,k), B(k,j), C(i,j)
    C.clear();

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t k = 0; k < n; ++k)
        {
            for (size_t j = 0; j < n; ++j)
            {
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }
}

// TODO 1.4: Векторизация (AoS)                 
void matrixMultiplyVectorized(const vector<vector<double>> &A,
                              const vector<vector<double>> &B,
                              vector<vector<double>> &C,
                              int n)
{
    // TODO: Имплементирайте тук
    // Hint: Използвайте AVX инструкции:
    // __m256d a_vec = _mm256_set1_pd(A[i][k]);
    // __m256d b_vec = _mm256_loadu_pd(&B[k][j]);
    // __m256d c_vec = _mm256_fmadd_pd(a_vec, b_vec, c_vec);
    // Обработвайте по 4 елемента наведнъж, после остатъка
    for (auto &row : C)
        fill(row.begin(), row.end(), 0.0);

    vector<vector<double>> B_T(n, vector<double>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            B_T[j][i] = B[i][j];

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j + 4 <= n; j += 4)
        {
            __m256d c_vec = _mm256_loadu_pd(&C[i][j]);

            for (int k = 0; k < n; ++k)
            {
                __m256d a_val = _mm256_set1_pd(A[i][k]);
                __m256d b_vec = _mm256_loadu_pd(&B_T[j][k]);
                c_vec = _mm256_fmadd_pd(a_val, b_vec, c_vec);
            }

            _mm256_storeu_pd(&C[i][j], c_vec);
        }

        for (int j = n - n % 4; j < n; ++j)
        {
            double sum = 0.0;
            for (int k = 0; k < n; ++k)
                sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    }
}

// TODO 1.4: Векторизация (SoA)
void multiplySoAVectorized(const MatrixSoA& A, const MatrixSoA& B, MatrixSoA& C, int n)
{
    // Hint: Същата логика като matrixMultiplyVectorized, но със SoA достъп
    // TODO: Имплементирайте тук
 
    C.clear();

    MatrixSoA B_T(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            B_T(j, i) = B(i, j);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j + 4 <= n; j += 4)
        {
            __m256d c_vec = _mm256_loadu_pd(&C(i, j));

            for (int k = 0; k < n; ++k)
            {
                __m256d a_val = _mm256_set1_pd(A(i, k));
                __m256d b_vec = _mm256_loadu_pd(&B_T(k, j));
                c_vec = _mm256_fmadd_pd(a_val, b_vec, c_vec);
            }

            _mm256_storeu_pd(&C(i, j), c_vec);
        }

        for (int j = n - n % 4; j < n; ++j)
        {
            double sum = 0.0;
            for (int k = 0; k < n; ++k)
                sum += A(i, k) * B(k, j);
            C(i, j) = sum;
        }
    }
}

// TODO 1.5: Паралелизация
void matrixMultiplyParallel(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C, int n) {
    for (auto &row : C) fill(row.begin(), row.end(), 0.0);

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            double a_ik = A[i][k];
            for (int j = 0; j < n; ++j) {
                C[i][j] += a_ik * B[k][j];
            }
        }
    }
}

// TODO 1.5: Hybrid (Tiling + Parallel)
void matrixMultiplyHybrid(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C, int n, int tileSize) 
{
    for (auto &row : C) fill(row.begin(), row.end(), 0.0);

    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int ii = 0; ii < n; ii += tileSize) {
        for (int jj = 0; jj < n; jj += tileSize) {
            for (int kk = 0; kk < n; kk += tileSize) {
                int i_max = std::min(ii + tileSize, n);
                int j_max = std::min(jj + tileSize, n);
                int k_max = std::min(kk + tileSize, n);

                for (int i = ii; i < i_max; ++i) {
                    for (int k = kk; k < k_max; ++k) {
                        double a_ik = A[i][k];
                        for (int j = jj; j < j_max; ++j) {
                            C[i][j] += a_ik * B[k][j];
                        }
                    }
                }
            }
        }
    }
}
