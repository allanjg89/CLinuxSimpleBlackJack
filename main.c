/*
 * main.c
 *
 *  Created on: Aug 22, 2017
 *      Author: root
 */

#include "SimpleBlackjack.h"

void testScoreComputer();

int main(int argc, char * argv[]){

	char * machineName = "Dealer";
	char username[MAXUSERNAMELEN+1] = {0};
	static char * welcomeMessage =  "Welcome To Simple Blackjack\n ";
	char continuePlaying[INPUTBUFFSIZE] = {'y',0};

	//testScoreComputer();

	printf(welcomeMessage, MAXUSERNAMELEN);

	getUsername(username);


	player * user = createPlayer(username);
	player * machine = createPlayer(machineName);

	srand(time(NULL));


	do{
		initializeHand(user);
		initializeHand(machine);

		//PrintGameSate(user, machine);
		startContest(user, machine);

		printf("Would you like to play again (y/n)? ");
		flushStdIn();
		fgets(continuePlaying, INPUTBUFFSIZE, stdin);

	}while(continuePlaying[0] == 'y');


	freePlayer(user);
	freePlayer(machine);
}

void testScoreComputer(){

		player aces = {0};
		player vareity = {0};
		int bestScore = 0;

		for(int i = 0; i < 5; i++){
			addCardToPlayer(&aces, createCardFromRank(1));
			addCardToPlayer(&vareity, generateRandomCard());
		}

		bestScore = computeBestScore(&aces);
		bestScore = computeBestScore(&vareity);

}
