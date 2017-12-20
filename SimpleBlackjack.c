/*
 * SimpleBlackjack.c
 *
 *  Created on: Aug 22, 2017
 *      Author: root
 */
#include "SimpleBlackjack.h"

static char * ranks[] = {"Ace", "Two", "Three", "Four", "Five", "Six",
						"Seven", "Eight", "Nine", "Ten", "Ace", "Jack", "Queen", "King"};

static char * suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};

void getUsername(char * username){
	do{
		printf("\nPlease provide a valid user name no longer than %d characters long:", MAXUSERNAMELEN);
		flushStdIn();
		fgets(username, MAXUSERNAMELEN, stdin);
	}while(strlen(username) == 0);

	username[strlen(username)-1] = 0x00;
}

char * copyStringToHeap(char * str){

	int len = strlen(str);
	char * strHeap = calloc(len, sizeof(char));
	strncpy(strHeap, str, len);

	return strHeap;

}

void checkCalloc(void * addr){
	if(addr == NULL){
		printf("calloc failed.\n");
		exit(-1);
	}
}

player * createPlayer(char * name){

	player * ret = (player *) calloc(1, sizeof(player));
	checkCalloc(ret);

	ret->username = copyStringToHeap(name);
	if(ret->username == NULL){
		printf("calloc failed\n");
		free(ret);
		exit(-1);
	}
	ret->numCards = 0;

	return ret;

}

void freePlayer(player * p){
	card * temp = NULL;
	if(p != NULL){
		if(p->username != NULL) free(p->username );
		for(int i = 0; i<MAXNUMBEROFCARDS; i++){
			temp = p->hand[i];
			if(temp!=NULL) free(temp);
		}
		free(p);
	}

}

card * createCardFromRank(int rankVal){

	card * ret = (card *) calloc(1, sizeof(card));
	checkCalloc(ret);

	rankVal--;

	if(rankVal == 10) rankVal = 0;

	if((ret->rank = copyStringToHeap(ranks[rankVal])) == NULL){
		printf("calloc failed\n");
		free(ret);
		exit(-1);
	}

	if((ret->suit = copyStringToHeap(suits[generateRandomNum(NUMBEROFSUITS)])) == NULL){
		printf("calloc failed\n");
		free(ret->rank);
		free(ret);
		exit(-1);
	}

	if(rankVal > 9) rankVal = 9;

	ret->rankVal = rankVal+1;
	return ret;

}

card * createCardFromRankAndSuit(int rankVal, int suitVal){

	card * ret = (card *) calloc(1, sizeof(card));
	checkCalloc(ret);

	rankVal--;
	suitVal--;

	if(rankVal == 10) rankVal = 0;

	if((ret->rank = copyStringToHeap(ranks[rankVal])) == NULL){
		printf("calloc failed\n");
		free(ret);
		exit(-1);
	}

	if((ret->suit = copyStringToHeap(suits[suitVal])) == NULL){
		printf("calloc failed\n");
		free(ret->rank);
		free(ret);
		exit(-1);
	}

	if(rankVal > 9) rankVal = 9;

	ret->rankVal = rankVal+1;
	return ret;

}

card * generateRandomCard(){

	int rankVal = generateRandomNum(NUMBEROFRANKS) +1;

	return createCardFromRank(rankVal);


}

void freeCard(card * c){

	if(c != NULL){
		if(c->suit != NULL) free(c->suit);
		if(c->rank != NULL) free(c->rank);
		free(c);
	}

}

void addCardToPlayer(player * p, card * c){

	if(p->numCards>20){
		printf("Maximum number of cards per hand (%d) exceeded..\n"
				"\tExiting now.\n", MAXNUMBEROFCARDS);
		exit(-1);
	}
	p->hand[p->numCards] = c;
	p->numCards += 1;

}

int generateRandomNum(int max){

	return rand()%max;
}

