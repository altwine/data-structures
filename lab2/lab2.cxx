#include <iostream>
#include <complex>
#include <chrono>
#include <random>
#include <cblas.h>

using namespace std;
using namespace std::chrono;

const long long n = 2048;
const long long formula_ops = 2LL * n * n * n;

void fillMatrix(int n, complex<double> *matrix) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    for (int i = 0; i < n * n; i++)
        matrix[i] = complex<double>(dis(gen), dis(gen));
}

void multiplyMatrices1(int n, const complex<double> *A, const complex<double> *B, complex<double> *C) {
    for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
        complex<double> sum = 0;
        for (int k = 0; k < n; k++)
            sum += A[i * n + k] * B[k * n + j];
        C[i * n + j] = sum;
    }
}

void multiplyMatrices2(int n, const complex<double>* A, const complex<double>* B, complex<double>* C) {
    const complex<double> alpha(1.0, 0.0);
    const complex<double> beta(0.0, 0.0);
    cblas_zgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, &alpha, A, n, B, n, &beta, C, n);
}

void multiplyMatrices3(int n, const complex<double>* A, const complex<double>* B, complex<double>* C) {
    const int chunk = 64;
    for (int i = 0; i < n * n; i++)
        C[i] = 0;
    for (int i = 0; i < n; i += chunk)
    for (int j = 0; j < n; j += chunk)
    for (int k = 0; k < n; k += chunk) {
        int i_end = min(i + chunk, n);
        int j_end = min(j + chunk, n);
        int k_end = min(k + chunk, n);
        for (int ii = i; ii < i_end; ii++) for (int jj = j; jj < j_end; jj++) {
            complex<double> sum = 0;
            for (int kk = k; kk < k_end; kk++)
                sum += A[ii * n + kk] * B[kk * n + jj];
            C[ii * n + jj] += sum;
        }
    }
}

bool validateMatrices (const complex<double>* C_1, const complex<double>* C_2, const complex<double>* C_3) {
    for (int i = 0; i < n * n; i++){
        bool cond1 = abs(C_1[i].real() - C_2[i].real()) > 1e-6 || abs(C_1[i].imag() - C_2[i].imag()) > 1e-6;
        bool cond2 = abs(C_1[i].real() - C_3[i].real()) > 1e-6 || abs(C_1[i].imag() - C_3[i].imag()) > 1e-6;
        if (cond1 || cond2) return false;
    }
    return true;
}

double toMFlops (int64_t duration) {
    double mflops = formula_ops / (duration / 1000.0) * 1e-6;
    return mflops;
}

int main() {
    // Создаю три матрицы, первые два заполняю случ. числами
    complex<double> *A = new complex<double>[n * n];
    complex<double> *B = new complex<double>[n * n];
    complex<double> *C_1 = new complex<double>[n * n];
    complex<double> *C_2 = new complex<double>[n * n];
    complex<double> *C_3 = new complex<double>[n * n];
    fillMatrix(n, A);
    fillMatrix(n, B);

    // Первый способ
    auto start = high_resolution_clock::now();
    multiplyMatrices1(n, A, B, C_1);
    auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
    cout << "Matrix 1: " << toMFlops(duration.count()) << " MFlops (" << duration.count() / 1000.0 <<  "s)" << endl;

    // Второй
    start = high_resolution_clock::now();
    multiplyMatrices2(n, A, B, C_2);
    duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
    cout << "Matrix 2: " << toMFlops(duration.count()) << " MFlops (" << duration.count() / 1000.0 <<  "s)" << endl;

    // Третий
    start = high_resolution_clock::now();
    multiplyMatrices3(n, A, B, C_3);
    duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
    cout << "Matrix 3: " << toMFlops(duration.count()) << " MFlops (" << duration.count() / 1000.0 <<  "s)" << endl;

    // Сравниваю три получившихся матрицы
    if (validateMatrices(C_1, C_2, C_3)) {
        cout << "Matrices are equal" << endl;
    } else {
        cout << "Matrices are not equal" << endl;
    }

    delete[] A;
    delete[] B;
    delete[] C_1;
    delete[] C_2;
    delete[] C_3;
    return 0;
}
