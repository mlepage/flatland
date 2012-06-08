//
//  Fixed++ - a 16.16 Fixed Point Class for Watcom C++ - v0.01
//

// This code is Copyright (C) 1995 by Nigel Brooke

// See README.DOC for a description of what you can and can't do with it.
// (Don't worry, it's pretty leniant).

//      Nigel Brooke (nbrooke@sfu.ca)


// mlepage changes:
// - Removed ifdef to make it work without Watcom
// - Removed inline assembly in favour of C++
// - Made user-defined conversion operators const
// - added sqrt
// - fixed non commutative operators
// - fixed precedence
// - make precision a macro


#define PREC 16


// To avoid including this twice
#ifndef _FIXED_HPP_
#define _FIXED_HPP_

// Make sure we are under 32-bit Watcom
#ifndef __WATCOMC__
#define _FLOAT_
#endif
#ifndef __386__
#define _FLOAT_
#endif


//#ifndef _FLOAT_

#ifndef __cplusplus
#error fixed.hpp is for use with C++
#endif

#if 0
// The chunk of assembly that performs the actual
// multiplication of two 16.16 fixed point values
extern long fixedmul(long a, long b);
#pragma aux fixedmul = \
  ".386"               \
  "imul ebx"           \
  "shld edx,eax,16"    \
  parm [eax] [ebx]     \
  value [edx];


// The chunk of assembly that performs the actual
// division of two 16.16 fixed point values
extern long fixeddiv(long a, long b);
#pragma aux fixeddiv = \
  ".386"               \
  "xor eax, eax"       \
  "shrd eax, edx, 16"  \
  "sar edx, 16"        \
  "idiv ebx"           \
  parm [edx] [ebx]     \
  value [eax];
#else
// The chunk of assembly that performs the actual
// multiplication of two 16.16 fixed point values
inline long fixedmul(long a, long b)
{
	__int64 n = a;
	n *= b;
	return n >> PREC;
}

// The chunk of assembly that performs the actual
// division of two 16.16 fixed point values
inline long fixeddiv(long a, long b)
{
	__int64 n = a;
	n <<= PREC;
	return n / b;
}
#endif


class fixed {

  // The 'real' value (DUH!)
  int value;

public:

  // Various constructors
  fixed() {};
  fixed( fixed const & a) {value = a.value;}
  fixed( int a) { value = a<<PREC;}
  fixed( float a) { value = (float)a*(1<<PREC);}
  fixed( double a) { value = (double)a*(1<<PREC);}


  // For directly manipulationg the real value of the number.
  // **DO NOT USE** if you intend for the code to compile in
  // floating point mode
  inline int getvalue(void) const { return value; }
  inline void setvalue(int a) { value = a; }

  // The cast operators, one for each built in type
  inline operator unsigned char () const {return value >> PREC;}
  inline operator char () const {return value >> PREC;}
  inline operator unsigned short () const {return value >> PREC;}
  inline operator short () const {return value >> PREC;}
  inline operator unsigned int () const {return value >> PREC;}
  inline operator int () const {return value >> PREC;}
  inline operator unsigned long () const {return value >> PREC;}
  inline operator long () const {return value >> PREC;}
  inline operator float () const { return float(value) / (1<<PREC);}
  inline operator double () const { return double(value) / (1<<PREC);}

  // All the operators where the fixed point number is the Lvalue (some
  // compiler-speak there :-)
  //
  // In general there are four of each operation , one each for fixed,
  // int, float and double. Thankfully, Watcom will automatically cast
  // any other ordinal types (short, long, char, unsigned int, etc.) to int
  inline fixed &operator= (fixed const& a) {value = a.value; return *this;}
  inline fixed &operator= (int a) { value = long(a)<<PREC; return *this;}
  inline fixed &operator= (float a) { value = (float)a*(1<<PREC); return *this;}
  inline fixed &operator= (double a) { value = (double)a*(1<<PREC); return *this;}

  fixed &operator += ( fixed const & a) {value += a.value; return *this;}
  fixed &operator += ( int a) {value += a<<PREC; return *this;}
  fixed &operator += ( float a) {value += a*(1<<PREC); return *this;}
  fixed &operator += ( double a) {value += a*(1<<PREC); return *this;}

  fixed &operator -= ( fixed const & a) {value -= a.value; return *this;}
  fixed &operator -= ( int a) {value -= a<<PREC; return *this;}
  fixed &operator -= ( float a) {value -= a*(1<<PREC); return *this;}
  fixed &operator -= ( double a) {value -= a*(1<<PREC); return *this;}

  fixed &operator *=(fixed const & a) {value = fixedmul(value,a.value); return *this;}
  fixed &operator *=(int a) {value = fixedmul(value,a<<PREC); return *this;}
  fixed &operator *=(float a) {value = fixedmul(value,a*(1<<PREC)); return *this;}
  fixed &operator *=(double a) {value = fixedmul(value,a*(1<<PREC)); return *this;}

