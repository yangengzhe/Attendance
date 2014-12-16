void one(const int ch1,const int ch2 ,float y,int x)//显示一个中文
{
  byte strA1[72]={0};
  int32_t count=0;
  count=((ch1 - 0xA0 - 1) * 94 + ch2 - 0xA0 - 1) * 72;
  SdRead(count,"Hzk24",strA1);
  int hang=0;
  for(int k=0;k<72;k++)
  {
    if(hang==24) hang=0;
    for(int m=0;m<8;m++)
    {
      byte b_temp = strA1[k] & 0x80;
      if(b_temp!=0) myGLCD.drawPixel(hang+x*24,(int)(k/3)+y*36);
      strA1[k]=strA1[k]<<1;
      hang++;
    }
  }
}

void one(const int ch1,const int ch2 ,int y,int x)//显示一个中文
{
  byte strA1[72]={0};
  int32_t count=0;
  count=((ch1 - 0xA0 - 1) * 94 + ch2 - 0xA0 - 1) * 72;
  SdRead(count,"Hzk24",strA1);
  int hang=0;
  for(int k=0;k<72;k++)
  {
    if(hang==24) hang=0;
    for(int m=0;m<8;m++)
    {
      byte b_temp = strA1[k] & 0x80;
      if(b_temp!=0) myGLCD.drawPixel(hang+x*24,(int)(k/3)+y*24);
      strA1[k]=strA1[k]<<1;
      hang++;
    }
  }
}

void one(const int ch1 ,int y,int x,char z)//显示一个英文或数字，‘a’显示在左侧
{
  byte strA1[48]={0};
  int32_t count=0;
  count=( ch1 - 0x20 ) * 48;
  for(int m=0;m<48;m++)
  strA1[m]=szASC24[count+m];
  if(z=='a')
  {  
    int hang=0;
    for(int k=0;k<48;k++)
    {
      if(hang==16) hang=0;
      for(int m=0;m<8;m++)
      {
        byte b_temp = strA1[k] & 0x80;
        if(b_temp!=0) myGLCD.drawPixel(hang+x*24,(int)(k/2)+y*24);
        strA1[k]=strA1[k]<<1;
        hang++;
      }
    }
  }
    else
    {
    int hang=0;
    for(int k=0;k<48;k++)
    {
      if(hang==16) hang=0;
      for(int m=0;m<8;m++)
      {
        byte b_temp = strA1[k] & 0x80;
        if(b_temp!=0) myGLCD.drawPixel(hang+x*24+12,(int)(k/2)+y*24);
        strA1[k]=strA1[k]<<1;
        hang++;
      }
    }
    }
}

void one(const int ch1 ,float y,int x,char z)//显示一个英文或数字，‘a’显示在左侧
{
  byte strA1[48]={0};
  int32_t count=0;
  count=( ch1 - 0x20 ) * 48;
  for(int m=0;m<48;m++)
  strA1[m]=szASC24[count+m];
  if(z=='a')
  {  
    int hang=0;
    for(int k=0;k<48;k++)
    {
      if(hang==16) hang=0;
      for(int m=0;m<8;m++)
      {
        byte b_temp = strA1[k] & 0x80;
        if(b_temp!=0) myGLCD.drawPixel(hang+x*24,(int)(k/2)+y*36);
        strA1[k]=strA1[k]<<1;
        hang++;
      }
    }
  }
    else
    {
    int hang=0;
    for(int k=0;k<48;k++)
    {
      if(hang==16) hang=0;
      for(int m=0;m<8;m++)
      {
        byte b_temp = strA1[k] & 0x80;
        if(b_temp!=0) myGLCD.drawPixel(hang+x*24+12,(int)(k/2)+y*36);
        strA1[k]=strA1[k]<<1;
        hang++;
      }
    }
    }
}


