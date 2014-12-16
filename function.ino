/**************************** state 0 ***********************************/
int ReadTeacher()
{
      int i=0;
      int j=0;//count
      int m=0;
      while(SdRead(i,"Teacher.txt")!= 255)
      {   
        if(j>=2000) return -1;//数组越界
        for(m=0;SdRead(i,"Teacher.txt") != 32;i++,m++)
        {
          if(m>8) return -2;//格式错误
        t[j].name[m]=SdRead(i,"Teacher.txt");
        }
        t[j].name[m]='\0';
        i++;
        for(m=0;SdRead(i,"Teacher.txt") != 32;i++,m++)
        {
          if(m>9) return -2;//格式错误
        t[j].number[m]=SdRead(i,"Teacher.txt");
        }
        i++;
        char temp[8];
        for( m=0;(SdRead(i,"Teacher.txt") != 13 && SdRead(i,"Teacher.txt")!= 255);i++,m++)
        {
          if(m>7) return -2;//格式错误
          //t[j].uid[m]=SdRead(i,"Teacher.txt");
          temp[m]=SdRead(i,"Teacher.txt");
        }
        t[j].uid=strtoul(temp,0,16);
        i=i+2;
        j++;
      }
      TeacherNum=j;
      return 0;
}
/**************************** state 2 ***********************************/
int RandPassword(char * password)
{ 
  randomSeed(analogRead(0)); 
  for(int i=0;i<4;i++) 
  { 
    *(password+i)=random(9)+'0';
  }
  *(password+4)='\0';
  return 0;
}

bool ChangePSWD(char *password)//change password
{
  bool result;
  hc05.begin(38400, 22, 24, HC05_MODE_COMMAND);
  delay(700);
  result=hc05.setPassword(password);
  hc05.begin(38400, 22, 24, HC05_MODE_DATA);
  return result;
}

/***************************** U 盘 **********************************/
void DIRTOSCREEN(int page)
{
   myGLCD.clrScr();
   s9();
   float hang=0;
   for(int i=0;i<10;i++)
   {
     hang=hang+1;
     if(i+page*10>fileNum) return;
     int k=5;//记一行个数
     if(f[i+page*10].type==0)
     {
       one(0x3C,hang,1,'a');
       one(0x44,hang,1,'b');
       one(0x49,hang,2,'a');
       one(0x52,hang,2,'b');
       one(0x3E,hang,3,'a');
     }
     
   for(int j=0;j<30;j++)
   {
   if(f[i+page*10].name[j]!=NULL)
   {
     if(f[i+page*10].name[j]<160)
     one(f[i+page*10].name[j],hang,k,'a'); 
     else 
     {
       one(f[i+page*10].name[j],f[i+page*10].name[j+1],hang,k);
      j++;
     } 
     k++;
   }
   else break;
   }
   
   }
}
/***************************************************************************************/
void CreateTime(int year,int month,int day,int hour,int minute,int seconds,char * c_time)
{
  unsigned long int i_time=0;
  i_time= ((year-1980)<<25) | (month << 21) | (day << 16) | (hour<<11) | (minute <<5) | (seconds/2);
  sprintf(c_time,"%8x",i_time);
}
int getbyte(Stream *Sera,int timeout)
{
  unsigned long start = millis();
  while((*Sera).available()<=0)
  if((int)(millis()-start)>timeout*1000) return -1;
  if((*Sera).available() > 0)  return (*Sera).read();
}

byte getbyte(Stream *Sera)
{
  while((*Sera).available()<=0);
  if((*Sera).available() > 0)  return (*Sera).read();
}

int SaveDM(const char * OpenFile,const char * SaveFile,int year,int month,int day,int hour,int minute,int seconds)
{
    char time[8];
    CreateTime(year,month,day,hour,minute,seconds,time);//创建时间
    byte buf[50];
    byte temp=0;
    SdOpen(OpenFile);
    OPW(SaveFile,time);
    while(temp!=255)
    {
    int i=0;
    for(int k=0;k<50;k++)
    {
    temp=SdRead(OpenFile);
    if(temp==255) 
    {     
      break;
    }
    buf[k]=temp;
    i++;
    }
    WR(buf,i);
    }
    ClOpen(OpenFile);
    CLF(SaveFile);  
    return 0;
}

