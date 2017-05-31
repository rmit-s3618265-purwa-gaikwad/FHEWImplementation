#include <iostream>
#include <cstdlib>
#include "../FHEW.h"
#include "common.h"
#include "variables.h"
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstring>

using namespace std;

FHEW::EvalKey* EK;

void help(char* cmd) {
  cerr << "\nusage: " << cmd << " EvalKeyFileName InCTFileName1 InCTFileName2 OutCTFileName  \n\n" 
       << "  Perform Homomorphic NAND computation.\n\n";
  exit(0);
}
LWE::CipherText *product(LWE::CipherText *ct1, LWE::CipherText *ct2);
void add(LWE::CipherText **ct1, LWE::CipherText **ct2, LWE::CipherText *ctIn, int arraySize);
LWE::CipherText **mul(LWE::CipherText **ct1, LWE::CipherText **ct2, LWE::CipherText *ctIn, int arraySize);
char * oct_fn;
int main(int argc, char *argv[]) 
{
  if (argc != 5) help(argv[0]);
  char* ek_fn = argv[1]; 
  char* ict1_fn = argv[2]; 
  char* ict2_fn = argv[3]; 
  oct_fn = argv[4]; 
  
  int arraySize = getValueFromFile("size.txt");
  FHEW::Setup();

  EK = LoadEvalKey(ek_fn);

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
  LWE::CipherText *ctIn = LoadCipherText("0.ct", 1, true);
  LWE::CipherText **Ans = mul(ct1, ct2, ctIn, arraySize);
}
LWE::CipherText *product(LWE::CipherText *ct1, LWE::CipherText *ct2)
{
  	LWE::CipherText* ct3 = new LWE::CipherText;
  	LWE::CipherText* ct4 = new LWE::CipherText;
	FHEW::HomNAND(ct3, *EK,*ct1,*ct2);
  	FHEW::HomNAND(ct4, *EK,*ct3,*ct3);
	return ct4;
}
void add(LWE::CipherText *ct1, LWE::CipherText *ct2, LWE::CipherText *ctIn, int arraySize)
{
  	LWE::CipherText *ct3,*ct4, *ct5, *ct6, *ct7, *ct8, *ct9, *ct10, *ct11, *ctOut;
  	ct3 = new LWE::CipherText;
  	ct4 = new LWE::CipherText;
  	ct5 = new LWE::CipherText;
  	ct6 = new LWE::CipherText;
  	ct7 = new LWE::CipherText;
  	ct8 = new LWE::CipherText;
  	ct9 = new LWE::CipherText;
  	ct10 = new LWE::CipherText;
  	ct11 = new LWE::CipherText; 
  	ctOut = new LWE::CipherText;
  	LWE::CipherText **temp = new LWE::CipherText*[arraySize];
  	for(int i=0; i<arraySize;i++)
  	{
 		FHEW::HomNAND(ct3, *EK,ct1[i],ct2[i]);	
  		FHEW::HomNAND(ct4, *EK,ct1[i],*ct3);
  		FHEW::HomNAND(ct5, *EK,*ct3,ct2[i]);	
  		FHEW::HomNAND(ct6, *EK,*ct4,*ct5);		
  		FHEW::HomNAND(ct7, *EK,*ct6,*ctIn);	
  		FHEW::HomNAND(ct8, *EK,*ct6,*ct7);	
  		FHEW::HomNAND(ct9, *EK,*ct7,*ctIn);	
  		ct10 = new LWE::CipherText;
  		FHEW::HomNAND(ct10, *EK,*ct8,*ct9);	
		copyCipherText(&ct1[i], ct10);
  		FHEW::HomNAND(ctOut, *EK,*ct7,*ct3);	
		ctIn = ctOut;
  	}
}

LWE::CipherText **mul(LWE::CipherText **ct1, LWE::CipherText **ct2, LWE::CipherText *ctIn, int arraySize)
{
  	LWE::CipherText *ans = new LWE::CipherText[arraySize*2];
  	LWE::CipherText *finalAns = new LWE::CipherText[arraySize*2];
	for(int i =0; i<arraySize*2; i++)
	{
		copyCipherText(&ans[i], ctIn);
		copyCipherText(&finalAns[i], ctIn);
	}
        int k = 0;
	for(int i=arraySize-1;i>=0;i--)
	{
		for(int j =arraySize-1; j>=0; j--)
  		{
			copyCipherText(&ans[k], product(ct1[j], ct2[i]));
			k++;
		}
					
		// add to the final ans array 
		add(finalAns, ans, ctIn, arraySize*2);
		// new ans array generation
		k = arraySize - i;
	
		for(int j =0; j< arraySize*2; j++)
		{
			copyCipherText(&ans[j], ctIn);
		}
	}
	for(int i=arraySize*2 - 1; i >= 0;i--)
        {
                SaveCipherText(&finalAns[i], oct_fn);
        }
  return &finalAns;
}