void spistar()                                     //SPI Start
{
  digitalWrite(CS,LOW);
  digitalWrite(DCLK,HIGH);
  digitalWrite(DIN,HIGH);
  digitalWrite(DCLK,HIGH);
 
}
//**********************************************************
void WriteCharTo7843(unsigned char num)          //SPI Write Data
{
  unsigned char count=0;
  unsigned char temp;
  unsigned nop;
  temp=num;
  digitalWrite(DCLK,LOW);
  delay(1);
  for(count=0;count<8;count++)
  {
    if(temp&0x80)
      digitalWrite(DIN,HIGH);
    else
      digitalWrite(DIN,LOW);
 
    temp=temp<<1; 
    delayMicroseconds(5);
    digitalWrite(DCLK,LOW);                
    delayMicroseconds(5);
    digitalWrite(DCLK,HIGH);
    delayMicroseconds(5);
  }
}
 
//**********************************************************
unsigned int ReadFromCharFrom7843()             //SPI Read Data
{ 
  unsigned nop;
  unsigned char count=0;
  unsigned int Num=0;
  for(count=0;count<12;count++)
  {
    Num<<=1;
    digitalWrite(DCLK,HIGH);//DCLK=1; _nop_();_nop_();_nop_();                
    delayMicroseconds(5);
    digitalWrite(DCLK,LOW);//DCLK=0; _nop_();_nop_();_nop_();
    delayMicroseconds(5);
    if(digitalRead(DOUT)) Num++;
    delayMicroseconds(5);
  }
  return(Num);
}
 
 void AD7843(void)              
{
  digitalWrite(CS,LOW);   
  delayMicroseconds(5); 
  WriteCharTo7843(0x90);  
  delayMicroseconds(5); 
  digitalWrite(DCLK,HIGH);
  delayMicroseconds(5);
  digitalWrite(DCLK,LOW); 
  delayMicroseconds(5);
 

  TP_Y=ReadFromCharFrom7843();
     digitalWrite(CS,HIGH);
     delayMicroseconds(5);
     digitalWrite(CS,LOW);   
     delayMicroseconds(5);
 
  WriteCharTo7843(0xD0);      
  delayMicroseconds(5);
  digitalWrite(DCLK,HIGH);
  delayMicroseconds(5);
  digitalWrite(DCLK,LOW);
  delayMicroseconds(5);
  TP_X=ReadFromCharFrom7843();
  delayMicroseconds(5);
  digitalWrite(CS,HIGH);
  delayMicroseconds(5);
  
  TP_X=(float)(4095-TP_X)/4095 * 800;
  TP_Y=(float)TP_Y/4095 * 480;
}

//int PressButton(int l_x, int l_y,int r_x,int r_y)
//{
//  int Judge=JudgeSelect(l_x,l_y,r_x,r_y);
//  if(Judge==1 && button==false)
//  {button=true;
//  return 1;}
//  else if(Judge==0)
//  button=false;
//  return 0;
//}

int PressButton(int l_x, int l_y,int r_x,int r_y)
{
  int Select=SelectButton(l_x,l_y,r_x,r_y);
  int times=0;
  for(;times<5;times++)
  {
    if(Select==0) break;
    //delay(50);
  }
  if(times==5) return 1;
  else return 0;
}

//int JudgeSelect(int l_x, int l_y,int r_x,int r_y)
//{
//  int Select=SelectButton(l_x,l_y,r_x,r_y);
//  int times=0;
//  for(;times<5;times++)
//  {
//    if(Select==0) break;
//    //delay(50);
//  }
//  if(times==5) return 1;
//  else return 0;
//}

int SelectButton(int l_x, int l_y,int r_x,int r_y)
{
  AD7843();
  if(TP_X<r_x && TP_X>l_x)
    if(TP_Y<r_y && TP_Y>l_y)
      return 1;
    else;
  else;
  return 0;
}

//int SelectDIR()
//{
//  AD7843();
//  if(TP_Y>24 && TP_Y<60)
//    return 0+DIRpage*10;
//  else if(TP_Y>60 && TP_Y<96)
//    return 1+DIRpage*10;
//  else if(TP_Y>96 && TP_Y<132)
//    return 2+DIRpage*10;
//  else if(TP_Y>132 && TP_Y<168)
//    return 3+DIRpage*10;
//  else if(TP_Y>168 && TP_Y<204)
//    return 4+DIRpage*10;
//  else if(TP_Y>204 && TP_Y<240)
//    return 5+DIRpage*10;
//  else if(TP_Y>240 && TP_Y<276)
//    return 6+DIRpage*10;
//  else if(TP_Y>276 && TP_Y<312)
//    return 7+DIRpage*10;
//  else if(TP_Y>312 && TP_Y<348)
//    return 8+DIRpage*10;
//  else if(TP_Y>348 && TP_Y<384)
//    return 9+DIRpage*10;
//  else return -1;
//}

