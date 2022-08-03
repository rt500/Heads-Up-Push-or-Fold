#ifndef STRATEGY_H
#define STRATEGY_H
#include <array>

class Strategy
{
private:
  // the strategy is given as probabilities of pushing for each
  // hand in index representation
  std::array<std::array<double,13>,13> strategy = {0};
public:
  Strategy(double value);
  Strategy() = default;

  void set_strategy(int card1, int card2, double value,
          bool is_index_rep = true);
  double get_strategy(int card1, int card2,
          bool is_index_rep = true);
  std::pair<int, int> get_index(int card1, int card2);
};

#endif
