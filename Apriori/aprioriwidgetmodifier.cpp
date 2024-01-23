#include "aprioriwidgetmodifier.h"
#include <fstream>
#include <sstream>

void AprioriWidgetModifier::changeSupportLevel(float value)
{
    supportLevel = value/100;
}

void AprioriWidgetModifier::changeConfidenceLevel(float value)
{
    confidenceLevel = value/100;
}

void AprioriWidgetModifier::changeMinSetSize(int value)
{
    minSetSize = value;
}

void AprioriWidgetModifier::read()
{
    std::fstream f;
    QString result;
    f.open("C:/Qt/Examples/Qt-6.6.1/datavisualization/graphgallery/resourses/dataset.txt");

    if (f.is_open())
    {
        transactions.clear();
        std::string num;

        while(std::getline(f, num)){

            std::stringstream stream(num);

            int n;

            std::set<int> st;

            while(stream >> n){

                st.insert(n);
            }

            transactions[currentMaxKey] = st;
            currentMaxKey++;
        }
    }

    f.close();

    for (auto& transaction : transactions)
    {
        for (int value : transaction.second)
            result += QString::number(value) + " ";
        result += "\n";
    }

    emit transactionsChanged(result);
}

void AprioriWidgetModifier::analyze()
{
    QString result;
    Apriori apriori(transactions, 0.25, 3);
    apriori.setMinConfidenceLevel(confidenceLevel);
    apriori.setMinSupportLevel(supportLevel);
    apriori.setMinSetSize(minSetSize);
    apriori.analyze();

    auto items = apriori.getItems();

    result += "Unique items:\n\n";

    for (int item : items)
    {
        result += QString::number(item) += " ";
    }

    result += "\n\n";

    auto associations = apriori.getAssociations();

    result += "Associations:\n\n";
    for(auto association: associations) {
        for (int i: association)
            result += QString::number(i) += " ";
        result += "\n";
    }

    result += "\n";
    result += "Strong rules:\n\n";

    auto rules = apriori.getRules();

    for (std::pair<std::set<int>, std::set<int>> rule : rules)
    {
        for (int i : rule.first)
            result += QString::number(i) += " ";
        result += "\t--->\t";
        for (int i : rule.second)
            result += QString::number(i) += " ";
        result += "\n";
    }

    emit analyzed(result);
}