int s0(int year,int month,int day,int h,int m,int tem)
{
    if(year<2013||year>2099||month>12||month<1||day<1||day>31||h<0||h>24||m<0||m>60)
  return -1;
  char c_year[4],c_month[2],c_day[2],c_h[2],c_m[2],c_tem[2];
  sprintf(c_year,"%d",year);
  sprintf(c_month,"%d",month);
  sprintf(c_day,"%d",day);
  sprintf(c_h,"%d",h);
  sprintf(c_m,"%d",m);
  sprintf(c_tem,"%d",tem);
  
  myGLCD.setColor(255,255,255);
  one(c_year[0],2,3,'a');
  one(c_year[1],2,3,'b');
  one(c_year[2],2,4,'a');
  one(c_year[3],2,4,'b');
  if(month<10) one(c_month[0],2,6,'a');
  else  
  {one(c_month[0],2,6,'a');
  one(c_month[1],2,6,'b');}
  if(day<10)   one(c_day[0],2,8,'a');
  else
  {one(c_day[0],2,8,'a');
  one(c_day[1],2,8,'b');}
  if(h<10) one(c_h[0],3,5,'a');
  else
  {one(c_h[0],3,5,'a');
  one(c_h[1],3,5,'b');}
  if(m<10) one(c_m[0],3,7,'a');
  else
  {one(c_m[0],3,7,'a');
  one(c_m[1],3,7,'b');}
  if(tem<10) one(c_tem[0],3,16,'a');
  else
  {one(c_tem[0],3,16,'a');
  one(c_tem[1],3,16,'b');}
  
  myGLCD.setColor(0,255,0);
  for(int i=0;i<144;i++) myGLCD.drawPixel(312,i);
  for(int i=0;i<144;i++) myGLCD.drawPixel(504,i);
  for(int i=0;i<800;i++) myGLCD.drawPixel(i,144);
  myGLCD.setColor(255,255,255);
  one(0xC4,0xEA,2,5);
  one(0xD4,0xC2,2,7);
  one(0xC8,0xD5,2,9);
  one(0xC7,0xEB,12,11);
  one(0xBD,0xCC,12,12);
  one(0xCA,0xA6,12,13);
  one(0xCB,0xA2,12,14);
  one(0xBF,0xA8,12,15);
  one(0xB5,0xE3,12,18);
  one(0xC3,0xFB,12,19);
  one(0xCE,0xB4,12,20);
  one(0xBF,0xAA,12,21);
  one(0xCA,0xBC,12,22);
  one(0xCA,0xB1,3,6);
  one(0xB7,0xD6,3,8);
  one(0xCE,0xC2,2,16);
  one(0xB6,0xC8,2,17);
  one(0xBD,0xCC,2,26);
  one(0xCA,0xD2,2,27);
  //导入教室信息
  int i=0;
  for(int k=0;k<5;)
  {
  if(SdRead(i,"room.txt")==255) break;
    //Serial.println(SdRead(i,"room.txt"));
    if(SdRead(i,"room.txt")<160)
    {one(SdRead(i,"room.txt"),3,24+k,'a');
    i++;
    k++;}
    else
    {one(SdRead(i,"room.txt"),SdRead(i+1,"room.txt"),3,24+k);
    i=i+2;
    k++;}
  }
  return 0;
}

