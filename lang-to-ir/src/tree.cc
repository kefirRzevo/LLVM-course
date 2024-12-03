
int foo(int x, int bar(int)) {
   return bar(x);
}
int bar(int x) {
   return x;
}

int main() {
   //int a[5][2] = {{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}};
   int x[10] = {1,2,3,4,5,6,7,8,9,0};
   return bar(x[1]);
}
