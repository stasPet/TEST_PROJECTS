#include <ios>

struct address_list
{
  char * beg;
  char * cur;
  char * end;

  void set( char * b, char * c, char * e )
  {
    beg = b;
    cur = c;
    end = e;
  }
};

struct flags
{
  bool good;
  bool eof;
  bool fail;
  bool bad;

  void set( std::ios & ios )
  {
    good = ios.good();
    eof  = ios.eof();
    fail = ios.fail();
    bad  = ios.bad();
  }
};