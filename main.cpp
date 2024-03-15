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

void testThreads(int iNumThreads, std::chrono::duration<double>* temp);
void countPrimes(int iInitial, int iFinal, int iThreadNumber,
                     int iNumThreads, bool bOptmized);
bool isPrime(int iNumber);

int main(){

    /*
    FILE* arqWord;
    FILE* arqBlock;
    arqWord = fopen("data//times.txt", "wt");
    arqBlock = fopen("data//block.txt", "wt");*/
    int repeats = 1;

    for (int i=1; i<5; i++){
        double tempWord = 0;
        double tempBlock = 0;
        cout << "\nTestando com " << i << " threads" << endl;
        for (int r = 0; r<repeats; r++)
        {
            cout << "Repeticao " << r << endl;
            std::chrono::duration<double> t[2];
            testThreads(i, t);
            iCountPrimes = 0;
            iPrimeNumbers.clear();
            // Tempo de contagem das palavras
            tempWord += t[0].count();
            // Tempo de preparação do texto
            tempBlock += t[1].count();
        }
        /*
        fprintf(arqWord, "%s", "\n");
        fprintf(arqWord, "%f", tempWord/repeats);
        fprintf(arqBlock, "%s", "\n");
        fprintf(arqBlock, "%f", tempBlock/repeats);*/
    }
    //fclose(arqWord);
    //fclose(arqBlock);
}

void testThreads(int iNumThreads, std::chrono::duration<double>* temp)
{ 
    vector<thread> threads;

    auto beginNotOptmize = high_resolution_clock::now();

    for (int i = 0; i < iNumThreads; i++) {
        threads.push_back(std::thread(countPrimes, 0, 1000, i, iNumThreads, true));
    }

    for (auto& th : threads) th.join();

    auto endNotOptmize = high_resolution_clock::now();

    cout << "Tempo: " << duration_cast<microseconds>(endNotOptmize - beginNotOptmize).count() << " microssegundos" << endl;
    cout << "Numero de Primos: " << iCountPrimes << endl;
    cout << "Primos Encontrados: ";
    // sort vector
    std::sort(iPrimeNumbers.begin(), iPrimeNumbers.end());
    
    for (int i = 0; i < iPrimeNumbers.size(); i++) {
        cout << iPrimeNumbers[i] << " ";
    }
    cout << endl;

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
