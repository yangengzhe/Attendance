const int chipSelect = 10;
#include <SdFat.h>
SdFat sd;
SdFile myFile;
void SdRead(int32_t pos,const char * file, byte * str)
{
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) 
  {Error0();
   sd.initErrorHalt();}
  if (!myFile.open(file, O_READ)) {
    sd.errorHalt("opening text for read failed");
  }
  myFile.seekSet(pos);
  for(int i=0;i<72;i++)
  *(str+i) = myFile.read();
  myFile.close();
}

char SdRead(int32_t pos,const char * file)
{
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) 
  {Error0();
   sd.initErrorHalt();}
  if (!myFile.open(file, O_READ)) {
    sd.errorHalt("opening text for read failed");
  }
  myFile.seekSet(pos);
  char temp=myFile.read();
  myFile.close();
  return temp;
}
void SdOpen(const char * file)
{
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) 
  {Error0();
   sd.initErrorHalt();}
  if (!myFile.open(file, O_READ)) {
    sd.errorHalt("opening text for read failed");
  }
}

byte SdRead(const char * file)
{
  return myFile.read();  
}

void ClOpen(const char * file)
{
  myFile.close();
}
