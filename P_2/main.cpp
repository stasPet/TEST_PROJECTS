struct my_struct
{
  void foo()
  {
  
  }

  void bar()
  {
    using p_foo = void ( my_struct::* ) ();
    p_foo p { &my_struct::foo };
    ( this->*p )();
  }
};

int main()
{
  my_struct ms;
  ms.bar();
}