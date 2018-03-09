  #include <stdio.h>

  int main (void) {

    char str[100];
    int sum = 0;
    int pow = 2;
    int length = 0;
    printf("Enter binary: ");
    fgets(str,100,stdin);
    printf("\n");
    for(int i = 0; str[i]!='\n'&&str[i]!='\0'; i++) {
      length++;
    }
    for(int i = length-1; i >= 0; i--) {
      if (str[i]!='1'&&str[i]!='0') {
        printf("Not binary!\n");
        return 0;
      }
      pow = 2;
      if (str[i]=='0')
        continue;

      if (i==length-1){
        sum +=1;
        continue;
      }
      for (int j = 2; j < length-i; j++) {
        pow *= 2;
      }
      sum += pow;
      pow=2;
    }
    printf("%d in decimal\n",sum);
    return 0;
  }
