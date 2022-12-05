#include <stdint.h>
#include <cstddef>


#ifndef _MY_PAKETS_H_
#define _MY_PAKETS_H_
/*
 *  размер данных
 *   и размер пакета
 *
  запись в пакет, и чтение из пакета

 *
 *
 * */


namespace common
{


class pak_v2
{
 enum  start {st='U'};//=st;
const uint8_t max_size; // максимальный размер буфера +s_size (размер сервесных данных)
const bool allocated;
uint8_t _sz; // текущий размер буфера включая заголовок
uint8_t head_sz{2};		// заголовок имеет размер три байта : start=фиксированному байту, и два байта размер последовательность
uint8_t *buf; // buff приема передачи

uint8_t wr_seek;
uint8_t rd_seek;


bool valid{true};
// обратить внимание, что реальный размер считанных|переданных данных, будет меньше на размер сервисных данных
public:
 pak_v2(uint8_t sz); 		// класс сам выделяет нужное количество данных в дин памяти
 pak_v2(uint8_t sz, uint8_t* b);  // класс использует предоставленную память
// копирование зарпрещено
 pak_v2 operator=(const pak_v2& pk)=delete;
pak_v2(const pak_v2& pk)=delete;
void init();
 bool isEmpty() const;
 bool isFull() const;
 bool isValid() const; // ошибка чтения
 bool isReady() const; // прием окончен isFull()??

void write(uint8_t* dd, uint8_t sz);
void read(uint8_t *dd, uint8_t &sz);

// интерфейс для отправки приема данных побайтно???

uint8_t tr_nxt(uint8_t *d);   // если возможно запихнуть данные, пихаем
uint8_t rc_nxt(uint8_t *dd); // сколько влезет
void clear();// сброс пакета

// вспомогательные функции
void write_head();
 bool read_head(uint8_t dd); // для чтения заголовка
bool write_head(uint8_t sz); // для записи заголовка
// для отправки данных
uint8_t size() const {return _sz;}
uint8_t const* buffer(){return buf;}

//~pak_v2();
// head???

};}







#endif
