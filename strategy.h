#ifndef STRATEGY_H
#define STRATEGY_H
#include <array>


/**
 * A strategy for a player for two player all-in or fold (Texas Hold'em).
 *
 * Every possible starting hand can be either a pair, unsuited or suited.
 * This is what I call the 'Index Representation'.
 * A strategy is represented as a 13x13 grid of numbers between 0 and 1
 * inclusive. For some games, a strategy might be 'pure' which means
 * you would take the same action every decision node. In this case, 
 * the numbers would only be 0 or 1. It turns out Nash Equilibrium
 * strategies can be 'mixed'. That is, you choose randomly to take a
 * certain action at a decision node. In the case of two player all-in
 * or fold, this means if you're dealt a hand and the strategy for
 * say 7,6 suited is 0.6, then that means the strategy is to randomly
 * push 60% of the time and 40% of the time fold.
 */
class Strategy
{
  private:

    // the strategy is given as probabilities of pushing for each
    // hand in index representation
    std::array<std::array<double,13>,13> strategy = {0};

  public:

    /**
     * A strategy for each hand in 'index' representation.
     *
     * For each 2 card starter hand, assign a probability (i.e. a number
     * between 0 and 1 inclusive) which is the probability a player 
     * would push. To determine whether to push or fold, you would
     * randomly choose a value in the range [0, 1] and if it's less
     * than the value in the 2d array, you would push else you would fold.
     *
     * @param value The initial value to set each hand's strategy.
     */
    Strategy(double value);
    
    Strategy() = default;

    /**
     * Set the strategy of a particular hand.
     *
     * The cards can be given in index representation (default) or
     * specific representation (you would need to set is_index_rep to false).
     *
     * @param card1 
     * @param card2
     * @param value The probability of going all in to set for that hand.
     * @param is_index_rep Whether or not it is in index or
     *                     specific representation.
     */
    void set_strategy(int card1, int card2, double value,
            bool is_index_rep = true);

    /**
     * Get the strategy for a particular hand.
     *
     * The hand can be given in index representation or specific
     * representation.
     *
     * @param card1
     * @param card2
     * @param is_index_rep Whether or not the cards are in index or specific
     *                     representation.
     * @return The probability of pushing for that particular hand.
     */
    double get_strategy(int card1, int card2, bool is_index_rep = true);

   /**
    * Get Index representation from Specific Representation of a starter hand.
    *
    * e.g. Since a player's strategy for an unsuited hand is the same no matter
    * the two suits of the hand, we can reduce the number of starter hands from
    * (52 choose 2) = 1326 to 13x13 = 169. Where there are 3 broad categories:
    * pairs, suited, unsuited.
    *
    * @param hand The specific representation.
    * @return The index representation of the hand.
    */
    std::pair<int, int> get_index(int card1, int card2);
};

#endif
