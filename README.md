This is a Nash Equilibrium solver for heads up (2 player) all-in or fold Texas Holdem given a blind structure and a stack size written in C++.

Note that a Nash Equilibrium is not a strategy for a particular player but two strategies one for each player. I prefer the term "Nash Equilibrium Profile" as it makes this more clear.

The algorithm used to compute the Nash Equilibrium is the Regret Matching algorithm as described by Hart and Mas-Collel in the early 2000s.

To shuffle the cards for each round of simulated play, we use the Fisher-Yates algorithm but instead of shuffling all the cards, we stop after 9 cards are chosen. This significantly reduces the runtime.

To quickly determine the winner of each hand, the so-called Two-Plus-Two Evaluator developed by the community at the twoplustwo poker forums.  A large array of about 30 million numbers is used to assign a number to the 7 cards that a player can use (2 of their own cards plus 5 of the community or 'board' cards).  This value is compared with the other player(s) value(s) to determine the outcome of the hand (can include ties). The program to compute the 30 million numbers is at https://github.com/tangentforks/TwoPlusTwoHandEvaluator

Note that it is not necessarily the strategies which are being updated each round of simulated play that converge to the Nash Equilibrium rather it is the average (taken over the number of times a decision node was reached; that is, the average taken over the number of times the 2 card starter hand was dealt) of those strategies.

The program outputs a Nash Equilibrium strategy for each player which consists of a 13x13 grid of probabilities which represents the frequency the player should randomly go all in with that hand. The indices of this grid start at 0 and suited unpaired hands are represented with the first index > second index (i.e. bottom left). A hand given in this 13x13 grid representation is what I call the "index" representation. This "index" representation is used because of the symmetry with regard to suits. e.g., the strategy for Ace of clubs, 3 of diamonds is the same for Ace of spades, 3 of hearts. The particular suit doesn't matter, the only thing that matters with regard to suits is if the two suits are the same or the two suits are different. So instead of dealing with (52 choose 2) = 1326 hands, you only have to deal with 13x13 = 169 hands. 

However, you cannot just put a uniform probability distribution over the 169 hands and randomly choose from the 13x13 grid to run your Monte Carlo simulations. You need to choose the specific cards. You can map the cards in a deck to the numbers 1, 2,..., 52.  e.g. 1 -> 2c, 2 -> 2d, 3 -> 2h, 4 -> 2s, 5 -> 3c, ..., 48 -> Ks, 49 -> Ac, 50 -> Ad, 51 -> Ah, 52 -> As where c,d,h,s represents clubs, diamonds, hearts, spades respectively.  A 2 card starter hand given in this representation is what I will call the "Specific Representation" for the hand.

A broad view of how the program works:

- shuffle cards with modified Fisher-Yates algorithm
- deal 2 cards to both players
- randomly choose 5 board cards from the remaining 48 cards
- determine the winner of the hand using the two-plus-two evaluator
- analyze what would have happend if the players took different actions and compute the regrets (regret of not taking a particular action)
- update strategies (these updated strategies don't necessarily converge to Nash Equilibrium)
- keep a running sum of the strategies to calculate the Nash Equilibrium at the end
- repeat

After this simulated play is done many times, the Nash Equilibrium is
approximated by taking the average of the strategies taken over the number
of times the hand was dealt (i.e. how many times that decision node was encountered).

Note that the Nash Equilibrium is not the one that can be the most profitable. If a player plays suboptimally, if you play the Nash Equilibrium strategy, you could win less than a "maximally exploitative" strategy.  However, playing a maximally exploitative (not Nash Equilibrium) strategy exposes you to being exploited. It is similar to when a boxer goes on the offense, in throwing a punch, they drop their defensive position and expose themselves to getting hit.

This algorithm can be modified to find the maximally exploitative strategy if one of the strategies is kept fixed.

Beyond 2 Players:

Although the program can be extend beyond 2 players (which I wasted many hours doing so) calculating the Nash Equilibrium for 3 or more players is not really practical.  This is because there could be many Nash Equilibriums and if 2 players are playing from one Nash Equilibrium profile and you are playing from a different Nash Equilibrium profile, then those 3 strategies taken together aren't necessarily a Nash Equilibrium. Which means you need to somehow know the other two players are playing from a particular Nash Equilibrium. This was explained with a nice counter example in a paper I read but I forget where I read it. I think it was a 3 player penny matching game.

To find a good strategy for 3 player all-in or fold poker, a different method can be used that involves guessing your opponents' strategies and running the algorithm on it except instead of updating all the strategies, you keep those 2 opponent's strategies fixed. This would not necessarily give a Nash Equilibrium strategy but it would give the most exploitative strategy, ie the most profitable strategy (if your opponents keep their strategies fixed which is unlikely as most players adjust their strategy to what the other players are doing)


Requirements: g++ with c++20, make

Usage: .\bin\headsup_nash.exe 1000000

The command line argument tells you how many iterations to run.
