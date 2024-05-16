
#include <iostream>
#include <cmath>
#include <omp.h>
#include <gmp.h>
#include <mpi.h>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <vector>
using namespace std;

#define THREAD_COUNT 10

// to determine whether 2 numbers are co-prime by checking gcd > 1
// calculating gc by Euclid's algorithm
unsigned int gcd(unsigned int a, unsigned int b)
{
    unsigned int x;
    while (b)
    {
        x = a % b;
        a = b;
        b = x;
    }
    return a;
}

unsigned int parallel_mod_exponent(int base, int exp, int modN)
{
    mpz_t num;
    mpz_t result;
    mpz_t mod;

    mpz_init(num);
    mpz_init(result);
    mpz_init(mod);

    mpz_set_ui(num, base); // set string, variable_name, value, base(decimal)
    mpz_set_ui(result, 1); // set unsigned integer, variable_name, value
    mpz_set_ui(mod, modN);

#pragma omp parallel num_threads(THREAD_COUNT)
    {
        int chunk = exp / omp_get_num_threads();
        int chunkRemainder = exp % omp_get_num_threads();
        int start = chunk * omp_get_thread_num();
        int end = start + chunk;

        if (omp_get_thread_num() == omp_get_num_threads() - 1)
        {
            end += chunkRemainder;
        }
        mpz_t localResult;
        mpz_init(localResult);
        mpz_set_ui(localResult, 1);

        for (int i = start; i < end; i++)
        {
            mpz_mul(localResult, localResult, num);
            mpz_mod(localResult, localResult, mod);
        }

#pragma omp critical
        mpz_mul(result, result, localResult);
        mpz_mod(result, result, mod);
    }
    return mpz_get_ui(result);
}

long double *cipher(int key, int n, long double *message, int length)
{
    long double *result = new long double[length];
#pragma omp parallel shared(result) num_threads(THREAD_COUNT)
    {
        for (int i = 0; i < length; i++)
        {
            result[i] = parallel_mod_exponent(message[i], key, n);
            // cout<<"Message: "<<message[i]<<" Result: "<<result[i]<<endl;
        }
    }
    return result;
}

int main(int argc, char *argv[])
{
    long double *message;
    unsigned int iP, iQ, iD, iE, iN, iPhiN;
    int numProcs, myRank, block_size, message_length = 0;

    ifstream fInput("input_trial.txt");

    if (!fInput)
    {
        cout << "Error opening the input file.\n";
        return -1;
    }

    fInput >> iP >> iQ >> message_length;
    message = new long double[message_length];
    char cItem;
    int i = 0;
    while (fInput >> cItem)
    {
        message[i] = cItem - 'A';
        i++;
    }
    fInput.close();
    cout << endl;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    // Broadcast size to all process from root
    MPI_Bcast(&message_length, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double startTime = omp_get_wtime();

    if (myRank == 0)
    {
        cout << "Message Length: " << message_length << endl;
        cout << "P: " << iP << endl;
        cout << "Q: " << iQ << endl;

        // Compute N, Phi, E, D
        iN = iP * iQ;
        iP -= 1;
        iQ -= 1;
        iPhiN = iP * iQ;

        iE = 7;
        int i = 0;
        while (i < (iN - 1))
        {
            if (gcd(iE, iPhiN) == 1)
                break;
            iE++;
        }

        i = 1;
        while (i < iN)
        {
            iD = (iE * i - 1) % iPhiN;
            if (!iD)
            {
                iD = i;
                break;
            }
            i++;
        }
        cout << "N: " << iN << endl;
        cout << "E: " << iE << endl;
        cout << "D: " << iD << endl;
    }

    MPI_Bcast(&iE, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&iD, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&iN, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    block_size = floor(message_length / numProcs);
    long double *block = new long double[block_size];

    // Scatter data equal to chunk size to all processes
    MPI_Scatter(message, block_size, MPI_LONG_DOUBLE, block, block_size, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);

    long double *encryptedBlock = new long double[message_length];
    // RSA Encryption
    encryptedBlock = cipher(iE, iN, block, block_size);

    MPI_Barrier(MPI_COMM_WORLD);

    long double *encryptedMessage = NULL;
    if (myRank == 0)
    {
        encryptedMessage = new long double[message_length];
    }

    MPI_Gather(encryptedBlock, block_size, MPI_LONG_DOUBLE, encryptedMessage, block_size, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);

    if (myRank == 0)
    {
        ofstream encryptOut;
        encryptOut.open("EncryptedMessage", ios::out);
        if (!encryptOut)
        {
            printf("Error in opening file\n");
            exit(-1);
        }
        for (int i = 0; i < message_length; i++)
        {
            encryptOut << encryptedMessage[i] + 'A';
            encryptOut << endl;
        }
        encryptOut.close();
        cout << "Result printed in EncryptedMessage.txt file." << endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    // Scatter data equal to chunk size to all processes
    MPI_Scatter(encryptedBlock, block_size, MPI_LONG_DOUBLE, block, block_size, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);

    long double *decryptedBlock = new long double[message_length];
    // RSA Decryption
    decryptedBlock = cipher(iD, iN, encryptedBlock, block_size);

    MPI_Barrier(MPI_COMM_WORLD);

    long double *decryptedMessage = NULL;
    if (myRank == 0)
    {
        decryptedMessage = new long double[message_length];
    }

    MPI_Gather(decryptedBlock, block_size, MPI_LONG_DOUBLE, decryptedMessage, block_size, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);

    if (myRank == 0)
    {
        ofstream decryptOut;
        decryptOut.open("DecryptedMessage", ios::out);
        if (!decryptOut)
        {
            printf("Error in opening file\n");
            exit(-1);
        }
        for (int i = 0; i < message_length; i++)
        {
            decryptOut << char(decryptedMessage[i] + 'A');
            decryptOut << endl;
        }
        decryptOut.close();
        cout << "Result printed in DecryptedMessage.txt file." << endl;
        double endTime = omp_get_wtime();
        cout << "Total Runtime: " << endTime - startTime << endl;
    }
    MPI_Finalize();
    return 0;
}