#include <vector>
//generate an episode
//returns a vector of tuples of (state, action, reward)
std::vector<std::tuple<std::vector<int>,std::vector<int>,int>> generateEpisode();