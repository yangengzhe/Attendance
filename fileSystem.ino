//-1:U盘错误  -2:格式错误

//导入目录
int DIR()
{
  int length=0;
  byte temp=0;
  while(Serial2.available() > 0)  Serial2.read();
  Serial2.print("DIR");
  Serial2.write(0x0D);
  //输入命令
  while(Serial2.available()<=0);
  if(Serial2.available() > 0)  temp=Serial2.read();
  if(temp!= 13)  return -1;//U盘错误
  else
  {
    while (Serial2.available() <= 0);
    temp=0;
    DirToFileinfo(temp,length);
    fileNum=length-2;
  }
  return 0;
}
//计算文件长度
int DIRFile(char filename[]){
  byte comData[4] = {0};
  unsigned long int length=0;
  byte temp=0;
  while(Serial2.available() > 0)  Serial2.read();
  Serial2.print("DIR " + (String)filename);
  Serial2.write(0x0D);
  //输入命令
  if(getbyte(&Serial2)!=13) return -1;//U盘错误
  for(int count=0;count<strlen(filename);count++)
  if(getbyte(&Serial2)!=filename[count]) return -1;//U盘错误
  if(getbyte(&Serial2)!=' ') return -1;//U盘错误
  for(int i=0;i<4;i++)  comData[i]=getbyte(&Serial2);
  //长度转换
  length=comData[3]*16777216+length;
  length=comData[2]*65536+length;
  length=comData[1]*256+length;
  length=comData[0]+length;
  fileSize=length;
  return 0;
}
//以写的方式打开文件
int OPW(const char filename[],char * time){
  Serial2.print((String)"OPW " + filename);
  Serial2.write(0x0D);
  CLF("00.txt"); //执行第一遍处理错误
  delay(500);
  while(Serial2.available() > 0)  Serial2.read();
  Serial2.print((String)"OPW " + filename);
  Serial2.write(0x20);
  Serial2.write(0x30);
  Serial2.write(0x78);
  for(int m= 0; m<8;m++)
  Serial2.write(time[m]);
  Serial2.write(0x0D);
  if(JudgeResult()==-1) return -1;//U盘错误
  return 0;
}
//关闭文件
int CLF(const char filename[]){
  delay(500);
  while(Serial2.available() > 0)  Serial2.read();
  Serial2.print("CLF " + (String)filename);
  Serial2.write(0x0D);
  if(JudgeResult()==-1) return -1;//U盘错误
  return 0;
}
//根据字节数读取文件
int RD(char filename[]){
  delay(500);
  while(Serial2.available() > 0)  Serial2.read();
  Serial2.print("RD " + (String)filename);
  Serial2.write(0x0D);
  unsigned long int count=0;//count
  int j=0;
  int i=0;
  int m=0;
  byte temp=0;
  while(count<fileSize){  
    //添加姓名
    for( m=0;temp != 44;m++)
      {
        temp=getbyte(&Serial2);
        if(temp=='>' && getbyte(&Serial2)==13) return 0;//判断D:\> 读取成功并结束
        if(temp==13) return -2;//格式不正确
        s[j].name[m]=temp;
        count++;
      }
      if(m>10) return -2;//格式不正确
        s[j].name[m-1]='\0';
    //添加学号
        temp=0;
      for( m=0;temp != 44;m++)
      {
        temp=getbyte(&Serial2);
        if(temp==13) return -2;//格式不正确
        if((temp<'0'|| temp>'9') && temp!=',' ) return -2;
        
        s[j].number[m]=temp;
        count++;
      }
      if(m>11) return -2;//格式不正确
        s[j].number[m-1]='\0';
     //添加年级 
        temp=0;
        for( m=0;temp != 44;m++)
      {
        temp=getbyte(&Serial2);
        if(temp==13) return -2;//格式不正确
        s[j].grade[m]=temp;
        count++;
      }
      if(m>14) return -2;//格式不正确
        s[j].grade[m-1]='\0';
      //添加UID
        temp=0;
        char UID[8];
        for( m=0;(temp != 13 && m<9);m++)
        {
          if(temp<48 && temp>57 || temp<65 && temp>70 || temp<97 && temp>102)  return -2;//格式不正确
          temp=getbyte(&Serial2);
          UID[m]=temp;
          count++;
        }
        s[j].uid=strtoul(UID,0,16);
        getbyte(&Serial2);
        count++;
        StudentNum=j++;
        i++;
  }
  return 0;//成功
}

