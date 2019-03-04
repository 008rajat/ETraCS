#define d1 2
#define d2 3
#define d3 4
#define d4 5
#define d5 6
#define d6 7
#define d7 8
#define d8 9 
#define d9 10
#define d10 11



typedef struct{
  int b1,b2;
}location;

location locs[9];
int path_flag=0;
int path[9],count=0;
int present[9]={0,0,0,0,0,0,0,0,0};
int no_of_sigs[9]={0,3,0,3,4,3,0,3,0};

unsigned a,b,c,d;
int val;
unsigned long time1,time2;
int lookup[9][9];

void setup() {
  // put your setup code here, to run once:
  int j;
  int i;
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  locs[1].b1=d1;
  locs[1].b2=d2;
  locs[3].b1=d3;
  locs[3].b2=d4;
  locs[4].b1=d5;
  locs[4].b2=d6;
  locs[5].b1=d7;
  locs[5].b2=d8;
  locs[7].b1=d9;
  locs[7].b2=d10;
  for(i=0;i<9;i++)
  {
    for(j=0;j<9;j++)
    {
      lookup[i][j]=-1;
    }
  }
  lookup[0][1]=2;
  lookup[2][1]=0;
  lookup[4][1]=1;
  lookup[0][3]=1;
  lookup[4][3]=2;
  lookup[6][3]=0;
  lookup[1][4]=2;
  lookup[3][4]=1;
  lookup[5][4]=3;
  lookup[7][4]=0;
  lookup[2][5]=0;
  lookup[4][5]=2;
  lookup[7][5]=1;
  lookup[4][7]=1;
  lookup[6][7]=0;
  lookup[8][7]=2;
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int i;
  setupLights();
  time1=millis();
  time2=0;
  while(time2<5000)
  {
    a=digitalRead(A0);
    b=digitalRead(A1);
    c=digitalRead(A2);
    d=digitalRead(A3);
    
    val=value(a,b,c,d);
    if(val==11 && path_flag==0)
    {
      path_flag=1;  
    }    
    
    if(path_flag==1 && val>=1 && val<=9)
    {
      if(count==0)
      {
        
        val--;
        path[count]=val;
        count++;
        Serial.println(val+1);
        
      }
      else if(val-1!=path[count-1])
      {
        
        val--;
        path[count]=val;
        count++;
                Serial.println(val+1);

      }
    }
    if(val==12 && path_flag==1)
    {
      for(int j=0;j<count;j++)
        Serial.print(path[j]);
     
      setupEmergency();
      count=0;
      path_flag=0;
      break;
    }
    delay(100);
    time2=millis()-time1;
  }
}

void setupLights()
{
  int i;
  for(i=0;i<9;i++)
  {
    present[i]=(present[i]+1)%no_of_sigs[i];
    sendSignal(i);
  }
}


void sendSignal(int j)
{
  
  if(j==0 || j==2 || j==6 || j==8)
    return;
  if(present[j]==0){
    digitalWrite(locs[j].b1,LOW);
    digitalWrite(locs[j].b2,LOW);
  }
  if(present[j]==1){
    digitalWrite(locs[j].b1,LOW);
    digitalWrite(locs[j].b2,HIGH);
  }
  if(present[j]==2){
    digitalWrite(locs[j].b1,HIGH);
    digitalWrite(locs[j].b2,LOW);
  }
  if(present[j]==3){
    digitalWrite(locs[j].b1,HIGH);
    digitalWrite(locs[j].b2,HIGH);
  }
  
}
void setupEmergency()
{
  int c1;
  int i;

  int a1,b1,prev;
  
  for(i=0;i<count-1;i++)
  {
    
    a1=path[i];
    b1=path[i+1];
    delay(5000);
    setupLights();
    c1=lookup[a1][b1];
    prev=present[b1];
    present[b1]=c1;
    
    sendSignal(b1);
    present[b1]=prev;
  }
}


int value(int a0,int b0,int c0,int d0)
{
  return a*8+b*4+c*2+d*1;
}
