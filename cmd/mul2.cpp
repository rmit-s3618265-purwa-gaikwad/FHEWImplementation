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
int main(int argc, char *argv[]) 
{
  if (argc != 6) help(argv[0]);
  char* ek_fn = argv[1]; 
  char* ict1_fn = argv[2]; 
  char* ict2_fn = argv[3]; 
  char* oct_fn = argv[4]; 
  char* ictIn_fn = argv[5];
  int arraySize = getValueFromFile("size.txt");
  FHEW::Setup();

  EK = LoadEvalKey(ek_fn);

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
  LWE::CipherText *ctIn = LoadCipherText(ictIn_fn, 1, true);
//  LWE::CipherText *ans = product(ct1[0], ct2[0]);
  LWE::CipherText **Ans = mul(ct1, ct2, ctIn, arraySize);
/*	for(int i=0;i<arraySize*2;i++)
        {
                SaveCipherText(Ans[i], "mul.ct");
        }
*/
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
  	for(int i=arraySize-1; i>=0;i--)
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
  		//temp[i] = ct10;
   		memcpy(&ct1[i], &ct10, sizeof(LWE::CipherText));
  		FHEW::HomNAND(ctOut, *EK,*ct7,*ct3);	
		ctIn = ctOut;
  	}
  	//SaveCipherText(ctOut, "11.ct");
	
	//return temp;
}

LWE::CipherText **mul(LWE::CipherText **ct1, LWE::CipherText **ct2, LWE::CipherText *ctIn, int arraySize)
{
//  	LWE::CipherText *ans = product(ct1[0], ct2[0]);
  	LWE::CipherText *ans = new LWE::CipherText[arraySize*2];
  	LWE::CipherText *finalAns = new LWE::CipherText[arraySize*2];
	for(int i =0; i<arraySize*2; i++)
	{
//		ans[l] = new LWE::CipherText;
		memcpy(&ans[i], &ctIn, sizeof(LWE::CipherText));
		//finalAns[l] = new LWE::CipherText;
		memcpy(&finalAns[i], &ctIn, sizeof(LWE::CipherText));
	}
	cout << "Array createed" << endl;
        int k = 0;
	for(int i=arraySize-1;i>=0;i--)
	{
		for(int j =arraySize-1; j>=0; j--)
  		{
			memcpy(&ans[k], product(ct1[j], ct2[i]), sizeof(LWE::CipherText));
			k++;
		}
		//for(int j = k; j < arraySize*2; j++)
		//	memcpy(&ans[j], &ctIn, sizeof(LWE::CipherText));	
					
		// add to the final ans array 
		add(finalAns, ans, ctIn, arraySize*2);
		// new ans array generation
		k = arraySize - i;
		for(int j =0; j< arraySize*2; j++)
		{
			memcpy(&ans[j], &ctIn, sizeof(LWE::CipherText));	
		}
		for(int j=arraySize*2 - 1; j >= 0;i--)
        	{
                	SaveCipherText(&finalAns[j], "mul.ct");
        	}
	}
	/*for(int i=arraySize*2 - 1; i >= 0;i--)
        {
                SaveCipherText(&finalAns[i], "mul.ct");
        }*/
  return &finalAns;
}