int WR(byte data[],int lenth){  
  char c_lenth[2];
  sprintf(c_lenth,"%2d",lenth);
  Serial2.print("WRF ");
  Serial2.write(c_lenth[0]);
  Serial2.write(c_lenth[1]);
  Serial2.write(0x0D);
  for(int i=0;i<lenth;i++)
      Serial2.write(data[i]);
  if(JudgeResult()==-1) return -1;//U盘错误
  return 0;
}

void CD(char file[]){ //671111091099711010032709710510810110013 Command FailedEnter //68,58,92,62,13,D:\>Enter
  int comData[100] = {0};
  Serial2.flush();
  Serial2.print((String)"CD " + file);
  Serial2.write(0x0D);
  int i = 0;
  while (Serial2.available() <= 0);
  while(check(comData) == 0){
     while (Serial2.available() <= 0);
     while(Serial2.available() > 0){  
       comData[i] = (int)Serial2.read();
       i++;   
    }
  }
}
int check(int s2[]){
  //Serial.println("checking.... ");
  int s1[6] = {68,58,92,62,13,0};//D:\>
  int s4[6] = {105,108,101,100,13,0};//Command Failed
  int s5[6] = {79,112,101,110,13,0};//File Open
  int s6[6] = {68,105,115,107,13,0};//No Disk
  
  int s3[6] = {0};
  int i = 0;
  while(s2[i] != 0)
    i++;
  //Serial.println("i = "+(String)i);
  if(i<5){
    return 0;
  }
  else{
    for(int k=i-5,j = 0;s2[k] != 0;j++,k++){
      s3[j] = s2[k];
    }
    for(int z = 0;s3[z] != 0;z++){
      //Serial.println((String)s1[z]+" = "+(String)s3[z]);
      if(s3[z]!=s1[z]){
          if(s3[z]!=s4[z])
              if(s3[z]!=s5[z])
                  if(s3[z]!=s6[z])
                      return 0; 
        }
      }
    return 1;
  } 
}
/*****************************************************************************************/
void DirToFileinfo(byte &temp,int &length)
{
  while(temp!=92)
  {
  temp=0;
  int i=0;
    //读取文件目录
  while(temp!=13 && temp !=92)
  {
    if(Serial2.available() > 0)  
    {
      temp=Serial2.read();
      f[length].name[i]=temp;
      i++;//此处i多加1
      if(i>29) break;
    }
  }
    //处理文件和文件夹
  f[length].name[i-1]='\0';
  if(f[length].name[i-2]=='R'&&f[length].name[i-3]=='I'&&f[length].name[i-4]=='D')
  {
    f[length].name[i-5]='\0';
    f[length].name[i-4]=0;
    f[length].name[i-3]=0;
    f[length].name[i-2]=0;
    f[length].type=0;
  }
  else
  {
      f[length].name[i-1]='\0';
      f[length].type=1;
  }
  length++;
  if(length>299) break;//数组越界
  }
}


int JudgeResult()
{
  if(getbyte(&Serial2)!=0x44)
  return -1;
  if(getbyte(&Serial2)!=0x3A)
  return -1;
  if(getbyte(&Serial2)!=0x5C)
  return -1;
  if(getbyte(&Serial2)!=0x3E)
  return -1;
  if(getbyte(&Serial2)!=0x0D)
  return -1;
}
