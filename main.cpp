#include <iostream>
#include <iomanip>
#include <vector>
//#include "Strassen'sAlgorithm.hh"

using namespace std;

class Matrix {
private:
    vector<vector<double>> mat;
    int n;

    void LUDecomposition(vector<vector<double>>& L, vector<vector<double>>& U) {
        for (int i = 0; i < n; i++) {
            for (int k = i; k < n; k++) {
                double sum = 0;
                for (int j = 0; j < i; j++) {
                    sum += (L[i][j] * U[j][k]);
                }
                U[i][k] = mat[i][k] - sum;
            }
            for (int k = i; k < n; k++) {
                if (i == k)
                    L[i][i] = 1;
                else {
                    double sum = 0;
                    for (int j = 0; j < i; j++) {
                        sum += (L[k][j] * U[j][i]);
                    }
                    L[k][i] = (mat[k][i] - sum) / U[i][i];
                }
            }
        }
    }

public:
    Matrix(int size) : n(size) {
        mat.resize(n, vector<double>(n, 0));
    }

    void inputMatrix() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cin >> mat[i][j];
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

    Matrix inverse() {
        Matrix inv(n);
        Matrix L(n);
        Matrix U(n);

        LUDecomposition(L.mat, U.mat);

        Matrix identity(n);
        for (int i = 0; i < n; i++) identity.mat[i][i] = 1;

        for (int i = 0; i < n; i++) {
            vector<double> y(n, 0);
            for (int j = 0; j < n; j++) {
                double sum = 0;
                for (int k = 0; k < j; k++) {
                    sum += L.mat[j][k] * y[k];
                }
                y[j] = (identity.mat[j][i] - sum) / L.mat[j][j];
            }
            for (int j = n - 1; j >= 0; j--) {
                double sum = 0;
                for (int k = j + 1; k < n; k++) {
                    sum += U.mat[j][k] * inv.mat[k][i];
                }
                inv.mat[j][i] = (y[j] - sum) / U.mat[j][j];
            }
        }
        return inv;
    }

    /*void multiply(const Matrix& B, Matrix& result) const {
        StrassenMultiple(mat, B.mat, result.mat, n);
    }*/
};

int main() {
    int n;

    cout << "Enter the dimension of the matrix: ";
    cin >> n;
    cout << "Enter the matrix:" << endl;

    Matrix A(n);
    A.inputMatrix();

    Matrix A_inv = A.inverse();

    cout << "The inverse matrix is:" << endl;
    A_inv.printMatrix();
    cout << endl;

    /*Matrix checkMatrix(n);

    cout << "The result of StrassenMultiple:" << endl;
    A.multiply(A_inv, checkMatrix);
    checkMatrix.printMatrix();*/

    return 0;
}