int SaveDM2(const char * SaveFile,int year,int month,int day,int hour,int minute,int seconds)
{
  char time[8];
  CreateTime(year,month,day,hour,minute,seconds,time);//创建时间
  byte str1[7]={'<','R','o','w','>',13,10};//<Row>
  byte str2[47]="<Cell ss:StyleID=\"s22\"><Data ss:Type=\"Number\">";
  byte str3[47]="<Cell ss:StyleID=\"s22\"><Data ss:Type=\"String\">";
  byte str4[16]={'<','/','D','a','t','a','>','<','/','C','e','l','l','>',13,10};
  byte str5[8]={'<','/','R','o','w','>',13,10};//<Row>
    int count=0;
    int k=0;
    OPW(SaveFile,time);
    char temp_count[3];

    for(count=0;count<=StudentNum;count++)
    {
    WR(str1,7);
    WR(str2,46);
    sprintf(temp_count,"%d",count+1);
    if(count+1<10)
    WR((byte *)temp_count,1);
    else if(count+1<99)
    WR((byte *)temp_count,2);
    else 
    WR((byte *)temp_count,3);
    WR(str4,14);
    
    WR(str3,46);
    
    for(k=0;k<14;k++)
    if(s[count].grade[k] == '\0') break; 
    WR((byte *)s[count].grade,k);
    
    WR(str4,14);
    
    WR(str3,46);
    
    WR((byte *)s[count].number,10);
    
    WR(str4,14);
    
    WR(str3,46);
    
    for(k=0;k<9;k++)
    if(s[count].name[k] == '\0') break; 
    WR((byte *)s[count].name,k);
    
    WR(str4,14);
    
    WR(str2,46);
    
    byte a[2]={'0','1'};
    if(s[count].sign==1)
    WR(&a[1],1);
    else
    WR(&a[0],1);
    
    WR(str4,14);
    
    WR(str5,8);
    }
    
    CLF(SaveFile);  
    
    return 0;
}
/************************************************************ 显示 **********************************************/
int DisplayStudent(int page)
{
  myGLCD.clrScr();
   s5();
  for(int count=0;count<15;count++)
  {
    if(page*15+count>StudentNum) return -1;
    for(int i=0;i<10;)
    {
      if(s[page*15+count].number[i]<'0'|| s[page*15+count].number[i]>'9') return-1;
    one(s[page*15+count].number[i],count+1,1+i/2,'a');
    one(s[page*15+count].number[i+1],count+1,1+i/2,'b');
    i=i+2;
    }
    for(int i=0;i<8;)
    {
      if(i>0) 
        if(s[page*15+count].name[i]=='\0' || s[page*15+count].name[i-1]=='\0') break;
      else 
        if(s[page*15+count].name[i]=='\0' ) break;
      if(s[page*15+count].name[i]>160)
      {
        one(s[page*15+count].name[i],s[page*15+count].name[i+1],count+1,9+i/2+i%2);
        i=i+2;
      }
      else
      {
        one(s[page*15+count].name[i],count+1,9+i/2+i%2,'a');
        if(s[page*15+count].name[i+1]<160 && s[page*15+count].name[i+1]>32)
        {
          one(s[page*15+count].name[i+1],count+1,9+i/2+i%2,'b');
          i=i+2;
        }
        else i++;
      }
    }
    
    for(int i=0;i<12;)
    {
      if(i>0)
        if(s[page*15+count].grade[i]=='\0' || s[page*15+count].grade[i-1]=='\0' ) break;
      else
        if(s[page*15+count].grade[i]=='\0') break;
      if(s[page*15+count].grade[i]>160)
      {
        one(s[page*15+count].grade[i],s[page*15+count].grade[i+1],count+1,15+i/2+i%2);
        i=i+2;
      }
      else
      {
        one(s[page*15+count].grade[i],count+1,15+i/2+i%2,'a');
        if(s[page*15+count].grade[i+1]<160 && s[page*15+count].grade[i+1]>32)
        {
          one(s[page*15+count].grade[i+1],count+1,15+i/2+i%2,'b');
          i=i+2;
        }
        else i++;
      }
    }
    
    if(s[page*15+count].sign==1) one(0xCA,0xC7,count+1,23);
  }
}

void NFC_init()
{
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    while (1); // halt
  }
  nfc.SAMConfig();  
  
}

unsigned long int ReadNFC(int Repeat)
{
  unsigned long int Read_UID=0;
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };	// Buffer to store the returned UID
  uint8_t uidLength;				// Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if (success) {
    //转换格式
    Read_UID=uid[0]*16777216+Read_UID;
    Read_UID=uid[1]*65536+Read_UID;
    Read_UID=uid[2]*256+Read_UID;
    Read_UID=uid[3]+Read_UID;
    if(Last_UID!=Read_UID)
    {
      Last_UID=Read_UID;
      return Read_UID;
    }
    else 
    {
      RepeatTime++;
      if(RepeatTime>=Repeat*5)
      {
        RepeatTime=0;
        return Read_UID;
      }
      else return 0;
    }
  }
  else return 0;
}

