//
//namespace N
//{
//  struct X{};
//
//  template<typename T>
//  int* operator+( T , unsigned ) { return 0; }
//}
//
//template< typename T >
//struct Proxi
//{
//  T t;
//};
//
//template< typename T >
//Proxi< T > operator+( Proxi< T > &, unsigned )
//{
//  return {};
//}
//
//template< typename T >
//struct Struct
//{
//  Proxi< T > store;
//
//  T & operator[]( int i )
//  {
//    //store + 1;
//    ::operator+( store, 1 );
//    return store.t;
//  }
//};
//
//int main()
//{
// Struct< N::X > v;
// v[0] = N::X{};
//}

#include <istream>
#include <streambuf>
#include <string>
#include <sstream>

class stream_buf : public std::streambuf
{
  using p_void = void ( stream_buf::* )( char *, char *, char * );

public:
  stream_buf( std::string::iterator begin, std::string::iterator end )
  {
    setg( &*begin, &*begin, &*begin + ( end - begin ) );
    setp( &*begin, &*begin, &*begin + ( end - begin ) );
  }

  bool seekoff_impl( off_type off,
                     std::ios_base::seekdir dir,
                     p_void set, char * beg, char * cur, char * end )
  {
    switch( dir )
    {
      case std::ios_base::beg:
      {
        if ( off < 0 ||
             std::distance( beg, end ) < off )
          return false;

        ( this->*set )( beg, beg + off, end );
      }
        break;

      case std::ios_base::cur:
      {
        if ( std::distance( cur, end ) < std::abs( off ) )
          return false;

        ( this->*set )( beg, cur + off, end );
      }
        break;

      case std::ios_base::end:
      {
        if ( off > 0 ||
             std::distance( beg, end ) < std::abs( off ) )
          return false;

        ( this->*set )( beg, end + off, end );
      }
        break;

      default:
        return false;
    }

    return true;
  }

  pos_type seekoff( off_type off,
                    std::ios_base::seekdir dir,
                    std::ios_base::openmode which ) override
  {
    if( which == std::ios_base::in &&
        seekoff_impl( off, dir, &stream_buf::setg,  eback(), gptr(), egptr() ) )
      return gptr() - eback();

    if( which == std::ios_base::out &&
        seekoff_impl( off, dir, &stream_buf::setp,  pbase(), pptr(), epptr() ) )
        return pptr() - pbase();

    return pos_type( off_type( -1 ) );
  }



  char_type * e_back() const
  {
    return eback();
  }

  char_type * g_ptr() const
  {
    return gptr();
  }

  char_type * e_gptr() const
  {
    return egptr();
  }

  char_type * p_base() const
  {
    return pbase();
  }

  char_type * p_ptr() const
  {
    return pptr();
  }

  char_type * e_pptr() const
  {
    return epptr();
  }
};

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

int main()
{
  std::string buff( 3, '\0' );
  stream_buf sbuff{ buff.begin(), buff.end() };

  std::iostream stream( &sbuff );

  //std::stringstream stream;
  //stream.set_rdbuf( &sbuff );

  address_list p_address_list;
  p_address_list.set( sbuff.p_base(), sbuff.p_ptr(), sbuff.e_pptr() );

  address_list g_address_list;
  g_address_list.set( sbuff.e_back(), sbuff.g_ptr(), sbuff.e_gptr() );

  std::string string;

  std::size_t g_pos = stream.tellg();
  std::size_t p_pos = stream.tellp();

  stream.write( "123", 3 );

  stream.seekp( 10, std::ios::beg );

  g_pos = stream.tellg();
  p_pos = stream.tellp();

  char b[254]{};
  stream.read( b, 3 );

  g_pos = stream.tellg();
  p_pos = stream.tellp();

  return 0;
}