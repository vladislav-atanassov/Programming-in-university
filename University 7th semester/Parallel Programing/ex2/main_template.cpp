#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include <omp.h>
#include "mm_mult.h"

using namespace std;

// ============================================================================
// ФУНКЦИИ ЗА ТЕСТВАНЕ
// ============================================================================

vector<vector<double>> createMatrix(int rows, int cols) {
    return vector<vector<double>>(rows, vector<double>(cols, 0.0));
}

void printHeader(const string& title) {
    cout << "\n" << string(60, '=') << endl;
    cout << title << endl;
    cout << string(60, '=') << endl;
}

void printTableHeader() {
    cout << "Method                   | Time (ms) | Speedup | Correct?" << endl;
    cout << "-------------------------|-----------|---------|----------" << endl;
}

void printResult(const string& method, long long time, double speedup, bool correct) {
    cout << left << setw(24) << method << " | "
        << setw(9) << time << " | "
        << setw(7) << fixed << setprecision(2) << speedup << " | "
        << (correct ? "Yes" : "No") << endl;
}

template<typename Func>
void testMethod(const string& name, Func func,
    const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C,
    int n, double baseline_time) {

    // Нулиране на резултатната матрица
    for (auto& row : C) fill(row.begin(), row.end(), 0.0);

    auto start = chrono::high_resolution_clock::now();
    func(A, B, C, n);
    auto end = chrono::high_resolution_clock::now();
    auto time_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    double speedup = baseline_time / static_cast<double>(time_ms);
    bool correct = verifyResult(A, B, C, n);
    
    printResult(name, time_ms, speedup, correct);
}

template<typename Func>
void testMethodWithParam(const string& name, Func func,
    const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C,
    int n, int param, double baseline_time) {

    for (auto& row : C) fill(row.begin(), row.end(), 0.0);

    auto start = chrono::high_resolution_clock::now();
    func(A, B, C, n, param);
    auto end = chrono::high_resolution_clock::now();
    auto time_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    double speedup = baseline_time / static_cast<double>(time_ms);
    bool correct = verifyResult(A, B, C, n);

    printResult(name, time_ms, speedup, correct);
}

// ============================================================================
// ТЕСТОВЕ - TODO: Разкоментирайте след като имплементирате функциите
// ============================================================================

void runSequentialTests(int matrixSize) {
    printHeader("SEQUENTIAL OPTIMIZATIONS");

    auto A = createMatrix(matrixSize, matrixSize);
    auto B = createMatrix(matrixSize, matrixSize);
    auto C = createMatrix(matrixSize, matrixSize);

    initializeMatrix(A, matrixSize);
    initializeMatrix(B, matrixSize);

    cout << "Matrix size: " << matrixSize << "x" << matrixSize << endl;
    printTableHeader();

    double baseline_time = 0.0;

    // TODO: РАЗКОМЕНТИРАЙТЕ ЕДИН ПО ЕДИН

    // БАЗОВ ТЕСТ
    for (auto& row : C) fill(row.begin(), row.end(), 0.0);
    auto start = chrono::high_resolution_clock::now();
    matrixMultiplyBaseline(A, B, C, matrixSize);
    auto end = chrono::high_resolution_clock::now();
    baseline_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    bool correct = verifyResult(A, B, C, matrixSize);
    printResult("Baseline", baseline_time, 1.0, correct);

    // LOOP INTERCHANGE
    testMethod("Loop Interchange", matrixMultiplyInterchanged, A, B, C, matrixSize, baseline_time);

    // TILING
    testMethodWithParam("Tiling (16)", matrixMultiplyTiled, A, B, C, matrixSize, 16, baseline_time);
    testMethodWithParam("Tiling (32)", matrixMultiplyTiled, A, B, C, matrixSize, 32, baseline_time);
    testMethodWithParam("Tiling (64)", matrixMultiplyTiled, A, B, C, matrixSize, 64, baseline_time);

    // VECTORIZED
    testMethod("Vectorized", matrixMultiplyVectorized, A, B, C, matrixSize, baseline_time);

    // SOA TESTS
    MatrixSoA A_soa(matrixSize), B_soa(matrixSize), C_soa(matrixSize);
    initializeMatrixSoA(A_soa, matrixSize);
    initializeMatrixSoA(B_soa, matrixSize);

    // SoA LAYOUT
    C_soa.clear();
    start = chrono::high_resolution_clock::now();
    multiplySoA(A_soa, B_soa, C_soa, matrixSize);
    end = chrono::high_resolution_clock::now();
    auto soa_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    correct = verifyResultSoA(A_soa, B_soa, C_soa, matrixSize);
    printResult("SoA Layout", soa_time, baseline_time / soa_time, correct);

    // SOA + VECTORIZED
    C_soa.clear();
    start = chrono::high_resolution_clock::now();
    multiplySoAVectorized(A_soa, B_soa, C_soa, matrixSize);
    end = chrono::high_resolution_clock::now();
    auto soa_vec_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    correct = verifyResultSoA(A_soa, B_soa, C_soa, matrixSize);
    printResult("SoA + Vectorized", soa_vec_time, baseline_time / soa_vec_time, correct);

    cout << "Sequential tests pending implementation..." << endl;
}

void runParallelTests(int matrixSize, int numThreads) {
    printHeader("PARALLEL OPTIMIZATIONS");

    auto A = createMatrix(matrixSize, matrixSize);
    auto B = createMatrix(matrixSize, matrixSize);
    auto C = createMatrix(matrixSize, matrixSize);

    initializeMatrix(A, matrixSize);
    initializeMatrix(B, matrixSize);

    // Базово време за сравнение
    for (auto& row : C) fill(row.begin(), row.end(), 0.0);
    auto start = chrono::high_resolution_clock::now();
    matrixMultiplyBaseline(A, B, C, matrixSize);
    auto end = chrono::high_resolution_clock::now();
    double baseline_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "Threads: " << numThreads << endl;
    printTableHeader();

    // TODO: РАЗКОМЕНТИРАЙТЕ
    omp_set_num_threads(numThreads);

    // PARALLEL BASELINE
    testMethod("Parallel", matrixMultiplyParallel, A, B, C, matrixSize, baseline_time);

    // HYBRID (TILING + PARALLEL)
    testMethodWithParam("Hybrid (Tiling+Parallel)", matrixMultiplyHybrid,
                       A, B, C, matrixSize, 32, baseline_time);

    cout << "Parallel tests pending implementation..." << endl;
}

int main(int argc, char* argv[]) {
    int matrixSize = 512;
    int numThreads = 4;

    if (argc > 1) matrixSize = atoi(argv[1]);
    if (argc > 2) numThreads = atoi(argv[2]);

    cout << "MATRIX MULTIPLICATION PERFORMANCE TEST" << endl;
    cout << "======================================" << endl;
    cout << "Matrix size: " << matrixSize << "x" << matrixSize << endl;
    cout << "Default threads: " << numThreads << endl;
    cout << "Available processors: " << omp_get_num_procs() << endl;

    runSequentialTests(matrixSize);
    runParallelTests(matrixSize, numThreads);

    cout << "\n=== PERFORMANCE SUMMARY ===" << endl;
   

    return 0;
}
