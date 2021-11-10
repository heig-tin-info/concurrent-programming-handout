#ifndef SHAREDSECTIONSETTINGS_H
#define SHAREDSECTIONSETTINGS_H
#include <vector>

class SharedSectionSettings
{
public:
    SharedSectionSettings(const int& requestContact, const int& enterOrWaitContact, const int& leaveContact, std::vector<std::pair<int, int>> switchSettings) :
        requestContact(requestContact), enterOrWaitContact(enterOrWaitContact), leaveContact(leaveContact), switchSettings(switchSettings)
    {}

    int requestContact;
    int enterOrWaitContact;
    int leaveContact;
    std::vector<std::pair<int, int>> switchSettings;
};

#endif // SHAREDSECTIONSETTINGS_H
