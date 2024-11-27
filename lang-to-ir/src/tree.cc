/*int f(int x) {
  int result = (x / 42);
  result++;
   return result ? 1 : 0;
}*/
/*
void test() {
   int* z;
   int x[2];
   x[0]+x[1];
   while(1);
   if(1){}
}

struct str{};
int bar(int u){
   int x=1,y[3];
   str q;
   x=y[2];
   y[1] = x;
   return 1;
}
*/

int foo(int x, int y) {
   return x;
}

int main() {
   int x = foo(1, 2);
   //int a[] = {1, 2};
   int a = foo(1, 2);
   int b = (a + 2) * 3;
   //b = foo(b) ? 1 : 2;
}