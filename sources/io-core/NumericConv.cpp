/* SECU-3  - An open source, free engine control unit
   Copyright (C) 2007 Alexey A. Shabelnikov. Ukraine, Gorlovka

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   contacts:
              http://secu-3.narod.ru
              email: secu-3@yandex.ru
*/

#include "stdafx.h"
#include "NumericConv.h"
#include "ctype.h"
#include "windows.h"

//������������ ������ ���������������� �������� ����� (��� ASCII ������� ������������ ���� ����) � ������ �������
//�������� ������ (unsigned char).
//size - ������ � ������!!!
bool CNumericConv::HexArrayToBin(const BYTE* i_buf, BYTE* o_buf, const int size)
{  
 int j=0;
 for(int i = 0; i < size; i++)
 {
  if (isxdigit(i_buf[j])&&(isxdigit(i_buf[j+1])))
  {
   o_buf[i] = HTOD(i_buf[j])*16 + HTOD(i_buf[j+1]);
  }
  else
   return false;  //error: not a HEX digit!
  j+=2;
 } //for
 return true;
}


//��������� ����������������� ����� �������� ����� ASCII ��������� � ����.
//���� ����������������� ����� �������� ������ �� ���������� false
bool CNumericConv::Hex8ToBin(const BYTE* i_buf,BYTE* o_byte)
{
 if ((i_buf==NULL)||(o_byte==NULL)) 
  return false;
 if (isxdigit(i_buf[0])&&(isxdigit(i_buf[1])))
 {
  *o_byte = HTOD(i_buf[0])*16 + HTOD(i_buf[1]);
  return true;
 }
 else
  return false;
}


//������� ����������� ����� (8 ���) ��� ������ ������ i_buf �������� size ������� �������
//(��������� �������� �� ������ 2)
//���������� ���� ����������� �����
BYTE CNumericConv::CheckSum_8_xor(const BYTE* i_buf,const int size)
{
 BYTE sum=0;
 for(int i = 0; i < size; i++)
 {
  sum = sum ^ i_buf[i]; //XOR
 }//for
 return sum;
}

//������������ ������ ������� �������� ������ (unsigned char)  � ������ ���������������� �������� �����
//(��� ASCII ������� ������������ ���� ����)
//size - ������ � ������!!!
bool CNumericConv::BinToHexArray(const BYTE* i_buf, BYTE* o_buf, const int size)
{
 int j=0;
 for(int i = 0; i < size; i++)
 {
  Bin8ToHex(i_buf[i],&o_buf[j]);	  //��������� �� 2 �������
  j+=2;
 } //for
 return true;
}

bool CNumericConv::Bin8ToHex(const BYTE i_byte,BYTE* o_hex_number)
{
 if (o_hex_number==NULL) 
  return false;

 o_hex_number[0] = DTOH(i_byte >> 4); //store HI part
 o_hex_number[1] = DTOH(i_byte);      //store LO part
 return true;
}


bool CNumericConv::Bin8ToHex(const BYTE i_byte,std::string& o_hex_number)
{ 
 o_hex_number+= DTOH(i_byte >> 4); //store HI part
 o_hex_number+= DTOH(i_byte);      //store LO part
 return true;
}


bool CNumericConv::Hex16ToBin(const BYTE* i_buf,int* o_word,bool i_signed /* = false*/)
{
 if (o_word==NULL) 
  return false;

 BYTE lo,hi;
 int word;
 if (isxdigit(i_buf[0])&&isxdigit(i_buf[1])&&isxdigit(i_buf[2])&&isxdigit(i_buf[3]))
 {
  if (false==Hex8ToBin(i_buf,&hi))
   return false;

  if (false==Hex8ToBin(i_buf+2,&lo))
   return false;

  word = MAKEWORD(lo,hi);

  //���� ����� �������� � ������������� �� ������ ��� �������������
  *o_word = (i_signed && word > 0x7FFF) ? -(0x10000 - word) : word;
  return true;
 }
 return false;
}



bool CNumericConv::_Hex32ToBin(const BYTE* i_buf,DWORD* o_dword)
{
 if (o_dword==NULL) 
  return false;

 BYTE b0,b1,b2,b3;
 if (isxdigit(i_buf[0])&&isxdigit(i_buf[1])&&isxdigit(i_buf[2])&&isxdigit(i_buf[3])&&
     isxdigit(i_buf[4])&&isxdigit(i_buf[5])&&isxdigit(i_buf[6])&&isxdigit(i_buf[7]))
 {
  if (false==Hex8ToBin(i_buf,&b3))
   return false;

  if (false==Hex8ToBin(i_buf+2,&b2))
   return false;

  if (false==Hex8ToBin(i_buf+4,&b1))
   return false;

  if (false==Hex8ToBin(i_buf+6,&b0))
   return false;


  WORD hi = MAKEWORD(b2,b3);
  WORD lo = MAKEWORD(b0,b1);

  *o_dword = MAKELONG(lo,hi);

  return true;
 }
 return false;
}

bool CNumericConv::Hex32ToBin(const BYTE* i_hex_number,signed long *o_dword)
{
 return CNumericConv::_Hex32ToBin(i_hex_number,(unsigned long*)o_dword);
}

bool CNumericConv::Hex32ToBin(const BYTE* i_hex_number,unsigned long *o_dword)
{
 return CNumericConv::_Hex32ToBin(i_hex_number,(unsigned long*)o_dword);
}

bool CNumericConv::Hex4ToBin(const BYTE i_hex_number,BYTE* o_byte)
{
 if (isxdigit(i_hex_number))
 {
  *o_byte = HTOD(i_hex_number);
  return true;
 }
 return false;
}

bool CNumericConv::Bin12ToHex(const int i_12bit, BYTE* o_hex_number)
{
 if (o_hex_number==NULL) 
  return false;

 o_hex_number[0] = DTOH(i_12bit >> 8); //store 1st 4 bits
 o_hex_number[1] = DTOH(i_12bit >> 4); //store 2nd 4 bits
 o_hex_number[2] = DTOH(i_12bit);      //store 3rd 4 bits
 return true;
}

bool CNumericConv::Bin16ToHex(const int i_word,BYTE* o_hex_number)
{
 if (!Bin8ToHex(HIBYTE(((WORD)i_word)),o_hex_number))
  return false;

 if (!Bin8ToHex(LOBYTE(((WORD)i_word)),o_hex_number+2))
  return false;

 return true;
}


bool CNumericConv::Bin16ToHex(const int i_word,std::string& o_hex_number)
{
 Bin8ToHex(HIBYTE(((WORD)i_word)),o_hex_number);
 Bin8ToHex(LOBYTE(((WORD)i_word)),o_hex_number);
 return true;
}

bool CNumericConv::Bin32ToHex(const unsigned long i_dword,std::string& o_hex_number)
{
 Bin8ToHex(HIBYTE(HIWORD(i_dword)),o_hex_number);
 Bin8ToHex(LOBYTE(HIWORD(i_dword)),o_hex_number);
 Bin8ToHex(HIBYTE(LOWORD(i_dword)),o_hex_number);
 Bin8ToHex(LOBYTE(LOWORD(i_dword)),o_hex_number);
 return true;
}

bool CNumericConv::Bin32ToHex(const signed long i_dword,std::string& o_hex_number)
{
 return CNumericConv::Bin32ToHex((unsigned long)i_dword,o_hex_number);
}

bool CNumericConv::Bin4ToHex(const BYTE i_byte,std::string& o_hex_number)
{
 o_hex_number+=DTOH(i_byte);
 return true;
}
