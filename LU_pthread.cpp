#include <iostream>
#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

bool findLU(float**, float**, float**, float**, float*);
void* modifyGivenMatrix(void*);

typedef struct threadArg {
    int column, threadNo;
    threadArg(int col, int th) {
        column = col;
        threadNo = th;
    }
} threadArg;

float **matrix, **u, **l, **pert, *p;
int n, curRow, threadCount;

int main(int argc, char* argv[]) {
    int temp, i, j;
    bool res;
    char str[40];
    //int threadCount;

    if(argc!=3) {
        cout<<"Give thread count and file name for reading matrix";
        return 0;
    }

    threadCount = atoi(argv[1]);

    ifstream myfile;
    myfile.open(argv[2]);
    myfile >> n;
    //vector<vector<int>> result;

    matrix = (float**) malloc(sizeof(float*)*n);
    u = (float**) malloc(sizeof(float*)*n);
    l = (float**) malloc(sizeof(float*)*n);
    pert = (float**) malloc(sizeof(float*)*n);
    p = (float*) malloc(sizeof(float)*n);

    for(i=0; i<n; i++) {
        matrix[i] = (float*) malloc(sizeof(float)*n);
        u[i] = (float*) malloc(sizeof(float)*n);
        l[i] = (float*) malloc(sizeof(float)*n);
        pert[i] = (float*) malloc(sizeof(float)*n);    
    }


    for(i=0; i<n; i++) {
        for(j=0; j<n; j++) {
            myfile >> temp;
            matrix[i][j]=temp;
            u[i][j] = 0;
            l[i][j] = 0;
            pert[i][j] = 0;
        }
    }
    myfile.close();
    
    res = findLU(matrix, u, l, pert, p);
    if(!res) {
        cout<< "This is a singular matrix";
        return 0;
    }
  
    //Generate permutaion matrix
    for(i=0;i<n;i++) {
        pert[i][(int)p[i]] = 1;
    }

    cout << "\np vector"<< endl;
    for(int i=0; i<n; i++) {
        cout << p[i]<<" ";
    }
    cout << endl;
    cout << "\nu matrix" << endl;
    for(i=0; i<n; i++) {
        for(j=0; j<n; j++) {
             if((int)u[i][j]==u[i][j]) {
                cout << u[i][j] << "     ";
            } else {
                sprintf(str, "%.3f", u[i][j]);
                cout << str <<" ";
            }
        }
        cout << endl;
    }
    cout << "\nl matrix" << endl;
    for(i=0; i<n; i++) {
        for(j=0; j<n; j++) {
            if((int)l[i][j]==l[i][j]) {
                cout << l[i][j] << "     ";
            } else {
                sprintf(str, "%.3f", l[i][j]);
                cout << str <<" ";
            }
        }
        cout << endl;
    }

    cout << "\nPermutation matrix" << endl;
    for(i=0; i<n; i++) {
        for(j=0; j<n; j++) {
            cout << pert[i][j]<<" ";
        }
        cout << endl;
    }
    free(matrix);
    free(u);
    free(l);
    free(p);
    free(pert);
}

bool findLU(float **matrix, float **u, float **l, float **pert, float *p) {
    int i, j, k, k1, temp, maxEle=0, reRows;
    pthread_t thVar[threadCount];

    for(i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if(i==j) {
                l[i][j] = 1;
            }
        }
    }
    
    for(i=0; i<n; i++) {
        p[i] = i;
    }

    for(k=0; k<n; k++) {
        maxEle=0;
        for(i=k; i<n; i++) {
            if( maxEle < abs(matrix[i][k]) ) {
                maxEle = abs(matrix[i][k]);
                k1 = i;
            }
        }
        if(maxEle==0) {
            return false; //error
        }
        swap(p[k1], p[k]);
        for(i=0; i<n; i++) {
            swap(matrix[k][i], matrix[k1][i]);
        }
        for(i=0; i<k; i++) {
            swap(l[k][i], l[k1][i]);
        }

        u[k][k] = matrix[k][k];
        for(i=k+1; i<n; i++) {
            l[i][k] = matrix[i][k]/u[k][k];
            u[k][i] = matrix[k][i];
        }

        reRows = n-(k+1);
        if(reRows == 0) {
            return true;
        }
        while(reRows<threadCount) {
            threadCount /= 2;
        }
        if(0==threadCount) {
            threadCount = 1;
        }

        threadArg* thArg[threadCount];
        for(i=0; i<threadCount; i++) {
            thArg[i] = new threadArg(k+1, i);
            pthread_create(&thVar[i], NULL, &modifyGivenMatrix, (void*)thArg[i]);
        }
        for(int i=0; i<threadCount; i++) {
            pthread_join(thVar[i], NULL);
        }
        /*for(i=k+1; i<n; i++) {
            for(j=k+1; j<n; j++) {
                matrix[i][j] -= l[i][k]*u[k][j];
            }
        }*/
    }
    return true;
}

void* modifyGivenMatrix(void* thArg) {
    threadArg* thAg = (threadArg*) thArg;
    int threadNo = thAg->threadNo;
    int curRow = thAg->column;
    int toBeProcessedRow = n-curRow;
    if(toBeProcessedRow == 1) {  //toBeProcessedRow == 1 iff k = n-2, so i starts from n-1 and ends at n-1 in serial execution
        if(threadNo==0) {
            for(int j=curRow; j<n; j++) {
                matrix[n-1][j] -= l[n-1][n-2]*u[n-2][j]; 
            }
        }
    } else {
        int rowCountPerThread = toBeProcessedRow/threadCount;
        int startRow = curRow + (rowCountPerThread*threadNo);
        int endRow = startRow + rowCountPerThread;
        if(endRow>n || threadNo==threadCount-1) {
            endRow=n;
        }
        for(int i=startRow; i<endRow; i++) {
            for(int j=curRow; j<n; j++) {
                matrix[i][j] -= l[i][curRow-1]*u[curRow-1][j];
            }
        }
    }
    return NULL;

}