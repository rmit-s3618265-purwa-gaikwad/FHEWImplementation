#include <iostream>
#include <cstdlib>
#include "../FHEW.h"
#include "common.h"
#include "variables.h"
#include <iostream>
#include <cstdlib>
#include <cassert>

using namespace std;

FHEW::EvalKey* EK;


void help(char* cmd) {
  cerr << "\nusage: " << cmd << " EvalKeyFileName InCTFileName1 InCTFileName2 OutCTFileName  \n\n" 
       << "  Perform Homomorphic NAND computation.\n\n";
  exit(0);
}

int main(int argc, char *argv[]) {
  if (argc != 6) help(argv[0]);
  char* ek_fn = argv[1]; 
  char* ict1_fn = argv[2]; 
  char* ict2_fn = argv[3]; 
  char* oct_fn = argv[4]; 
  char* ictIn_fn = argv[5];
  int arraySize = getValueFromFile("size.txt");
  FHEW::Setup();

  EK = LoadEvalKey(ek_fn);

  LWE::CipherText *ct3,*ct4, *ct5, *ct6, *ct7, *ctIn, *ct8, *ct9, *p0, *p1, *p2, *p3, *ctOut;
 cout<<"array size is : "<<arraySize<<endl;
  LoadCipherText(ict1_fn, arraySize, true);
  LWE::CipherText **ct1 = new LWE::CipherText*[arraySize];
  for(int i=0;i<arraySize;i++)
  {
 	 ct1[i] = get(i, true);
  }
  LoadCipherText(ict2_fn, arraySize, false);
  LWE::CipherText **ct2 = new LWE::CipherText*[arraySize];
  for(int i=0;i<arraySize;i++)
  {     
         ct2[i] = get(i, false);
  }
  ct3 = new LWE::CipherText;
  ct4 = new LWE::CipherText;
  ct5 = new LWE::CipherText;
  ct6 = new LWE::CipherText;
  ct7 = new LWE::CipherText;
  ct8 = new LWE::CipherText;
  ct9 = new LWE::CipherText;
  p0 = new LWE::CipherText;
  p1 = new LWE::CipherText; 
  p2 = new LWE::CipherText;
  p3 = new LWE::CipherText;
  ctOut = new LWE::CipherText;
  ctIn = LoadCipherText(ictIn_fn, 1, true);

  // P0 = B0A0
  FHEW::HomNAND(ct3, *EK,*ct1[1],*ct2[1]);
  FHEW::HomNAND(p0, *EK,*ct3,*ct3);

// ct4 = B0A1
  FHEW::HomNAND(ct3, *EK,*ct1[0],*ct2[1]);
  FHEW::HomNAND(ct4, *EK,*ct3,*ct3);

// ct5 = B1A0
  FHEW::HomNAND(ct3, *EK,*ct1[1],*ct2[0]);
  FHEW::HomNAND(ct5, *EK,*ct3,*ct3);

// ct6 = B1A1
  FHEW::HomNAND(ct3, *EK,*ct1[0],*ct2[0]);
  FHEW::HomNAND(ct6, *EK,*ct3,*ct3);

//  HALF ADDER
 	FHEW::HomNAND(ct7, *EK,*ct4,*ct5);	
  	FHEW::HomNAND(ct8, *EK,*ct4,*ct7);
  	FHEW::HomNAND(ct9, *EK,*ct5,*ct7);
	FHEW::HomNAND(p1, *EK,*ct8,*ct9);		
  	FHEW::HomNAND(ctOut, *EK,*ct7,*ct7);	

//  HALF ADDER
        FHEW::HomNAND(ct7, *EK,*ctOut,*ct6);      
        FHEW::HomNAND(ct8, *EK,*ctOut,*ct7);
        FHEW::HomNAND(ct9, *EK,*ct6,*ct7);   
        FHEW::HomNAND(p2, *EK,*ct8,*ct9); 
        FHEW::HomNAND(p3, *EK,*ct7,*ct7);


  	SaveCipherText(p3, oct_fn);
  	SaveCipherText(p2, oct_fn);
  	SaveCipherText(p1, oct_fn);
  	SaveCipherText(p0, oct_fn);

}