int s1(char *str)
{
  int i=0;
  myGLCD.setColor(255,255,255);
  one(0xBB,0xB6,5,12);
  one(0xD3,0xAD,5,13);
  for(i=0;*str!=0;i=i+2)
  {
    if(i>=8) break;
    one(*(str+i),*(str+i+1),5,14+i/2);
  }
  one(0xC0,0xCF,5,14+i/2);
  one(0xCA,0xA6,5,15+i/2);
  
  one(0xC7,0xEB,8,15);
  one(0xD1,0xA1,8,16);
  one(0xD4,0xF1,8,17);
  one(0xC1,0xAC,8,18);
  one(0xBD,0xD3,8,19);
  one(0xB7,0xBD,8,20);
  one(0xCA,0xBD,8,21);
  
  for(int i=264;i<360;i++) myGLCD.drawPixel(i,288);
  for(int i=456;i<552;i++) myGLCD.drawPixel(i,288);
  for(int i=264;i<360;i++) myGLCD.drawPixel(i,360);
  for(int i=456;i<552;i++) myGLCD.drawPixel(i,360);
  for(int i=288;i<360;i++) myGLCD.drawPixel(264,i);
  for(int i=288;i<360;i++) myGLCD.drawPixel(360,i);
  for(int i=288;i<360;i++) myGLCD.drawPixel(456,i);
  for(int i=288;i<360;i++) myGLCD.drawPixel(552,i);
  one(0xC0,0xB6,13,12);
  one(0xD1,0xC0,13,13);
  one(0x55,13,20,'a');
  one(0xC5,0xCC,13,21);
  return 0;
}

int s2(char * password)
{
  myGLCD.setColor(255,255,255);
  one(0xB0,0xF3,6,14);
  one(0xB6,0xA8,6,16);
  one(0xC3,0xDC,6,18);
  one(0xD4,0xBF,6,20);
  for(int i=288;i<576;i++) myGLCD.drawPixel(i,240);
  for(int i=288;i<576;i++) myGLCD.drawPixel(i,312);
  for(int i=240;i<312;i++) myGLCD.drawPixel(288,i);
  for(int i=240;i<312;i++) myGLCD.drawPixel(360,i);
  for(int i=240;i<312;i++) myGLCD.drawPixel(432,i);
  for(int i=240;i<312;i++) myGLCD.drawPixel(504,i);
  for(int i=240;i<312;i++) myGLCD.drawPixel(576,i);
  one(password[0],11,13,'a');
  one(password[1],11,16,'a');
  one(password[2],11,19,'a');
  one(password[3],11,22,'a');
  return 0;
}

int s3()
{
  myGLCD.setColor(255,255,255);
  one(0xB5,0xC8,8,13);
  one(0xB4,0xFD,8,14);
  one(0xD1,0xA1,8,15);
  one(0xD4,0xF1,8,16);
  one(0xB5,0xBC,8,17);
  one(0xC8,0xEB,8,18);
  one(0xC3,0xFB,8,19);
  one(0xB5,0xA5,8,20);
  one(0xB7,0xBD,8,21);
  one(0xCA,0xBD,8,22);
  one(0x2E,8,23,'a');
  one(0x2E,8,23,'b');
  one(0x2E,8,24,'a');
  return 0;
}

int s4()
{
  myGLCD.setColor(255,255,255);
  one(0xB5,0xC8,8,14);
  one(0xB4,0xFD,8,15);
  one(0xC3,0xFB,8,16);
  one(0xB5,0xA5,8,17);
  one(0xB5,0xBC,8,18);
  one(0xC8,0xEB,8,19);
  one(0x2E,8,20,'a');
  one(0x2E,8,20,'b');
  one(0x2E,8,21,'a');
  return 0;
}

