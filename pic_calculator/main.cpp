#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// key for signs ( + - * /
 int key1  = 98765;
 int key2  = 87654;
 int key3  = 76543;
 int key4  = 65432;
 int key5  = 54321;

 // for tmp math solve

int siz;
int tmpsiz;
int slv[50][3];
int tmpf;
int tmpl;
int stk[50];

void lpush ( int x ) // PUSH new node at end of my stack
{
    stk[siz] = x;
    siz++;
    return;
}

int lpop () // POP from end of my stack and it returns the value of the node that POPED
{
    int x = stk[siz-1];
    siz--;
    return x; // returning the last value
}

int solve () // for solving the arr that it do not contain any ( )
{
    int crs = tmpsiz - 1;
    while (crs >=0) // moving from end to the first and solve * and /
    {
        if(slv[crs][0] == key4) // it is *
        {
            slv[crs][0] = slv[ slv[crs][1] ][0] * slv[ slv[crs][2] ][0] ; // put ans in sign node

            /* changing the pointers to the right place */

            slv[crs][1] = slv[ slv[crs][1] ][1];
            slv[crs][2] = slv[ slv[crs][2] ][2];
            slv[ slv[crs][1] ][2] = crs;
            if ( slv[crs][2]  >= 0 ) slv[ slv[crs][2] ][1] = crs;
        }
        else if(slv[crs][0] == key5) // it is /
        {
            slv[crs][0] = slv[ slv[crs][1] ][0] / slv[ slv[crs][2] ][0] ; // put ans in sign node

            /* changing the pointers to the right place */
            slv[crs][1] = slv[ slv[crs][1] ][1];
            slv[crs][2] = slv[ slv[crs][2] ][2];
            slv[ slv[crs][1] ][2] = crs;
            if ( slv[crs][2]  >= 0 ) slv[ slv[crs][2] ][1] = crs;
        }
        crs = slv[crs][2]; // moving to next node
    }


    crs = tmpsiz - 1;
    while (crs >=0)
    {
        if(slv[crs][0] == key2) // it is +
        {
            slv[crs][0] = slv[ slv[crs][1] ][0] + slv[ slv[crs][2] ][0] ; // put ans in sign node

            /* changing the pointers to the right place */
            slv[crs][1] = slv[ slv[crs][1] ][1];
            slv[crs][2] = slv[ slv[crs][2] ][2];
            if ( slv[crs][2]  < tmpsiz ) slv[ slv[crs][1] ][2] = crs;
            if ( slv[crs][2]  >= 0 ) slv[ slv[crs][2] ][1] = crs;
        }
        else if(slv[crs][0] == key3) // it is -
        {
            slv[crs][0] = slv[ slv[crs][1] ][0] - slv[ slv[crs][2] ][0] ; // put ans in sign node

            /* changing the pointers to the right place */
            slv[crs][1] = slv[ slv[crs][1] ][1];
            slv[crs][2] = slv[ slv[crs][2] ][2];
            if ( slv[crs][2]  < tmpsiz ) slv[ slv[crs][1] ][2] = crs;
            if ( slv[crs][2]  >= 0 ) slv[ slv[crs][2] ][1] = crs;
        }
        crs = slv[crs][2]; // moving to next node
    }

    /* finding the answer node . we can prove that it is the first node from end that it is -1 */
    int ans = 0;
    crs = tmpsiz - 1;
    while (slv[crs][2]  >= 0)
    {
        ans = slv[crs][0];
        crs = slv[crs][2];
    }
    ans = slv[crs][0] ;

    return ans;

}

void tmp_add ( int x ) // ADD to my tmp arr to solve
{
    slv[tmpsiz][0] = x;
    slv[tmpsiz][1] = tmpsiz + 1;
    slv[tmpsiz][2] = tmpsiz - 1;
    tmpsiz++;
    tmpl++;
    return;
}

int main()
{
    lpush(1);
    lpush(key2);
    lpush(2);
    lpush(key4);
    lpush(3);
    tmp_add(lpop());
    tmp_add(lpop());
    tmp_add(lpop());
    tmp_add(lpop());
    tmp_add(lpop());
    return solve();


    /* inputing */
    char s[1000*1000];
    fgets( s , 1000*1000 , stdin );

    /* checking that if '(' always in the sting is more than ')' and they are equal at last */

    int t=0;
    for ( int i = 0 ; i < strlen(s) - 1 ; i++)
    {
        if(s[i]=='(') t++;
        else if(s[i]==')') t--;

        if( t < 0 )
        {
            printf("invalid\n");
            return 0;
        }
    }
    if (t != 0)
    {
        printf("invalid\n");
        return 0;
    }

    /* push backing to my stack base on my keys */

    int x=0;
    lpush(key1); // push one ( at the firs of stack
    for( int i = 0 ; i < strlen(s) ; i++)
    {
        if( s[i]== '(' ) lpush(key1);
        else if( s[i]== ')' ) // in this condition we should pop back of our stack until arriving to the ( and solve it
        {
            tmpf = 0;
            tmpl = 0;
            tmpsiz = 0 ;

            x = lpop();
            while ( x != key1)
            {
                tmp_add ( x );
                x = lpop();
            }
            lpush ( solve() );
        }
        else if( s[i]== '+') lpush(key2);
        else if( s[i]== '-') lpush(key3);
        else if( s[i]== '*') lpush(key4);
        else if( s[i]== '/') lpush(key5);
        else
        {
            x = 0;
            while ( s[i] >= '0' && s[i] <= '9' )
            {
                x = x*10 + ( s[i] - '0' );
                i++;
            }
            i--;
            lpush(x);
        }
    }

    /* for our last solving for the rest */

    tmpf = 0;
    tmpl = 0;
    tmpsiz = 0 ;

    x = lpop();
    while ( x != key1)
    {
        tmp_add ( x );
        x = lpop();
    }
    printf("%d\n" ,solve());

    return solve();
}
