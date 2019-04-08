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

//locs array holds the pin numbers of the signals at junction i. locs[i] structure holds the pin numbers through which the signal code for the signal to be turned green will be sent.
location locs[9];

//path flag when set indicates that the user has begun to enter the path for emergency service cycle
int path_flag=0;

//path array holds the path for emergency vehicle will take
int path[9];

//count indicates the number of nodes present 
int count=0;

//present[i] holds the signal code of the signal currently green at junction 'i'.
int present[9]={0,0,0,0,0,0,0,0,0};

//no_of_sigs is the matrix where no_of_sigs[i] denotes the number of signals at junction i.
int no_of_sigs[9]={0,3,0,3,4,3,0,3,0};

//a,b,c and d are used to hold input binary values taken from DTMF module 
unsigned a,b,c,d;

//val holds the decoded decimal value received from DTMF module
int val;

//time1 and time2 are used to keep track of the signal cycle time
unsigned long time1,time2;

//lookup[i][j] in the lookup matrix indicates code for the signal to be activated when the vehicle moves from junction i to j. 
//-1 indicates that there is no direct route from i to j 
int lookup[9][9]={
  {-1, 2,-1, 1,-1,-1,-1,-1,-1},   
  {-1,-1,-1,-1, 2,-1,-1,-1,-1},
  {-1, 0,-1,-1,-1, 0,-1,-1,-1},
  {-1,-1,-1,-1, 1,-1,-1,-1,-1},
  {-1, 1,-1, 2,-1, 2,-1, 1,-1},
  {-1,-1,-1,-1, 3,-1,-1,-1,-1},
  {-1,-1,-1, 0,-1,-1,-1, 0,-1},
  {-1,-1,-1,-1, 0, 1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1, 2,-1}
};

void setup() {
  //setup of input and output ports
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

  //initialize the pin numbers to the respective junctions
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
}

void loop() {
  int i;
  setupLights();
  time1=millis();
  time2=0;
  while(time2<5000)
  {
    //read values from DTMF
    a=digitalRead(A0);
    b=digitalRead(A1);
    c=digitalRead(A2);
    d=digitalRead(A3);

    //calculate decimal value
    val=value(a,b,c,d);

    //start recording path if user 
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
       
        
      }
      else if(val-1!=path[count-1])
      {
        
        val--;
        path[count]=val;
        count++;
                

      }
    }
    if(val==12 && path_flag==1)
    {
      for(int j=0;j<count;j++)
        
     
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
  
  if(j==0 || j==2 || j==6 || j==8)  //freeway as there are no signals at these nodes
    return;

  //
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
