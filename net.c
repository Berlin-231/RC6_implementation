#include <stdio.h>

#include <stdlib.h>
#include <string.h>



#define r 20    
#define w 32 

#define b_len   (w / 8)     
#define c       ((kl + b_len - 1) / b_len)  


#define LEFT(a,b) (((a)<<(b&(w-1))) | ((a)>>(w-(b&(w-1)))))
#define RIGHT(a,b) (((a)>>(b&(w-1))) | ((a)<<(w-(b&(w-1)))))
 
unsigned int S[(2 * r + 4) - 1];        /* Key schedule */

void  flipper(unsigned int *ct, char resultct[4][8]){
    int i;
    char  temp[4];
    for(i=0;i<4;i++){
    sprintf(resultct[i], "%x", ct[i]);
    
   // temp[0] = resultct[0]+resultct[1];
   
    temp[0] = resultct[i][0];
    temp[1] = resultct[i][1];
    temp[2] = resultct[i][2];
    temp[3] = resultct[i][3];
    resultct[i][0] =resultct[i][6];
    resultct[i][1] = resultct[i][7];
    resultct[i][2] = resultct[i][4];
    resultct[i][3] = resultct[i][5];
    resultct[i][4] = temp[2];
    resultct[i][5] = temp[3];
    resultct[i][6] = temp[0];
    resultct[i][7] = temp[1];
    }
   // return  resultct;
}

void rc6_key_setup(unsigned char *K, int kl)
{
    int i, j;
    int s, v = (2 * r + 4);
    unsigned int L[(32 + b_len - 1) / b_len];
    unsigned int A;
    unsigned int  B;
 
    L[c - 1] = 0;
    for (i = kl - 1; i >= 0; i--)
        L[i / b_len] = (L[i / b_len] << 8) + K[i];
 
    S[0] = 0xB7E15163;
    for (i = 1; i <= 2 * r + 3; i++)
        S[i] = S[i - 1] + 0x9E3779B9;
 
    A = B = i = j = 0;
    if (c > v) v = c;
    v =v * 3;
 
    for (s = 1; s <= v; s++)
    {
        A = S[i] = LEFT(S[i] + A + B, 3);
        B = L[j] = LEFT(L[j] + A + B, A + B);
        i = (i + 1) % (2 * r + 4);
        j = (j + 1) % c;
    }
}
 
void rc6_block_encrypt(unsigned int *pt, unsigned int *ct)
{
    unsigned int A, B, C, D, t, u, tmp;
    int i, j;
 
    A = pt[0];  B = pt[1]; C = pt[2];  D = pt[3];

    B = B + S[0];
    D = D + S[1];
    for (i = 2; i <= r * 2; i = i + 2)
    {
        t = LEFT(B * (2 * B + 1), 5);
        u = LEFT(D * (2 * D + 1), 5);
        A = LEFT(A ^ t, u) + S[i];
        C = LEFT(C ^ u, t) + S[i + 1];

        tmp = A;  A = B; B = C; C = D;  D = tmp;
    }
    A = A + S[2 * r + 2];
    C = C + S[2 * r + 3];

    ct[0] = A; ct[1] = B; ct[2] = C; ct[3] = D;

}
 