/***************************** 蓝牙 **********************************/

int SendStr(byte *str1,int num,Stream *Sera)
{
  for(int i=0;i<num;i++)
  {
    (*Sera).write(*(str1+i));
  }
}

int SendMD(Stream *Sera)
{
  byte Data[37]={0xFF,0xFF,0x03,0xFF};
  for(int Cur=0;Cur<=StudentNum;Cur++)
  {
    int k,m;
    for(int i=0;i<10;i++)
    Data[i+4]=s[Cur].number[i];
    Data[14]=0xFF;
    for(k=15;k<15+8;k++)
    {
      if(s[Cur].name[k-15]=='\0') break;
      Data[k]=s[Cur].name[k-15];
    }
    Data[k]=0xFF;
    m=k+1;
    for(m;m<k+13;m++)
    {
      if(s[Cur].grade[m-k-1]=='\0') break;
      Data[m]=s[Cur].grade[m-k-1];
    }
    Data[m]=0xFF;
    SendStr(Data,m+1,Sera);
  }
  
  return 0;//成功
}

int SendStudent(int Cur,Stream *Sera)
{
  byte Data[37]={0xFF,0xFF,0x04,0xFF};
  int k,m;
  for(int i=0;i<10;i++)
  Data[i+4]=s[Cur].number[i];
  Data[14]=0xFF;
  for(k=15;k<15+8;k++)
  {
    if(s[Cur].name[k-15]=='\0') break;
    Data[k]=s[Cur].name[k-15];
  }
  Data[k]=0xFF;
  m=k+1;
  for(m;m<k+13;m++)
  {
    if(s[Cur].grade[m-k-1]=='\0') break;
    Data[m]=s[Cur].grade[m-k-1];
  }
  Data[m]=0xFF;
  SendStr(Data,m+1,Sera);
}
/*************************** 读取 ***************************/
int GetMD(int timeout,Stream *Sera)
{
  int Temp_StudentNum=0;
  byte temp_str[50];
  int i,k,m;
  int judge;
  while(judge=GetStudent(temp_str,timeout,Sera))
  {
    byte str[3]={0xFF,0xFF,0x06};
    SendStr(str,3,&Serial1);
    
    for(i=4;i<14;i++)
      s[Temp_StudentNum].number[i-4]=temp_str[i];
    for(i=0;i<8;i++)
    {
      if(temp_str[15+i]==0xFF) break;
      s[Temp_StudentNum].name[i]=temp_str[15+i];
    }
    s[Temp_StudentNum].name[i]='\0';
    for(k=i+1;k<i+13;k++)
    {
      if(temp_str[15+k]==0xFF) break;
      s[Temp_StudentNum].grade[k-i-1]=temp_str[15+k];
    }
    s[Temp_StudentNum].grade[k]='\0';
    byte temp_UID[4];
    for(m=k+1;m<k+5;m++)
      temp_UID[m-k-1]=temp_str[15+m];
    s[Temp_StudentNum].uid=temp_UID[0]*16777216+s[Temp_StudentNum].uid;
    s[Temp_StudentNum].uid=temp_UID[1]*65536+s[Temp_StudentNum].uid;
    s[Temp_StudentNum].uid=temp_UID[2]*256+s[Temp_StudentNum].uid;
    s[Temp_StudentNum].uid=temp_UID[3]+s[Temp_StudentNum].uid;
    Temp_StudentNum++;
  }
  if(judge==-1) return -1;//超时
  StudentNum=Temp_StudentNum;
  return 0;//成功
}
int GetStudent(byte *str,int timeout,Stream *Sera)
{
  int temp;
  int count=0;//记FD出现次数，判断一条数据
  for(int i=0;i<50;i++)
  {
    temp=getbyte(Sera,timeout);
   
    if(temp==-1) return -1;//超时
    if(temp == 0xFF) count++;
    *(str+i)=temp;
    if(count==7) return 1;//成功
    if(i>4 && *(str+5)==0xFD && *(str+4)==0x04) return 0;//结束
  }
  return -2;//错误
}
//int Compare(byte *str,int strsize,int timeout,Stream *Sera)
//{
//  byte temp_str[10];
//  int i;
//  int temp=GetStr(temp_str,strsize,timeout,Sera);
//  if(temp == -1) return -1;//超时
//  else 
//    for(i=0;i<strsize;i++)
//      if(temp_str[i]!=*(str+i)) break;
//  if(i==strsize) return 1;//成功
//  else return 0;//比对失败
//}
int GetSelect(byte *a,byte *b,int timeout,Stream *Sera)
{
  byte temp_str[6];
  int i;
  int temp=GetStr(temp_str,6,timeout,Sera);
  if(temp == -1) return -1;//超时
  else
    for(i=0;i<2;i++)
      if(temp_str[i]!=0xFF) break;
  if(i==2) //前两个字节校对成功
  *a=temp_str[2];
  else return -1;//接收到的数据格式错误
  if(temp_str[3]==0xFD)//字节校对成功
  *b=temp_str[4];
  else return -1;//接收到的数据格式错误
  if(temp_str[5]==0xFD)
  return 0;//成功
  else return -1;//接收到的数据格式错误
}
int GetStr(byte *str,int num,int timeout,Stream *Sera)
{
  int temp;
  for(int i=0;i<num;i++)
  {
    temp=getbyte(Sera,timeout);
    if(temp == -1) return -1;
    *(str+i)=temp;
  }
  return 0;
}


