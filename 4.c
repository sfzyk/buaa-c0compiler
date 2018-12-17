void main()
{
  int a,b;
  
  printf("Input a :\n"); 
  scanf(a);
  printf("Input b :\n"); 
  scanf(b);
  
  while ( b != 0 )
  {
    printf("a+b=",a+b);printf("\n");
    printf("a-b=",a-b);printf("\n");
    printf("a*b=",a*b);printf("\n");
    printf("a/b=",a/b);printf("\n");
    printf("Input a :\n");
    scanf(a);
    printf("Input b :\n"); 
    scanf(b);
  }
}
