
#include "paket.h"

namespace common {
 //paket::paket():_size(0),data(nullptr),alc_flg(false)  {}

//******************************************************

pak_v2::pak_v2(uint8_t sz):max_size(sz), allocated (true)//,start{st}
        {
_sz=0;
buf=new uint8_t[max_size];
wr_seek=0;
rd_seek=0;
head_sz=2;
        }

pak_v2::pak_v2(uint8_t sz,uint8_t* b):max_size(sz),allocated(false) //,start{st}
{_sz=0;
buf=b; 		// использует внешнюю память
wr_seek=0;
rd_seek=0;

}
//pak_v2::~pak_v2()
//{
//if(allocated)
//    delete[]buf;
//}

void pak_v2::init()
{
	_sz=0;
	//buf=new uint8_t[max_size];
	wr_seek=0;
	rd_seek=0;
	head_sz=2;

}
void pak_v2::clear()
{
wr_seek=0;
rd_seek=0;
_sz=0;
valid=true;
}
 // чтение полученных данных
void pak_v2::read(uint8_t* dd,uint8_t &s)
{
if(isValid()&& isReady()){ // isFull
   s=_sz-head_sz-1;
  for(uint8_t i=0;i<s;i++)
 {
   dd[i]=buf[head_sz+i];
    }
  dd[s]='\0';
  clear();
 }
else  // ????
 {	*dd=0;
     s=0;
 }
    }

void pak_v2::write(uint8_t* dd,uint8_t sz)
{ // если пакет пуст, можем записать данные для отправки
    // сформировать заголовок, и crs8
 if(isEmpty())
 { // сформировать заголовок и src8
    _sz=sz+head_sz+1; // новый размер 2+sz+1
uint8_t src8=0;
for(;wr_seek<_sz;) //uint8_t cnt=0;cnt<_sz;++cnt)
{
 if(wr_seek<head_sz)
       write_head();  // запись заголовка для данных размера sz
  else if(wr_seek==_sz-1 ) {
          buf[wr_seek++]=src8;
      //   break;

  }
  else
  {
   buf[wr_seek]=*(dd+(wr_seek-head_sz));

   src8+=buf[wr_seek];
 wr_seek++;
 }
 //std::cout<<(int)buf[cnt]<<" "    ;
}
   } else
 {

       // попытка записи в занятый массив
 }

}

// прием следующих байтов
uint8_t pak_v2:: rc_nxt(uint8_t* dd)
{ // если уже приняли заголовок просто копировать данные
if(isValid()){
    if(wr_seek>=head_sz && wr_seek<_sz-1 ) //
  { 	buf[wr_seek]=*dd;
      wr_seek++;
  return 1;
   } else if(wr_seek==_sz-1)  //
   { // принятие crs
  buf[wr_seek]=*dd;
        wr_seek++;
 // valid=false;
     }// если заголовок не принят
     //
   else if(wr_seek<head_sz)
	   return write_head(*dd);
    }
return 0;
}

// как узнать сколько можно писать в dd ? // если предоставлять информацию сколько можно писать
uint8_t pak_v2::tr_nxt(uint8_t *dd)  // (uint8_t* , uint8_t sz)
{ // при передачи wr_seek==_sz-1;
 // передача возможна когда пакет заполнен isFull() {wr_seek==_size-1}
    // увеличиваем rd_seek;

if(isEmpty())
 { valid=false;
    return 0;

 }
*dd=buf[rd_seek];
rd_seek++;
return 1;
//    if(isFull()){




//    clear();
//}

//if(isEmpty()) // если есть что отправлять
//{
// return 0;
//  } else
//  {
//      *dd=buf[rd_seek];
//        rd_seek++;
//         return 1;
//  }
}


bool pak_v2::isEmpty()const
{
// wr_seek == _sz когда пакет пуст, данных нет и когда все отправлено

    return rd_seek==_sz;
}
bool pak_v2::isFull()const
{
if(_sz)
return wr_seek==_sz;
 // если пакет не содержит данных, то он не может быть полным
 return false;
}


bool pak_v2:: isReady() const
{
if(_sz)
    return wr_seek==_sz ;
   return false;

}

bool pak_v2::isValid() const
{
    return valid;
}


// write_head вызыватся в двух случаях, 1.когда формируются данные для отправки, (все данные известны)
// 2. случай по приему данных, тогда следует считывать побайтно,
void pak_v2::write_head()
{
 //  for(int i=0;i<head_sz;++i){

    switch (wr_seek)
    {
     case 0:
     buf[wr_seek]='U';//start;

      break;
     case 1:     // case 2:
     buf[wr_seek]=_sz;
        break;
   }
    wr_seek++;
   }
//}

 bool pak_v2::write_head(uint8_t sz)
{
  if(wr_seek>0) // если уже записаны данные
  { // здесь, если стартбайт соответствует
     _sz=sz;
     buf[wr_seek++]=sz;
 return true;
  } else // данных еще нет проверка на start_byte
   {
     if(sz==start::st)
     {
    	 buf[wr_seek++]=sz;
    	      return true;
     } else
     {
    	 valid=false; // если старт байт не соответствует , сбрасываем чтение
    	        return false;

        }
  }
}


//*****************************************************
}