int s5()
{
    myGLCD.setColor(255,255,255);
    one(0xD1,0xA7,0,2);
    one(0xBA,0xC5,0,4);
    one(0xD0,0xD5,0,9);
    one(0xC3,0xFB,0,11);
    one(0xB0,0xE0,0,16);
    one(0xBC,0xB6,0,18);
    one(0xC7,0xA9,0,22);
    one(0xB5,0xBD,0,24);
    one(0xBD,0xCC,4,28);
    one(0xCA,0xA6,5,28);
    one(0xCB,0xA2,6,28);
    one(0xBF,0xA8,7,28);
    one(0xBD,0xE1,10,28);
    one(0xCA,0xF8,11,28);
    one(0xB5,0xE3,12,28);
    one(0xC3,0xFB,13,28);
    
    one(0xD1,0xA7,4,30);
    one(0xC9,0xFA,5,30);
    one(0xCB,0xA2,6,30);
    one(0xBF,0xA8,7,30);
    one(0xBD,0xF8,10,30);
    one(0xD0,0xD0,11,30);
    one(0xC7,0xA9,12,30);
    one(0xB5,0xBD,13,30);
    
    one(0xC9,0xCF,17,4);
    one(0xD2,0xBB,17,5);
    one(0xD2,0xB3,17,6);
    one(0xCF,0xC2,17,26);
    one(0xD2,0xBB,17,27);
    one(0xD2,0xB3,17,28);
    myGLCD.setColor(0,255,0);
    for(int j ; j<17 ;j++)
    for(int i=0;i<624;i++)
    myGLCD.drawPixel(i,j*24);
    for(int i=0;i<800;i++) myGLCD.drawPixel(i,384);
    for(int i=0;i<384;i++) myGLCD.drawPixel(168,i);
    for(int i=0;i<384;i++) myGLCD.drawPixel(336,i);
    for(int i=0;i<384;i++) myGLCD.drawPixel(504,i);
    for(int i=0;i<384;i++) myGLCD.drawPixel(624,i);
    for(int i=384;i<480;i++) myGLCD.drawPixel(240,i);
    for(int i=384;i<480;i++) myGLCD.drawPixel(528,i);
    myGLCD.setColor(255,255,255);
    return 0;
}


void Error0()//SD卡出错
{
  myGLCD.setColor(0,0,255);
  myGLCD.fillRect(288,168,576,336);
  
  myGLCD.setColor(0,0,0);
  one(0x53,8,15,'a');
  one(0x44,8,15,'b');
  one(0xBF,0xA8,8,16);
  one(0xB6,0xC1,8,17);
  one(0xC8,0xA1,8,18);
  one(0xCA,0xA7,8,19);
  one(0xB0,0xDC,8,20);
  
  one(0xC7,0xEB,11,16);
  one(0xBC,0xEC,11,17);
  one(0xB2,0xE9,11,18);
  one(0x53,11,19,'a');
  one(0x44,11,19,'b');
  one(0xBF,0xA8,11,20);
  myGLCD.setColor(255,255,255);
}

void Error1()//无效卡
{
  myGLCD.setColor(0,0,255);
  myGLCD.fillRect(241,385,527,479);
  
  myGLCD.setColor(0,0,0);
  one(0xCE,0xDE,17,15);
  one(0xD0,0xA7,17,16);
  one(0xBF,0xA8,17,17);
  
  one(0xC7,0xEB,18,14);
  one(0xD6,0xD8,18,15);
  one(0xD0,0xC2,18,16);
  one(0xCB,0xA2,18,17);
  one(0xBF,0xA8,18,18);
  myGLCD.setColor(255,255,255);
}

void Error2()//已签到
{
  myGLCD.setColor(255,0,0);
  myGLCD.fillRect(241,385,527,479);
  myGLCD.setColor(0,0,0);
  one(0xD2,0xD1,17,15);
  one(0xC7,0xA9,17,16);
  one(0xB5,0xBD,17,17);
  
  one(0xC7,0xEB,18,14);
  one(0xD6,0xD8,18,15);
  one(0xD0,0xC2,18,16);
  one(0xCB,0xA2,18,17);
  one(0xBF,0xA8,18,18);
  myGLCD.setColor(255,255,255);
}

void Success()//签到成功
{
  myGLCD.setColor(0,255,0);
  myGLCD.fillRect(241,385,527,479);
  myGLCD.setColor(0,0,0);
  one(0xC7,0xA9,17,14);
  one(0xB5,0xBD,17,15);
  one(0xB3,0xC9,17,16);
  one(0xB9,0xA6,17,17);
  myGLCD.setColor(255,255,255);
}

int s6()
{
    myGLCD.setColor(255,255,255);
    one(0xB5,0xE3,7,14);
    one(0xC3,0xFB,7,16);
    one(0xBD,0xE1,7,18);
    one(0xCA,0xF8,7,20);
    one(0xB5,0xC8,9,12);
    one(0xB4,0xFD,9,13);
    one(0xC3,0xFB,9,14);
    one(0xB5,0xA5,9,15);
    one(0xC9,0xFA,9,16);
    one(0xB3,0xC9,9,17);
    one(0xC7,0xEB,9,18);
    one(0xCE,0xF0,9,19);
    one(0xB0,0xCE,9,20);
    one(0xB3,0xF6,9,21);
    one(0x55,9,22,'a');
    one(0xC5,0xCC,9,23);
    return 0;
}

