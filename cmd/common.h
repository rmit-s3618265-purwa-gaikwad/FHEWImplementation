#ifndef COMMON_H
#define COMMON_H


#include <iostream>
#include <cstdlib>
#include "../LWE.h"
#include "../FHEW.h"
#include <cassert>

using namespace std;

void SaveSecretKey(const LWE::SecretKey* ct, char* filepath);
LWE::SecretKey* LoadSecretKey(char* filepath);


void SaveEvalKey(const FHEW::EvalKey *EK, char* filepath);
FHEW::EvalKey* LoadEvalKey(char* filepath);


void SaveCipherText(const LWE::CipherText* ct, char* filepath);
LWE::CipherText* LoadCipherText(char* filepath, int arr_size, bool arrType);
LWE::CipherText* get(int index, bool arrType);

void setValueToFile(int size, char* file);
int getValueFromFile(char* file);
//void setArraySize(int size);
//int getArraySize();

#endif
