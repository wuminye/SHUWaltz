#ifndef POKERUTIL_H_INCLUDED
#define POKERUTIL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "arrays.h"
#define	STRAIGHT_FLUSH	1
#define	FOUR_OF_A_KIND	2
#define	FULL_HOUSE      3
#define	FLUSH           4
#define	STRAIGHT        5
#define	THREE_OF_A_KIND	6
#define	TWO_PAIR        7
#define	ONE_PAIR        8
#define	HIGH_CARD       9

#define	RANK(x)		((x >> 8) & 0xF)



#define CLUB	0x8000
#define DIAMOND 0x4000
#define HEART   0x2000
#define SPADE   0x1000

#define Deuce	0
#define Trey	1
#define Four	2
#define Five	3
#define Six     4
#define Seven	5
#define Eight	6
#define Nine	7
#define Ten     8
#define Jack	9
#define Queen	10
#define King	11
#define Ace     12



// Poker hand evaluator
//
// Kevin L. Suffecool
// suffecool@bigfoot.com
//


// perform a binary search on a pre-sorted array
//
int findit( int key )
{
    int low = 0, high = 4887, mid;

    while ( low <= high )
    {
        mid = (high+low) >> 1;      // divide by two
        if ( key < products[mid] )
            high = mid - 1;
        else if ( key > products[mid] )
            low = mid + 1;
        else
            return( mid );
    }
    fprintf( stderr, "ERROR:  no match found; key = %d\n", key );
    return( -1 );
}

//
//   This routine initializes the deck.  A deck of cards is
//   simply an integer array of length 52 (no jokers).  This
//   array is populated with each card, using the following
//   scheme:
//
//   An integer is made up of four bytes.  The high-order
//   bytes are used to hold the rank bit pattern, whereas
//   the low-order bytes hold the suit/rank/prime value
//   of the card.
//
//   +--------+--------+--------+--------+
//   |xxxbbbbb|bbbbbbbb|cdhsrrrr|xxpppppp|
//   +--------+--------+--------+--------+
//
//   p = prime number of rank (deuce=2,trey=3,four=5,five=7,...,ace=41)
//   r = rank of card (deuce=0,trey=1,four=2,five=3,...,ace=12)
//   cdhs = suit of card
//   b = bit turned on depending on rank of card
//

void init_deck( int *deck )
{
    int i, j, n = 0, suit = 0x8000;

    for ( i = 0; i < 4; i++, suit >>= 1 )
        for ( j = 0; j < 13; j++, n++ )
            deck[n] = primes[j] | (j << 8) | suit | (1 << (16+j));
}


//  This routine will search a deck for a specific card
//  (specified by rank/suit), and return the INDEX giving
//  the position of the found card.  If it is not found,
//  then it returns -1
//

int find_card( int rank, int suit, int *deck )
{
	int i, c;

	for ( i = 0; i < 52; i++ )
	{
		c = deck[i];
		if ( (c & suit)  &&  (RANK(c) == rank) )
			return( i );
	}
	return( -1 );
}


//
//  This routine takes a deck and randomly mixes up
//  the order of the cards.
//

void shuffle_deck( int *deck )
{
    int i, n, temp[52];

    for ( i = 0; i < 52; i++ )
        temp[i] = deck[i];

    for ( i = 0; i < 52; i++ )
    {
        do {
            n = (int)(rand()%52);
        } while ( temp[n] == 0 );
        deck[i] = temp[n];
        temp[n] = 0;
    }
}


void print_hand( int *hand, int n )
{
    int i, r;
    char suit;
    static char rank[] = "23456789TJQKA";

    for ( i = 0; i < n; i++ )
    {
        r = (*hand >> 8) & 0xF;
        if ( *hand & 0x8000 )
            suit = 'c';
        else if ( *hand & 0x4000 )
            suit = 'd';
        else if ( *hand & 0x2000 )
            suit = 'h';
        else
            suit = 's';

        printf( "%c%c ", rank[r], suit );
        hand++;
    }
}


int hand_rank( short val )
{
    if (val > 6185) return(HIGH_CARD);        //    1277 high card
    if (val > 3325) return(ONE_PAIR);         //    2860 one pair
    if (val > 2467) return(TWO_PAIR);         //    858 two pair
    if (val > 1609) return(THREE_OF_A_KIND);  //    858 three-kind
    if (val > 1599) return(STRAIGHT);         //    10 straights
    if (val > 322)  return(FLUSH);            //    1277 flushes
    if (val > 166)  return(FULL_HOUSE);       //    156 full house
    if (val > 10)   return(FOUR_OF_A_KIND);   //    156 four-kind
    return(STRAIGHT_FLUSH);                   //    10 straight-flushes
}


short eval_5cards( int c1, int c2, int c3, int c4, int c5 )
{
    int q;
    short s;

    q = (c1|c2|c3|c4|c5) >> 16;

    /* check for Flushes and StraightFlushes
    */
    if ( c1 & c2 & c3 & c4 & c5 & 0xF000 )
	return( flushes[q] );

    /* check for Straights and HighCard hands
    */
    s = unique5[q];
    if ( s )  return ( s );

    /* let's do it the hard way
    */
    q = (c1&0xFF) * (c2&0xFF) * (c3&0xFF) * (c4&0xFF) * (c5&0xFF);
    q = findit( q );

    return( values[q] );
}


short eval_5hand( int *hand )
{
    int c1, c2, c3, c4, c5;

    c1 = *hand++;
    c2 = *hand++;
    c3 = *hand++;
    c4 = *hand++;
    c5 = *hand;

    return(eval_5cards(c1,c2,c3,c4,c5));
}


#define Equivalence 7462
int deck[52], hand[5], freq[10];
int Punique[Equivalence];

void SystemInit()
{
    init_deck(deck);
    for (int j = 0, m = 1; j < Equivalence; j++ )
    {
        if (j+1>=1 && j+1<=166)
        {
            Punique[j] = m*4;
            m++;
        }
        else if(j+1>=167 && j+1<=322)
        {
            if(j+1==167)
                m=1;
            else
                m++;
            Punique[j]=Punique[166-1]+m*24;
        }
        else if(j+1>=323 && j+1 <= 1599)
        {
            if(j+1==323)
                m=1;
            else
                m++;
            Punique[j]=Punique[322-1]+m*4;
        }
        else if(j+1>=1600 && j+1 <= 1609)
        {
            if(j+1==1600)
                m=1;
            else
                m++;
            Punique[j]=Punique[1599-1]+m*1020;
        }
        else if(j+1>=1610 && j+1 <= 2467)
        {
            if(j+1==1610)
                m=1;
            else
                m++;
            Punique[j]=Punique[1609-1]+m*64;
        }
        else if(j+1>=2468 && j+1 <= 3325)
        {
            if(j+1==2468)
                m=1;
            else
                m++;
            Punique[j]=Punique[2467-1]+m*144;
        }
        else if(j+1>=3326 && j+1 <= 6185)
        {
            if(j+1==3326)
                m=1;
            else
                m++;
            Punique[j]=Punique[3325-1]+m*384;
        }
        else if(j+1>=6186 && j+1 <= 7462)
        {
            if(j+1==6186)
                m=1;
            else
                m++;
            Punique[j]=Punique[6185-1]+m*1020;
        }
    }

}

#endif // POKERUTIL_H_INCLUDED
