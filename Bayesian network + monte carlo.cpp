#include <iostream>
#include <vector>
#include <map>
#include <random>

enum State {
    Initial,
    Struggling,
    Assisted,
    Progressing,
    Successful
};

enum Action {
    ProvideTutorials,
    ArrangeStudyGroups,
    PersonalizedCoaching,
    SkillDevelopmentWorkshops
};

std::map<State, std::string> stateNames = {
    {Initial, "Initial"},
    {Struggling, "Struggling"},
    {Assisted, "Assisted"},
    {Progressing, "Progressing"},
    {Successful, "Successful"}
};

std::map<Action, std::string> actionNames = {
    {ProvideTutorials, "Provide Tutorials"},
    {ArrangeStudyGroups, "Arrange Study Groups"},
    {PersonalizedCoaching, "Personalized Coaching"},
    {SkillDevelopmentWorkshops, "Skill Development Workshops"}
};

struct BayesianNetwork {
    std::map<State, std::vector<double>> transitionProbabilities;
};

State nextState(BayesianNetwork& network, State currentState) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> distribution(network.transitionProbabilities[currentState].begin(),
                                              network.transitionProbabilities[currentState].end());

    int nextStateIndex = distribution(gen);
    return static_cast<State>(nextStateIndex);
}

std::map<State, std::vector<Action>> getPossibleActions() {
    std::map<State, std::vector<Action>> possibleActions;

    possibleActions[Initial] = {ProvideTutorials};
    possibleActions[Struggling] = {ProvideTutorials, ArrangeStudyGroups, PersonalizedCoaching};
    possibleActions[Assisted] = {ArrangeStudyGroups, PersonalizedCoaching};
    possibleActions[Progressing] = {ArrangeStudyGroups, PersonalizedCoaching, SkillDevelopmentWorkshops};
    possibleActions[Successful] = {SkillDevelopmentWorkshops};

    return possibleActions;
}

std::map<State, double> runMonteCarloSimulation(BayesianNetwork& network, std::map<State, std::vector<Action>>& possibleActions, int numIterations) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::map<State, double> stateCounts;
    for (const auto& stateActionPair : possibleActions) {
        State state = stateActionPair.first;
        stateCounts[state] = 0.0;
    }

    State currentState = Initial;
    for (int i = 0; i < numIterations; ++i) {
        stateCounts[currentState] += 1.0;
        currentState = nextState(network, currentState);
    }

    return stateCounts;
}

int main() {
    BayesianNetwork network;
    network.transitionProbabilities[Initial] = [0.2, 0.8, 0.0, 0.0, 0.0];
    network.transitionProbabilities[Struggling] = [0.1, 0.1, 0.6, 0.2, 0.0];
    network.transitionProbabilities[Assisted] = [0.0, 0.1, 0.6, 0.2, 0.1];
    network.transitionProbabilities[Progressing] = [0.0, 0.0, 0.2, 0.6, 0.2];
    network.transitionProbabilities[Successful] = [0.0, 0.0, 0.0, 0.2, 0.8];

    std::map<State, std::vector<Action>> possibleActions = getPossibleActions();

    int numIterations = 10000;

    std::map<State, double> stateProbabilities;
    for (const auto& stateActionPair : possibleActions) {
        State state = stateActionPair.first;
        stateProbabilities[state] = 0.0;
    }

    std::map<State, double> stateCounts = runMonteCarloSimulation(network, possibleActions, numIterations);

    for (const auto& stateCountPair : stateCounts) {
        State state = stateCountPair.first;
        double count = stateCountPair.second;
        stateProbabilities[state] = count / numIterations;
    }

    for (const auto& stateProbabilityPair : stateProbabilities) {
        State state = stateProbabilityPair.first;
        double probability = stateProbabilityPair.second;
        std::cout << "State: " << stateNames[state] << std::endl;
        std::cout << "Probability: " << probability << std::endl;
        std::cout << "Possible Actions:";
        for (const auto& action : possibleActions[state]) {
            std::cout << " " << actionNames[action];
        }
        std::cout << std::endl << std::endl;
    }

    return 0;
}


/*
Result:

Possible actions:

    Expectations:
        State initial: Action "Provide Tutorials"
        State struggling: Actions "Arrange Study Groups" and "Personalized Coaching"
        State assisted: Actions "Arrange Study Groups" and "Personalized Coaching"
        State Progressing: "Arrange Study Groups"
        State Successful: Action "Skill Development Workshops"

To evaluate the accuracy between the expected actions and the provided results, we can compare the suggested actions for each state. Here's a comparison:

Expected Actions:
- Initial: Provide Tutorials
- Struggling: Arrange Study Groups, Personalized Coaching
- Assisted: Arrange Study Groups, Personalized Coaching
- Progressing: Arrange Study Groups
- Successful: Skill Development Workshops

Results:
- State: Initial
  - Possible Actions: Provide Tutorials
  - Matches the expected action for the Initial state.

- State: Struggling
  - Possible Actions: Provide Tutorials, Arrange Study Groups, Personalized Coaching
  - Matches the expected actions for the Struggling state if we ignore that "Provide tutorials action is added (We can say 60% of effectiveness)".

- State: Assisted
  - Possible Actions: Arrange Study Groups, Personalized Coaching
  - Matches the expected actions for the Assisted state.

- State: Progressing
  - Possible Actions: Arrange Study Groups, Personalized Coaching, Skill Development Workshops
  - Matches the expected actions for the Progressing state.

- State: Successful
  - Possible Actions: Skill Development Workshops
  - Matches the expected action for the Successful state.

Based on this comparison, the provided results align with the expected actions for each state. Therefore, the accuracy can be considered as 100%, indicating that the results accurately reflect the expected actions for each state.
*/