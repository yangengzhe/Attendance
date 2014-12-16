#include <rtc_clock.h>
#include "HardwareSerial.h"
#include <Bluetooth_HC05.h>
#include <UTFT.h>
#include <ASC24.h>
#include <dht11.h>
#include <Adafruit_PN532.h>

#define delay_time 300//等待延迟,单位秒 
#define tip_time 3000//提示持续时间,单位毫秒 

#define FLASH_CS_PIN 52 
#define SPI_RATE 4

#define BVS_13 10
#define BVS_15 12
#define BVS_19 14
#define BVS_22 18
#define BVS_28 22
#define BVS_34 28
#define BVS_43 38
#define BVS_52 53
#define BVS_74 78
#define BVS_112 122

#define DCLK     15
#define DIN      14 
#define CS       8  
#define DOUT     9

#define SCK  (2)
#define MOSI (3)
#define SS   (4)
#define MISO (5)
//配置数据
int a =0;
int DIRpage=0;
int MDpage=0;//学生名单页数
int Mode=0;//连接方式，默认0 为蓝牙，1为U盘
int InMode=0;//导入方式，默认0 为蓝牙，1为U盘
//读卡
Adafruit_PN532 nfc(SCK, MISO, MOSI, SS);
unsigned long int Last_UID=0;
int RepeatTime=0;
//温度
dht11 DHT11;
//时钟
RTC_clock rtc_clock(RC);
int hh,mm,ss,dow,dd,mon,yyyy;
int yy1,m1,d1,h1,mm1,tem1;
//显示
unsigned int TP_X,TP_Y;    
unsigned int TouchCount=0;
 int  pacy=0;
bool button=false;
UTFT myGLCD(CTE70); 
//状态
int state=0;
//蓝牙
bool bluetoothpswd=false;
Bluetooth_HC05 hc05(Serial1); 
//U盘
int fileNum = 0;//0开始
int inLenth = 0;
int model=0;
unsigned long int fileSize = 0;
//文件信息
typedef struct {
   char name[30];
   int type;//type=1 :file,type=0 :directory
 } fileInfo;
 fileInfo f[300];
//教师信息
typedef struct 
{
   char name[9];
   char number[10];
   unsigned long int uid;
 } teacher;
teacher t[2000];
int TeacherSign=0;
int CurTeacher=0;//当前教师序号
int TeacherNum=0;//从0开始计数
//学生信息
struct student
{
  char name[9];
  char number[10];
  char grade[13];
  unsigned long int uid;
  int sign;
} s[200];
int StudentNum=0;

void setup() {
//时钟
  rtc_clock.init();
  rtc_clock.set_time(__TIME__);
  rtc_clock.set_date(__DATE__);
//U盘串口复位
  pinMode(21, OUTPUT);
  digitalWrite(21, LOW);
  delay(100);
  digitalWrite(21, HIGH);
//蓝牙模块  
  hc05.begin(38400, 22, 24, HC05_MODE_COMMAND);
  delay(700);
  hc05.setRole(HC05_ROLE_SLAVE);         //设置蓝牙模块为从端
  hc05.begin(38400, 22, 24, HC05_MODE_DATA); 
//SPI
  myGLCD.SPI_Flash_init(FLASH_CS_PIN,SPI_RATE);
  pinMode( DCLK ,OUTPUT);
  pinMode(DIN ,OUTPUT);
  pinMode(CS,OUTPUT);
  pinMode(DOUT,INPUT);  
  Serial1.begin(38400);
  Serial2.begin(115200);
  Serial.begin(9600);
  myGLCD.InitLCD();
  myGLCD.clrScr();
  NFC_init();
}