void initializeHand(player * p){
	addCardToPlayer(p, generateRandomCard());
	p->numCards = 1;
	computeBestScore(p);
}


void printPlayer(player * p){

	card * currCard = NULL;

	printf("%s:\n"
			"\tHand:\n", p->username);

	for(int i = 0; i < p->numCards; i++){
		currCard = p->hand[i];
		printf("\t\t%s of %s\n", currCard->rank, currCard->suit);
	}

	printf("\n\tCurrent Score:\t%d", p->bestScore);
}

int computeBestScore(player * p){
	int baseVal = 0;
	int numberOfAces = 0;
	int aceCombo = 0;
	int bestScore = 0;
	int numberOfIterations = 0;

	card * currCard = NULL;

	for(int i = 0; i < p->numCards; i++){
		currCard = p->hand[i];

		if(currCard->rankVal > 1){
			baseVal += currCard->rankVal;
		}else{
			numberOfAces++;
		}
	}

	//The following accounts for the fact that Aces can be either 1 or 11
	//One wants the highest possible score that is not greater than 21.
	do{
		aceCombo = MINACEVAL*numberOfIterations + MAXACEVAL*(numberOfAces-numberOfIterations);

		bestScore = baseVal+aceCombo;

		numberOfIterations++;

	}while(bestScore>BLACKJACK && (numberOfAces-numberOfIterations>=0));

	p->bestScore = bestScore;

	return bestScore;
}

void PrintGameSate(player * user, player * machine){
	static char * iterationSeperator = "\n**********************************\n";
	static char * playerSeperator = "\n------------------------------------\n";


	printf(iterationSeperator);
	printPlayer(machine);
	printf(playerSeperator);
	printPlayer(user);
	printf(iterationSeperator);
}

void startContest(player * user, player * machine){

	char hit[INPUTBUFFSIZE] = {'y', 0};
	char * winMessage = "YOU WIN!\n";
	char * looseMessageBust = "BUST!\n";
	char * dealerLooseMessage = "Dealer BUST! YOU WIN!\n";
	char * delaerWinsMessage = "Dealer Wins... Sorry :(\n";
	char * hitMessage = "Would you like to hit (y/n)? ";
	char * tie = "Its a TIE!\n";

	while(hit[0] == 'y'){

		addCardToPlayer(user, generateRandomCard());
		computeBestScore(user);

		PrintGameSate(user, machine);

		if(user->bestScore == BLACKJACK){
			printf(winMessage);
			return;
		}else if(user->bestScore > BLACKJACK){
			printf(looseMessageBust);
			return;
		}

		printf(hitMessage);

		flushStdIn();
		fgets(hit, INPUTBUFFSIZE, stdin);



	}

	dealerHitStatement();

	while(machine->bestScore < DEALERGOALSCORE){
		addCardToPlayer(machine, generateRandomCard());
		computeBestScore(machine);
		PrintGameSate(user, machine);
		dealerHitStatement();
	}

	if(machine->bestScore > BLACKJACK){
		printf(dealerLooseMessage);
		return;
	}

	if(machine->bestScore > user->bestScore){
		printf(delaerWinsMessage);
		return;
	}


	if(machine->bestScore == user->bestScore){
		printf(tie);
		return;
	}

	printf(winMessage);
	return;



}

void dealerHitStatement(){
	char temp[INPUTBUFFSIZE] = {0};
	char * dealerHit = "Dealer will now hit or stay.\n"
				"\tPress <ENTER> to continue...\n";
	printf(dealerHit);
	flushStdIn();
	fgets(temp, INPUTBUFFSIZE, stdin);

}

void flushStdIn(){

    int nextChar = 0;

    //make stdin non-blocking
    int flags = fcntl(0, F_GETFL);
    fcntl (0, F_SETFL, O_NONBLOCK);

    while(nextChar != '\n' && nextChar != -1){
        nextChar = getc(stdin);
    }

    //restore original flags
    fcntl (0, F_SETFL, flags);

}

