/*
 * SimpleBlackjack.h
 *
 *  Created on: Aug 22, 2017
 *      Author: root
 */

#ifndef SIMPLEBLACKJACK_H_
#define SIMPLEBLACKJACK_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <error.h>
	#include <time.h>
	#include <string.h>
	#include<fcntl.h>

	#define MAXUSERNAMELEN 10
	#define MAXNUMBEROFCARDS 21
	#define NUMBEROFRANKS 13
	#define NUMBEROFSUITS 4
	#define DEALERGOALSCORE 17
	#define BLACKJACK 21
	#define MAXACEVAL 11
	#define MINACEVAL 1
	#define INPUTBUFFSIZE 2

	typedef struct _card{
		int rankVal;
		char * rank;
		char * suit;
	} card;

	typedef struct _player{
		char * username;
		int numCards;
		int bestScore;
		card * hand[MAXNUMBEROFCARDS];
	} player;

	void getUsername(char * username);
	char * copyStringToHeap(char * str);
	void checkCalloc(void * addr);
	player * createPlayer(char * name);
	void freePlayer(player * p);
	card * createCardFromRank(int rankVal);
	card * createCardFromRankAndSuit(int rankVal, int suitVal);
	card * generateRandomCard();
	void freeCard(card * c);
	void addCardToPlayer(player * p, card * c);
	int generateRandomNum(int max);
	void initializeHand(player * p);
	void printPlayer(player * p);
	int computeBestScore(player * p);
	void PrintGameSate(player * user, player * machine);
	void startContest(player * user, player * machine);
	void dealerHitStatement();
	void flushStdIn();


#endif /* SIMPLEBLACKJACK_H_ */
