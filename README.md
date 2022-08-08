# Nash Equilibrium Solver for Heads Up Push or Fold Texas Holdem

This is a Nash Equilibrium solver for heads up (2 player) all-in or fold Texas Holdem given a blind structure and a stack size written in C++.  A Nash Equilibrium is not a strategy for a particular player but two strategies; one for each player. I prefer the term "Nash Equilibrium Profile" as it makes this more clear.

The motivation behind this was because I've seen these Heads Up "Jam or Fold" tables but couldn't find an explanation for how they were computed.

The algorithm I used to compute the Nash Equilibrium is the Regret Matching algorithm as described by Hart and Mas-Collel in the early 2000s. To shuffle the cards for each round of simulated play, we use the [Fisher-Yates algorithm](https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle) but instead of shuffling all the cards, it is halted after nine cards are chosen. This significantly reduces the runtime.

One of the most important algorithms that is needed is a fast method to determine the winner of the hand. To quickly determine the winner of each hand, I use the so called [Two Plus Two Hand Evaluator](https://github.com/tangentforks/TwoPlusTwoHandEvaluator) developed by the community at the Two Plus Two poker forums. A large array of about 30 million numbers is used to assign a number to the 7 cards that a player can use to make their best five-card hand (2 of their own cards plus 5 of the community or 'board' cards).  This value is compared with the other player(s) value(s) to determine the outcome of the hand (can include ties). You can also determine the type of hand (e.g. high card, straight, fullhouse, etc.) by bitshifting the hand value to the right by 12.


## Convergence
Note that it is not necessarily the strategies which are being updated each round of simulated play that converge to the Nash Equilibrium rather it is the average (taken over the number of times a decision node was reached; that is, the average taken over the number of times the 2 card starter hand was dealt) of those strategies. See one of the authors of the original counterfacutal regret minimization paper's answer on [Quora](https://www.quora.com/What-is-an-intuitive-explanation-of-counterfactual-regret-minimization/answer/Michael-Johanson-2?ch=10&oid=9932436&share=8c0546a0&srid=MiWr&target_type=answer)

## "Index" vs "Specific" Representations
The program outputs a Nash Equilibrium strategy for each player which consists of a 13x13 grid of probabilities which represents the frequency the player should randomly go all in with that hand. The indices of this grid start at 0 and suited unpaired hands are represented with the first index > second index (i.e. bottom left). A hand given in this 13x13 grid representation is what I call the "index" representation. This "index" representation is used because of the symmetry with regard to suits. e.g., the strategy for Ace of clubs, 3 of diamonds is the same for Ace of spades, 3 of hearts. The particular suit doesn't matter, the only thing that matters with regard to suits is if the two suits are the same or the two suits are different. So instead of dealing with (52 choose 2) = 1326 hands, you only have to deal with 13x13 = 169 hands. 

However, you cannot just put a uniform probability distribution over the 169 hands and randomly choose from the 13x13 grid to run your Monte Carlo simulations.  This is because unsuited hands have 12 combinations while their corresponding suited hands have 4 combinations. You need to choose the specific cards. You can map the cards in a deck to the numbers 1, 2,..., 52.  e.g. 1 -> 2c, 2 -> 2d, 3 -> 2h, 4 -> 2s, 5 -> 3c, ..., 48 -> Ks, 49 -> Ac, 50 -> Ad, 51 -> Ah, 52 -> As where c,d,h,s represents clubs, diamonds, hearts, spades respectively.  A two-card starter hand given in this representation is what I will call (in the code's comments) the "Specific Representation" for the hand.  The get_index() function converts a starter hand given in the Specific representation to the Index representation.

## A broad overview of how the program works:

- shuffle cards with a modified [Fisher-Yates algorithm](https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle)
- deal 2 cards to both players
- randomly choose 5 board cards from the remaining 48 cards
- determine the winner of the hand using the two-plus-two evaluator
- analyze what would have happend if the players took different actions and compute the regrets (regret of not taking a particular action)
- update strategies (these updated strategies don't necessarily converge to Nash Equilibrium as explained earlier about averaging out the strategies) using "Regret Matching"
- keep a running sum of the strategies to calculate the Nash Equilibrium at the end
- repeat until a sufficient sample size is obtained

After this simulated play is done many times, the Nash Equilibrium is approximated by taking the average of the strategies taken over the number of times the hand was dealt (i.e. how many times that decision node was encountered).  

This algorithm can be modified to find the maximally exploitative strategy if one of the strategies is kept fixed.

## Profitability

The Nash Equilibrium is not always the one that can be the most profitable. If a player plays suboptimally, you could play the Nash Equilibrium strategy and win but you could win less than a "maximally exploitative" strategy.  However, playing a maximally exploitative (not Nash Equilibrium) strategy exposes you to being exploited. It is similar to when a boxer goes on the offense, in throwing a punch, they drop their defensive position and expose themselves to getting hit. Daniel "Jungleman" Cates talks about this in [Daniel Cates - Heads up Play](https://www.youtube.com/watch?v=7OCfVuxNsdg)



It is interesting to note that in the Nash Equilibrium for effective stack size of 10bb, the player in the big blind has an advantage of about 0.8 big blinds per 100 hands. This is evened out as the players swap positions.  This makes sense because usually the player "in position" (the player who acts last) has the advantage. The Nash Equilibrium strategy for the button is just the "least bad" strategy.

## Beyond 2 Players:

Although the program can be extended beyond 2 players (which I wasted many hours calculating) finding the Nash Equilibrium for 3 or more players is not really practical (which I discovered when I lost a couple hundred bucks on GGPoker).  This is because there could be many Nash Equilibriums and if 2 players are playing from one Nash Equilibrium profile and you are playing from a different Nash Equilibrium profile, then those 3 strategies taken together aren't necessarily a Nash Equilibrium. Which means you need to somehow know the other two players are playing from a particular Nash Equilibrium. A nice counter example was given in section 2.4 of "Using Counterfactual Regret Minimization to Create Competitive Multiplayer Poker Agents" by Abou Risk and Szafron in 2010.

To find a good strategy for 3 player all-in or fold poker, a different method can be used that involves guessing your opponents' strategies and running the algorithm on it except instead of updating all the strategies, you keep those two opponents' strategies fixed. This would not necessarily give a Nash Equilibrium strategy but it would give the most exploitative strategy, ie the most profitable strategy (if your opponents keep their strategies fixed which is unlikely as most players adjust their strategy to what the other players are doing)

# Other Works

[Holdem Resources](https://www.holdresources.net) published a chart and says to compute the chart, they stepped through every possible "effective stack size" (the minimum of the two players' stack sizes) of big blinds in 0.05 bb increments.

Bill Chen's "The Mathematics of Poker" (2006) also published "Jam or Fold" tables. It used fictitious play as well.

## Requirements: 

- g++ with c++20 standard
- make or Mingw32-make (for Windows)
- handranks.dat (the 123MB file of 30 million integers from the two-plus-two evaluator)

# To Build and Run

Linux:

    $ make
    $ ./headsup_nash 1000000
    
Windows:

    > mingw32-make
    > .\headsup_nash.exe 1000000
    
#### Example in Windows Powershell:
```
PS D:\code\poker2> .\headsup_nash.exe 10000000
Loading handranks.dat ...Done
Button Strategy:
  |  A |  K |  Q |  J |  T |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |
A |1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|
K |1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|
Q |1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|0.99|1.00|1.00|0.99|1.00|
J |1.00|1.00|1.00|1.00|1.00|1.00|0.99|1.00|0.99|0.84|0.94|0.42|0.09|
T |1.00|1.00|1.00|1.00|1.00|0.99|0.99|0.99|0.93|0.69|0.54|0.03|0.20|
9 |1.00|1.00|1.00|1.00|0.99|1.00|1.00|0.98|0.98|0.96|0.01|0.00|0.00|
8 |1.00|1.00|1.00|0.95|0.99|0.97|1.00|1.00|0.99|0.96|0.59|0.00|0.00|
7 |1.00|0.99|0.59|0.01|0.00|0.85|0.99|1.00|1.00|1.00|0.87|0.01|0.01|
6 |1.00|1.00|0.01|0.00|0.00|0.02|0.48|0.63|1.00|0.98|0.92|0.97|0.00|
5 |1.00|1.00|0.04|0.00|0.00|0.01|0.00|0.01|0.17|1.00|0.99|0.97|0.56|
4 |1.00|1.00|0.00|0.01|0.00|0.00|0.00|0.00|0.00|0.00|1.00|1.00|0.00|
3 |1.00|0.98|0.00|0.01|0.00|0.00|0.00|0.00|0.00|0.00|0.00|1.00|0.02|
2 |1.00|1.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|1.00|

Big blind Strategy:
  |  A |  K |  Q |  J |  T |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |
A |1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|
K |1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|1.00|0.99|1.00|1.00|
Q |1.00|1.00|1.00|1.00|1.00|1.00|1.00|0.67|0.35|0.11|0.00|0.00|0.00|
J |1.00|1.00|1.00|1.00|1.00|0.99|0.70|0.06|0.00|0.00|0.00|0.00|0.00|
T |1.00|1.00|1.00|1.00|1.00|0.97|0.00|0.00|0.00|0.00|0.00|0.00|0.00|
9 |1.00|1.00|1.00|0.39|0.00|1.00|0.02|0.00|0.00|0.00|0.00|0.00|0.00|
8 |1.00|1.00|0.07|0.01|0.00|0.01|1.00|0.01|0.00|0.00|0.00|0.00|0.00|
7 |1.00|0.99|0.00|0.00|0.00|0.00|0.00|1.00|0.00|0.00|0.01|0.00|0.00|
6 |1.00|0.97|0.00|0.00|0.00|0.00|0.00|0.00|1.00|0.00|0.00|0.00|0.00|
5 |1.00|0.87|0.00|0.00|0.00|0.00|0.00|0.00|0.00|1.00|0.00|0.00|0.00|
4 |1.00|0.02|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|1.00|0.00|0.00|
3 |1.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|1.00|0.00|
2 |1.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|1.00|

iterations: 10000000
blinds: 1/2
stack size (in chips): 20
Note: Unsuited in bottom left, suited in top right.
```

# Future Directions

- solve the full heads up no limit texas holdem game
- GUI
- All-in or Fold with more than two players (not necessarily a Nash Equilibrium as explained above)
- Find a faster hand evaluator if possible
- Multithreading
- web app
