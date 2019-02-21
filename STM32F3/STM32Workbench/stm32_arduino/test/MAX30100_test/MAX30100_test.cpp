#include <Wire.h>
#include <Arduino.h>



char tablica_cyfr[10];
int waga_tablica[10];
int waga_cyfr[]={1,5,10,50,100,500,1000};
int wynik=0;
int liczba_znakow;
int x=0;



void setup() {
  Serial.begin(9600);
  
}

void loop() {
 
 zaladuj_cyfre();
 delay(2);
 
 dopisz_wagi();
 delay(2);
 
 rachunek();
 delay(2);
  
    
  

}
void zaladuj_cyfre(){
  
liczba_znakow=Serial.available();
  for(int i; i<liczba_znakow;i++)
  {  
    char znak=Serial.read();
    tablica_cyfr[i]=znak;
     //Serial.print(tablica_cyfr[0]);
     //Serial.print(tablica_cyfr[1]);
     //Serial.println(tablica_cyfr[2]);
     //Serial.println("koniec1");
     }
 
  }

void dopisz_wagi(){
  
  for(int j;j<liczba_znakow;j++){
    if(tablica_cyfr[j]=='I'){
      waga_tablica[j]=1;
      //Serial.println(waga_tablica[j]);
    }
    if(tablica_cyfr[j]=='V'){
      waga_tablica[j]=5;
      //Serial.println(waga_tablica[j]);
    }
    if(tablica_cyfr[j]=='X'){
      waga_tablica[j]=10;
      //Serial.println(waga_tablica[j]);
    }
    if(tablica_cyfr[j]=='L'){
      waga_tablica[j]=50;
      //Serial.println(waga_tablica[j]);
    }
    if(tablica_cyfr[j]=='C'){
      waga_tablica[j]=100;
      //Serial.println(waga_tablica[j]);
    }
    if(tablica_cyfr[j]=='D'){
      waga_tablica[j]=500;
      //Serial.println(waga_tablica[j]);
    }
    if(tablica_cyfr[j]=='M'){
      waga_tablica[j]=1000;
      //Serial.println(waga_tablica[j]);
    }
     //Serial.println("koniec2");
    
  }
}
void rachunek(){
  for(int i;(i<liczba_znakow)&&x==1;i++){
    if(waga_tablica[i+1]>waga_tablica[i]){
      wynik=wynik-waga_tablica[i];
      //Serial.println(i);
      
      //Serial.println("koniec3.1");
    }
    
    if(waga_tablica[i]>=waga_tablica[i+1]){
      wynik=wynik+waga_tablica[i];
      //Serial.println(i);
      
      //Serial.println("koniec3.2");

    if(i==liczba_znakow-1){
      Serial.println(wynik);
      wynik=0;

    }
    }
     
}
}
  



