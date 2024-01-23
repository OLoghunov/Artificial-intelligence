//
// Created by brend on 03.12.2023.
//

#include "Apriori.h"


void Apriori::getItemsFromTransactions() {
    for(const auto& transaction: transactions){
        for(const auto& transactionItem: transaction.second){
            this->items.insert(transactionItem);
        }
    }
}

int Apriori::frequency(const std::set<int> items) {
    int freq{0};

    for(const auto& transaction: transactions){
        int count = 0;
        for(const auto& item: items){
            if(transaction.second.count(item))
                count++;
        }
        if(count == items.size())
            freq++;
    }

    return freq;
}

double Apriori::support(const std::set<int> items) {
    return (double)frequency(items) / transactions.size();
}

void Apriori::analyze() {
    std::set<int> firstGen;
    std::vector<std::set<int>> candidates;

    //selecting the first generation
    for(const auto& item: items){
        if(support(std::set<int>{item}) > minSupportLevel)
            firstGen.insert(item);
    }

    //make gen2 (pair elements)
    for(const auto& item1: firstGen){
        for(const auto& item2: firstGen){
            std::set<int> pair{item1};
            if(item1 == item2 ) continue;
            pair.insert(item2);
            if(std::count(candidates.begin(), candidates.end(),pair) == 0)
                candidates.push_back(pair);
        }
    }

    while(candidates.size() != items.size() && !candidates.empty()){
        for(auto candidate: candidates){
            if(support(candidate) < minSupportLevel)
                candidates.erase(std::find(candidates.begin(), candidates.end(),candidate));
        }
        if(candidates[0].size() >= minSetSize)
            for(auto candidate: candidates)
                associations.push_back(candidate);
        candidates = getNewGeneration(candidates);
    }

    makeConfidenceList();

}

std::vector<std::set<int>> Apriori::getNewGeneration(const std::vector<std::set<int>> prewGeneration) {

    std::vector<std::set<int>> newGeneration;

    for(const auto element1: prewGeneration){
        for(const auto element2: prewGeneration){
            if(element1 == element2)
                continue;
            if(hasCrossingPossibility(element1, element2)){
                auto newGenItems = join(element1,element2);
                if( std::count(newGeneration.begin(), newGeneration.end(),newGenItems) == 0
                    && support(newGenItems) > minSupportLevel ) //todo подумать можно ли тут производить сравнения с std::set?
                    newGeneration.push_back(newGenItems);
            }
        }
    }

    return newGeneration;
}

bool Apriori::hasCrossingPossibility(std::set<int> items1, std::set<int> items2) {

    int count{0};

    for(const auto element1: items1){
        for(const auto element2: items2){
            if(element1 == element2)
                count++;
        }
    }

    return count == items1.size() - 1;

}

std::set<int> Apriori::join(std::set<int> items1, std::set<int> items2) {
    items1.insert(items2.begin(), items2.end());
    return items1;
}

Apriori::Apriori(std::map<int, std::set<int>> transactions, double minSupportLevel, int minSetSize): transactions{transactions},minSupportLevel{minSupportLevel} {
    this->minSetSize = 3;
    minConfidenceLevel = 0.65;
    rules = {};
    getItemsFromTransactions();
}

std::vector<std::set<int>> Apriori::getAssociations() {
    return associations;
}

void Apriori::setMinSetSize(int size) {
    this->minSetSize = size;
}

double Apriori::confidence(const std::set<int> from, const std::set<int> to) {
    std::set<int> allItems{};
    allItems.insert(from.begin(), from.end());
    allItems.insert(to.begin(), to.end());
    return (double)support(allItems) / support(from);
}

void Apriori::makeConfidenceList() {
    for(const auto& association: associations ){
        for(const auto& item: association){
            std::pair<std::set<int>, std::set<int>> pair{};
            std::set<int> first{association};
            first.erase(item);
            pair.first = first;
            pair.second = std::set{item};
            double probability=confidence(pair.first, std::set{pair.second});
            if(probability > minConfidenceLevel){
                rules.push_back(pair);
            }
            if(association.size() > 2){
                probability = confidence(std::set{pair.second}, pair.first);
                if(probability > minConfidenceLevel)
                    rules.push_back(std::pair{pair.second, pair.first});
            }

        }
    }
}

std::vector<std::pair<std::set<int>, std::set<int>>> Apriori::getRules() {
    return rules;
}

void Apriori::setMinSupportLevel(double minSupportLevel) {
    this->minSupportLevel = minSupportLevel;
}

void Apriori::setMinConfidenceLevel(double minConfidenceLevel) {
    this->minConfidenceLevel = minConfidenceLevel;
}

std::set<int> Apriori::getItems() {
    return items;
}
