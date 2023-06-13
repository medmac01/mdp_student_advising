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

struct DecisionTree {
    std::map<State, Action> actionDecisions;
};

Action nextState(const DecisionTree& decisionTree, State currentState) {
    return decisionTree.actionDecisions.at(currentState);
}

std::map<Action, double> buildActionProbabilities() {
    std::map<Action, double> actionProbabilities;

    actionProbabilities[ProvideTutorials] = 0.0;
    actionProbabilities[ArrangeStudyGroups] = 0.0;
    actionProbabilities[PersonalizedCoaching] = 0.0;
    actionProbabilities[SkillDevelopmentWorkshops] = 0.0;

    return actionProbabilities;
}

int main() {
    DecisionTree decisionTree;
    decisionTree.actionDecisions[Initial] = ProvideTutorials;
    decisionTree.actionDecisions[Struggling] = PersonalizedCoaching;
    decisionTree.actionDecisions[Assisted] = ArrangeStudyGroups;
    decisionTree.actionDecisions[Progressing] = SkillDevelopmentWorkshops;
    decisionTree.actionDecisions[Successful] = SkillDevelopmentWorkshops;

    int numIterations = 10000;

    std::map<Action, double> actionProbabilities = buildActionProbabilities();

    Action currentAction = ProvideTutorials;
    for (int i = 0; i < numIterations; ++i) {
        actionProbabilities[currentAction] += 1.0;
        currentAction = nextState(decisionTree, static_cast<State>(currentAction));
    }

    for (const auto& actionProbabilityPair : actionProbabilities) {
        Action action = actionProbabilityPair.first;
        double probability = actionProbabilityPair.second;
        std::cout << "Action: " << action << std::endl;
        std::cout << "Probability: " << probability / numIterations << std::endl << std::endl;
    }

    return 0;
}