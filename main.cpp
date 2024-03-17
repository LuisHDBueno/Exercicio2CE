#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include<fstream>
#include <mutex>
#include <algorithm>

using std::cout;
using std::endl;
using std::thread;
using std::string;
using std::vector;
using namespace std::chrono;

vector<int> iPrimeNumbers;
int iCountPrimes = 0;
std::mutex mtx;
int iMaxNum = 100000;

void testThreads(int iNumThreads, double* temp);
void countPrimes(int iInitial, int iFinal, int iThreadNumber,
                     int iNumThreads, bool bOptmized);
bool isPrime(int iNumber);

int main(){


    FILE* arqTime;
    arqTime = fopen("data//times.txt", "wt");
    int repeats = 1;
    int iThreads = 20;

    for (int i=1; i<iThreads; i++){
        double tempTime = 0;
        cout << "\nTestando com " << i << " threads" << endl;
        for (int r = 0; r<repeats; r++)
        {
            cout << "Repeticao " << r << endl;
            double t[2];
            testThreads(i, t);
            iCountPrimes = 0;
            iPrimeNumbers.clear();
            // Tempo de execução
            tempTime += t[0];
        }
        fprintf(arqTime, "%s", "\n");
        fprintf(arqTime, "%f", tempTime/repeats);
    }
    fclose(arqTime);
}

void testThreads(int iNumThreads, double* temp)
{
    vector<thread> threads;

    auto beginNotOptmize = high_resolution_clock::now();

    for (int i = 0; i < iNumThreads; i++) {
        threads.push_back(std::thread(countPrimes, 0, iMaxNum, i, iNumThreads, true));
    }

    for (auto& th : threads) th.join();

    auto endNotOptmize = high_resolution_clock::now();

    cout << "Tempo: " << duration_cast<microseconds>(endNotOptmize - beginNotOptmize).count() << " microssegundos" << endl;
    cout << "Foram avaliados: " << iMaxNum << " numeros" << endl;
    cout << "Numero de Primos: " << iCountPrimes << endl;
    cout << "Primos Encontrados: ";
    // sort vector
    std::sort(iPrimeNumbers.begin(), iPrimeNumbers.end());
    FILE* arqNumbers;
    arqNumbers = fopen("data//numbers.txt", "wt");
    for (int i = 0; i < iPrimeNumbers.size(); i++) {
        cout << iPrimeNumbers[i] << " ";
        fprintf(arqNumbers, "%s", " ");
        fprintf(arqNumbers, "%i", iPrimeNumbers[i]);
    }
    cout << endl;
    fclose(arqNumbers);
    *temp = duration_cast<microseconds>(endNotOptmize - beginNotOptmize).count()* microseconds::period::num / static_cast<double>(microseconds::period::den);

    return;
}

bool isPrime(int i){
    if (i <= 1) {
        return false;
    }
    for (int j = 2; j < i; j++) {
        if (i % j == 0) {
            return false;
        }
    }
    return true;
}

void countPrimes(int iInitial, int iFinal, int iThreadNumber,
                     int iNumThreads, bool bOptmized)
{
    int iTempCount = 0;
    vector<int> iTempPrimes;
    int iBlockSize = (iFinal - iInitial) / iNumThreads;
    if (bOptmized) {
        for (int j = 0; j < iBlockSize; j++) {
            int i = iNumThreads * j + iThreadNumber;
            if (isPrime(i)) {
                iTempCount++;
                iTempPrimes.push_back(i);
            }
        }
    } else {
        for (int i = iInitial + iBlockSize * iThreadNumber;
                 i < iInitial + iBlockSize * (iThreadNumber + 1); i++) {
            if (isPrime(i)) {
                iTempCount++;
                iTempPrimes.push_back(i);
            }
        }
    }
    mtx.lock();
    iCountPrimes += iTempCount;
    iPrimeNumbers.insert(iPrimeNumbers.end(), iTempPrimes.begin(),
                          iTempPrimes.end());
    mtx.unlock();

}