  fixed &operator /=(fixed const & a) {value = fixeddiv(value,a.value); return *this;}
  fixed &operator /=(int a) {value = fixeddiv(value,a<<PREC); return *this;}
  fixed &operator /=(float a) {value = fixeddiv(value,a*(1<<PREC)); return *this;}
  fixed &operator /=(double a) {value = fixeddiv(value,a*(1<<PREC)); return *this;}

  // The negation operator
  fixed operator - () const { fixed temp; temp.value = -value; return temp;}

};

//  All the binary operators
//
// There are generally seven of each of these, one for each of the four
// types, and then three more for the built in types that simply have
// the arguments reversed

inline fixed operator + (const fixed& a, const fixed& b) {
      fixed temp;
      temp.setvalue(a.getvalue() + b.getvalue());
      return temp;
}

inline fixed operator + ( fixed const & a, int b) {
      fixed temp;
#if 0
      temp.setvalue(a.getvalue() + b<<PREC);
#else
// mlepage Precedence is wrong here.
      temp.setvalue(a.getvalue() + (b<<PREC));
#endif
      return temp;
}

inline fixed operator + ( fixed const & a, float b) {
      fixed temp;
      temp.setvalue(a.getvalue() + (b*(1<<PREC)));
      return temp;
}

inline fixed operator + ( fixed const &a, double b ){
      fixed temp;
      temp.setvalue(a.getvalue() + (b*(1<<PREC)));
      return temp;
}

inline fixed operator + ( int a, fixed const &b ) {return b+a;}
inline fixed operator + ( float a, fixed const &b ) {return b+a;}
inline fixed operator + ( double a, fixed const &b ) {return b+a;}


inline fixed operator - (const fixed& a, const fixed& b) {
      fixed temp;
      temp.setvalue(a.getvalue() - b.getvalue());
      return temp;
}

inline fixed operator - ( fixed const & a, int b) {
      fixed temp;
#if 0
      temp.setvalue(a.getvalue() - b<<PREC);
#else
// mlepage Precedence is wrong here.
      temp.setvalue(a.getvalue() - (b<<PREC));
#endif
      return temp;
}

inline fixed operator - ( fixed const & a, float b) {
      fixed temp;
      temp.setvalue(a.getvalue() - (b*(1<<PREC)));
      return temp;
}

inline fixed operator - ( fixed const &a, double b ){
      fixed temp;
      temp.setvalue(a.getvalue() - (b*(1<<PREC)));
      return temp;
}

#if 0
inline fixed operator - ( int a, fixed const &b ) {return b-a;}
inline fixed operator - ( float a, fixed const &b ) {return b-a;}
inline fixed operator - ( double a, fixed const &b ) {return b-a;}
#else
// mlepage For christ's sake subtraction is not commutative!
inline fixed operator - ( int a, fixed const &b ) {return -b+a;}
inline fixed operator - ( float a, fixed const &b ) {return -b+a;}
inline fixed operator - ( double a, fixed const &b ) {return -b+a;}
#endif

inline fixed operator *(const fixed& a, const fixed& b) {
      fixed temp;
      temp.setvalue(fixedmul(a.getvalue(),b.getvalue()));
      return temp;
}

inline fixed operator * ( fixed const &a, int b) {
      fixed temp;
      temp.setvalue(fixedmul(a.getvalue(),b<<PREC));
      return temp;
}

inline fixed operator * ( fixed const &a, float b) {
      fixed temp;
      temp.setvalue(fixedmul(a.getvalue(),b*(1<<PREC)));
      return temp;
}


inline fixed operator * ( fixed const &a, double b) {
      fixed temp;
      temp.setvalue(fixedmul(a.getvalue(),b*(1<<PREC)));
      return temp;
}

inline fixed operator * ( int a, fixed const &b ) {return b*a;}
inline fixed operator * ( float a, fixed const &b ) {return b*a;}
inline fixed operator * ( double a, fixed const &b ) {return b*a;}

inline fixed operator /(const fixed& a, const fixed& b) {
      fixed temp;
      temp.setvalue(fixeddiv(a.getvalue(),b.getvalue()));
      return temp;
}

inline fixed operator / ( fixed const &a, int b) {
      fixed temp;
      temp.setvalue(fixeddiv(a.getvalue(),b<<PREC));
      return temp;
}

inline fixed operator / ( fixed const &a, float b) {
      fixed temp;
      temp.setvalue(fixeddiv(a.getvalue(),b*(1<<PREC)));
      return temp;
}


inline fixed operator / ( fixed const &a, double b) {
      fixed temp;
      temp.setvalue(fixeddiv(a.getvalue(),b*(1<<PREC)));
      return temp;
}