void rc6_block_decrypt(unsigned int *ct, unsigned int *pt)
{
    unsigned int A, B, C, D, t, u, tmp;
    int i, j;
 
    A = ct[0];  B = ct[1];  C = ct[2];  D = ct[3];
    C = C - S[2 * r + 3];
    A =  A - S[2 * r + 2];
    for (i = r * 2; i >= 2; i = i - 2)
    {
        tmp = D; D = C; C = B;  B = A; A = tmp;

        u = LEFT(D * (2 * D + 1), 5);
        t = LEFT(B * (2 * B + 1), 5);
        C = RIGHT(C - S[i + 1], t) ^ u;
        A = RIGHT(A - S[i], u) ^ t;
    }
    D = D - S[1];  B = B - S[0];
    
    pt[0] = A; pt[1] = B;  pt[2] = C;  pt[3] = D;  
}
 
 
int main()
{   unsigned int ct[4], pt[4];
    //unsigned int pt2[4]={0x35241302, 0x79685746, 0xbdac9b8a, 0xf1e0dfce};
    int i;unsigned int a,b,q,d; unsigned char key[32];
    //unsigned char key2[32] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
    //     0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78};
    char concat[8];
    int j =0;
    int n=0;char inp[2];
    char *result = malloc(10);
    char *result2 = malloc(10);
    //unsigned char b;
    //struct test_struct *p;
    char vecName[20];
    char textname[20];
    FILE *fp;
    char * a1= malloc(2);
    char * a2= malloc(2);
    char * a3= malloc(2);
    char * a4= malloc(2);
    fp = fopen ("input.txt","r");
    fscanf(fp,"%s\n",&vecName);
    
    int op = 0;

    if (!strcmp(vecName,"encryption")){
        //printf("\n running enc");
        fscanf(fp,"%[^:]: ",&textname);
        //printf("%s\n",textname);
        if(!strcmp(textname,"plaintext")){
        op = 1;
        }
        else{
           // usage();
            return 0;
        }
    }
    else if(!strcmp(vecName,"decryption")){
              //printf("\n running dec");
              fscanf(fp,"%[^:]: ",&textname);
        if(!strcmp(textname,"ciphertext")){
        op = 2;
        }
        else{
           // usage();
            return 0;
        }
    }
    else{
        //usage();
        return 0;
    }
        //printf("%s\n",vecName);
        while(fscanf(fp,"%s",&inp)!=EOF ){
        j++;
            //printf(" ");
            //printf("%s",inp);
            if(j==1){
                strcpy(a1,inp);
                //sscanf(inp,"%s",concat[y]);
                //y--;
                //printf("\n%s \n",concat[y]);
            //strcpy(result,inp);
            }
            else if(j==2){
                strcpy(a2,inp);
                //strcat(result,inp);
            }        
            else if(j==3){
                strcpy(a3,inp);
                //strcat(result,inp);
            } 
            else if(j==4){
                // n=1;
                //printf("\n result %d:: %s",n,result);
                //  sscanf(result, "%08p", &pt[n]);
                // n++;
                // j=0;
                    strcpy(a4,inp);
                    strcpy(result,a4);
                    strcat(result,a3);
                    strcat(result,a2);
                    strcat(result,a1);
                    sscanf(result, "%08p", &pt[n]);
                    //printf("\n result %d:: %s",n,result);
                    n++;
                    j=0;
                    result = malloc(10);
            }

            if(!strcmp(inp,"userkey:")){
                //printf("\n Breaking after - %s",inp);
            // printf("\n concat%2c\n",concat[0]);
                break;
            }
            //b = b+a 
        }
        free(result);
    
        /*for (i =0 ; i< n ; i++)
        printf(" pt[%d] :: %08p",i,pt[i]);
        */
        i=0;
        
        int x = 0;
        while(fscanf(fp,"%s",&inp)!=EOF){
        //printf("\n Readme Key :: %s ",inp);
            sscanf(inp,"%08p",&key[x]);
            x++;
        }
       // for (i =0 ; i< 16 ; i++)
        //printf(" key2[%d] :: %08p",i,key[i]);
        
       
        rc6_key_setup(key, x);
        if(op == 1){
        rc6_block_encrypt(pt, ct);
        }
        else if(op == 2)
        {
            rc6_block_decrypt(pt,ct);
        }
        
        char resultct[4][8];
        flipper(ct,resultct);
        //resultct[2] =resultct[1];
        //resultct[3] =temp[0];
        //printf("\nTest 1 :: generated cipher :  %s  %08x %08x %08x %08x\n",
        //        resultct[0],ct[0], ct[1], ct[2], ct[3]);

        FILE *fpw;
        fpw = fopen("output.txt","w");
        
        if(op == 1){
        fprintf(fpw,"Ciphertext: ");
        printf("\n Encryption Complete");
        }
        else if(op == 2){
        fprintf(fpw,"Plaintext: ");
        printf("\n Decryption Complete");
        }

        for(i=0 ; i < 32 ; i++){
            fprintf(fpw,"%c",resultct[0][i++]);
            fprintf(fpw,"%c ",resultct[0][i]);
        }

        fclose(fpw);
        return 0;
    }

    

