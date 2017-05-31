#include <iostream>
#include <cstdlib>
#include "../LWE.h"
#include "../FHEW.h"
#include <cassert>
#include "common.h"
#include "variables.h"
using namespace std;
#define ARRSIZE 100
LWE::CipherText ct2[ARRSIZE];
LWE::CipherText ct3[ARRSIZE];
int arraySize;

void SaveSecretKey(const LWE::SecretKey* LWEsk, char* filepath) {
  FILE * f;
  f = fopen(filepath, "wb"); // wb -write binary
  if (f == NULL) {
    cerr << "Failed to open "<< filepath <<  " in Write-Binary mode .\n";
    exit(1);
  }
  cerr << "Writing Secret key to "<< filepath <<  ".\n";
  fwrite(LWEsk, sizeof(LWE::SecretKey), 1, f);
  fclose(f);
}

LWE::SecretKey* LoadSecretKey(char* filepath) {
  FILE * f;
  f = fopen(filepath, "rb"); // wb -write binary
  if (f == NULL) {
    cerr << "Failed to open "<< filepath <<  " in Read-Binary mode .\n";
    exit(1);
  }
  LWE::SecretKey* LWEsk = (LWE::SecretKey*) malloc(sizeof(LWE::SecretKey));  
  cerr << "Reading Secret key From "<< filepath <<  ".\n";
  assert(fread(LWEsk, sizeof(LWE::SecretKey), 1, f));
  cerr << "Secret Key read.\n";
  fclose(f);
  return LWEsk;
}

void SaveEvalKey(const FHEW::EvalKey *EK, char* filepath) {
  FILE * f;
  f = fopen(filepath, "wb"); // wb -write binary
  if (f == NULL) {
    cerr << "Failed to open "<< filepath <<  " in Write-Binary mode .\n";
    exit(1);
  }
  cerr << "Writing Evaluation key to "<< filepath <<  ".\n";
  FHEW::fwrite_ek(*EK, f);
  fclose(f);
}

FHEW::EvalKey* LoadEvalKey(char* filepath) {
  	FHEW::EvalKey* EK;
  	FILE * f;
  	f = fopen(filepath, "rb"); // rb -read binary
  	if (f == NULL){
   		cerr << "Failed to open "<< filepath <<  " in Read-Binary mode .\n";
    		exit(1);
  	}
  	cerr << "Reading Evaluation key from "<< filepath <<  ".\n";
  	EK = FHEW::fread_ek(f);
  	cerr << "KSKey Read : " << N << "\t" << KS_base << "\t" << KS_exp << " .\n";
  	fclose(f);
  	return EK;
}

void SaveCipherText(const LWE::CipherText* ct, char* filepath){
  	FILE *f;
  	f = fopen(filepath, "ab"); // wb -write binary
  	if (f == NULL){
    		cerr << "Failed to open "<< filepath <<  " in Write-Binary mode .\n";
    		exit(1);
  	}
//  	cerr << "Writing CipherText to "<< filepath <<  ".\n";
  	assert(fwrite(ct, sizeof(LWE::CipherText), 1, f));
  	fclose(f);
}

LWE::CipherText* LoadCipherText(char* filepath, int arr_size, bool arrType) {
  	FILE *f;
	f = fopen(filepath, "rb"); // rb -read binary
  	if (f == NULL) {
    		cerr << "Failed to open "<< filepath <<  " in Read-Binary mode.\n";
    		exit(1);
  	}	
	
	if(arr_size == 1)
	{
		LWE::CipherText *ct1 = new LWE::CipherText;
	//	cerr << "Loading CipherText from "<< filepath <<  ".\n";
		assert(fread(ct1, sizeof(LWE::CipherText), 1, f));
		fclose(f);
		return ct1;
	}
	else
	{
  	//	cerr << "Loading CipherText from "<< filepath <<  ".\n";
		if(arrType)
		{
			assert(fread(ct2, sizeof(LWE::CipherText), arr_size, f));
			fclose(f);
			return ct2;
		}
		else
		{
			assert(fread(ct3, sizeof(LWE::CipherText), arr_size, f));
			fclose(f);
			return ct3;
		}
	}
}

LWE::CipherText* get(int index, bool arrType)
{
	if(arrType)
		return &ct2[index];
	else
		return &ct3[index];
}

void setValueToFile(int size, char* filepath)
{
	FILE *f;
	f = fopen(filepath,"w");
	if (f == NULL) {
	    cerr << "Failed to open  file in Write  mode"<< filepath << " .\n";
  	    exit(1);
 	 }
  	//cerr << "Writing values to file"<<filepath<< ".\n";
  	//assert(fwrite(size, sizeof(size), 1, f));
	assert(fprintf(f, "%d",size));
}

int getValueFromFile(char* filepath)
{
	FILE *f;
	int size = 0;
	f = fopen(filepath,"r");
	if (f == NULL) {
            cerr << "Failed to open  file in Read  mode"<<filepath << ".\n";
            exit(1);
         }
        //cerr << "Reading values from file"<<filepath<< ".\n";
        //return assert(fread(size, sizeof(size), 1, f));
	fscanf(f, "%d",&size);
	return size;
}
