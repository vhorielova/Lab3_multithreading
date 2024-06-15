#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Matrix {
private:
    vector<vector<double>> mat;
    int n;

    void computeU(int i, vector<vector<double>>& L, vector<vector<double>>& U) {
        for (int k = i; k < n; k++) {
            double sum = 0;
            for (int j = 0; j < i; j++) {
                sum += (L[i][j] * U[j][k]);
            }
            U[i][k] = mat[i][k] - sum;
        }
    }

    void computeL(int i, vector<vector<double>>& L, vector<vector<double>>& U) {
        for (int k = i; k < n; k++) {
            if (i == k) {
                L[i][i] = 1;
            }
            else {
                double sum = 0;
                for (int j = 0; j < i; j++) {
                    sum += (L[k][j] * U[j][i]);
                }
                L[k][i] = (mat[k][i] - sum) / U[i][i];
            }
        }
    }

    void LUDecomposition(vector<vector<double>>& L, vector<vector<double>>& U) {
        for (int i = 0; i < n; i++) {
            thread t1(&Matrix::computeU, this, i, ref(L), ref(U));
            thread t2(&Matrix::computeL, this, i, ref(L), ref(U));

            t1.join();
            t2.join();
        }
    }

    void LUDecompositionSingleThread(vector<vector<double>>& L, vector<vector<double>>& U) {
        for (int i = 0; i < n; i++) {
            computeU(i, L, U);
            computeL(i, L, U);
        }
    }

public:
    Matrix(int size) : n(size) {
        mat.resize(n, vector<double>(n, 0));
    }

    void randomizeMatrix() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 100);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                mat[i][j] = dis(gen);
            }
        }
    }

    void printMatrix() const {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << fixed << setprecision(4) << mat[i][j] << " ";
            }
            cout << '\n';
        }
    }

    void benchmark() {
        Matrix L(n);
        Matrix U(n);

        auto start = high_resolution_clock::now();
        LUDecompositionSingleThread(L.mat, U.mat);
        auto end = high_resolution_clock::now();
        auto durationSingle = duration_cast<microseconds>(end - start).count();
        cout << "Single-threaded LU decomposition took " << durationSingle << " microseconds.\n";

        start = high_resolution_clock::now();
        LUDecomposition(L.mat, U.mat);
        end = high_resolution_clock::now();
        auto durationMulti = duration_cast<microseconds>(end - start).count();
        cout << "Multi-threaded LU decomposition took " << durationMulti << " microseconds.\n";
    }
};

int main() {
    vector<int> sizes = { 4, 16, 64, 128, 256, 512 };

    for (int n : sizes) {
        cout << "Benchmarking matrix of size " << n << "x" << n << ":\n";
        Matrix A(n);
        A.randomizeMatrix();
        A.benchmark();
        cout << endl;
    }

    return 0;
}