inline fixed operator / ( int a, fixed const &b ) {return b/a;}
inline fixed operator / ( float a, fixed const &b ) {return b/a;}
inline fixed operator / ( double a, fixed const &b ) {return b/a;}



// The comparison operators
//
// Remember, these cast to fixed, then compare, so. for example
//   fixed(200.1) != int(200.1)
//   (not too surprising, really)
//
// Also fixed point looses acuracy a lot faster than floating point,
// so :
//   fixed(1.0000001) != float(1.0000001)
//   fixed(1.0000001) == int(1.0000001)
//   (a bit more surprising)

inline int operator == ( fixed const &a, fixed const &b ) {
  return a.getvalue() == b.getvalue();
}

inline int operator == ( fixed const &a, int b) {
  return a.getvalue() == b<<PREC;
}


inline int operator == ( fixed const &a, float b) {
  return a.getvalue() == (int)b*(1<<PREC);
}

inline int operator == ( fixed const &a, double b) {
  return a.getvalue() == (int)b*(1<<PREC);
}

inline int operator == ( int a, fixed const &b ) {return b==a;}
inline int operator == ( float a, fixed const &b ) {return b==a;}
inline int operator == ( double a, fixed const &b ) {return b==a;}


inline int operator != ( fixed const &a, fixed const &b ) {
  return a.getvalue() != b.getvalue();
}

inline int operator != ( fixed const &a, int b) {
  return a.getvalue() != b<<PREC;
}


inline int operator != ( fixed const &a, float b) {
  return a.getvalue() != (int)b*(1<<PREC);
}

inline int operator != ( fixed const &a, double b) {
  return a.getvalue() != (int)b*(1<<PREC);
}

inline int operator != ( int a, fixed const &b ) {return b!=a;}
inline int operator != ( float a, fixed const &b ) {return b!=a;}
inline int operator != ( double a, fixed const &b ) {return b!=a;}


inline int operator > ( fixed const &a, fixed const &b ) {
  return a.getvalue() > b.getvalue();
}

inline int operator > ( fixed const &a, int b) {
  return a.getvalue() > b<<PREC;
}


inline int operator > ( fixed const &a, float b) {
  return a.getvalue() > (int)b*(1<<PREC);
}

inline int operator > ( fixed const &a, double b) {
  return a.getvalue() > (int)b*(1<<PREC);
}

inline int operator > ( int a, fixed const &b ) {return b>a;}
inline int operator > ( float a, fixed const &b ) {return b>a;}
inline int operator > ( double a, fixed const &b ) {return b>a;}


inline int operator < ( fixed const &a, fixed const &b ) {
  return a.getvalue() < b.getvalue();
}

inline int operator < ( fixed const &a, int b) {
  return a.getvalue() < b<<PREC;
}


inline int operator < ( fixed const &a, float b) {
  return a.getvalue() < (int)b*(1<<PREC);
}

inline int operator < ( fixed const &a, double b) {
  return a.getvalue() < (int)b*(1<<PREC);
}

inline int operator < ( int a, fixed const &b ) {return b<a;}
inline int operator < ( float a, fixed const &b ) {return b<a;}
inline int operator < ( double a, fixed const &b ) {return b<a;}

inline int operator >= ( fixed const &a, fixed const &b ) {
  return a.getvalue() >= b.getvalue();
}

inline int operator >= ( fixed const &a, int b) {
  return a.getvalue() >= b<<PREC;
}


inline int operator >= ( fixed const &a, float b) {
  return a.getvalue() >= (int)b*(1<<PREC);
}

inline int operator >= ( fixed const &a, double b) {
  return a.getvalue() >= (int)b*(1<<PREC);
}

inline int operator >= ( int a, fixed const &b ) {return b>=a;}
inline int operator >= ( float a, fixed const &b ) {return b>=a;}
inline int operator >= ( double a, fixed const &b ) {return b>=a;}


inline int operator <= ( fixed const &a, fixed const &b ) {
  return a.getvalue() <= b.getvalue();
}

inline int operator <= ( fixed const &a, int b) {
  return a.getvalue() <= b<<PREC;
}


inline int operator <= ( fixed const &a, float b) {
  return a.getvalue() <= (int)b*(1<<PREC);
}

inline int operator <= ( fixed const &a, double b) {
  return a.getvalue() <= (int)b*(1<<PREC);
}

inline int operator <= ( int a, fixed const &b ) {return b<=a;}
inline int operator <= ( float a, fixed const &b ) {return b<=a;}
inline int operator <= ( double a, fixed const &b ) {return b<=a;}


// mlepage added sqrt
inline fixed sqrt(const fixed f) { return sqrt((float)f); };


//#else
// If _FLOAT_ was defined, or __WATCOMC__ or __386__ weren't
//typedef double fixed;

//#endif
#endif