void s0_Init()
{
    MDpage=0;//学生名单页数
    Mode=0;//连接方式，默认0 为蓝牙，1为U盘
    InMode=0;//导入方式，默认0 为蓝牙，1为U盘
    bluetoothpswd=false;
    Last_UID=0;
    RepeatTime=0;
    TeacherSign=0;
    CurTeacher=0;//当前教师序号
    StudentNum=0;
}

int return_s0()
{
    unsigned long int temp_UID=0; 
    int i;
    temp_UID=ReadNFC(3);
    if(temp_UID!=0)
    {
      for(i=0;i<TeacherNum;i++)
        if(temp_UID==t[i].uid) break;
      if(i<TeacherNum)
      {
        state=0; 
        myGLCD.clrScr();
        return 1;
      }
      else
      {
//        Error1();
//        delay(3000);
//        myGLCD.clrScr();
      }
    }
    return 0;
}


int SelectDIR()
{  
  if(PressButton(0,24,800,60)==1)
    return 0+DIRpage*10;
  else if(PressButton(0,60,800,96)==1)
    return 1+DIRpage*10;
  else if(PressButton(0,96,800,132)==1)
    return 2+DIRpage*10;
  else if(PressButton(0,132,800,168)==1)
    return 3+DIRpage*10;
  else if(PressButton(0,168,800,204)==1)
    return 4+DIRpage*10;
  else if(PressButton(0,204,800,240)==1)
    return 5+DIRpage*10;
  else if(PressButton(0,240,800,276)==1)
    return 6+DIRpage*10;
  else if(PressButton(0,276,800,312)==1)
    return 7+DIRpage*10;
  else if(PressButton(0,312,800,348)==1)
    return 8+DIRpage*10;
  else if(PressButton(0,348,800,384)==1)
    return 9+DIRpage*10;
    
  return -1;
}

/******************************************************************************/

int XBEE_SendTeacher(int Cur,Stream *Sera)
{
  byte Data[21]={'T'};
  int k;
  for(int i=0;i<10;i++)
  Data[i+1]=t[Cur].number[i];
  for(k=11;k<11+8;k++)
  {
    if(t[Cur].name[k-11]=='\0') break;
    Data[k]=t[Cur].name[k-11];
  }
  SendStr(Data,k+1,Sera);
}

void XBEE_SendMD(Stream *Sera)
{
  byte Data[25]={0};
  for(int Cur=0;Cur<=StudentNum;Cur++)
  {
    int k;
    for(int i=0;i<10;i++)
    Data[i]=s[Cur].number[i];
    for(k=10;k<10+8;k++)
    {
      if(s[Cur].name[k-10]=='\0') break;
      Data[k]=s[Cur].name[k-10];
    }
    SendStr(Data,k+1,Sera);
    delay(10);
  }
  XBEE_SendTeacher(CurTeacher,&Serial);
}

void XBEE_SendStudent(int Cur,Stream *Sera)
{
  byte Data[20]={0};
  int k;
  for(int i=0;i<10;i++)
  Data[i]=s[Cur].number[i];
  for(k=10;k<10+8;k++)
  {
    if(s[Cur].name[k-10]=='\0') break;
    Data[k]=s[Cur].name[k-10];
  }
  SendStr(Data,k+1,Sera);
}