int s7()
{
    myGLCD.setColor(255,255,255);
    one(0x55,8,14,'a');
    one(0xC5,0xCC,8,15);
    one(0xB3,0xF5,8,16);
    one(0xCA,0xBC,8,17);
    one(0xBB,0xAF,8,18);
    one(0xD6,0xD0,8,19);
    one(0x2E,8,20,'a');
    one(0x2E,8,20,'b');
    one(0x2E,8,21,'a');
    return 0;
}

int s8()
{
  myGLCD.setColor(255,255,255);
  one(0xC7,0xEB,6,14);
  one(0xD6,0xD8,6,15);
  one(0xD0,0xC2,6,16);
  one(0xB2,0xE5,6,17);
  one(0xC8,0xEB,6,18);
  one(0x55,6,19,'a');
  one(0xC5,0xCC,6,20);
  one(0xC8,0xB7,13,16);
  one(0xB6,0xA8,13,18);
  for(int i=360;i<480;i++) myGLCD.drawPixel(i,288);
  for(int i=360;i<480;i++) myGLCD.drawPixel(i,360);
  for(int i=288;i<360;i++) myGLCD.drawPixel(360,i);
  for(int i=288;i<360;i++) myGLCD.drawPixel(480,i);

  return 0;
}

int s9()
{
    myGLCD.setColor(255,255,255);
    one(0xC7,0xEB,0,14);
    one(0xD1,0xA1,0,15);
    one(0xD4,0xF1,0,16);
    one(0xD1,0xA7,0,17);
    one(0xC9,0xFA,0,18);
    one(0xC3,0xFB,0,19);
    one(0xB5,0xA5,0,20);
    
    one(0xC9,0xCF,17,4);
    one(0xD2,0xBB,17,5);
    one(0xD2,0xB3,17,6);
    one(0xCF,0xC2,17,26);
    one(0xD2,0xBB,17,27);
    one(0xD2,0xB3,17,28);
    myGLCD.setColor(0,255,0);
    for(int i=0;i<800;i++) myGLCD.drawPixel(i,24);
    for(int i=0;i<800;i++) myGLCD.drawPixel(i,384);
    for(int i=384;i<480;i++) myGLCD.drawPixel(240,i);
    for(int i=384;i<480;i++) myGLCD.drawPixel(528,i);
    myGLCD.setColor(255,255,255);

    return 0;
}

int s10()
{
    myGLCD.setColor(0,0,255);
    one(0xC3,0xFB,12,14);
    one(0xB5,0xA5,12,15);
    one(0xD2,0xD1,12,16);
    one(0xC9,0xFA,12,17);
    one(0xB3,0xC9,12,18);
    myGLCD.setColor(255,255,255);
    return 0;
}

void DisplayClock(int tem)
{
  myGLCD.setColor(0,0,0);
  rtc_clock.get_time(&hh,&mm,&ss);
  rtc_clock.get_date(&dow,&dd,&mon,&yyyy);
  if(yy1!=yyyy)  {
  myGLCD.fillRect(24,24,72,48);
  yy1=yyyy;  }
  if(m1!=mon)  {
  myGLCD.fillRect(96,24,120,48);
  m1=mon;  }
  if(d1!=dd)  {
  myGLCD.fillRect(144,24,168,48);
  d1=dd;  }
  if(h1!=hh)  {
  myGLCD.fillRect(72,48,96,72);
  h1=hh;  }
  if(mm1!=mm)  {
  myGLCD.fillRect(120,48,144,72);
  mm1=mm;  }
  if(tem1!=tem)  {
  myGLCD.fillRect(264,48,288,72);
  tem1=tem;  }
  myGLCD.setColor(255,255,255);
  s0(yyyy,mon,dd,hh,mm,tem);//year,month,day,hour,minute,temperature
}
