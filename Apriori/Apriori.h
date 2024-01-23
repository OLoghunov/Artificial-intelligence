//
// Created by brend on 03.12.2023.
//

#ifndef APRIORI_APRIORI_H
#define APRIORI_APRIORI_H
#include <vector>
#include <map>
#include <algorithm>
#include <set>

class Apriori {
private:
    double minSupportLevel;
    double minConfidenceLevel;
    int minSetSize;

    std::set<int> items;
    std::map<int, std::set<int>> transactions;
    std::vector<std::set<int>> associations;
    std::vector<std::pair<std::set<int>, std::set<int>>> rules;

    void getItemsFromTransactions();
    int frequency(const std::set<int> items);
    double support(const std::set<int> items);
    double confidence(const std::set<int> from, const std::set<int> to);
    void makeConfidenceList();
    std::vector<std::set<int>> getNewGeneration(const std::vector<std::set<int>> prewGeneration);
    bool hasCrossingPossibility(std::set<int> items1, std::set<int> items2);
    std::set<int> join(std::set<int> items1, const std::set<int> items2);

public:
    /// <param name = 'transactions'> Словарь со всеми транзакциями по типу [Код операции; Множество товаров] </param>
    /// <param name = 'minSupportLevel'> Минимальный уровень поддержки. Пусть А - частота встречи товара в транзакциях, B - размер транзакций. SupportLvl = A / B  </param>
    /// <param name = 'minSetSize'> Минимальная величина множеств, которые надо сохранять. </param>
    Apriori(std::map<int, std::set<int>> transactions, double minSupportLevel, int minSetSize);

    /// <summary>
    /// Находит закономерности в покупках(транзакциях) в виде вектора с ассоциативными множествами. Сохраняет в Aprioti::associations.
    /// </summary>
    void analyze();

    ///<summary>
    ///Устанавливает минимальную длинну множества, которое можно добавлять в ассоциации.
    ///<exmple>Если setMinSetSize = 3, то в ассоциации нельзя добавить множества типа: {a,b}, но можно {a,b,c}</example>
    ///</summary>
    void setMinSetSize(int size);

    /// <summary>
    /// Создает сильные правила. Кол-во таких правил зависит от параметра MinConfidenceLevel. Выше уровнь, меньше правил.
    /// </summary>
    /// <returns>Список ассоциаций</returns>
    std::vector<std::pair<std::set<int>, std::set<int>>> getRules();

    /// <summary>
    /// Находит все ассоциации в транзакциях. Кол-во таких транзакций зависит от параметра minSupportLevel. Выше уровень, меньше ассоциаций.
    /// </summary>
    /// <returns>Список ассоциаций</returns>
    std::vector<std::set<int>> getAssociations();

    void setMinSupportLevel(double minSupportLevel);
    void setMinConfidenceLevel(double minConfidenceLevel);

    /// <summary>
    /// Разбивает входящие транзакции на уникальные продукты
    /// </summary>
    /// <returns>Список уникальых продуктов</returns>
    std::set<int> getItems();
};


#endif //APRIORI_APRIORI_H
