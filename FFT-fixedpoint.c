#include <stdint.h>
#define SHIFT_AMOUNT 16 //16 bits for decimal, 16 bits for coefficent
#define SHIFT_MASK ((1 << SHIFT_AMOUNT) - 1) // 65535

void FFT(int dir,int points2, int32_t* real, int32_t* img){ //points 2 for lower clock cycles
    int32_t i1,l,l1,l2,c1,c2,tx,ty,t1,t2,u1,u2,z,i,j,ti;
    int32_t tempx, tempy;
    int32_t index,swap,bit,msb,points;
    int32_t count=0; //indexes no need to be shifted prabably better not to in order to accomadate higher # of points
    //bit reversal algorithum
   points = 1;
   count+=1;
   for (index=0;index<points2;index++) {
      count+=2;
      points *= 2;
   }
    msb = points >> 1; //The MSB
    count+=2;
    swap = 0; //
    count+=1;
    for (index=0;index<points-1;index++) { 
      if (index < swap) { //swap the elements 
         tempx = real[index];
         tempy = img[index];
         real[index] = real[swap];
         img[index] = img[swap];
         real[swap] = tempx;
         img[swap] = tempy;
      }
      bit = msb; //assiging the most important bit in reverse d
      while (bit <= swap) { 
         swap -= bit;
         bit >>= 1;
      }
      swap += bit;
   }
   //Total operations N-1 Memory Swaps, N memory assigns, some N*? amount of memerory assigns
   c1 = -1 << SHIFT_AMOUNT; // just multplicative constents no need to multiply by shift
   c2 = 0 << SHIFT_AMOUNT; // just multplicative constents no need to multiply by shift
   l2 = 1 << SHIFT_AMOUNT;
   for (index=0;index<points2;index++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1 << SHIFT_AMOUNT; 
      u2 = 0 << SHIFT_AMOUNT;

       for (j=0;j<l1;j+=(1<< SHIFT_AMOUNT)) {
         for (i=j;i<(points << SHIFT_AMOUNT);i+=l2) {
            i1 = (i + l1) >> SHIFT_AMOUNT; //index need to be downshifted
            t1 = multpF2F(u1,real[i1]) - multpF2F(u2,img[i1]);
            t2 = multpF2F(u1,img[i1]) + multpF2F(u2,real[i1]);
            ti = i >> SHIFT_AMOUNT;
            real[i1] = real[ti] - t1; 
            img[i1] = img[ti] - t2;
            real[ti] += t1;
            img[ti] += t2;
         }
         z = multpF2F(u1,c1) - multpF2F(u2,c2);
         u2 = multpF2F(u1,c2) + multpF2F(u2,c1);
         u1 = z;
      }
      c2 = sqrtF2F(((1 << SHIFT_AMOUNT) - c1) / 2);
      if (dir == 1) 
         c2 = -c2;
      c1 = sqrtF2F(((1 << SHIFT_AMOUNT) + c1) / 2);
   }

 if (dir == 1) {
      for (i=0;i<points;i++) {
         real[i] /= points;
         img[i] /= points;
      }
   }
   
}
int32_t multpF2F (int32_t x,int32_t y){
    int64_t temp = (int64_t)x*(int64_t)y;
    return (temp >> SHIFT_AMOUNT);
}
int32_t sqrtF2F ( int32_t x)
{
    int32_t t, q, b, r;
    r = x;
    b = 0x40000000;
    q = 0;
    while( b > 0x40 )
    {
        t = q + b;
        if( r >= t )
        {
            r -= t;
            q = t + b; // equivalent to q += 2*b
        }
        r <<= 1;
        b >>= 1;
    }
    q >>= 8;
    return q;
}
int main(){
    int32_t signal[4];
    signal[0]=1;
    signal[1]=0;
    signal[2]=2;
    signal[3]=5;
    int32_t signal2[4];
    signal2[0]=0;
    signal2[1]=0;
    signal2[2]=0;
    signal2[4]=0;
    int l;    
    FFT(1,2,signal,signal2);
    FFT(-1,2,signal,signal2);
   
    for(l=0;l<4;l++){
        printf("The real number is %d and the imagianry is %d \n",signal[l],signal2[l]);
    }
}