void loop() {
  /************************************************ state 0 *********************************************/
  if(state==0)//初始状态
  {
    s0_Init();
    //显示屏幕内容s1，变量为温度
    DHT11.read(29);
    DisplayClock((int)DHT11.temperature);
    //判断是否加载教师信息，若未加载并加载
    if(TeacherSign==0)
      if(ReadTeacher()!=0) 
      {
        Error0();
        delay(3000);
        myGLCD.clrScr();
      }
      else TeacherSign=1;
      
      
    //判断是否教师刷卡
    unsigned long int temp_UID=0; 
    int i;
    temp_UID=ReadNFC(3);
    if(temp_UID!=0)
    {
      for(i=0;i<TeacherNum;i++)
        if(temp_UID==t[i].uid) break;
      if(i<TeacherNum)
      {
        CurTeacher=i;
        XBEE_SendTeacher(CurTeacher,&Serial);
        state=1; 
        myGLCD.clrScr();
      }
      else
      {
        Error1();
        delay(tip_time);
        myGLCD.clrScr();
      }
    }
  }
  /************************************************ state 1 *********************************************/
  else if(state==1)//选择U盘还是蓝牙
  {
    if(return_s0()) return;//判断是否教师刷卡,刷卡则返回
    s1(t[CurTeacher].name);

  
    if(PressButton(264,288,360,360)==1)
    {
      state=2;
      Mode=0;
      myGLCD.clrScr();
    }
    else if(PressButton(456,288,552,360)==1)
    {
      state=7;
      Mode=1;
      myGLCD.clrScr();
    }
  }
  /************************************************ state 2 *********************************************/
  else if(state==2)//蓝牙配对
  {
    
    if(bluetoothpswd==false)
    {
      char password[5];
      RandPassword(password);
      s2(password);
      ChangePSWD(password);
      bluetoothpswd=true;
    }
    byte a,b;
    if(GetSelect(&a,&b,delay_time,&Serial1)==0)
      if(a==0x01 && b==0x01)
      {
        byte str[6]={0xFF,0xFF,0x01,0xFD,0x01,0xFD};
        SendStr(str,6,&Serial1);
        state=3;
        Mode=0;
        myGLCD.clrScr();
      }
      else;
    else 
    {
      state=0;
      myGLCD.clrScr();
    }
    
  }
  /************************************************ state 3 *********************************************/
  else if(state==3)//选择名单导入方式,蓝牙还是U盘
  {
    if(return_s0()) return;//判断是否教师刷卡,刷卡则返回
    s3();
    
    byte a,b;
    if(GetSelect(&a,&b,delay_time,&Serial1)==0)
      if(a==0x01 && b==0x02)
      {
        state=4;
        InMode=0;
        byte str[3]={0xFF,0xFF,0x06};
        SendStr(str,3,&Serial1);//收到后返回
        myGLCD.clrScr();
      }
      else if(a==0x01 && b==0x03)
      {
        state=7;
        InMode=1;
        byte str[3]={0xFF,0xFF,0x06};
        SendStr(str,3,&Serial1);//收到后返回
        myGLCD.clrScr();
      }
      else;
    else 
    {
      state=0;
      myGLCD.clrScr();
    }
  }
  /************************************************ state 4 *********************************************/
  else if(state==4)//通过蓝牙导入名单
  {
    if(return_s0()) return;//判断是否教师刷卡,刷卡则返回
    s4();
    
    if(GetMD(delay_time,&Serial1)==0)
    {
      XBEE_SendMD(&Serial);
      state=5;
      myGLCD.clrScr();
      DisplayStudent(0);
    }
    else//失败
    {
      state=0;
      myGLCD.clrScr();
    }
  }
  /************************************************ state 5 *********************************************/
  else if(state==5)//点名列表
  {    
    //翻页
    
    //DisplayStudent(MDpage);
    if(PressButton(528,384,800,480)==1)
    {
      MDpage++;
      if(MDpage<=StudentNum/15)  DisplayStudent(MDpage);
      else MDpage--;
    }
    else if(PressButton(0,384,240,480)==1)
    {
      MDpage--;
      if(MDpage>=0)  DisplayStudent(MDpage);
      else MDpage++;
    }
    
    
    //判断是否学生刷卡
    unsigned long int temp_UID=0; 
    int i;
    temp_UID=ReadNFC(3);
    if(temp_UID!=0)
    {
      //任课教师刷卡
      if(temp_UID==t[CurTeacher].uid)
      {
        XBEE_SendTeacher(CurTeacher,&Serial);
        byte str[6]={0xFF,0xFF,0x01,0xFD,0x05,0xFD};
        if(Mode==0)//如果是蓝牙模式则传回名单
              SendStr(str,6,&Serial1);
        myGLCD.clrScr();
        state=6;
        return;
      }
      //其它教师刷卡
      for(i=0;i<TeacherNum;i++)
        if(temp_UID==t[i].uid) break;
      if(i<TeacherNum)
      {
        state=0; 
        myGLCD.clrScr();
        return;
      }
      else//非教师卡
      {
        int k=0;
        for(k=0;k<StudentNum;k++)
          if(s[k].uid==temp_UID) break;
        if(k<StudentNum)//学生卡
        {
          if(s[k].sign==1)//已签到
          {
            Error2();
            delay(tip_time);
            myGLCD.setColor(0,0,0);
            myGLCD.fillRect(241,385,527,479);
            myGLCD.setColor(255,255,255);
//            DisplayStudent(MDpage);
          }
          else//未签到
          {
            s[k].sign=1;
            Success();
            
  for(int count=0;count<15;count++)
  {
    if(MDpage*15+count>StudentNum) break;
    if(s[MDpage*15+count].sign==1) one(0xCA,0xC7,count+1,23);
  }
            XBEE_SendStudent(k,&Serial);
            if(Mode==0)//如果是蓝牙模式则传回名单
              SendStudent(k,&Serial1);
            delay(tip_time);
            myGLCD.setColor(0,0,0);
            myGLCD.fillRect(241,385,527,479);
            myGLCD.setColor(255,255,255);
//            DisplayStudent(MDpage);
          }
        }
        else //无效卡
        {
          Error1();
          delay(tip_time);
          myGLCD.setColor(0,0,0);
          myGLCD.fillRect(241,385,527,479);
          myGLCD.setColor(255,255,255);
//          DisplayStudent(MDpage);
        }
      }
    }
  }
  /************************************************ state 6 *********************************************/
  else if(state==6)//点名结束
  {
    s6();
    
    delay(tip_time);
    if(Mode==1) state=10;
    else  
    {state=0;
    myGLCD.clrScr();}
  }
  /************************************************ state 7 *********************************************/
  else if (state==7)//U盘初始化
  {
    if(return_s0()) return;//判断是否教师刷卡,刷卡则返回
    s7();
    if(DIR()==-1) 
    {state=8;
    myGLCD.clrScr();}
    else
    {
      state=9;//成功
      DIR();
      myGLCD.clrScr();
      DIRTOSCREEN(0);
    }
  }
  /************************************************ state 8 *********************************************/
  else if (state==8)//初始化失败
  {
    if(return_s0()) return;//判断是否教师刷卡,刷卡则返回
    s8();
    
    delay(tip_time);
    myGLCD.clrScr();
    state=7;
  }
  /************************************************ state 9 *********************************************/
  else if (state==9)//初始化成功
  {
    int CurDir=-1;
    if(return_s0()) return;//判断是否教师刷卡,刷卡则返回
    if(PressButton(528,384,800,480)==1)
    {
      DIRpage++;
      if(DIRpage<=fileNum/10)  DIRTOSCREEN(DIRpage);
      else DIRpage--;
    }
    else if(PressButton(0,384,240,480)==1)
    {
      DIRpage--;
      if(DIRpage>=0)  DIRTOSCREEN(DIRpage);
      else DIRpage++;
    }
    else if((CurDir=SelectDIR()) != -1)//选择目录
    {
      CurDir--;
      if(f[CurDir].type==0)//选中文件夹
      {
        CD(f[CurDir].name);
        myGLCD.clrScr();
        DIR();
        DIRTOSCREEN(0);
        return;
      }
      else if(f[CurDir].type==1)//选中文件
      {
        DIRFile(f[CurDir].name);
        if(RD(f[CurDir].name)!=0)
        {
          DIR();
          myGLCD.clrScr();
          DIRTOSCREEN(DIRpage);
          return;
        }
        if(InMode==1 && Mode==0)//U盘导入,连接为蓝牙
        {
          byte str[6]={0xFF,0xFF,0x01,0xFD,0x04,0xFD};
          if(SendMD(&Serial1)==0)
          SendStr(str,6,&Serial1);
        }
        myGLCD.clrScr();
        XBEE_SendMD(&Serial);
        DisplayStudent(0);
        state=5;
        
      }
    }
//    ShowDIR();    
  }
  /************************************************ state 10 *********************************************/
  else if(state==10)
  {
   rtc_clock.get_time(&hh,&mm,&ss);
   rtc_clock.get_date(&dow,&dd,&mon,&yyyy);
   char str[20],c_yyyy[4],c_mon[2],c_dd[2],c_hh[2];
   int k;
   sprintf(c_yyyy,"%d",yyyy);
   if(mon<10)
   { c_mon[0]='0';
     c_mon[1]='0'+mon;}
   else   sprintf(c_mon,"%d",mon);
   
   if(dd<10)
   { c_dd[0]='0';
     c_dd[1]='0'+dd;}
   else   sprintf(c_dd,"%d",dd);
   
   if(hh<10)
   { c_hh[0]='0';
     c_hh[1]='0'+hh;}
   else   sprintf(c_hh,"%d",hh);
   for(k=0;k<2;k++) str[k]=c_yyyy[2+k];
   for(k=2;k<4;k++) str[k]=c_mon[k-2];
   for(k=4;k<6;k++) str[k]=c_dd[k-4];
   for(k=6;k<8;k++) str[k]=c_hh[k-6];
   str[k]='.';
   str[k+1]='x';
   str[k+2]='m';
   str[k+3]='l';
   str[k+4]='\0';
   SaveDM("front.txt",str,yyyy,mon,dd,hh,mm,ss);
   SaveDM2(str,yyyy,mon,dd,hh,mm,ss);
   SaveDM("foot.txt",str,yyyy,mon,dd,hh,mm,ss);
   s10();
   delay(tip_time);
   state=0;
   myGLCD.clrScr();
  }
}
